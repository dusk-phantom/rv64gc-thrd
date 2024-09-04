#ifndef __CTX_H__
#define __CTX_H__

#include "thrd.h"

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
    uint64_t ra; // 返回到 main 的哪一行
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

#endif // __CTX_H__