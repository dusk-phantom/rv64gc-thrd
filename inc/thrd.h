#pragma once


#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <linux/futex.h>
#include <linux/sched.h> /* Definition of struct clone_args */
#include <sched.h>
#include <sys/syscall.h>
#include <unistd.h>


#define THREADS_NUM 20
#define TMP_MEM_SIZE 1600

extern pid_t tids[THREADS_NUM];
extern char tmp_mem[TMP_MEM_SIZE];

/**
 * @brief 创建一个线程并且返回线程的编号(主线程返回0,其他线程返回非0正值),线程中的函数自当前之后下一行开始执行,如果创建线程失败返回-1
 *
 * @return int
 */
#include <sys/types.h>
int thrd_create();

/**
 * @brief 等待之前所有创建的子线程结束, 在这之后只有主线程继续执行, 失败返回-1
 *
 * @return int
 */
int thrd_join();

/**
 * @brief 创建一个互斥锁并且返回锁的编号(大于0的值),如果创建失败,返回-1
 *
 * @return int
 */
int mtx_create();

/**
 * @brief 对指定编号的互斥锁在 共享区 上锁
 *
 * @param mtx
 */
void mtx_lock(int mtx);

/**
 * @brief 对指定编号的互斥锁在 共享区 解锁
 *
 * @param mtx
 */
void mtx_unlock(int mtx);

/**
 * @brief 把所有的锁销毁
 *
 */
void destroy_mtxs();
