#ifndef __CTX_H__
#define __CTX_H__

#include "thrd.h"

#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdio.h>
#include <sched.h>
#include <linux/sched.h>
#include <linux/futex.h>

typedef struct {
    uint64_t s1;
    uint64_t s2;
    uint64_t s3;
    uint64_t s4;
    uint64_t s5;
    uint64_t s6;
    uint64_t s7;
    uint64_t s8;
    uint64_t s9;
    uint64_t s10;
    uint64_t s11;
} create_t;

typedef struct {
    create_t* crea; // -> 0
    tid_t tid; // 子线程的 tid -> 8
    uint64_t thrd_create_next; // 返回地址 -> 16
    uint64_t chld_thrd_s0; // -> 24
    uint64_t chld_thrd_sp; // -> 32
} clone_t;

// 子线程的入口地址
int son(void*) __attribute__((naked));

tid_t __thrd_create(create_t* crea, uint64_t main_sp, uint64_t main_size);

extern uint64_t stack_alloc[];

#define MAX_THREAD_NUM 10 // 至多有 10 个线程
#define STACK_SIZE 65536 // 栈的大小

#define gettid() syscall(SYS_gettid)
#define getpid() syscall(SYS_getpid)

#define FLAGS (CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD | CLONE_SYSVSEM | CLONE_CHILD_CLEARTID | CLONE_CHILD_SETTID | CLONE_PARENT_SETTID)

#define futex(uaddr, futex_op, val, timeout, uaddr2, val3) syscall(SYS_futex, uaddr, futex_op, val, timeout, uaddr2, val3)

extern uint64_t live; // 线程的个数

#endif // __CTX_H__