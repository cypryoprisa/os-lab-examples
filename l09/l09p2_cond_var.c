#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

#define NR_THREADS 7
#define AMOUNT_DEPOSIT 11
#define AMOUNT_WITHDRAW 7

typedef struct {
    int id;
    pthread_mutex_t *lock;
    pthread_cond_t *cond;
} TH_STRUCT;

int balance = 0;

void *thread_withdraw(void *arg)
{
    TH_STRUCT *s = (TH_STRUCT*)arg;
    usleep(10000 * s->id);

    pthread_mutex_lock(s->lock);
    while(balance < AMOUNT_WITHDRAW) {
        printf("[Th%d] Not enough money (%d). Will wait...\n", s->id, balance);
        pthread_cond_wait(s->cond, s->lock);
    }
    balance -= AMOUNT_WITHDRAW;
    printf("[Th%d] Withdrawn %d. Current balance is %d.\n", 
            s->id, AMOUNT_WITHDRAW, balance);
    pthread_mutex_unlock(s->lock);

    return NULL;
}

void *thread_deposit(void *arg)
{
    TH_STRUCT *s = (TH_STRUCT*)arg;
    usleep(10000 * s->id);

    pthread_mutex_lock(s->lock);
    balance += AMOUNT_DEPOSIT;
    printf("[Th%d] Deposited %d. Current balance is %d.\n", 
            s->id, AMOUNT_DEPOSIT, balance);
    if(balance >= 2 * AMOUNT_WITHDRAW) {
        pthread_cond_broadcast(s->cond);
    } else if (balance >= AMOUNT_WITHDRAW) {
        pthread_cond_signal(s->cond);
    }
    pthread_mutex_unlock(s->lock);

    return NULL;
}

int main()
{
    int i;
    TH_STRUCT params[NR_THREADS];
    pthread_t tids[NR_THREADS];
    pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
    
    for(i=0; i<NR_THREADS; i++) {
        params[i].id = i;
        params[i].lock = &lock;
        params[i].cond = &cond;
        if(i == 0 || i == 2 || i == 3 || i == 4) {
            pthread_create(&tids[i], NULL, thread_withdraw, &params[i]);
        } else {
            pthread_create(&tids[i], NULL, thread_deposit, &params[i]);
        }
        
    }
    for(i=0; i<NR_THREADS; i++) {
        pthread_join(tids[i], NULL);
    }
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&lock);

    return 0;
}
