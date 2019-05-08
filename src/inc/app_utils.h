#include <dirent.h>
#include <sys/types.h>

#define HEADER_LEN 5 // chat-


typedef struct{
    char** elements;
    int length;
}string_list;


/*
    Safely get a string from the user.
*/
char* get_str(unsigned int buffer_size);


/*
    Sends a message to a user.
*/
int send_message(char* sender, char* receiver, char* msg);


/*
    Broadcasts a message to all users.
*/
int broadcast_message(char* sender, char* msg);


/*
    Extract username from a queue name.
    E.g. chat-user -> return user
*/
char* get_username(char* str);

int is_chat(char* str);

string_list find_available_users();