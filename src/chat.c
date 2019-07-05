#include "include/chat.h"

Chat new_chat(char username[10]){
    char* chatname = get_queue_name(username);
    Chat new_chat;

    strcpy(new_chat.username, username);
    strcpy(new_chat.chatname, chatname);
    
    return new_chat;
}

Chat new_chat_inbox(char username[10], mqd_t inbox){
    char* chatname = get_queue_name(username);
    Chat new_chat;

    strcpy(new_chat.username, username);
    strcpy(new_chat.chatname, chatname);
    new_chat.queue = inbox;

    if(inbox == NULL){
        create_q(username);
        new_chat.queue = read_q(new_chat.username);
    }

    return new_chat;
}


chat_array_t new_chat_array(){
    chat_array_t new_array;
    new_array.length = 0;

    return new_array;
}


/* Add a chat to a chat array, return the new length */
int add_chat(chat_array_t* chat_array, Chat chat){
    chat_array->elements[chat_array->length] = chat;
    chat_array->length += 1;

    return chat_array->length;
}

int is_chat(char* str){
    regex_t regex;

    if (regcomp(&regex , "chat-.", REG_EXTENDED|REG_NOSUB) != 0) {
		fprintf(stderr,"erro regcomp\n");
		exit(1);
	}

    if ((regexec(&regex, str, 0, (regmatch_t *)NULL, 0)) == 0)
		return 1;
	else
		return 0;
}

chat_array_t get_chats(){
    struct dirent *entry;
    DIR *dir = opendir("/dev/mqueue/");
    chat_array_t all_chats = new_chat_array();

    if(dir == NULL) {
        return all_chats;
    }

    int number_of_chats = 0;

    while((entry = readdir(dir)) != NULL) {
        if(is_chat(entry->d_name) == 1){
            char tmp[256];
            strcpy(tmp, entry->d_name);
            strtok(tmp, "-");
            char* username = strtok(NULL, "-");
            Chat chat = new_chat(username);
            add_chat(&all_chats, chat);
       }
    }
    closedir(dir);
    return all_chats;
}
