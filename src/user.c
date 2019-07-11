#include "inc/user.h"
#include "inc/queue_utils.h"

User* new_user(char* username){
    User* user = malloc(sizeof(User));
    user->name = username;
    user->queue_name = get_queue_name(username);
    user->last_message_id = malloc(32);
    user->last_message_id = NULL;

    return user;
}