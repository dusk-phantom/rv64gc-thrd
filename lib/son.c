#include "thrd.h"
#include "ctx.h"

#define CLONE_ARGS(ARGS) ((clone_args_t*)ARGS)

int son(void* _)
{
    __asm__ volatile(
        // "ld tp, 8(a0)\n"
        "ld t1, 0(a0)\n" // create_t * crea
        "ld sp, 32(a0)\n"
        "ld s0, 24(a0)\n"
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
        "ld ra, 16(a0)\n"
        "ld a0, 8(a0)\n"
        "ret\n");
}
