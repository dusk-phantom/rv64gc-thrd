#pragma once

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include <linux/futex.h>
#include <linux/sched.h> /* Definition of struct clone_args */
#include <sched.h>
#include <sys/syscall.h>
#include <unistd.h>

#define THREADS_NUM 20
#define TMP_MEM_SIZE ((1 << 8) * 20)

extern pid_t tids[THREADS_NUM + 1]; // 因为  tids[0] 存放的是可以创建线程的数量
extern char tmp_mem[TMP_MEM_SIZE];

/**
 * @brief 创建一个线程并且返回线程的编号(主线程返回0,其他线程返回非0正值),线程中的函数自当前之后下一行开始执行,如果创建线程失败返回-1
 *
 * @return int
 */

int _thrd_create(long stackSize);

int thrd_create(long num);

#define futex(uaddr, futex_op, val, timeout, uaddr2, val3) syscall(SYS_futex, uaddr, futex_op, val, timeout, uaddr2, val3)

// static inline __attribute__((always_inline)) int thrd_create(int num)
// {
//     int id = 0;
//     asm volatile(
//         "mv a0, %1\n"
//         "jal ra, __thrd_create_utils\n"
//         "mv %0, a0\n"
//         : "=r"(id)
//         : "r"(num)
//         :);
//     return id;
// }

// static inline __attribute__((always_inline)) int thrd_create(int num)
// {
//     long s0_sp_difference;
//     __asm__ volatile(
//         "sub %0, s0, sp\n" // 计算s0与sp的差值
//         : "=r"(s0_sp_difference) // 输出到变量
//         :
//         : "memory");

//     int id = 0;
//     int* p_num = &num;
//     while (num > 0) {
//         if (id == 0) {
//             id = _thrd_create(s0_sp_difference);
//             if (id == -1) {
//                 exit(-1);
//             } else if (id == 0) {
//                 // 主线程
//                 num--;
//             } else {
//                 while (*p_num > 1) {
//                     __asm__ volatile("nop");
//                 }
//                 return id;
//             }
//         }
//     }
//     return id;
// }

// #define CREATE(ID, NUM)                      \
//     for (int ___i = 0; ___i < NUM; ___i++) { \
//         ID = thrd_create();                  \
//         if (ID < 0) {                        \
//             exit(-1);                        \
//         }                                    \
//     }

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
