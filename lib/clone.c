#include "thrd.h"
#include "ctx.h"

// 只有主线程能进入这个函数, 子线程创建出来以后, 不用管这个函数, 直接返回到 thrd_create
tid_t __thrd_create(create_t* crea, uint64_t main_sp, uint64_t main_size)
{

    volatile tid_t id = live; // 担心被优化掉
    /* ---------- 分配栈 ---------- */
    void* lo;
    if (posix_memalign(&lo, 16, STACK_SIZE) != 0) {
        abort();
    }
    uint64_t hi = (uint64_t)lo + STACK_SIZE;

    stack_alloc[id] = (uint64_t)lo;

    /* ---------- main ---------- */
    memcpy((void*)(hi - main_size), (void*)(main_sp), main_size);

    /* ---------- thrd_create ---------- */
    uint64_t thrd_create_sp = (uint64_t)__builtin_frame_address(0);
    uint64_t thrd_create_size = (uint64_t)__builtin_frame_address(1) - thrd_create_sp;
    memcpy((void*)(hi - main_size - thrd_create_size), (void*)thrd_create_sp, thrd_create_size);

    clone_t args = {
        .crea = crea,
        .tid = id,
        .thrd_create_next = (uint64_t)__builtin_return_address(0),
        .chld_thrd_s0 = hi - main_size,
        .chld_thrd_sp = hi - main_size - thrd_create_size,
    };

    __asm__ volatile(
        "sd %0, -16(%1)\n"
        :
        : "r"(hi), "r"(args.chld_thrd_s0)
        : "memory");

    // 这里, 子线程直接到达了 thrd_create 的返回地点, 也就是 ra
    clone(son, (void*)hi, FLAGS, (void*)&args);

    return 0; // 主线程返回 0
}
