#include "thrd.h"
#include "ctx.h"

uint64_t live = 1; // 有几个存活的线程

uint64_t stack_alloc[MAX_THREAD_NUM] = { 0 };
