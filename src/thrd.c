#include "thrd.h"

volatile pid_t tids[THREADS_NUM + 1];
volatile char tmp_mem[TMP_MEM_SIZE];

volatile const uint64_t STACK_SIZE = 512 * 1024 * 1024;