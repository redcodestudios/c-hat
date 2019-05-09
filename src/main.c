#include <stdio.h>
#include <stdlib.h>

#include "inc/app_utils.h"
#include "inc/custom_types.h"
#include "inc/queue_utils.h"

#define USERNAME_MAX_LEN 10
#define MSG_MAX_LEN 500


int main(){
    const char* username;
    printf("Type your username: ");
    username = get_str(USERNAME_MAX_LEN);

    create_q("pablo");
    str_array_t chats = find_available_chats();

    int a = send_message("rodrigo", "pablo", "ola", chats);
    printf("send msg result %d\n", a);

    // printf("%s\n", get_username("chat-hugo"));
    // printf("%s\n", get_username("chat-pablo"));
    // destroy_q("pablo");
    return 0;
}