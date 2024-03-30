#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <threads.h>

// 使用 thread_local 关键字声明线程局部变量
thread_local int counter = 0;

void* thread_function(void* arg)
{
    counter += (int)(intptr_t)arg;
    printf("Thread %d, Counter: %d\n", (int)(intptr_t)arg, counter);
    return NULL;
}

int main()
{
    pthread_t thread1, thread2;

    // 创建两个线程，分别传递不同的参数
    pthread_create(&thread1, NULL, thread_function, (void*)(intptr_t)1);
    pthread_create(&thread2, NULL, thread_function, (void*)(intptr_t)2);

    // 等待线程结束
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
