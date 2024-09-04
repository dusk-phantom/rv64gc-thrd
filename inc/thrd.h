#ifndef __THRD_H__
#define __THRD_H__

#define _GNU_SOURCE

#include <stdint.h>

typedef uint64_t tid_t;

/**
 * @brief
 *
 * @param num
 * @return uint64_t 线程 id, 主线程是 0
 */
tid_t thrd_create(uint64_t num);

void thrd_join();

#endif //  __THRD_H__