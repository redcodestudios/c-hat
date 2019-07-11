#include "inc/client.h"

int is_online(char* username){
    char* chatname = get_queue_name(username);
    chat_array_t av_chats = get_chats();

    for(int i=0; i<av_chats.length; i++){
        if(strcmp(av_chats.elements[i].chatname, chatname) == 0){
            return 1; 
        }
    }
    return 0;
}

int send_message(User* user, Message* message){
    user->last_message_id = message->id;
    raw_send(message->to, message->repr);
}

int send_broadcast(User* user, Message* message){
    chat_array_t av_chats = get_chats();
    user->last_message_id = message->id;
    for(int i=0; i<av_chats.length; i++){
        raw_send(av_chats.elements[i].username, message->repr);
    }
}

int raw_send(char* receiver, char* message){
    if(is_online(receiver) == 1){
        mqd_t q = write_q(receiver);
        if (mq_send(q, message, MAX_MESSAGE_SIZE, 0) < 0) {
            printf("\nERRO %s", message);
            return -1;
        }
        return 0;
    }
    printf("\nUnknow user \"%s\"\n", receiver);
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
    if(is_online(username) == 1){
        printf("Invalid username\n");
        exit(0);
    }
}

int is_message(char* str){
    regex_t regex;

    if (regcomp(&regex , ".*:.*", REG_EXTENDED|REG_NOSUB) != 0) {
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

char* get_sender(char* raw){
    char* sender;
    char* tmp = malloc(strlen(raw));
    tmp = strcpy(tmp, raw);

    sender = strtok(tmp, ":");
    return sender;
}

char* get_message_id(char* raw){
    char* message_id;
    char* tmp = malloc(strlen(raw));
    tmp = strcpy(tmp, raw);

    strtok(tmp, ":");
    strtok(NULL, ":");
    message_id = strtok(NULL, ":");
    remove_bars(message_id);
    
    return message_id;
}

char* remove_bars(char* str){
    char *pr = str, *pw = str;
    while (*pr) {
        *pw = *pr++;
        pw += (*pw != '|');
    }
    *pw = '\0';
}

void show_message(Message* message){
    if(strcmp(message->to, "all") == 0){
        printf("\nBroadcast de %s: %s\n", message->from, message->content);
    }else{
        printf("\n%s: %s\n", message->from, message->content);
    }
}

int is_auth_request(char* msg){
    regex_t regex;
    if(strstr(msg, "|") != NULL){
        return 1;
    }else{
        return 0;
    }
    if (regcomp(&regex , "^.*:.*:\\|.*\\|$", REG_EXTENDED|REG_NOSUB) != 0) {
		fprintf(stderr,"erro regcomp\n");
		exit(1);
	}

    if ((regexec(&regex, msg, 0, (regmatch_t *)NULL, 0)) == 0)
		return 1;
	else
		return 0;
}

int is_authentic(char* str){
    regex_t regex;

    if (regcomp(&regex , "^.*:.*:\\|.*\\|Y\\|$", REG_EXTENDED|REG_NOSUB) != 0) {
		fprintf(stderr,"erro regcomp\n");
		exit(1);
	}

    if ((regexec(&regex, str, 0, (regmatch_t *)NULL, 0)) == 0)
		return 1;
	else
		return 0;
}

int is_auth_response(char* msg){
    regex_t regex;

    if (regcomp(&regex , "^.*:.*:\\|.*\\|(Y|N)\\|$", REG_EXTENDED|REG_NOSUB) != 0) {
		fprintf(stderr,"erro regcomp\n");
		exit(1);
	}

    if ((regexec(&regex, msg, 0, (regmatch_t *)NULL, 0)) == 0)
		return 1;
	else
		return 0;
}

void request_auth(Message* message){
    Message* inverted = invert_sender(message);
    char* auth_message = new_auth_message(inverted);
    raw_send(inverted->to, auth_message);
}

void failed_auth(char* auth_message){
    char* failed_message = malloc(strlen(auth_message) + 3);
    strcpy(failed_message, auth_message);
    strcat(failed_message, "N|\0");

    raw_send(get_receiver(auth_message), failed_message);
}

void success_auth(char* auth_message){
    char* inverted_auth_msg = raw_invert_sender(auth_message);
    char* success_message = malloc(strlen(inverted_auth_msg) + 3);
    strcpy(success_message, inverted_auth_msg);
    strcat(success_message, "Y|\0");
    
    raw_send(get_receiver(inverted_auth_msg), success_message);
}

char* raw_invert_sender(char* message){
    char* tmp = malloc(strlen(message));
    strcpy(tmp, message);
    char* receiver = strtok(tmp, ":");
    char* sender = strtok(NULL, ":");
    char* content = strtok(NULL, ":");

    char* result = malloc(strlen(message));
    strcpy(result, sender);
    strcat(result, ":");
    strcat(result, receiver);
    strcat(result, ":");
    strcat(result, content);
}