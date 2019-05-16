#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

#include "inc/menu.h"
#include "inc/app_utils.h"
#include "inc/custom_types.h"
#include "inc/queue_utils.h"
#include "inc/chat_threads.h"

#define USERNAME_MAX_LEN 10
#define MSG_MAX_LEN 500

void exit_handler(int signum);

int main(){
    signal(SIGINT, exit_handler);
    
    const char* username;
    printf("Type your username: ");
    username = get_str(USERNAME_MAX_LEN);

    pthread_t sender_tid;
    pthread_t listener_tid;

    pthread_create(&sender_tid, NULL, &sender, NULL);
    pthread_create(&listener_tid, NULL, &listener, NULL);
    
    while(1){
        int option  = menu();
        switch (option){
            case SEND_MESSAGE:
                pthread_kill(sender_tid, SIGUSR1);
                break;
            case LIST_USERS:
                break;
            case BROADCAST:
                pthread_kill(sender_tid, SIGUSR2);
                break;
            case EXIT:
                return 0;
        }
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
