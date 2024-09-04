#include "thrd.h"
#include "ctx.h"

#define CLONE_ARGS(ARGS) ((clone_args_t*)ARGS)

int son(void*)
{
    __asm__ volatile(
        "ld s0, 24(a0)\n"
        "ld sp, 32(a0)\n"
        "ld ra, 16(a0)\n"
        "ld t1, 0(a0)\n" // context_t * ctx
        "ld s1, 0(t1)\n"
        "ld s2, 8(t1)\n"
        "ld s3, 16(t1)\n"
        "ld s4, 24(t1)\n"
        "ld s5, 32(t1)\n"
        "ld s6, 40(t1)\n"
        "ld s7, 48(t1)\n"
        "ld s8, 56(t1)\n"
        "ld s9, 64(t1)\n"
        "ld s10, 72(t1)\n"
        "ld s11, 80(t1)\n"
        "ret\n"
        :
        :
        : "t1", "s1", "s2", "s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "s0", "ra", "sp");
}