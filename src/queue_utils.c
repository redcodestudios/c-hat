#include "inc/queue_utils.h"

#define MSG_MAX_SIZE 522

// Create the user queue with the desired permissions and name conventions.
mqd_t create_q(char* user_name) {
    // descritor da fila de mensagens
    mqd_t queue;
    // atributos da fila de mensagens
    struct mq_attr attr;
    // define os atributos da fila de mensagens
    // capacidade para 10 mensagens
    attr.mq_maxmsg = 10;
    // as mensagens são números inteiros
    attr.mq_msgsize = MSG_MAX_SIZE;  // tamanho de cada mensagem
    attr.mq_flags = 0;
    
    // 00700 | 00020 | 00002
    char* queue_name = get_queue_name(user_name);
    if ((queue = mq_open(queue_name, O_CREAT, 00622, &attr)) < 0) {
        perror("mq_open");
        exit(1);
    }
    return queue;
}

// Open the user queue in read mode.
mqd_t read_q(char* user_name){
    mqd_t queue;
    char* queue_name = get_queue_name(user_name);
    
    if ((queue = mq_open(queue_name, O_RDONLY)) < 0) {
        perror("mq_open");
        exit(1);
    }
    return queue;
}

// Open the user queue in write mode.
mqd_t write_q(char* user_name){
    mqd_t queue;
    char* queue_name = get_queue_name(user_name);
    
    if ((queue = mq_open(queue_name, O_WRONLY)) < 0) {
        perror("mq_open");
        exit(1);
    }

    return queue;
}

// Closes an open Queue when it is no longer being used.
void close_q(mqd_t queue) {
    if (mq_close(queue) != 0) {
        fprintf(stderr, "Queue name invalid, can't close");
        exit(1);
    }
}

// Removes the desired user queue,
void destroy_q(char* user_name) {
    char* queue_name = get_queue_name(user_name);
    
    if (mq_unlink(queue_name) != 0) {
        switch (errno) {
            case EACCES:
                fprintf(stderr, "No permission to unlink this message queue\n");
                break;
            case ENAMETOOLONG:
                fprintf(stderr, "Queue name too long\n");
                break;
            case ENOENT:
                fprintf(stderr, "No message queue with given name\n");
                break;
        }
        exit(1);
    }
}

// Based on the username, returns the queue name with the preset prefix.
char* get_queue_name(char* user_name) {
    const char* prefix = "/chat-";

    char* queue_name;
    queue_name = malloc(sizeof(prefix) + sizeof(user_name));

    strcpy(queue_name, prefix);
    strcat(queue_name, user_name);

    return queue_name;
}
