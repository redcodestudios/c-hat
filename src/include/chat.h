#pragma once

#include <string.h>
#include <dirent.h>
#include <regex.h>

#include "queue_utils.h"

#define MAX_USERNAME_LEN 10
#define MAX_CHATNAME_LEN MAX_USERNAME_LEN + 6
#define MAX_CHAT_ARRAY_ELEMENTS 200

typedef struct Chat{
    char username[MAX_USERNAME_LEN];
    char chatname[MAX_CHATNAME_LEN];
    mqd_t queue;
}Chat;

typedef struct chat_array_t{
    Chat elements[MAX_CHAT_ARRAY_ELEMENTS];
    int length;
}chat_array_t;

Chat new_chat_inbox(char username[10], mqd_t inbox);
Chat new_chat(char username[10]);

int add_chat(chat_array_t* chat_array, Chat chat);
int is_chat(char* str);

chat_array_t new_chat_array();
chat_array_t get_chats();