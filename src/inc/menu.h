#include <stdio.h>

#define SEND_MESSAGE 1
#define LIST_USERS 2
#define BROADCAST 3
#define EXIT 4

int menu(){
    printf("\n==================== Menu ====================\n");
    printf("1- Send message\n");
    printf("2- List online users\n");
    printf("3- Broadcast message\n");
    printf("4- Exit\n");

    int answer;
    scanf("%d", &answer);
    return answer;
}
