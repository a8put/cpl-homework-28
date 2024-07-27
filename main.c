#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

void fib(int n) {
    if (n <= 2) {
        exit(1);
    }
    pid_t child1 = fork();
    int x, y = 0;
    if (child1) {
        waitpid(child1, &x, 0);
        pid_t child2 = fork();
        if (child2) {
            waitpid(child2, &y, 0);
            exit(WEXITSTATUS(x) + WEXITSTATUS(y));
        } else {
            char buf[16];
            sprintf(buf, "%d", n - 2);
            execl("fib", "fib", buf, NULL);
        }
    } else {
        char buf[16];
        sprintf(buf, "%d", n - 1);
        execl("fib", "fib", buf, NULL);
    }
}

int main(int argc, char **argv) {
    if (argc == 1) {
        int x;
        scanf("%d", &x);
        pid_t master = fork();
        if (master) {
            waitpid(master, &x, 0);
            printf("%d\n", (WEXITSTATUS(x)));
        } else {
            char buf[16];
            sprintf(buf, "%d", x);
            execl("fib", "fib", buf, NULL);
        }
    } else if (argc == 2) {
        char *end;
        int x = (int) strtol(argv[1], &end, 10);
        fib(x);
    }
}
