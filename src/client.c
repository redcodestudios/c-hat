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

void show_message(char* msg, char* BUFFER){

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
        printf("\nBroadcast de %s: %s\n", sender, msg_buff);
    }else{
        printf("\n%s: %s\n", sender, msg_buff);
    }
    printf("%s", BUFFER);
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

int gen_message_id(char username[10]){
    int hash = 0;
    int salt = rand() % 100 + 1;

    for(int i=0; i<10; i++){
        hash += username[i] - '0';
    }

    return (hash + salt);
}

int is_auth_request(char* msg){
    //regex_t regex;
    if(strstr(msg, "|") != NULL){
        return 1;
    }else{
        return 0;
    }
    //if (regcomp(&regex , "/^.*:.*:\|.*\|$/g", REG_EXTENDED|REG_NOSUB) != 0) {
	//	fprintf(stderr,"erro regcomp\n");
	//	exit(1);
	//}

    //if ((regexec(&regex, msg, 0, (regmatch_t *)NULL, 0)) == 0)
	//	return 1;
	//else
//		return 0;
}

char* reply(char* msg){
    char* msg_tmp = malloc(strlen(msg));
    char* usr1_tmp = malloc(10);
    char* usr2_tmp = malloc(10);
    char* msg_content = malloc(20);

    strcpy(msg_tmp, msg);
    usr1_tmp = strtok(msg_tmp, ":");
    usr2_tmp = strtok(NULL, ":");
    msg_content = strtok(NULL, ":");

    char* msg_reply = malloc(strlen(msg));
    strcat(msg_reply, usr2_tmp);
    strcat(msg_reply, ":");
    strcat(msg_reply, usr1_tmp);
    strcat(msg_reply, ":");
    strcat(msg_reply, msg_content);

    return msg_reply;
}

int is_authentic(char* str){
    regex_t regex;

    if (regcomp(&regex , "^.*:.*:\|.*\|Y\|$", 0) != 0) {
		fprintf(stderr,"erro regcomp\n");
		exit(1);
	}

    if ((regexec(&regex, str, 0, (regmatch_t *)NULL, 0)) == 0)
		return 1;
	else
		return 0;
}