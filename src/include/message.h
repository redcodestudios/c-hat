#pragma once

#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MSG_MAX_SIZE 500

typedef struct Message{
    int id;
    const char* sender;
    const char* receiver;
    const char* content;
    const char* raw;
}Message;

int gen_message_id(char username[10]);
int is_authentic(Message msg, char* username);
Message* new_message(char *username, char *raw);