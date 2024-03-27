#include <assert.h>
#include <stdlib.h>
#include <syscall.h>
#include <unistd.h>

#include "thrd.h"

// TODO: 测试mtx
int main()
{
    int sum = 0;

    int mtx = mtx_create();

    int id = thrd_create();
    // if (id == -1) {
    //     abort();
    // } else if (id == 0) {
    //     for (int i = 0; i < 100; i++) {
    //         mtx_lock(mtx);
    //         sum++;
    //         mtx_unlock(mtx);
    //     }
    // } else {
    //     for (int i = 0; i < 100; i++) {
    //         mtx_lock(mtx);
    //         sum++;
    //         mtx_unlock(mtx);
    //     }
    // }
    switch (id) {
    case -1:
        abort();
    case 0:
        for (int i = 0; i < 100; i++) {
            mtx_lock(mtx);
            sum++;
            mtx_unlock(mtx);
        }
        // break;
    default:
        for (int i = 0; i < 100; i++) {
            mtx_lock(mtx);
            sum++;
            mtx_unlock(mtx);
        }
        // break;
    }

    thrd_join();

    // assert(sum == 200);

    return sum;
}