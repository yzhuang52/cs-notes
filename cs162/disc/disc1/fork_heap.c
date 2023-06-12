#include "stdio.h"
#include "sys/types.h"
#include "unistd.h"
#include "stdlib.h"

int main(void) {
    int* stuff = malloc(sizeof(int));
    *stuff = 5;
    pid_t fork_ret = fork();
    printf("The last digit of pi is %d\n", *stuff);
    if (fork_ret == 0) {
        *stuff = 6;
    }
    return 0;
}