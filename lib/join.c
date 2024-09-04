#include "thrd.h"

#include "ctx.h"

static volatile uint32_t mtx = 0;

void thrd_join()
{
    if (gettid() == getpid()) {
        // while (__atomic_load_n(&live, __ATOMIC_SEQ_CST) > 1) { // 陷入等待
        //     futex(&mtx, FUTEX_WAIT, mtx, NULL, NULL, 0);
        // }

        // uint64_t stack = 0;
        // for (int i = 0; i < MAX_THREAD_NUM; i++) {
        //     if (stack_alloc[i] != 0) {
        //         free((void*)stack_alloc[i]);
        //     }
        // }
        return;
    } else {
        __atomic_sub_fetch(&live, 1, __ATOMIC_SEQ_CST);

        futex(&mtx, FUTEX_WAKE, 1, NULL, NULL, 0);

        while (1) { // 子线程
            __asm__ volatile(
                "li a7, 93\n"
                "ecall"
                :
                :
                : "memory");
        }
    }
}