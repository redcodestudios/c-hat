#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MESSAGE_SIZE 522

typedef struct Message{
    char* id; 
    char* from;
    char* to;
    char* content;
    char* raw;
    char* repr;
}Message;

/*
Receive a string in format "from:to:content" and
create a new message, generating a random id with hash.
*/
Message* new_message(char* raw);

/*
Receive a string in format "from:to:content:id" and
create a new message.
*/
Message* new_message_with_id(char* raw);

/*
Receive a message and invert from - to .
*/
Message* invert_sender(Message* message);

/* Generate a random id with username and message hash */
int gen_message_id(char* username, char* message);

/* Generate a auth message in format from:to:|id| */
char* new_auth_message(Message* message);

/* Make a message repr in format from:to:content:id */
char* make_repr(char* sender, char* receiver, char* content, char* msg_id);