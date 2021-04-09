#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;

    pid = fork();
    if(pid == -1) {
        perror("Could not create child process");
        return -1;
    } else if(pid == 0) {
        //this code is executed only by child
        printf("[CHILD] My PID is %d. My parent's PID is %d.\n", getpid(), getppid());
    } else {
        //this code is executed only by parent
        printf("[PARENT] My PID is %d. I created a child with PID %d.\n", getpid(), pid);
        wait(NULL);
    }
    return 0;
}