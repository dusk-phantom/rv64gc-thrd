#include <pthread.h>
#include <stdio.h>

#include "mtx.h"

int sum = 0;

int mtx = 1;

void* func(void*)
{
    for (int i = 0; i < 1000; i++) {
        mtx_lock(mtx);
        // printf("lock");
        sum++;
        mtx_unlock(mtx);
        // printf("unlock");
    }

    return NULL;
}

int main(void)
{

    mtx = mtx_create();

    printf("start threads\n");

    pthread_t arr[10];
    for (int i = 0; i < 10; i++) {
        pthread_create(&arr[i], NULL, func, NULL);
    }

    printf("start over\n");

    for (int i = 0; i < 10; i++) {
        pthread_join(arr[i], NULL);
    }

    printf("sum=%d\n", sum); // 期望是 1,000 * 10 = 10,000
}
