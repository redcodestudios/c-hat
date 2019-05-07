// Compile com: gcc tickets2.c -o prog.o -lrt -pthread

#include <mqueue.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/stat.h>

#define GETQUEUE "/get_queue"
#define RETURNQUEUE "/return_queue"
#define MAX_TICKETS 100

int TICKET_VEC[MAX_TICKETS];
int N_TICKETS = 0;

sem_t sem;

int get_ticket() {
    sem_wait(&sem);
    if (N_TICKETS == MAX_TICKETS) {
        return -1;
    }
    sem_post(&sem);

    sem_wait(&sem);
    for (int i = 0; i < MAX_TICKETS; i++) {
        if (TICKET_VEC[i] == 0) {
            TICKET_VEC[i] = 1;
            N_TICKETS += 1;
            sem_post(&sem);
            return i;
        }
    }
}

void return_ticket(int n) {
    sem_wait(&sem);
    N_TICKETS -= 1;
    TICKET_VEC[n] = 0;
    sem_post(&sem);
}

void *manageGetTickets(void *pqueue) {
   
    mqd_t *queue = (mqd_t *)pqueue;
    for (;;) {
        int msg = get_ticket();  
        if (mq_send(*queue, (void *)&msg, sizeof(msg), 0) < 0) {
            perror("mq_send");
            exit(1);
        }
        printf("Server created ticket: %d\n", msg);
    }
    pthread_exit(NULL);
}

void *manageReturnTickets(void *pqueue) {
    int msg;
    mqd_t queue = *(mqd_t *)pqueue;
    for (;;) {
        if ((mq_receive(queue, (void *)&msg, sizeof(msg), 0)) < 0) {
            perror("mq_receive:");
            pthread_exit(NULL);
            exit(1);
        }
        printf("Server restored ticket : %d\n", msg);
    }
    pthread_exit(NULL);
}

void rodaServidor(mqd_t getQueue, mqd_t returnQueue) {
    sem_init(&sem, 0, 1);

    pthread_t getManager, returnManager;
    pthread_create(&getManager, NULL, manageGetTickets, &getQueue);
    pthread_create(&returnManager, NULL, manageReturnTickets, &returnQueue);

    pthread_join(getManager, NULL);
    pthread_join(returnManager, NULL);

    sem_destroy(&sem);
}

int clientGetTicket(mqd_t queue) {
    int ticket;
    if ((mq_receive(queue, (void *)&ticket, sizeof(ticket), 0)) < 0) {
        perror("mq_receive:");
        exit(1);
    }
    return ticket;
}

void clientReturnTicket(mqd_t queue, int ticket) {
    int msg = ticket;  
    
    if (mq_send(queue, (void *)&msg, sizeof(msg), 0) < 0) {
        perror("mq_send");
        exit(1);
    }
    printf("Client %d returned ticket %d\n", getpid(), msg);
    sleep(1);
}

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

void rodaCliente(mqd_t getQueue, mqd_t returnQueue) {
    for (int i = 0; i < 10; i++) {
        int ticket = clientGetTicket(getQueue);
        printf("Client %d got ticket: %d\n", getpid(), ticket);
        sleep(4);
        clientReturnTicket(returnQueue, ticket);
    }
}

int main(int argc, char *argv[]) {
    mqd_t getQueue = createQueue(GETQUEUE);
    mqd_t returnQueue = createQueue(RETURNQUEUE);

    if (fork() > 0) {
        if (fork() > 0) {
            if (fork() > 0) {
                if (fork() > 0) {
                    if (fork() > 0) {
                        rodaServidor(getQueue, returnQueue);
                        mq_close(getQueue);
                        mq_close(returnQueue);
                    } else
                        rodaCliente(getQueue, returnQueue);
                } else
                    rodaCliente(getQueue, returnQueue);
            } else
                rodaCliente(getQueue, returnQueue);
        } else
            rodaCliente(getQueue, returnQueue);
    } else
        rodaCliente(getQueue, returnQueue);
}