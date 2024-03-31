/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2024-03-26
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <syscall.h>
#include <unistd.h>

#include "thrd.h"

int main(void)
{
    int arr[2];
    // fork();

    int sum = 0;

    int mtx1 = mtx_create();

    int ret1 = thrd_create();

    // for (int i = 0; i < 100; i++) {
    //     mtx_lock(mtx1);
    //     sum++;
    //     mtx_unlock(mtx1);
    // }

    mtx_lock(mtx1);
    for (int i = 0; i < 100; i++) {
        sum++;
    }
    mtx_unlock(mtx1);

    thrd_join();

    printf("sum=%d\n", sum);
}