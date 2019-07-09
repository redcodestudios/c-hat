#include "inc/message.h"

int gen_message_id(char* username, char* message){
    srand(time(NULL));
    int hash = 0;
    int salt = rand() % 10 + 1;

    for(int i=0; i<strlen(username); i++){
        hash += username[i] - '0';
    }

    for(int i=0; i<strlen(message); i++){
        hash += message[i] - '0';
    }

    return (hash + salt);
}

Message* new_message(char* raw){
    char* tmp = malloc(strlen(raw));
    char* sender = malloc(10);
    char* receiver = malloc(10);
    char* content = malloc(500);
    char* msg_id = malloc(32);

    Message* new_msg = malloc(sizeof(Message));
    tmp = strcpy(tmp, raw);
    
    sender = strtok(tmp, ":");
    receiver = strtok(NULL, ":");
    content = strtok(NULL, ":");

    int int_id = gen_message_id(sender, content);
    sprintf(msg_id, "%d", int_id);

    new_msg->id = msg_id;
    new_msg->sender = sender;
    new_msg->receiver = receiver;
    new_msg->content = content;
    new_msg->raw = raw;

    return new_msg;
}

Message* new_message_with_id(char* raw){
    char* tmp = malloc(strlen(raw));
    char* sender = malloc(10);
    char* receiver = malloc(10);
    char* content = malloc(500);
    char* msg_id = malloc(32);
    char* raw_msg_id = malloc(32);

    Message* new_msg = malloc(sizeof(Message));
    tmp = strcpy(tmp, raw);
    
    sender = strtok(tmp, ":");
    receiver = strtok(NULL, ":");
    content = strtok(NULL, ":");
    raw_msg_id = strtok(NULL, ":");
    printf("raw msg %s\n", raw);
    for(int i=0; i<strlen(raw_msg_id); i++){
        if(raw_msg_id[i] != '|'){
            msg_id += raw_msg_id[i];
        }
    }
    msg_id[strlen(raw_msg_id)] = '\0';

    new_msg->id = msg_id;
    new_msg->sender = sender;
    new_msg->receiver = receiver;
    new_msg->content = content;
    new_msg->raw = raw;

    return new_msg;
}

Message* invert_sender(Message* message){
    Message* inverted = malloc(sizeof(Message));
    inverted->id = message->id;
    inverted->sender = message->receiver;
    inverted->receiver = message->sender;
    inverted->content = message->content;
    inverted->raw = message->raw;

    return inverted;
}

char* new_auth_message(Message* message){ 
    char* raw_message = malloc(100);

    strcpy(raw_message, message->sender);

    strcat(raw_message, ":");
    strcat(raw_message, message->receiver);
    strcat(raw_message, ":|");
    strcat(raw_message, message->id);
    strcat(raw_message, "|");

    printf("new auth message %s\n", raw_message);
    return raw_message;
}