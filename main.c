#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define BUFF_SIZE 16

void close_and_wr(int *fd, char *buffer, size_t nbytes) {
    int new = dup(fd[0]);
    close(fd[0]);
    write(fd[1], buffer, nbytes);
    fd[0] = new;
}

void close_and_rd(int *fd, char *buffer) {
    int new = dup(fd[1]);
    close(fd[1]);
    for (size_t i = 0; i < BUFF_SIZE; i++, buffer++) {
        read(fd[0], buffer, 1);
        if (*buffer == 0)
            break;
    }
    fd[1] = new;
}

void fib(int *fd, int *fd_n) {
    char buf[16];
    close_and_rd(fd_n, buf);
    int n = (int) strtol(buf, NULL, 10);

    if (n <= 2) {
        close_and_wr(fd, "1", 2);
        return;
    }

    pid_t ch1 = fork();
    if (ch1) {
        waitpid(ch1, NULL, 0);
        pid_t ch2 = fork();
        if (ch2) {
            close_and_rd(fd, buf);
            int f1 = (int) strtol(buf, NULL, 10);
            waitpid(ch2, NULL, 0);
            close_and_rd(fd, buf);
            int f2 = (int) strtol(buf, NULL, 10);
            sprintf(buf, "%d", f1 + f2);
            close_and_wr(fd, buf, strlen(buf) + 1);
        } else {
            sprintf(buf, "%d", n - 2);
            close_and_wr(fd_n, buf, strlen(buf) + 1);
            fib(accum, fd, fd_n);
            return;
        }
    } else {
        sprintf(buf, "%d", n - 1);
        close_and_wr(fd_n, buf, strlen(buf) + 1);
        fib(accum, fd, fd_n);
        return;
    }
}

int main(void) {
    int fd[2], fd_n[2];
    char buf[16];
    pipe(fd);
    pipe(fd_n);
    int n;
    scanf("%d", &n);
    pid_t master = fork();
    if (master) {
        waitpid(master, NULL, 0);
        close_and_rd(fd, buf);
        printf("%s\n", buf);
    } else {
        sprintf(buf, "%d", n);
        close_and_wr(fd_n, buf, strlen(buf) + 1);
        fib(fd, fd_n);
    }
    return 0;
}
