#include "message.h"

#define MAX_MSG_HISTORY 5000

typedef struct User{
    char* name;
    char* queue_name;
    char* last_message_id;
}User;

User* new_user(char* username);