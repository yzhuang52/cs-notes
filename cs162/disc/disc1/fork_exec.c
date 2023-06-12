#include "stdio.h"
#include "sys/types.h"
#include "unistd.h"
#include "stdlib.h"

int main(void) {
    char** argv = malloc(3*sizeof(char*));
    argv[0] = "/bin/ls";
    argv[1] = ".";
    argv[2] = NULL;
    pid_t fork_return = fork();
    for (int i = 0; i < 10; i++) {
        printf("%d\n", i);
    }
    if (fork_return == 0) {
        execv("/bin/ls", argv);
    }
    return 0;
}