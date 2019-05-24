#ifndef APP_UTILS_H
#define APP_UTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>
#include <mqueue.h>
#include <sys/types.h>

#include "queue_utils.h"
#include "custom_types.h"

#define HEADER_LEN 5 // chat-


/* Safely get a string from the user. */
char* get_str(unsigned int buffer_size);


/* Safely get a string from the user and returns in lower case. */
char* get_strl(unsigned int buffer_size);


/* Treat a raw input like "user: hey"  */
struct Input treat_input(char* raw, const char* sender);


/* Sends a message to a user. */
int send_message(const char* sender, char* receiver, char* msg);


/* Broadcasts a message to all users. */
int broadcast_message(const char* sender, char* msg);


/* 
    Extract username from a queue name. 
    E.g. chat-user -> return user
*/
char* get_username(char* str);


/* 
    Verify if a string is in the chat id format.
    E.g. chat-user
*/
int is_chat(char* str);


int is_online(char* user, str_array_t av_users);

int is_valid_username(const char* username);

/* Find all chats that are available to sen messages. */
str_array_t find_available_chats();

void print_msg(char* msg);

#endif