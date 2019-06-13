#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#include "inc/queue_utils.h"
#include "inc/chat.h"
#include "inc/client.h"

void* sender_thread(void* arg);
void* resender_thread(void* arg);
void* receiver_thread(void* arg);

void exit_handler(int signum);

Chat INBOX;
char BUFFER[MAX_MESSAGE_SIZE];
resend_queue_t* RESEND_QUEUE;
pthread_t resender_tid;

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
    pthread_create(&resender_tid, NULL, &resender_thread, NULL);
    pthread_create(&receiver_tid, NULL, &receiver_thread, NULL);
    
    while(1){
        fgets(BUFFER, MAX_MESSAGE_SIZE, stdin);
        if ((strlen(BUFFER) > 0) && (BUFFER[strlen (BUFFER) - 1] == '\n'))
            BUFFER[strlen (BUFFER) - 1] = '\0';

        if(is_message(BUFFER) == 1){
            if(is_broadcast(BUFFER) == 1){
                pthread_cond_signal(&cv_send);
                pthread_kill(sender_tid, SIGUSR2);
            }else{
                pthread_cond_signal(&cv_send);
                pthread_kill(sender_tid, SIGUSR1);
            }
        }else{
            if(strcmp(BUFFER, "list") == 0){
                printf("Listing online users!\n");
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
        close_q(INBOX.queue);
        destroy_q(INBOX.username);
        exit(0);
    }else{
        signal(SIGINT, exit_handler);
    }
    getchar();
}

void sender_handler(int signum){
    char* receiver = get_receiver(BUFFER);
    char* tmp = calloc(MAX_MESSAGE_SIZE,sizeof(char));
    tmp = strcpy(tmp, BUFFER);
    BUFFER[0]='\0';

    // pass the msg to a resend thread when send fails
    if(send_message(receiver, tmp) < 0){
        queued_msg_t queued_msg = {receiver, tmp};
        // add_to_queue(RESEND_QUEUE, queued_msg);
        // pthread_kill(resender_tid, SIGUSR1);
    }
}

void resender_handler(int signum){
    fprintf(stderr, "Resend\n");
    if(RESEND_QUEUE->size > 0){
        for(int i=0; i<3; i++){
            sleep(5);
            int index = RESEND_QUEUE->size - 1;
            char* receiver = RESEND_QUEUE->elements[index].receiver;
            char* msg = RESEND_QUEUE->elements[index].raw_msg;
            send_message(receiver, msg);
        }
        RESEND_QUEUE->size -= 1;
    }
}

void broadcast_handler(int signum){
    chat_array_t av_chats = get_chats();
    char* tmp = calloc(MAX_MESSAGE_SIZE,sizeof(char));
    tmp = strcpy(tmp, BUFFER);
    BUFFER[0]='\0';
    for(int i=0; i<av_chats.length; i++){
        send_message(av_chats.elements[i].username, tmp);
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

void* resender_thread(void* arg){
    RESEND_QUEUE = new_resend_queue();
    // signal(SIGUSR1, resender_handler);
}

void* receiver_thread(void* arg){
    char* msg;
    while(1){
        msg = (char*) calloc(MAX_MESSAGE_SIZE, sizeof(char));
        if ((mq_receive(INBOX.queue, (char*) msg, MAX_MESSAGE_SIZE, 0)) < 1) {
            perror("mq_receive: ");
        }else{
            show_message(msg, BUFFER);
        }
    }
}