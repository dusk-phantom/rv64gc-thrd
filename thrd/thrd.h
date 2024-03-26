#pragma once

#include <stdint.h>
#include <unistd.h>

/**
 * @brief threads[0] 表示 cnt ，有多少个线程，当然也可以表示：最后一个线程的下标
 * 假设是：全局静态变量初始化为 0
 * threads[1] 用来表示 主线程
 * 最多 20 个线程，应该不会需要太多
 *
 */
#define THREADS_num 20
extern pid_t tids[THREADS_num];

// 这个可以用于 join
extern uint32_t futexs[THREADS_num];

extern char tmp_mem[1600];

/**
 * @brief 创建一个线程并且返回线程的编号(主线程返回0,其他线程返回非0正值),线程中的函数自当前之后下一行开始执行,如果创建线程失败返回-1
 *
 * @return int
 */
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

int _get_tid(void);
