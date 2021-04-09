#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
    pid_t pid;
    int fd, status;

    if(argc != 2) {
        fprintf(stderr, "Usage: %s <result_file>\n", argv[0]);
        return 1;
    }

    pid = fork();
    if(pid == -1) {
        perror("Could not create child process");
        return -1;
    } else if(pid == 0) {
        fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0644);
        if(fd < 0) {
            perror("Could not open the result file");
            exit(1);
        }
        dup2(fd, 1); //use fd as standard output
        //ls will inherit the standard output
        execlp("ls", "ls", "/", NULL);
        perror("execlp failed");
        exit(2);
    } else {
        wait(&status);
        if(WEXITSTATUS(status) == 0) {
            printf("Command completed successfuly!\n");
        } else {
            printf("Command failed!\n");
        }
    }
    return 0;
}
