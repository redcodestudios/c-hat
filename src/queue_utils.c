#include <mqueue.h>

mqd_t createQueue(char *queueName) {
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

    // abre ou cria a fila com permissoes 0666
    if ((queue = mq_open(queueName, O_RDWR | O_CREAT, 0666, &attr)) < 0) {
        perror("mq_open");
        exit(1);
    }
    return queue;
}


mqd_t findAvailableQueues(char* userName){
    //TODO: Based on the user name, find all other people available
    // Queue names are 'chat-<username>' on /dev/mqueue/
}