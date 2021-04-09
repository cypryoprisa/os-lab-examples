#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#define NR_THREADS 4

void *thread_function(void *arg)
{
    pthread_mutex_t *lock = (pthread_mutex_t*)arg;

    //common region
    sleep(1);

    //critical region
    pthread_mutex_lock(lock);
    sleep(1);
    pthread_mutex_unlock(lock);
    
    return NULL;
}

int main()
{
    int i;
    pthread_t tids[NR_THREADS];
    pthread_mutex_t lock;

    if(pthread_mutex_init(&lock, NULL) != 0) {
        perror("error initializing the mutex");
        return 1;
    }
    for(i=0; i<NR_THREADS; i++) {
        pthread_create(&tids[i], NULL, thread_function, &lock);
    }
    for(i=0; i<NR_THREADS; i++) {
        pthread_join(tids[i], NULL);
    }
    pthread_mutex_destroy(&lock);

    return 0;
}
