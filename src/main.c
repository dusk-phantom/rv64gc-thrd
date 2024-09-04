#include "thrd.h"

#include <stdio.h>

// void func()
// {
//     thrd_create(1);
//     void* sp;
//     void* s0;
//     __asm__ volatile(
//         "mv %0, sp\n"
//         "mv %1, s0\n"
//         : "=r"(sp), "=r"(s0)
//         :
//         :);
//     printf("main: sp=%p, s0=%p\n", sp, s0);

//     sleep(1);
// }

int main()
{

    int _a[10] = { 1, 2, 3 };
    int* a = _a;

    int id = thrd_create(2);

    a[id] = a[id] * 2;

    thrd_join();

    printf("%d, %d, %d\n", a[0], a[1], a[2]); // 2, 4, 6

    return 0;
}