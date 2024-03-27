#include "thrd.h"

#include <stdint.h>
#include <sys/types.h>

#define THREADS_num 20

pid_t tids[THREADS_num];
// uint32_t futexs[THREADS_num];
char tmp_mem[1600];