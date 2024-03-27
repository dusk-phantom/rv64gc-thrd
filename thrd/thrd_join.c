#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <linux/futex.h>
#include <linux/sched.h> /* Definition of struct clone_args */
#include <sched.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "thrd.h"

/* ---------- ---------- 全局变量 ---------- ---------- */

#define THREADS_num 20
extern pid_t tids[THREADS_num];
// extern uint32_t futexs[THREADS_num];
extern char tmp_mem[1600];

/* ---------- ---------- 线程相关函数 ---------- ---------- */

#define futex(uaddr, futex_op, val, timeout, uaddr2, val3) syscall(SYS_futex, uaddr, futex_op, val, timeout, uaddr2, val3)

volatile uint32_t futex_var = 0;

int thrd_join(void)
{
    if (gettid() == getpid()) { // 主线程
        // wait

        while (__atomic_load_n(&tids[0], __ATOMIC_ACQUIRE) > 1) { // 陷入等待
            futex(&futex_var, FUTEX_WAIT, futex_var, NULL, NULL, 0);
        }

        // 释放 线程的栈空间
        // 注意一下，这个时候有一个问题：就是我已经不知道创建过多少个线程
        // 如果 tmp_mem[i << 8] != 0 说明曾经有过东西，释放！
        uint64_t stack = 0;
        for (int i = 2; tmp_mem[i << 8]; i++) {
            stack = *(uint64_t*)((char*)tmp_mem + (i << 8) + 112);
            free((void*)stack);
        }

    } else {

        __atomic_sub_fetch(&tids[0], 1, __ATOMIC_RELEASE); // 保证下面的总是可见

        // futex_wait 的 val 表示唤醒的线程数
        futex(&futex_var, FUTEX_WAKE, 1, NULL, NULL, 0);

        while (1) { // 子线程
            syscall(SYS_exit, 0);
        }
    }

    return 0;
}
