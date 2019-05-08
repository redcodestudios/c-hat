#include <stdio.h>
#include <stdlib.h>

#include "inc/app_utils.h"

#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <string.h>


#define USERNAME_MAX_LEN 10
#define MSG_MAX_LEN 500



void test(){
    char *line[] = { "ls", "/dev/mqueue", NULL};

    execvp(line[0], line);
}



int main(){
    // char* username;
    // printf("Type your username: ");
    // username = get_str(USERNAME_MAX_LEN);
    // printf("Your username is: %s\n", username);
    // test2();
    string_list users = find_available_chats();
    // printf("%d", sizeof(users)/sizeof(*users));
    // fprintf(stderr, "%d", sizeof(users)/sizeof(char**));
    for(int i=0; i<users.length; i++){
        printf("%s\n", users.elements[i]);
    }
    // printf("%d", is_chat("chat-rodrigocam"));
    // while(1) {
        
    // }

    return 0;
}