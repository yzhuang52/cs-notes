#include "stdio.h"
#include "pthread.h"

void *helper(void* arg) {
    int *num = (int*) arg;
    *num = 2;
    return NULL;
}

int main() {
    pthread_t thread;
    int i = 0;
    pthread_create(&thread, NULL, &helper, &i);
    pthread_join(thread, NULL);
    printf("i = %d\n", i);
    return 0;
}