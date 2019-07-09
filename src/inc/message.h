#pragma once

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct Message{
    char* id; 
    char* sender;
    char* receiver;
    char* content;
    char* raw;
}Message;

Message* new_message(char* raw);
Message* new_message_with_id(char* raw);
Message* invert_sender(Message* message);
int gen_message_id(char* username, char* message);
char* new_auth_message(Message* message);