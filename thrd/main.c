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

#if 0
#include "thrd.h"
#else
#define THREADS_num 20
extern pid_t tids[THREADS_num];

// 这个可以用于 join
extern uint32_t futexs[THREADS_num];

extern char tmp_mem[1600];

int thrd_create();

int _get_tid(void);
#endif

#if 1
int _get_tid(void)
{
    pid_t pid = gettid(); // 得到当前线程真正的 tid
    int cnt = tids[0];
    int i = cnt;
    while (i > 0) {
        if (tids[i] == pid) {
            return i;
        }
        i--;
    }

    return -1;
}
#else
int _get_tid(void)
{
    pid_t pid = gettid();
    for (int i = tids[tids[0]]; i > 0; i--) {
        if (tids[i] == pid) {
            return i;
        }
    }
    return -1;
}
#endif

int main(void)
{
    int arr[2];
    // fork();
    int ret = thrd_create();
    if (ret == -1) { // 错误
        return -1;
    } else if (ret == 0) { // 子线程

        arr[0] = 20;

        // thrd_join();
        _get_tid();

    } else { // 父线程

        arr[1] = 100;
        printf("%d\n", ret);

        sleep(5);
    }

    printf("%d\n", arr[1] + arr[0]);
}