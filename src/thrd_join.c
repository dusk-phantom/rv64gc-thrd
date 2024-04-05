
#include "thrd.h"
#include <stdlib.h>
#include <string.h>

/* ---------- ---------- 线程相关函数 ---------- ---------- */

#define futex(uaddr, futex_op, val, timeout, uaddr2, val3) syscall(SYS_futex, uaddr, futex_op, val, timeout, uaddr2, val3)

volatile uint32_t futex_var = 0;

// int thrd_join(void)
// {
//     if (syscall(SYS_gettid) == tids[1]) { // 主线程

//         while (__atomic_load_n(&tids[0], __ATOMIC_SEQ_CST) > 1) {
//             __asm__ volatile("nop");
//         }

//         // 释放 线程的栈空间
//         // 注意一下，这个时候有一个问题：就是我已经不知道创建过多少个线程
//         // 如果 tmp_mem[i << 8] != 0 说明曾经有过东西，释放！
//         for (int i = 2; tmp_mem[i << 8]; i++) {
//             char* tmp = (char*)tmp_mem + (i << 8);
//             uint64_t stack = *(uint64_t*)(tmp + 112);
//             free((void*)stack);
//             stack = 0; // stack = NULL
//             memset(tmp, 0, (1 << 8)); // 清空，以免影响循环判断条件
//             tmp = NULL;
//         }

//         memset(tids, 0, sizeof(pid_t) * THREADS_NUM);

//     } else {

//         __atomic_sub_fetch(&tids[0], 1, __ATOMIC_SEQ_CST); // 保证下面的总是可见

//         while (1) { // 子线程
//             syscall(SYS_exit, 0);
//         }
//     }

//     return 0;
// }

// int thrd_join(void)
// {
//     if (syscall(SYS_gettid) == tids[1]) { // 主线程
//         // wait

//         while (__atomic_load_n(&tids[0], __ATOMIC_SEQ_CST) > 1) { // 陷入等待
//             futex(&futex_var, FUTEX_WAIT, futex_var, NULL, NULL, 0);
//         }

//         for (int i = 2; tmp_mem[i << 8]; i++) {
//             char* tmp = (char*)tmp_mem + (i << 8);
//             uint64_t stack = *(uint64_t*)(tmp + 112);
//             // free((void*)stack);
//             stack = 0; // stack = NULL
//             memset(tmp, 0, (1 << 8)); // 清空，以免影响循环判断条件
//             tmp = NULL;
//         }

//     } else {

//         __atomic_sub_fetch(&tids[0], 1, __ATOMIC_SEQ_CST); // 保证下面的总是可见

//         // futex_wait 的 val 表示唤醒的线程数
//         futex(&futex_var, FUTEX_WAKE, 1, NULL, NULL, 0);

//         while (1) { // 子线程
//             syscall(SYS_exit, 0);
//         }
//     }

//     return 0;
// }

int thrd_join(void)
{
    if (syscall(SYS_gettid) == tids[1]) { // 主线程
        // wait

        __atomic_sub_fetch(&tids[0], 1, __ATOMIC_SEQ_CST); // 主线程自己 --

        while (__atomic_load_n(&tids[0], __ATOMIC_SEQ_CST) > 0) {
            __asm__ volatile("nop");
        }

        // // 释放 线程的栈空间
        // // 注意一下，这个时候有一个问题：就是我已经不知道创建过多少个线程
        // // 如果 tmp_mem[i << 8] != 0 说明曾经有过东西，释放！
        // for (int i = 2; tmp_mem[i << 8]; i++) {
        //     char* tmp = (char*)tmp_mem + (i << 8);
        //     uint64_t stack = *(uint64_t*)(tmp + 112);
        //     free((void*)stack);
        //     memset(tmp, 0, 128); // 清空，以免影响循环判断条件
        //     memset(tids, 0, sizeof(pid_t) * THREADS_NUM);
        //     stack = 0; // stack = NULL
        // }

        // memset(tids, 0, sizeof(pid_t) * THREADS_NUM);

    } else {

        __atomic_sub_fetch(&tids[0], 1, __ATOMIC_SEQ_CST); // 保证下面的总是可见

        while (1) { // 子线程
            syscall(SYS_exit, 0);
        }
    }

    return 0;
}
