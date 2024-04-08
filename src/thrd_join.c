
#include "thrd.h"

/* ---------- ---------- 线程相关函数 ---------- ---------- */

int thrd_join(void)
{
    if (syscall(SYS_gettid) == tids[1]) { // 主线程
        // wait

        __atomic_sub_fetch(&tids[0], 1, __ATOMIC_SEQ_CST); // 主线程自己 --

        while (__atomic_load_n(&tids[0], __ATOMIC_SEQ_CST) > 0) {
            __asm__ volatile("nop"); // 空转
        }

        // 释放 线程的栈空间
        // 如果 tmp_mem[i << 8] != 0 说明曾经有过东西，释放！
        for (int i = 2; tmp_mem[i << 8]; i++) {
            char* tmp = (char*)tmp_mem + (i << 8);
            uint64_t stack = *(uint64_t*)(tmp + 112);
            free((void*)stack);
            memset(tmp, 0, 128); // 清空，以免影响循环判断条件
            stack = 0; // stack = NULL
        }

        memset(tids, 0, sizeof(pid_t) * (THREADS_NUM + 1));

    } else {

        __atomic_sub_fetch(&tids[0], 1, __ATOMIC_SEQ_CST);

        while (1) { // 子线程
            syscall(SYS_exit, 0);
        }
    }

    return 0;
}
