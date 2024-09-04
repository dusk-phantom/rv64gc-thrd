#include "thrd.h"
#include "ctx.h"

// 只有主线程能进入这个函数
tid_t __thrd_create(uint64_t main_sp, uint64_t main_s0)
{
    /* ---------- 保存上下文 callee saved ---------- */
    // 这些 callee saved 是 thrd_create 调用 __thrd_create 时的
    context_t ctx;
    // 注意编译选项, 保存的是: thrd_create 中的 callee saved 寄存器
    __asm__ volatile(
        "mv %0, s1\n"
        "mv %1, s2\n"
        "mv %1, s3\n"
        "mv %1, s4\n"
        "mv %1, s5\n"
        "mv %1, s6\n"
        "mv %1, s7\n"
        "mv %1, s8\n"
        "mv %1, s9\n"
        "mv %1, s10\n"
        "mv %1, s11\n"
        : "=r"(ctx.s1), "=r"(ctx.s2), "=r"(ctx.s3), "=r"(ctx.s4), "=r"(ctx.s5), "=r"(ctx.s6), "=r"(ctx.s7), "=r"(ctx.s8), "=r"(ctx.s9), "=r"(ctx.s10), "=r"(ctx.s11)
        :
        :);
    uint64_t ra;
    __asm__ volatile(
        "mv %0, ra\n"
        : "=r"(ra)
        :
        :);

    /* ---------- 分配栈 ---------- */
    void* lo;
    if (posix_memalign(&lo, 16, STACK_SIZE) != 0) {
        abort();
    }
    char* hi = (char*)lo + STACK_SIZE;
    uint64_t main_stack_size = main_s0 - main_sp;
    hi = hi - main_stack_size;
    uint64_t child_s0 = (uint64_t)hi;
    memcpy(hi, (void*)main_sp, main_stack_size);

    uint64_t thrd_create_s0 = (uint64_t)__builtin_frame_address(1);
    uint64_t thrd_create_sp = (uint64_t)__builtin_frame_address(0);
    uint64_t thrd_create_stack_size = thrd_create_s0 - thrd_create_sp;
    hi = hi - thrd_create_stack_size;
    uint64_t child_sp = (uint64_t)hi;
    memcpy(hi, (void*)thrd_create_sp, thrd_create_stack_size);

    clone_args_t args = { &ctx, cnt /* tid */, ra, child_s0, child_sp };

    int ret = clone(son, hi, FLAGS, (void*)&args);

    pause();

    return 0;
}
