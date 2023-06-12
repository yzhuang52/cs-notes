#include "stdio.h"
#include "sys/types.h"
#include "unistd.h"
#include "sys/wait.h"

int main(void) {
    pid_t fork_ret = fork();
    int exit;
    if (fork_ret != 0) {
        wait(&exit);
    }
    printf("Hello World %d\n", fork_ret);
    return 0;
}