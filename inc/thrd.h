#ifndef __THRD_H__
#define __THRD_H__

#define _GNU_SOURCE

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <sched.h>
#include <linux/sched.h>

#define MAX_THREAD_NUM 10 // 至多有 10 个线程
#define STACK_SIZE 65536 // 栈的大小

#define gettid() syscall(SYS_gettid)
#define getpid() syscall(SYS_getpid)

typedef uint64_t tid_t;

#define FLAGS (CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD | CLONE_SYSVSEM | CLONE_CHILD_CLEARTID | CLONE_CHILD_SETTID | CLONE_PARENT_SETTID)

extern uint64_t cnt; // 线程的个数

tid_t __thrd_create(uint64_t main_sp, uint64_t main_s0);

/**
 * @brief
 *
 * @param num
 * @return uint64_t 线程 id, 主线程是 0
 */
tid_t thrd_create(uint64_t num);

#endif //  __THRD_H__