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

#define _GNU_SOURCE

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
    if (ret1 == -1) { // 错误
        return -1;
    } else if (ret1 == 0) { // 主线程

        arr[0] = 20;

        for (int i = 0; i < 100; i++) {
            mtx_lock(mtx1);
            sum++;
            mtx_unlock(mtx1);
        }

        int ret2 = thrd_create();
        if (ret2 == -1) { // 错误
            return -1;
        } else if (ret2 == 0) { // 主线程
            for (int i = 0; i < 100; i++) {
                mtx_lock(mtx1);
                sum++;
                mtx_unlock(mtx1);
            }
        } else {
            // 另一个子线程
            for (int i = 0; i < 100; i++) {
                mtx_lock(mtx1);
                sum++;
                mtx_unlock(mtx1);
            }
        }

    } else { // 子线程 tid = 1

        arr[1] = 100;

        for (int i = 0; i < 100; i++) {
            mtx_lock(mtx1);
            sum++;
            mtx_unlock(mtx1);
        }

        int ret2 = thrd_create();
        if (ret2 == -1) { // 错误
            return -1;
        } else if (ret2 == 1) { // 子线程
            for (int i = 0; i < 100; i++) {
                mtx_lock(mtx1);
                sum++;
                mtx_unlock(mtx1);
            }
        } else {
            // 理论上不会执行这一段
            for (int i = 0; i < 100; i++) {
                mtx_lock(mtx1);
                sum++;
                mtx_unlock(mtx1);
            }
        }
    }

    thrd_join();

    printf("father thread pid=%d\n", getpid());

    printf("%d\n", arr[1] + arr[0]);
    printf("sum=%d\n", sum);
}