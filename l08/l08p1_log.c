#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

void write_log(const char *message)
{
    sem_t *logSem = NULL;
    int fd = -1;

    logSem = sem_open("l08p1_log_semaphore", O_CREAT, 0644, 1);
    if(logSem == NULL) {
        perror("Could not aquire the semaphore");
        return;
    }

    //entering the critical region
    sem_wait(logSem);

    fd = open("log.txt", O_CREAT | O_WRONLY | O_APPEND, 0644);
    if(fd < 0) {
        perror("Could not open log for writing");
    } else {
        write(fd, message, strlen(message));
        close(fd);
    }

    //exiting the critical region
    sem_post(logSem);

    sem_close(logSem);
}

int main(void)
{
    write_log("Log entry 1\n");
    write_log("Log entry 2\n");
    write_log("Log entry 3\n");
    return 0;
}
