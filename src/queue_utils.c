#include "inc/queue_utils.h"

// Create the user queue with the desired permissions and name conventions.
mqd_t create_q(char* user_name) {
    mqd_t queue;
    // atributos da fila de mensagens
    struct mq_attr attr;
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = MSG_MAX_SIZE;
    attr.mq_flags = 0;
    
    mode_t prev_umask = umask(0000);
    char* queue_name = get_queue_name(user_name);
    
    if ((queue = mq_open(queue_name, O_RDWR|O_CREAT, 00622, &attr)) < 0) {
        perror("mq_open");
        exit(1);
    }
    umask(prev_umask);
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
        perror("mq_unlink");
        exit(1);
    }
}

// Based on the username, returns the queue name with the preset prefix.
char* get_queue_name(char* user_name) {
    const char prefix[] = "/chat-";
    char* queue_name = (char*) calloc(6 + 64, sizeof(char));
    strcpy(queue_name, prefix);
    strcat(queue_name, user_name);

    return queue_name;
}
