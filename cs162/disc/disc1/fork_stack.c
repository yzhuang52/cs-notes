#include "stdio.h"
#include "sys/types.h"
#include "unistd.h"

int main(void) {
    int staff = 5;
    pid_t fork_ret = fork();
    printf("The last digit of pi is %d\n", staff);
    if (fork_ret == 0) {
        staff = 6;
    }
    return 0;
}