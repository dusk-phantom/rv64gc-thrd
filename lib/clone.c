#include "thrd.h"

typedef struct {
    uint64_t s1;
    uint64_t s2;
    uint64_t s3;
    uint64_t s4;
    uint64_t s5;
    uint64_t s6;
    uint64_t s7;
    uint64_t s8;
    uint64_t s9;
    uint64_t s10;
    uint64_t s11;
} context_t;

typedef struct {
    context_t* ctx;
    tid_t tid; // 子线程的 tid
    uint64_t ra; // 返回地址
    uint64_t s0;
} clone_args_t;

// 子线程的入口地址
int son(void*);

// 只有主线程能进入这个函数
tid_t __thrd_create(uint64_t main_sp, uint64_t main_s0)
{
    /* ---------- 保存上下文 ---------- */
    // 这些 callee saved 是 thrd_create 调用 __thrd_create 时的
    context_t ctx;
    // 注意编译选项, 保存的是: thrd_create 中的 callee saved 寄存器
    asm volatile(
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
    asm volatile(
        "mv %0, ra\n"
        : "=r"(ra)
        :
        :);

    void* lo;
    if (posix_memalign(&lo, 16, STACK_SIZE) != 0) {
        abort();
    }
    char* hi = (char*)lo + STACK_SIZE;
    printf("__thrd_create: hi=%p\n", hi);
    uint64_t main_stack_size = main_s0 - main_sp;
    hi = hi - main_stack_size;
    memcpy(hi, (void*)main_sp, main_stack_size);
    printf("__thrd_create: main_sp=%p, main_s0=%p\n", main_sp, main_s0);
    printf("__thrd_create: hi=%p\n", hi);
    uint64_t thrd_create_s0 = (uint64_t)__builtin_frame_address(1);
    uint64_t thrd_create_sp = (uint64_t)__builtin_frame_address(0);
    uint64_t thrd_create_stack_size = thrd_create_s0 - thrd_create_sp;
    hi = hi - thrd_create_stack_size;
    memcpy(hi, (void*)thrd_create_sp, thrd_create_stack_size);
    printf("__thrd_create: hi=%p\n", hi);
    printf("__thrd_create: thrd_create_sp=%p, thrd_create_s0=%p\n", thrd_create_sp, thrd_create_s0);
    uint64_t self_s0, self_sp;
    asm volatile(
        "mv %0, s0\n"
        "mv %1, sp\n"
        : "=r"(self_s0), "=r"(self_sp)
        :
        :);
    printf("__thrd_create: self_sp=%p, self_s0=%p\n", self_sp, self_s0);

    clone_args_t args = { &ctx, cnt /* tid */, ra, (uint64_t)hi };

    int ret = clone(son, hi, FLAGS, (void*)&args);

    sleep(1);

    return 0;
}

/**
 * @brief 这个要返回到 thrd_create 中
 *
 * @param args
 * @return int
 */
int son(void* _args)
{
    clone_args_t* args = (clone_args_t*)_args;
    uint64_t self_s0, self_sp;
    asm volatile(
        "mv %0, s0\n"
        "mv %1, sp\n"
        : "=r"(self_s0), "=r"(self_sp)
        :
        :);
    printf("son: self_sp=%p, self_s0=%p, args->s0=%p\n", self_sp, self_s0, args->s0);
    return 0;
}
