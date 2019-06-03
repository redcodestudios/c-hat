#include "inc/app_utils.h"


char* get_str(unsigned int buffer_size){
    char* buffer = calloc(buffer_size, sizeof(char));
    char tmp;

    for(int i=0; i < buffer_size; i++){
        scanf("%c", &tmp);
        if(tmp == '\n') {
            buffer[i] = '\0';
            break;
        }
        buffer[i] = tmp;        
    }

    return buffer;
}


char* get_strl(unsigned int buffer_size){
    char* tmp = get_str(buffer_size);
    
    for(int i = 0; tmp[i]; i++){
        tmp[i] = tolower(tmp[i]);
    }
    return tmp;
}

struct Input treat_input(char* raw, const char* sender){
    char* receiver;
    char* msg;
    char* tmp = malloc(strlen(raw));
    tmp = strcpy(tmp, raw);

    receiver = strtok(tmp, ":");
    receiver = strtok(NULL, ":");
    msg = strtok(NULL, ": ");

    struct Input i = {sender, receiver, msg, raw};
    return i;
}

// int send_message(const char* sender, char* receiver, char* msg){
//     fprintf(stderr, "xush\n");
//     str_array_t av_chats = find_available_chats();
//     fprintf(stderr, "xish\n");
//     if(is_online(receiver, av_chats)){
//         fprintf(stderr, "xish %s\n", receiver);
//         mqd_t q = write_q(receiver);

//         if (mq_send(q, msg, strlen(msg), 0) < 0) {
//             perror("\nError sending message\n");
//             return -1;
//         }
//         return 0;
//     }
//     printf("\nUNKNOWNUSER %s\n", receiver);
//     return -1;
// }


int broadcast_message(const char* sender, char* msg){
    str_array_t av_chats = find_available_chats();
    for(int i=0; i<av_chats.length; i++){
        fprintf(stderr, "lalala %s\n", av_chats.elements[i]);
    }
    // char* msg = 
    for(int i=0; i<av_chats.length; i++){
        fprintf(stderr, "av %s\n", av_chats.elements[i]);
        send_message(sender, get_username(av_chats.elements[i]), msg);
    }
    return 0;
}


char* get_username(char* str) {
    int length = strlen(str) - HEADER_LEN;
    fprintf(stderr, "HOW\n");
    char* tmp = malloc(length);
    fprintf(stderr, "HEY\n");    
    
    for(int i=5,j=0; i<strlen(str); i++, j++){
        tmp[j] = str[i];
    }
    tmp[length-1] = '\0';
    fprintf(stderr, "username %s\n", tmp);
    return tmp;
}


// int is_chat(char* str){
//     if(strlen(str) > 15 || strlen(str) < 5){
//         return 0;
//     }

//     char tmp[5];

//     for(int i=0; i<5; i++) {
//         tmp[i] = str[i];
//     }

//     return strcmp(tmp, "chat-\0") == 0;
// }


// int is_online(char* user, str_array_t av_chats){
//     for(int i=0; i<av_chats.length; i++){
//         fprintf(stderr, "yay\n");
//         char* tmp =  get_username(av_chats.elements[i]);
//         fprintf(stderr, "yay2\n");
//         if(strcmp(user, tmp) == 0){
//             return 1;
//         }
//     }
//     return 0;
// }

int is_valid_username(const char* username){
    str_array_t av_chats = find_available_chats();
    
    if(strcmp(username, "all") == 0){
        return 0;
    }else if(is_online(username, av_chats)){
        return 0;
    }

    return 1;
}


str_array_t find_available_chats() {
    struct dirent *entry;
    DIR *dir = opendir("/dev/mqueue/");
    str_array_t chats = new_str_array();

    if(dir == NULL) {
        return chats;
    }

    int number_of_chats = 0;

    while((entry = readdir(dir)) != NULL) {
        if(is_chat(entry->d_name)){
            add_str(number_of_chats, entry->d_name, &chats);
            number_of_chats++;
        }
    }
    closedir(dir);
    return chats;
}


void print_msg(char* msg){
    char* sender;    
    char* receiver;
    char* new_msg;
    char* msg_buff = malloc(MSG_MAX_SIZE);

    sender = strtok(msg, ":");
    receiver = strtok(NULL, ":");
    
    new_msg = strtok(NULL, ":");
    
    while (new_msg != NULL){
        strcat(msg_buff, new_msg);        
        new_msg = strtok(NULL, ":");
    }
    printf("%s: %s\n", sender, msg_buff);
}