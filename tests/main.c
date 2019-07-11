#include "chat.h"
#include "client.h"
#include "queue_utils.h"
#include "message.h"
#include <regex.h>

#include <string.h>

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
    Message* m = new_message("dummy:lully:ola todo mundo");
    Message* m2 = new_message_with_id("dummy:lully:oi galera:122");

    printf("sender: %s\n", m->from);
    printf("receiver: %s\n", m->to);
    printf("content: %s\n", m->content);
    printf("id: %s\n", m->id);
    printf("raw: %s\n", m->raw);
    printf("repr: %s\n", m->repr);
    printf("auth message: %s\n\n", new_auth_message(invert_sender(m)));

    printf("%s\n", m2->from);
    printf("%s\n", m2->to);
    printf("%s\n", m2->content);
    printf("%s\n", m2->id);
    printf("%s\n", m2->raw);
    printf("%s\n", m2->repr);

    printf("auth message %s\n\n", new_auth_message(invert_sender(m2)));

    

   printf("msg_id: %s\n", get_message_id("dummy:lully:|122|"));
   printf("%d\n", strcmp("122", get_message_id("dummy:lully:|122|")));
   int a  = is_false("dummy:lully:|122|N|");
   printf("l ulasdasda %d\n", a);
}