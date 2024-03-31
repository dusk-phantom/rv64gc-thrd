#include "thrd.h"

#include <stdint.h>
#include <sys/types.h>

#define THREADS_NUM 20

pid_t tids[THREADS_NUM];
// uint32_t futexs[THREADS_num];
char tmp_mem[TMP_MEM_SIZE];