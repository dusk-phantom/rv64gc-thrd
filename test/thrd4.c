// 用来测试线程栈

#include <stdio.h>
#include <stdlib.h>

#include "thrd.h"

// 首先定义一个函数用于递归计算,递归则会占用栈空间
int fib(int n)
{
    if (n <= 1) {
        return n;
    }
    return fib(n - 1) + fib(n - 2);
}

int main(void)
{
    // 并行,每个线程计算一个fib
    int count[4] = { 10, 12, 13, 14 };
    int expects[4] = { fib(count[0]), fib(count[1]), fib(count[2]), fib(count[3]) };
    int results[4];
    int id;
    for (int i = 0; i < 4; i++) {
        // FIXME 没有等所有的线程创建完
        id = thrd_create();
        if (id < 0) {
            // 异常情况
            exit(-1);
        } else if (id != 0) {
            break;
        }
        // 主线程继续创建线程
    }

    // 并行区域
    results[id] = fib(count[id]);

    // 等待所有线程执行完毕
    thrd_join();

    // 检查结果
    for (int i = 0; i < 4; i++) {
        printf("results[%d]=%d\n", i, results[i]);
        printf("expects[%d]=%d\n", i, expects[i]);
        if (results[i] != expects[i]) {
            printf("Error: results[%d] != expects[%d]\n", i, i);
            // exit(-1);
        }
    }
    return 0;
}
