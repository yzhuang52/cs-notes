#include "stdio.h"
#include "pthread.h"

int hello = 0;
pthread_mutex_t lock;
pthread_cond_t cv;
void print_hello() {
    hello += 1;
    printf("First line (hello=%d)\n", hello);
    pthread_cond_signal(&cv);
    pthread_exit(0);
}

void main() {
    pthread_t thread;
    pthread_create(&thread, NULL, (void *)print_hello, NULL);
    while (hello < 1)
        pthread_cond_wait(&cv, &lock);
    printf("Second line (hello=%d)\n", hello);
}