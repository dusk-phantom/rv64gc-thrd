#include "thrd.h"
#include "ctx.h"

uint64_t live_son = 0; // 有几个存活的线程

clone_t args_vec[MAX_THREAD_NUM];
