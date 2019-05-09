#ifndef QUEUE_UTILS_H
#define QUEUE_UTILS_H

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <mqueue.h>
#include <dirent.h>
#include <sys/types.h>

mqd_t create_q(char* user_name);
mqd_t read_q(char* user_name);
mqd_t write_q(char* user_name);
void close_q(mqd_t queue);
void destroy_q(char* user_name);
char* get_queue_name(char* user_name);

#endif