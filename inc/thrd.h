#ifndef __THRD_H__
#define __THRD_H__

#define _GNU_SOURCE

typedef unsigned long uint64_t;
typedef uint64_t tid_t;

/// @brief
/// @param num
/// @return
tid_t thrd_create(uint64_t num);

void thrd_join();

#endif //  __THRD_H__