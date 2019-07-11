#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#include "inc/channel.h"
#include "inc/chat.h"
#include "inc/client.h"
#include "inc/message.h"

void* sender_thread(void* arg);
void* channel_thread(void* arg);
void* receiver_thread(void* arg);

void exit_handler(int signum);

//Chat INBOX;
char INPUT_BUFFER[MAX_MESSAGE_SIZE];
Message* MESSAGE_BUFFER = NULL;
User* USER = NULL;
Channel* CHANNEL = NULL;

// thread mutex
pthread_cond_t cv_send;
pthread_mutex_t lock_send;

int main(int argc, char **argv){
    signal(SIGINT, exit_handler);

    if(argc < 2){
        printf("Blank username is not valid!\n");
        exit(0);
    }

    char* username = argv[1];

    validate_username(username);
    
    USER = new_user(username);
    printf("Your username is: %s\n", argv[1]);

    pthread_t sender_tid;
    pthread_t channel_tid;
    pthread_t receiver_tid;

    pthread_create(&sender_tid, NULL, &sender_thread, NULL);
    pthread_create(&receiver_tid, NULL, &receiver_thread, NULL);
    
    char* concat_buffer = malloc(MAX_MESSAGE_SIZE);
    
    while(1){
        fgets(INPUT_BUFFER, MAX_MESSAGE_SIZE, stdin);
        if ((strlen(INPUT_BUFFER) > 0) && (INPUT_BUFFER[strlen(INPUT_BUFFER) - 1] == '\n'))
            INPUT_BUFFER[strlen (INPUT_BUFFER) - 1] = '\0';

        if(is_message(INPUT_BUFFER) == 1){
            strcpy(concat_buffer, USER->name);
            strcat(concat_buffer, ":");
            strcat(concat_buffer, INPUT_BUFFER);

            MESSAGE_BUFFER = new_message(concat_buffer);

            if(is_broadcast(concat_buffer) == 1){
                pthread_cond_signal(&cv_send);
                pthread_kill(sender_tid, SIGUSR2);
            }else{
                pthread_cond_signal(&cv_send);
                pthread_kill(sender_tid, SIGUSR1);
            }
        }else{
            if(strcmp(INPUT_BUFFER, "list") == 0){
                printf("Listing online users!\n");
                chat_array_t av_chats = get_chats();
                for(int i=0; i<av_chats.length; i++){
                    printf("%s\n", av_chats.elements[i].username);
                }
            }else if(is_channel_create(INPUT_BUFFER) == 1){
                CHANNEL = new_channel(USER, INPUT_BUFFER);
                pthread_create(&channel_tid, NULL, &channel_thread, NULL);
            }
        }
    }

    return 0;
}

void sender_handler(int signum){
    fprintf(stderr, "sending %s\n", MESSAGE_BUFFER->content);
    if(send_message(USER, MESSAGE_BUFFER) < 0){
        for(int i=0; i<3; i++){
            sleep(10);
            if(send_message(USER, MESSAGE_BUFFER) == 0){
                break;
            }
        }
    }
}

void broadcast_handler(int signum){
    send_broadcast(USER, MESSAGE_BUFFER);
}

void* receiver_thread(void* arg){
    char* incoming_raw_message;
    create_q(USER->name);
    mqd_t inbox = read_q(USER->name);
    while(1){
        incoming_raw_message = (char*) calloc(MAX_MESSAGE_SIZE, sizeof(char));
        if ((mq_receive(inbox, (char*) incoming_raw_message, MAX_MESSAGE_SIZE, 0)) < 1) {
            perror("mq_receive: ");
        }else{
            if(is_auth_response(incoming_raw_message) == 1){
                // if the message is not authentic, warning user
                if(is_authentic(incoming_raw_message) == 0){
                    printf("\nWARNING! This message is not authentic!\n");
                }
            }
            else if(is_auth_request(incoming_raw_message) == 1){
                if(strcmp(USER->last_message_id, get_message_id(incoming_raw_message)) == 0){
                    success_auth(incoming_raw_message);
                }else{
                    failed_auth(incoming_raw_message);
                }
            }
            else{
                Message* incoming_message = new_message_with_id(incoming_raw_message);
                User* message_author = new_user(incoming_message->from);

                if(strcmp(incoming_message->content, "JOIN") == 0){
                    if(CHANNEL != NULL){
                        printf("%s joined in %s channel!\n", message_author->name, CHANNEL->name);
                        add_member(message_author, CHANNEL);
                    }
                }else if(strcmp(incoming_message->content, "LEAVE") == 0){
                    if(CHANNEL != NULL){
                        leave_channel(message_author, CHANNEL);
                    }
                }else{
                    if(is_channel_msg(incoming_message) == 1){
                        show_channel_message(incoming_message);
                    }else{
                        show_message(incoming_message);
                        request_auth(incoming_message);
                    }
                }
            }
        }
    }
}

void* channel_thread(void* arg){
    char* incoming_raw_message;
    mqd_t channel_q = read_q(CHANNEL->queue_name);

    while(1){
        incoming_raw_message = (char*) calloc(MAX_MESSAGE_SIZE, sizeof(char));
        if ((mq_receive(channel_q, (char*) incoming_raw_message, MAX_MESSAGE_SIZE, 0)) < 1) {
            perror("mq_receive: ");
        }else{
            Message* incoming_message = new_message_with_id(incoming_raw_message);
            for(int i=0; i<CHANNEL->last_member; i++){
                if(CHANNEL->members[i]->valid == 1){
                    raw_send(CHANNEL->members[i]->user->name, incoming_message->repr);
                }
            }
            show_channel_message(incoming_message);
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
        if(USER != NULL){
            destroy_q(USER->name);
        }
        if(CHANNEL != NULL){
            destroy_q(CHANNEL->queue_name);
        }
        exit(0);
    }else{
        signal(SIGINT, exit_handler);
    }
    getchar();
}
