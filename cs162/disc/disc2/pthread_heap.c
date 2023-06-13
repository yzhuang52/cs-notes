#include "stdio.h"
#include "pthread.h"
#include "string.h"
#include "stdlib.h"

void *helper(void* arg) {
    char* message = (char*) arg;
    strcpy(message, "I'm the helper");
    return NULL;
}

int main() {

    pthread_t thread;
    char* message = malloc(100);
    strcpy(message, "I am the main");
    pthread_create(&thread, NULL, &helper, message);
    printf("%s\n", message);
    pthread_join(thread, NULL);
    return 0;
}