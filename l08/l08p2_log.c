#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>

#define NR_THREADS 4

typedef struct {
    int value;
    sem_t *logSem;
} TH_STRUCT;

void write_log(const char *message, sem_t *logSem)
{
    int fd = -1;   
    //entering the critical region
    sem_wait(logSem);
    fd = open("log2.txt", O_CREAT | O_WRONLY | O_APPEND, 0644);
    if(fd < 0) {
        perror("Could not open log for writing");
    } else {
        write(fd, message, strlen(message));
        close(fd);
    }
    //exiting the critical region
    sem_post(logSem);
}

void *thread_function(void *p)
{
    TH_STRUCT *s = (TH_STRUCT*)p;
    int i, count;
    char message[20];

    for(i=0; i<3; i++){
        usleep(1000 * (rand() % 20));
        count = s->value + i + 1;
        snprintf(message, 20, "Log entry %d\n", count);
        write_log(message, s->logSem);
    }
    return NULL;
}

int main(void)
{
    sem_t logSem;
    int i;
    TH_STRUCT params[NR_THREADS];
    pthread_t tids[NR_THREADS];

    //initialize random number generator
    srand(time(NULL));
    //initialize the semaphore
    if(sem_init(&logSem, 0, 1) != 0) {
        perror("Could not init the semaphore");
        return -1;
    }
    //create the threads
    for(i=0; i<NR_THREADS; i++) {
        params[i].value = 10 * (i + 1);
        params[i].logSem = &logSem;
        pthread_create(&tids[i], NULL, thread_function, &params[i]);
    }
    //wait for the threads to finish
    for(i=0; i<NR_THREADS; i++) {
        pthread_join(tids[i], NULL);
    }
    //destroy the semaphore
    sem_destroy(&logSem);

    return 0;
}
