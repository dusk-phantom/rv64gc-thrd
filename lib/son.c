#include "thrd.h"
#include "ctx.h"

int son(void *_) {
  __asm__ volatile("ld t1, 0(a0)\n" // create_t * crea
                   "ld s0, 16(a0)\n"
                   "ld sp, 24(a0)\n"
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
                   "ld ra, 8(a0)\n"
                   "ld a0, 32(a0)\n"
                   "mv tp, a0\n"
                   "ret\n");
}
