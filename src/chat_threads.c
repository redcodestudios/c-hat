#include "inc/chat_threads.h"

void send_handler(){
    printf("sended");
}

void* listener(void *p){
}

void* sender(void *p){
    signal(SIGUSR1, send_handler);
    while(1){
        sleep(1);
    }
}