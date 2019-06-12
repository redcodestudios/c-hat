#include "inc/client.h"

int is_online(char* chatname){
    chat_array_t av_chats = get_chats();

    for(int i=0; i<av_chats.length; i++){
        if(strcmp(av_chats.elements[i].chatname, chatname) == 0){
            return 1; 
        }
    }
    return 0;
}

int send_message(char* username, char* raw_msg){
    char *tmp = (char*) calloc(MAX_USERNAME_LEN, sizeof(char));
    strcpy(tmp, username);
    if(is_online(get_queue_name(tmp)) == 1){
        mqd_t q = write_q(tmp);
        if (mq_send(q, raw_msg, strlen(raw_msg), 0) < 0) {
            printf("\nERRO %s", raw_msg);
            return -1;
        }
        return 0;
    }
    printf("\n Unknow user \"%s\"\n", username);
    return -1;
}

void validate_username(char* username){
    if(strlen(username) > 10){
        printf("Invalid username\n");
        exit(0);
    }
    if(strcmp(username, "all") == 0){
        printf("Invalid username\n");
        exit(0);
    }
    if(is_online(get_queue_name(username)) == 1){
        printf("Invalid username\n");
        exit(0);
    }
}

int is_message(char* str){
    regex_t regex;

    if (regcomp(&regex , ".*:.*:.*", REG_EXTENDED|REG_NOSUB) != 0) {
		fprintf(stderr,"erro regcomp\n");
		exit(1);
	}

    if ((regexec(&regex, str, 0, (regmatch_t *)NULL, 0)) == 0)
		return 1;
	else
		return 0;
}

int is_broadcast(char* str){
    regex_t regex;

    if (regcomp(&regex , ".*:all:.*", REG_EXTENDED|REG_NOSUB) != 0) {
		fprintf(stderr,"erro regcomp\n");
		exit(1);
	}

    if ((regexec(&regex, str, 0, (regmatch_t *)NULL, 0)) == 0)
		return 1;
	else
		return 0;
}

char* get_receiver(char* raw){
    char* receiver;
    char* tmp = malloc(strlen(raw));
    tmp = strcpy(tmp, raw);

    receiver = strtok(tmp, ":");
    receiver = strtok(NULL, ":");
    return receiver;
}

void show_message(char* msg){

    char* sender;    
    char* receiver;
    char* msg_content = (char*) calloc(MAX_MESSAGE_SIZE, sizeof(char));
    char* msg_buff = (char*) calloc(MAX_MESSAGE_SIZE, sizeof(char));
    
    sender = strtok(msg, ":");
    receiver = strtok(NULL, ":");
    
    msg_content = strtok(NULL, ":");

    while (msg_content != NULL){
        strcat(msg_buff, msg_content);        
        msg_content = strtok(NULL, ":");
    }
    
    msg_content = (char*) calloc(MAX_MESSAGE_SIZE, sizeof(char));
    
    if(strcmp(receiver, "all") == 0){
        printf("Broadcast de %s: %s\n", sender, msg_buff);
    }else{
        printf("%s: %s\n", sender, msg_buff);
    }
}

resend_queue_t* new_resend_queue(){
    resend_queue_t* new_q = (resend_queue_t*) malloc(sizeof(resend_queue_t));
    new_q->size = 0;

    return new_q;
}

void add_to_queue(resend_queue_t* q, queued_msg_t msg){
    q->elements[q->size] = msg;
    q->size++;
}