#include <stdio.h>
#include <stdlib.h>
#include "inc/utils.h"

#define USERNAME_MAX_LEN 10
#define MSG_MAX_LEN 500

int main(){
    char* username;
    printf("Type your username: ");
    username = get_str(USERNAME_MAX_LEN);
    printf("Your username is: %s\n", username);
    
    // while(1) {
        
    // }

    return 0;
}