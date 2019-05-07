#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mqueue.h>

mqd_t createQueue(char* userName);
mqd_t readQueue(char* userName);
mqd_t writeQueue(char* userName);
void closeQueue(mqd_t queue);
void destroyQueue(char* userName);
char* getQueueName(char* userName);
mqd_t findAvailableQueues(char* userName);