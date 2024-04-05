#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <syscall.h>
#include <unistd.h>

#include "thrd.h"

// TODO: 测试mtx
int main()
{
    int sum = 0;

    int mtx = mtx_create();

    int id = thrd_create(1);
    switch (id) {
    case -1:
        abort();
    case 0: // 主线程 ，因为 主线程的 tid == 0
        for (int i = 0; i < 100; i++) {
            mtx_lock(mtx);
            sum++;
            mtx_unlock(mtx);
        }
        break;
    case 1: // 子线程，因为子线程的 tid == 0
        for (int i = 0; i < 100; i++) {
            mtx_lock(mtx);
            sum++;
            mtx_unlock(mtx);
        }
        break;
    }

    thrd_join();

    printf("sum=%d\n", sum);

    return sum;
}