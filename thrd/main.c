#if 0
#include "my_thrd.h"
#else
int thrd_create();
#endif
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void)
{
    int arr[2];
    // fork();
    int ret = thrd_create();
    if (ret == -1) { // 错误
        return -1;
    } else if (ret == 0) { // 子线程

        arr[0] = 20;

    } else { // 父线程

        arr[1] = 100;

        sleep(1);
    }

    printf("%d\n", arr[1] + arr[0]);
}