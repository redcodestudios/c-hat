#include <stdlib.h>
#include <stdio.h>
#include "inc/app_utils.h"


char* get_str(unsigned int buffer_size){
    char* buffer = malloc(sizeof(char)*buffer_size);
    char tmp;

    for(int i=0; i < buffer_size; i++){
        scanf("%c", &tmp);
        buffer[i] = tmp;        
        
        if(tmp == '\n') {
            break;
        }
    }

    return buffer;
}

/*
    Sends a message to a user.
*/
int send_message(char* sender, char* receiver, char* msg){
    return 0;
}

/*
    Broadcasts a message to all users.
*/
int broadcast_message(char* sender, char* msg){
    return 0;
}
