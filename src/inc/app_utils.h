#ifndef APP_UTILS_H
#define APP_UTILS_H

#include <dirent.h>
#include <mqueue.h>
#include <sys/types.h>

#include "queue_utils.h"
#include "custom_types.h"

#define HEADER_LEN 5 // chat-


/* Safely get a string from the user. */
char* get_str(unsigned int buffer_size);


/* Sends a message to a user. */
int send_message(char* sender, char* receiver, char* msg, str_array av_chats);


/* Broadcasts a message to all users. */
int broadcast_message(char* sender, char* msg);


/* 
    Extract username from a queue name. 
    E.g. chat-user -> return user
*/
username_t get_username(char* str);


/* 
    Verify if a string is in the chat id format.
    E.g. chat-user
*/
int is_chat(char* str);


int is_online(char* user, str_array av_users);


/* Find all chats that are available to sen messages. */
str_array find_available_chats();

#endif