#include <pthread.h>
#include <signal.h>

void send_handler();

void* listener(void *p);
void* sender(void *p);