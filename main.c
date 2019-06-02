#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "inc/menu.h"
#include "inc/app_utils.h"
#include "inc/custom_types.h"
#include "inc/queue_utils.h"

#define USERNAME_MAX_LEN 10
#define MSG_MAX_LEN 500
#define SENDING_HEADER_LEN USERNAME_MAX_LEN + 2

void* sender_thread(void *p);
void* receiver_thread(void *p);

void exit_handler(int signum);
void send_handler(int signum);
void broadcast_handler(int signum);

char* username = "";
struct Input INPUT = {"", ""};
mqd_t inbox;

int main(){
    signal(SIGINT, exit_handler);
    char* raw_input;
    
    printf("Type your username: ");
    username = get_str(USERNAME_MAX_LEN);
    
    if(!is_valid_username(username)){
        fprintf(stderr, "Invalid username!\n");
        return 0;
    }

    inbox = create_q(username);

    pthread_t sender_tid;
    pthread_t receiver_tid;
    
    pthread_create(&sender_tid, NULL, &sender_thread, NULL);
    pthread_create(&receiver_tid, NULL, &receiver_thread, NULL);
    
    while(1){
        raw_input = get_str(SENDING_HEADER_LEN + MSG_MAX_LEN);
        
        if(strcmp(raw_input, "list") == 0){
            str_array_t online_users = find_available_chats();
            printf("\n---------- Online users ----------\n");
            for(int i=0; i<online_users.length; i++){
                printf("%s\n", get_username(online_users.elements[i]));
            }
        }else{
            INPUT = treat_input(raw_input, username);

            if(strcmp(INPUT.receiver, "all") == 0){
                pthread_kill(sender_tid, SIGUSR2);
            }else{
                pthread_kill(sender_tid, SIGUSR1);
            }
        }
    }

    return 0;
}

void exit_handler(int signum){
    signal(signum, SIG_IGN);
    printf("\nDo you really wan't to exit? [y/N] ");
    char answer = getchar();

    if(answer == 'y' || answer == 'Y'){
        close_q(inbox);
        destroy_q(username);
        exit(0);
    }else{
        signal(SIGINT, exit_handler);
    }
    getchar();
}

void send_handler(int signum){
    str_array_t av_chats = find_available_chats();
    send_message(INPUT.sender, INPUT.receiver, INPUT.raw);
}

void broadcast_handler(int signum){
    broadcast_message(INPUT.sender, INPUT.raw);
}

void* sender_thread(void *p){
    signal(SIGUSR1, send_handler);
    signal(SIGUSR2, broadcast_handler);
    while(1);
}

void* receiver_thread(void *p){
    char* msg = (char*) malloc(MSG_MAX_SIZE);
    inbox = read_q(username);
    while(1){
        if ((mq_receive(inbox, (char*) msg, MSG_MAX_SIZE, 0)) < 1) {
            perror("mq_receive:");
        }else{
            print_msg(msg);
        }
    }
}