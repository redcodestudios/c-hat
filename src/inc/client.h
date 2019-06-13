#pragma once

// #include <string.h>
#include <regex.h>

#include "chat.h"
#include "queue_utils.h"

#define MAX_MESSAGE_SIZE 522

typedef struct queued_msg_t{
    char receiver[MAX_USERNAME_LEN];
    char raw_msg[MAX_MESSAGE_SIZE];
    int send_attempts;
}queued_msg_t;

typedef struct resend_queue_t{
    queued_msg_t elements[MAX_CHAT_ARRAY_ELEMENTS];
    int size;
}resend_queue_t;

resend_queue_t* new_resend_queue();

void add_to_queue(resend_queue_t* q, queued_msg_t msg);

int send_message(char* receiver, char* raw_msg);
int broadcast_message(char* raw_msg);

void show_message(char* msg, char* BUFFER);

int is_online(char* chatname);
void validate_username(char* username);
int is_message(char* msg);
int is_broadcast(char* msg);
char* get_receiver(char* raw);