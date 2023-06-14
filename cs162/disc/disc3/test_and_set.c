#include "stdio.h"
#include "pthread.h"

int value = 0;
int hello = 0;

int test_and_set(int* value) {
    int old_value = *value;
    *value = 1;
    return old_value;
}

void print_hello() {
    while (test_and_set(&value));
    hello += 1;
    printf("Child thread: %d\n", hello);
    value = 0;
    pthread_exit(0);
}

void main() {
    pthread_t thread1;
    pthread_t thread2;
    pthread_create(&thread1, NULL, &print_hello, NULL);
    pthread_create(&thread1, NULL, &print_hello, NULL);
    while (test_and_set(&value));
    printf("Parent thread: %d\n", hello);
    value = 0;
}