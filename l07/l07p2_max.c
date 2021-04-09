#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define SIZE_ARRAY 1000
#define NR_THREADS 6

typedef struct {
    int *arr;
    int from;
    int to;
} TH_STRUCT;

void generate_random_array(int *v, int size)
{
    int i;
    for(i=0; i<size; i++){
        v[i] = rand() % 1000000;
    }
}

void *thread_fn_max(void *param)
{
    TH_STRUCT *s = (TH_STRUCT*)param;
    int i, max;
    max = 0;
    for(i=s->from; i<=s->to; i++){
        if(s->arr[i] > max){
            max = s->arr[i];
        }
    }
    return (void*)(long)max;
}

int main()
{
    int v[SIZE_ARRAY];
    pthread_t tid[NR_THREADS];
    TH_STRUCT params[NR_THREADS];
    int i;
    void *result;
    int thMax, max = 0;

    srand(time(NULL));
    generate_random_array(v, SIZE_ARRAY);

    for(i=0; i<NR_THREADS; i++){
        params[i].arr = v;
        if(i == 0){
            params[i].from = 0;
        }else{
            params[i].from = params[i-1].to + 1;
        }
        params[i].to = params[i].from + SIZE_ARRAY / NR_THREADS - 1;
        if(i < SIZE_ARRAY % NR_THREADS){
            params[i].to++;
        }

        pthread_create(&tid[i], NULL, thread_fn_max, &params[i]);
    }
    for(i=0; i<NR_THREADS; i++){
        pthread_join(tid[i], &result);
        thMax = (int)(long)result;
        if(thMax > max){
            max = thMax;
        }
    }
    printf("The maximum value is %d.\n", max);
    
    return 0;
}
