#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "thrd.h"

int main()
{
    int a[3] = { 1, 2, 3 };
    int id = thrd_create();
    a[id] = a[id] * 2;
    thrd_join();

    printf("%d, %d, %d\n", a[0], a[1], a[2]); // 2, 4, 3

    return 0;
}