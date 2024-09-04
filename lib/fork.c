#include "thrd.h"

tid_t thrd_create(uint64_t num)
{
    if (gettid() != getpid()) { // 判断是否是主线程
        // 子线程返回 tid
        uint64_t tp;
        asm volatile(
            "mv %0, tp\n"
            : "=r"(tp)
            :
            :);
        return tp; // 这个是返回的子线程的 tid
    }

    if (num == 0 || num > MAX_THREAD_NUM) { // 判断 num 是否合法
        return 0; // 返回主线程的 tid
    }

    // 1 thrd_create's s0 == main's sp
    // 0 main's s0
    uint64_t main_s0 = (uint64_t)__builtin_frame_address(1);
    uint64_t main_sp = (uint64_t)__builtin_frame_address(0);

    uint64_t self_s0, self_sp;
    asm volatile(
        "mv %0, s0\n"
        "mv %1, sp\n"
        : "=r"(self_s0), "=r"(self_sp)
        :
        :);
    printf("thrd_create: main_sp=%p, main_s0=%p\n", main_sp, main_s0);
    printf("thrd_create: self_sp=%p, self_s0=%p\n", self_sp, self_s0);

    /* ---------- 创建线程 ---------- */
    tid_t tid = __thrd_create(main_sp, main_s0);
    switch (tid) {
    case 0:
        // 主线程的出口
        break;

    default:
        // 子线程的出口
        break;
    }
    return 0;
}
