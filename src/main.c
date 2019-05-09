#include <stdio.h>
#include <stdlib.h>

#include "inc/app_utils.h"
#include "inc/custom_types.h"

#define USERNAME_MAX_LEN 10
#define MSG_MAX_LEN 500


void print_chats(str_array chats){
    for(int i=0; i<chats.length; i++) {
        printf("%s\n", chats.elements[i]);
    }
}

int main(){
    const char* username;
    printf("Type your username: ");
    username = get_str(USERNAME_MAX_LEN);

    // leak de memória?
    str_array chats = find_available_chats();

    int a = send_message("rodrigo", "pablo", "ola", chats);
    printf("send msg result %d\n", a);

    printf("%s\n", get_username("chat-rodrigo").value);
    printf("%s\n", get_username("chat-pablo").value);

    return 0;
}