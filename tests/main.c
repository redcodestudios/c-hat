#include "chat.h"
#include "client.h"
#include "queue_utils.h"

#define USERNAME "dummy"

void flood_myself(){
    for(int i; i<10; i++){
        send_message(USERNAME, "dummy:dummy:flooding");
    }
}

int main(){
    mqd_t inbox = create_q(USERNAME);
    flood_myself();
}