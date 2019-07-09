#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#include "inc/chat.h"
#include "inc/client.h"
#include "inc/message.h"

void* sender_thread(void* arg);
void* receiver_thread(void* arg);

void exit_handler(int signum);

Chat INBOX;
char INPUT_BUFFER[MAX_MESSAGE_SIZE];

Message* MSG_HISTORY[5000];
int LAST_MSG = 0;

// thread mutex
pthread_cond_t cv_send;
pthread_mutex_t lock_send;

int main(){
    signal(SIGINT, exit_handler);

    char username[MAX_USERNAME_LEN];
    printf("Type your username: ");
    scanf("%s", username);
    validate_username(username);
    INBOX = new_chat_inbox(username, NULL);

    pthread_t sender_tid;
    pthread_t receiver_tid;

    pthread_create(&sender_tid, NULL, &sender_thread, NULL);
    pthread_create(&receiver_tid, NULL, &receiver_thread, NULL);
    
    while(1){
        fgets(INPUT_BUFFER, MAX_MESSAGE_SIZE, stdin);
        if ((strlen(INPUT_BUFFER) > 0) && (INPUT_BUFFER[strlen (INPUT_BUFFER) - 1] == '\n'))
            INPUT_BUFFER[strlen (INPUT_BUFFER) - 1] = '\0';

        if(is_message(INPUT_BUFFER) == 1){
            if(is_broadcast(INPUT_BUFFER) == 1){
                pthread_cond_signal(&cv_send);
                pthread_kill(sender_tid, SIGUSR2);
            }else{
                pthread_cond_signal(&cv_send);
                pthread_kill(sender_tid, SIGUSR1);
            }
        }else{
            if(strcmp(INPUT_BUFFER, "list") == 0){
                printf("Listing online users!\n");
            }
        }
    }

    return 0;
}

void sender_handler(int signum){
    Message* message = new_message(INPUT_BUFFER);
    MSG_HISTORY[LAST_MSG] = message;
    printf("%s\n", message->id);
    LAST_MSG++;

    send_message(message);
}

void broadcast_handler(int signum){
    Message* message = new_message(INPUT_BUFFER);
    MSG_HISTORY[LAST_MSG] = message;
    printf("%s\n", message->id);
    LAST_MSG++;

    chat_array_t av_chats = get_chats();
    for(int i=0; i<av_chats.length; i++){
        send_direct_message(av_chats.elements[i].username, message->raw);
    }    
}

void* receiver_thread(void* arg){
    char* str_msg;
    while(1){
        str_msg = (char*) calloc(MAX_MESSAGE_SIZE, sizeof(char));
        if ((mq_receive(INBOX.queue, (char*) str_msg, MAX_MESSAGE_SIZE, 0)) < 1) {
            perror("mq_receive: ");
        }else{
            Message* message = new_message_with_id(str_msg);
            show_message(message);
            request_auth(message);
        }
    }
}

void* sender_thread(void* arg){
    signal(SIGUSR1, sender_handler);
    signal(SIGUSR2, broadcast_handler);
    while(1){
        pthread_mutex_lock(&lock_send);
        pthread_cond_wait(&cv_send, &lock_send);
        pthread_mutex_unlock(&lock_send);
    }
}

void exit_handler(int signum){
    signal(signum, SIG_IGN);
    printf("\nDo you really wan't to exit? [y/N] ");
    char answer = getchar();

    if(answer == 'y' || answer == 'Y'){
        close_q(INBOX.queue);
        destroy_q(INBOX.username);
        exit(0);
    }else{
        signal(SIGINT, exit_handler);
    }
    getchar();
}
