#include "inc/message.h"

Message* new_message(char* raw){
    char* tmp = malloc(strlen(raw));

    Message* new_msg = malloc(sizeof(Message));
    tmp = strcpy(tmp, raw);

    char* sender = strtok(tmp, ":");
    char* msg_id = malloc(32);
    sprintf(msg_id, "%d", gen_message_id(sender, raw));

    char* receiver = strtok(NULL, ":");
    char* content = strtok(NULL, ":");
    char* msg_repr = make_repr(sender, receiver, content, msg_id);

    new_msg->id = msg_id;
    new_msg->from = sender;
    new_msg->to = receiver;
    new_msg->content = content;
    new_msg->raw = raw;
    new_msg->repr = msg_repr;

    return new_msg;
}

Message* new_message_with_id(char* raw){
    char* tmp = malloc(strlen(raw));

    Message* new_msg = malloc(sizeof(Message));
    tmp = strcpy(tmp, raw);
    
    char* sender = strtok(tmp, ":");
    char* receiver = strtok(NULL, ":");
    char* content = strtok(NULL, ":");
    char* msg_id = strtok(NULL, ":");

    new_msg->id = msg_id;
    new_msg->from = sender;
    new_msg->to = receiver;
    new_msg->content = content;
    new_msg->raw = raw;
    new_msg->repr = make_repr(sender, receiver, content, msg_id);

    return new_msg;
}

Message* invert_sender(Message* message){
    Message* inverted = malloc(sizeof(Message));
    inverted->id = message->id;
    inverted->from = message->to;
    inverted->to = message->from;
    inverted->content = message->content;
    inverted->raw = message->raw;

    return inverted;
}

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

char* new_auth_message(Message* message){ 
    char* raw_message = malloc(100);

    strcpy(raw_message, message->from);

    strcat(raw_message, ":");
    strcat(raw_message, message->to);
    strcat(raw_message, ":|");
    strcat(raw_message, message->id);
    strcat(raw_message, "|");

    return raw_message;
}

char* make_repr(char* sender, char* receiver, char* content, char* msg_id){
    int separator_len = 3;
    char* repr = malloc(
        strlen(sender) +
        strlen(receiver) +
        strlen(content) +
        strlen(msg_id) +
        separator_len + 1
    );

    strcpy(repr, sender);
    strcat(repr, ":");
    strcat(repr, receiver);
    strcat(repr, ":");
    strcat(repr, content);
    strcat(repr, ":");
    strcat(repr, msg_id);

    repr[strlen(repr)] = '\0';

    return repr;
}