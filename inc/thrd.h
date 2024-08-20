#pragma once

#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <linux/futex.h>
#include <linux/sched.h> /* Definition of struct clone_args */
#include <sched.h>
#include <sys/syscall.h>
#include <unistd.h>

#define THREADS_NUM 8
#define TMP_MEM_SIZE ((1 << 8) * (THREADS_NUM + 1))

extern volatile pid_t tids[THREADS_NUM + 1]; // 因为  tids[0] 存放的是可以创建线程的数量
extern volatile char tmp_mem[TMP_MEM_SIZE];

extern volatile const uint64_t STACK_SIZE;

/**
 * @brief 创建一个线程并且返回线程的编号(主线程返回0,其他线程返回非0正值),线程中的函数自当前之后下一行开始执行,如果创建线程失败返回-1
 *
 * @return int
 */

int thrd_create(volatile long num);

/**
 * @brief 等待之前所有创建的子线程结束, 在这之后只有主线程继续执行, 失败返回-1
 *
 * @return int
 */
int thrd_join();