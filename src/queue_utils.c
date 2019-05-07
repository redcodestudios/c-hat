#include <errno.h>
#include <mqueue.h>
#include <stdio.h>
#include <stdlib.h>

// Create the user queue with the desired permissions and name conventions.
mqd_t createQueue(char* userName) {
    // descritor da fila de mensagens
    mqd_t queue;
    // atributos da fila de mensagens
    struct mq_attr attr;
    // define os atributos da fila de mensagens
    // capacidade para 10 mensagens
    attr.mq_maxmsg = 10;
    // as mensagens são números inteiros
    attr.mq_msgsize = sizeof(int);  // tamanho de cada mensagem
    attr.mq_flags = 0;
    
    char* queueName = getQueueName(userName);
    if ((queue = mq_open(queueName, O_CREAT, 0622, &attr)) < 0) {
        perror("mq_open");
        exit(1);
    }
    return queue;
}

// Open the user queue in read mode.
mqd_t readQueue(char* userName){
    mqd_t queue;
    char* queueName = getQueueName(userName);
    if ((queue = mq_open(queueName, O_RDONLY)) < 0) {
        perror("mq_open");
        exit(1);
    }
    return queue;
}

// Open the user queue in write mode.
mqd_t writeQueue(char* userName){
    mqd_t queue;
    char* queueName = getQueueName(userName);
    if ((queue = mq_open(queueName, O_WRONLY)) < 0) {
        perror("mq_open");
        exit(1);
    }
    return queue;

}

// Closes an open Queue when it is no longer being used.
void closeQueue(char* userName) {
    char* queueName = getQueueName(userName);
    if (mq_close(queueName) != 0) {
        fprintf(stderr, "Queue name invalid, can't close");
        exit(1);
    }
}

// Removes the desired user queue,
void destroyQueue(char* userName) {
    char* queueName = getQueueName(userName);
    if (mq_unlink(queueName) != 0) {
        switch (errno) {
            case EACCES:
                fprintf(stderr, "No permission to unlink this message queue\n");
                break;
            case ENAMETOOLONG:
                fprintf(stderr, "Queue name too long");
                break;
            case ENOENT:
                fprintf(stderr, "No message queue with given name");
                break;
        }
        exit(1);
    }
}

// Based on the Username, returns the queue name with the preset prefix.
char* getQueueName(char* userName) {
    const char* prefix = "chat-";

    char* queueName;
    queueName = malloc(sizeof(prefix) + sizeof(userName));

    strcpy(queueName, prefix);
    strcat(queueName, userName);

    return queueName;
}

mqd_t findAvailableQueues(char* userName) {
    // TODO: Based on the user name, find all other people available
    // Queue names are 'chat-<username>' on /dev/mqueue/
    return NULL;
}