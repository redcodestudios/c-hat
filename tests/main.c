#include "chat.h"
// #include "client.h"
#include "queue_utils.h"
#include "message.h"

// #define USERNAME "dummy"

// void flood_myself(){
//     for(int i; i<10; i++){
//         send_message(USERNAME, "dummy:dummy:flooding");
//     }
// }

// int main(){
//     mqd_t inbox = create_q(USERNAME);
//     flood_myself();
// }

int main(){
    Message* m = new_message_raw("dummy:lully:oi:122");
    Message* m2 = new_message("dummy:lully:ola");

    printf("%s\n", m->sender);
    printf("%s\n", m->receiver);
    printf("%s\n", m->content);
    printf("%s\n\n", m->id);

    printf("%s\n", m2->sender);
    printf("%s\n", m2->receiver);
    printf("%s\n", m2->content);
    printf("%s\n\n", m2->id);
}