#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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

int send_message(char* sender, char* receiver, char* msg){
    return 0;
}

int broadcast_message(char* sender, char* msg){
    return 0;
}

char* get_available_receivers(){
    return "c";
}

char* get_username(char* str) {
    char* tmp = malloc(sizeof(char) * strlen(str) - HEADER_LEN);
    
    for(int i=5, j=0; i<strlen(str); i++, j++){
        tmp[j] = str[i];
    }

    return tmp;
}

int is_chat(char* str){
    if(strlen(str) > 15){
        return 0;
    }

    if(strlen(str) < 5){
        return 0;
    }
    char tmp[5];

    for(int i=0; i<5; i++) {
        tmp[i] = str[i];
    }

    return strcmp(tmp, "chat-\0") == 0;
}


string_list find_available_chats() {
    struct dirent *entry;
    DIR *dir = opendir("/dev/mqueue/");
    string_list chats; 
    chats.elements = malloc(sizeof(char**)); 

    // fprintf(stderr, "aaaa\n");
    if(dir == NULL) {
        return chats;
    }

    int count = 0;
    // fprintf(stderr, "b\n");

    while((entry = readdir(dir)) != NULL) {
        // fprintf(stderr, "c\n");
        if(is_chat(entry->d_name)){
            // fprintf(stderr, "%s\n", entry->d_name);
            count++;
            chats.elements = realloc(chats.elements, sizeof(char**) * count);
            chats.elements[count-1] = entry->d_name;
            chats.length = count;
            // fprintf(stderr, "%s\n", chats.elements[count-1]);
        }
    }
    closedir(dir);
    // fprintf(stderr, "d\n");
    return chats;
}
