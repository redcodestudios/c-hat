#pragma once

// #include <string.h>
#include <regex.h>
#include <time.h>

#include "user.h"
#include "chat.h"
#include "queue_utils.h"
#include "message.h"

int raw_send(char* receiver, char* message);
int send_broadcast(User* user, Message* message);
int send_message(User* user, Message* message);
void request_auth(Message* message);

void show_message(Message* message);
void show_channel_message(Message* message);

int is_online(char* chatname);
void validate_username(char* username);
int is_message(char* msg);
int is_broadcast(char* msg);

char* get_receiver(char* raw);
char* get_sender(char* raw);
char* get_message_id(char* raw);

int is_auth_request(char* msg);
int is_auth_response(char* msg);
int is_authentic(char* str);

void failed_auth(char* auth_message);
void success_auth(char* auth_message);

char* remove_bars(char* str);

char* raw_invert_sender(char* message);

int is_channel_create(char* message);
int is_channel_msg(Message* message);