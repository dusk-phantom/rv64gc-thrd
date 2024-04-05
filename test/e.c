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
    // fork();

    int _sum = 0;
    int* sum = &_sum;

    int mtx1 = mtx_create();

    thrd_create(1);

    for (int i = 0; i < 100; i++) {
        mtx_lock(mtx1);
        (*sum)++;
        mtx_unlock(mtx1);
    }

    thrd_join();

    printf("sum=%d\n", _sum);
}