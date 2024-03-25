#if 0
#include "my_thrd.h"
#else
int thrd_create();
#endif
#include <stdio.h>

int main(void)
{
    int ret = thrd_create();
    if (ret) {
        return -1;
    } else if (ret == 0) { // 子线程
        printf("child\n");
    } else { // 父线程
        printf("father: %d\n", ret);
    }
}