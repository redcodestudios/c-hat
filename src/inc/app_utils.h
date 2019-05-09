#include <dirent.h>
#include <sys/types.h>

#include "custom_types.h"

#define HEADER_LEN 5 // chat-


/* Safely get a string from the user. */
char* get_str(unsigned int buffer_size);


/* Sends a message to a user. */
int send_message(char* sender, char* receiver, char* msg);


/* Broadcasts a message to all users. */
int broadcast_message(char* sender, char* msg);


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

/* Find all chats that are available to sen messages. */
str_array find_available_chats();