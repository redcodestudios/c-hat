#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <pthread.h>

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

struct Input INPUT = {"", ""};

int main(){
    signal(SIGINT, exit_handler);
    
    printf("Type your username: ");
    const char* username = get_str(USERNAME_MAX_LEN);

    pthread_t sender_tid;
    pthread_t receiver_tid;
    
    pthread_create(&sender_tid, NULL, &sender_thread, NULL);
    pthread_create(&receiver_tid, NULL, &receiver_thread, NULL);
    
    char* raw_input;

    while(1){
        printf("chat: ");
        raw_input = get_str(SENDING_HEADER_LEN + MSG_MAX_LEN);
        INPUT = treat_input(raw_input, username);

        if(strcmp(INPUT.receiver, "all") == 0){
            pthread_kill(sender_tid, SIGUSR2);
        }else{
            pthread_kill(sender_tid, SIGUSR1);
        }
        
        // int option  = menu();
        // switch (option){
        //     case SEND_MESSAGE:
        //         pthread_kill(sender_tid, SIGUSR1);
        //         break;
        //     case LIST_USERS:
        //         break;
        //     case BROADCAST:
        //         pthread_kill(sender_tid, SIGUSR2);
        //         break;
        //     case EXIT:
        //         return 0;
        // }
    }
    // create_q("pablo");
    // str_array_t chats = find_available_chats();

    // int a = send_message("rodrigo", "pablo", "ola", chats);
    // printf("send msg result %d\n", a);

    return 0;
}

void exit_handler(int signum){
    signal(signum, SIG_IGN);
    printf("\nDo you really wan't to exit? [y/N] ");
    char answer = getchar();

    if(answer == 'y' || answer == 'Y'){
        exit(0);
    }else{
        signal(SIGINT, exit_handler);
    }
    getchar();
}

void send_handler(int signum){
    send_message(INPUT.sender, INPUT.receiver, INPUT.msg);
}

void broadcast_handler(int signum){
    broadcast_message(INPUT.sender, INPUT.msg);
}

void* sender_thread(void *p){
    signal(SIGUSR1, send_handler);
    signal(SIGUSR2, broadcast_handler);
    while(1);
}

void* receiver_thread(void *p){
    printf("\npablo says: iae\n");
}