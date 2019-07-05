#include "include/message.h"


Message* new_message(char *username, char *raw){
    Message* msg = (Message*) malloc(sizeof(Message));
    char* tmp = raw;

    msg-> id = gen_message_id(username);

    msg->sender = strtok(tmp, ":");
    msg->receiver = strtok(NULL, ":");
    msg->content = strtok(NULL, ":");

    return msg;
}

int gen_message_id(char username[10]){
    int hash = 0;
    int salt = rand() % 100 + 1;

    for(int i=0; i<10; i++){
        hash += username[i] - '0';
    }

    return (hash + salt);
}
