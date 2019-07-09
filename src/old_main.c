// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>
// #include <signal.h>
// #include <unistd.h>

// #include "inc/queue_utils.h"
// #include "inc/chat.h"
// #include "inc/client.h"

// void* sender_thread(void* arg);
// void* resender_thread(void* arg);
// void* receiver_thread(void* arg);

// void exit_handler(int signum);

// int MSG_HISTORY[5000];
// int LAST_MSG = 0;

// Chat INBOX;
// char BUFFER[MAX_MESSAGE_SIZE];
// resend_queue_t* RESEND_QUEUE;
// pthread_t resender_tid;

// pthread_cond_t cv_send;
// pthread_mutex_t lock_send;

// int main(){
//     MSG_HISTORY[LAST_MSG] = 666;
//     signal(SIGINT, exit_handler);
//     char username[MAX_USERNAME_LEN];
//     printf("Type your username: ");
//     scanf("%s", username);
//     validate_username(username);
//     INBOX = new_chat_inbox(username, NULL);

//     pthread_t sender_tid;
//     pthread_t receiver_tid;

//     pthread_create(&sender_tid, NULL, &sender_thread, NULL);
//     pthread_create(&resender_tid, NULL, &resender_thread, NULL);
//     pthread_create(&receiver_tid, NULL, &receiver_thread, NULL);
    
//     while(1){
//         fgets(BUFFER, MAX_MESSAGE_SIZE, stdin);
//         if ((strlen(BUFFER) > 0) && (BUFFER[strlen (BUFFER) - 1] == '\n'))
//             BUFFER[strlen (BUFFER) - 1] = '\0';

//         if(is_message(BUFFER) == 1){
//             if(is_broadcast(BUFFER) == 1){
//                 pthread_cond_signal(&cv_send);
//                 pthread_kill(sender_tid, SIGUSR2);
//             }else{
//                 pthread_cond_signal(&cv_send);
//                 pthread_kill(sender_tid, SIGUSR1);
//             }
//         }else{
//             if(strcmp(BUFFER, "list") == 0){
//                 printf("Listing online users!\n");
//             }
//         }
//     }

//     return 0;
// }

// void exit_handler(int signum){
//     signal(signum, SIG_IGN);
//     printf("\nDo you really wan't to exit? [y/N] ");
//     char answer = getchar();

//     if(answer == 'y' || answer == 'Y'){
//         close_q(INBOX.queue);
//         destroy_q(INBOX.username);
//         exit(0);
//     }else{
//         signal(SIGINT, exit_handler);
//     }
//     getchar();
// }

// void sender_handler(int signum){
//     char* receiver = get_receiver(BUFFER);
//     char* tmp = calloc(MAX_MESSAGE_SIZE,sizeof(char));
//     tmp = strcpy(tmp, BUFFER);
//     BUFFER[0]='\0';

//     int msg_id = gen_message_id(INBOX.username);
//     char str_msg_id[32];
//     sprintf(str_msg_id, "%d", msg_id);

//     strcat(tmp, ":");
//     strcat(tmp, str_msg_id);

//     MSG_HISTORY[LAST_MSG] = msg_id;
//     printf("%d\n", MSG_HISTORY[LAST_MSG]);
//     LAST_MSG++;

//     // pass the msg to a resend thread when send fails
//     if(send_message(receiver, tmp) < 0){
//         queued_msg_t queued_msg = {receiver, tmp};
//         // add_to_queue(RESEND_QUEUE, queued_msg);
//         // pthread_kill(resender_tid, SIGUSR1);
//     }else{
//         // MSG_HISTORY[LAST_MSG] = gen_message_id(INBOX.username);
//         // printf("%d\n", MSG_HISTORY[LAST_MSG]);
//         // LAST_MSG++;
//     }
// }

// void resender_handler(int signum){
//     fprintf(stderr, "Resend\n");
//     if(RESEND_QUEUE->size > 0){
//         for(int i=0; i<3; i++){
//             sleep(5);
//             int index = RESEND_QUEUE->size - 1;
//             char* receiver = RESEND_QUEUE->elements[index].receiver;
//             char* msg = RESEND_QUEUE->elements[index].raw_msg;
//             send_message(receiver, msg);
//         }
//         RESEND_QUEUE->size -= 1;
//     }
// }

// void broadcast_handler(int signum){
//     chat_array_t av_chats = get_chats();
//     char* tmp = calloc(MAX_MESSAGE_SIZE,sizeof(char));
//     tmp = strcpy(tmp, BUFFER);
//     BUFFER[0]='\0';
//     for(int i=0; i<av_chats.length; i++){
//         send_message(av_chats.elements[i].username, tmp);
//     }    
// }

// void* sender_thread(void* arg){
//     signal(SIGUSR1, sender_handler);
//     signal(SIGUSR2, broadcast_handler);
//     while(1){
//         pthread_mutex_lock(&lock_send);
//         pthread_cond_wait(&cv_send, &lock_send);
//         pthread_mutex_unlock(&lock_send);
//     }
// }

// void* resender_thread(void* arg){
//     RESEND_QUEUE = new_resend_queue();
//     // signal(SIGUSR1, resender_handler);
// }

// void* receiver_thread(void* arg){
//     char* msg;
//     while(1){
//         msg = (char*) calloc(MAX_MESSAGE_SIZE, sizeof(char));
//         if ((mq_receive(INBOX.queue, (char*) msg, MAX_MESSAGE_SIZE, 0)) < 1) {
//             perror("mq_receive: ");
//         }else{
//             if(is_auth_request(msg) == 1){
//                 if(strstr(msg, "|Y") == NULL && strstr(msg, "|N") == NULL){
//                     char* msg_id;
//                     char* sender;
//                     char* receiver;
//                     char* tmp1 = malloc(strlen(msg));
//                     char* tmp2 = malloc(strlen(msg));

//                     tmp1 = strcpy(tmp1, msg);
//                     tmp2 = strcpy(tmp2, msg);

//                     sender = strtok(tmp1, ":");
//                     receiver = strtok(NULL, ":");

//                     msg_id = strtok(tmp2, "|");
//                     msg_id = strtok(NULL, "|");

//                     char* msg_yes =  calloc(strlen(msg) + 2, sizeof(char));
//                     char* msg_no = calloc(strlen(msg) + 2, sizeof(char));
//                     char* msg_reply = reply(msg);

//                     strcat(msg_yes, msg_reply);
//                     strcat(msg_yes, "Y|");

//                     strcat(msg_no, msg_reply);
//                     strcat(msg_no, "N|");
                    
//                     int int_id;
//                     sscanf(msg_id, "%d", &int_id);

//                     for(int i=0; i<LAST_MSG; i++){
//                         if(MSG_HISTORY[i] == int_id){
//                             send_message(sender, msg_yes);
//                             break;
//                         }else{
//                             if(i == LAST_MSG-1 ){
//                                 send_message(sender, msg_no);
//                             }
//                         }
//                     }
//                 }else{
//                     if(is_authentic(msg) == 0){
//                         printf("WARNING: This message is not authentic!\n");
//                     }
//                 }
//             }else{
//                 fprintf(stderr, "asdsadas %s\n", msg);
//                 show_message(msg, BUFFER);
//                 char* msg_invert = reply(msg);
//                 char* tmp_receiver = get_receiver(msg);
//                 send_message(tmp_receiver, msg_invert);
//             }
//         }
//     }
// }