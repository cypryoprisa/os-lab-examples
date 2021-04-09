#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void)
{
    int fd[2];
    int x = 0;

    if(pipe(fd) != 0) {
        perror("Could not create pipe");
        return 1;
    }

    if(fork() != 0) {
        //parent
        close(fd[0]); //no use for read end
        x = 42;
        write(fd[1], &x, sizeof(x));
        printf("Parent: wrote %d to pipe\n", x);
        close(fd[1]);
        wait(NULL);
    } else {
        //child
        close(fd[1]); //no use for write end
        read(fd[0], &x, sizeof(x));
        printf("Child: read %d from pipe\n", x);
        close(fd[0]);
    }

    return 0;
}
