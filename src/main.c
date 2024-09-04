#include "thrd.h"

#include <stdio.h>

void func()
{
    thrd_create(1);
    void* sp;
    void* s0;
    asm volatile(
        "mv %0, sp\n"
        "mv %1, s0\n"
        : "=r"(sp), "=r"(s0)
        :
        :);
    printf("main: sp=%p, s0=%p\n", sp, s0);
}

int main(void)
{
    func();
    return 0;
}