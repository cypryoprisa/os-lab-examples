#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *threadFn(void *unused)
{
    printf("Secondary thread\n");
    return NULL;
}

int main()
{
    pthread_t tid = -1;

    if(pthread_create(&tid, NULL, threadFn, NULL) != 0){
        perror("Error creating thread");
        return 1;
    }
    printf("Main thread\n");
    pthread_join(tid, NULL);
    return 0;
}
