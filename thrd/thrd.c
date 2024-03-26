#include "thrd.h"

pid_t tids[THREADS_num];

uint32_t futexs[THREADS_num];

char tmp_mem[1600];