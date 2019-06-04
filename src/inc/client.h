#pragma once

// #include <string.h>
#include <regex.h>

#include "chat.h"
#include "queue_utils.h"

#define MAX_MESSAGE_SIZE 522

int send_message(char* receiver, char* raw_msg);
int broadcast_message(char* raw_msg);

void show_message(char* msg);

int is_online(char* chatname);
void validate_username(char* username);
int is_message(char* msg);
int is_broadcast(char* msg);
char* get_receiver(char* raw);