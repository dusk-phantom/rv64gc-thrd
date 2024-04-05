#include "thrd.h"

#include <stdint.h>
#include <sys/types.h>

#define THREADS_NUM 20

pid_t tids[THREADS_NUM + 1];
char tmp_mem[TMP_MEM_SIZE];

const uint64_t STACK_SIZE = 1024 * 1024;