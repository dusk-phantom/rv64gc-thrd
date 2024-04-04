#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "thrd.h"

int main()
{
    // int s0_sp_difference;
    // __asm__ volatile(
    //     "sub %0, s0, sp\n" // 计算s0与sp的差值
    //     : "=r"(s0_sp_difference) // 输出到变量
    //     :
    //     : "memory");
    // printf("s0 - sp = %d\n", s0_sp_difference);

    int a[100] = { 1, 2, 3 };
    // int* b = a;

    int id = thrd_create();

    // b[id] = b[id] * 2;
    a[id] = a[id] * 2;

    thrd_join();

    printf("%d, %d, %d\n", a[0], a[1], a[2]); // 2, 4, 3

    return 0;
}