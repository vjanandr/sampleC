#include <stdio.h>
#include <pthread.h>

#define MAX_COUNT_LOOP 100000000
volatile int count= 0;

void*
count_inc_cbk (void *arg)
{
    int i = 0;
    while (i < MAX_COUNT_LOOP) {
        __sync_fetch_and_add( &count, 1 );
        i++;
    }
    return NULL;
}

int main ()
{
    pthread_t tid1;
    pthread_t tid2;

    pthread_create(&tid1, NULL, count_inc_cbk, NULL);
    pthread_create(&tid2, NULL, count_inc_cbk, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    printf("\n Count at the end of the threads %d\n",count);
}
