/**
 * @file thrd_fork.c
 * @author your name (you@domain.com)
 * @brief 假设不会子线程中再创建 子线程
 * @version 0.1
 * @date 2024-03-23
 *
 * @copyright Copyright (c) 2024
 *
 */

#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <linux/sched.h> /* Definition of struct clone_args */
#include <sched.h>
#include <sys/syscall.h>
#include <unistd.h>

/**
 * @brief threads[0] 表示 cnt ，有多少个线程，当然也可以表示：最后一个线程的下标
 * 假设是：全局静态变量初始化为 0
 * threads[1] 用来表示 主线程
 * 最多 20 个线程，应该不会需要太多
 *
 */
#define THREADS_num 20
static pid_t tids[THREADS_num];

// 这个可以用于 join
static uint32_t futexs[THREADS_num];

int arr[2];

int dummy(void* regs)
{
    // 我会在这里进行一些寄存器的保存工作
    arr[1] = 10;
    free(regs);
    return 0;
}

int thrd_create()
{
    if ((int)tids[0] == 0) {
        tids[++tids[0]] = syscall(SYS_gettid);
    }
    if (tids[1] != syscall(SYS_gettid)) {
        // perror("not main thread\n");
        return -1;
    } // 只有主线程才会走下来

    // Allocate stack for child task.
    static const uint64_t STACK_SIZE = 65536;
    void* stack;
    posix_memalign(&stack, 16, STACK_SIZE); // 注意内存对齐
    if (!stack) {
        // perror("posix_memalign stack error\n");
        return -1;
    }
    char* stack_top = (char*)stack + STACK_SIZE; // 指向栈顶

    void* regs = malloc(8 * 12); // 有 12 个 s 寄存器需要暂存
    if (regs == NULL) {
        // perror("malloc regs error\n");
        return -1;
    }

    int flags
        = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD | CLONE_SYSVSEM | CLONE_CHILD_CLEARTID
        | CLONE_CHILD_SETTID | CLONE_PARENT_SETTID;
    // 先这样吧，我也不知道后面会不会再用上 clone_parent_settid

    // 就直接认为这个 clone 会成功吧，如果失败的话，也就是 ret < 0 ，fatal 错误

    int cnt = ++tids[0];
    int ret = clone(dummy, stack_top, flags, regs, &tids[cnt], NULL, &futexs[cnt]);

    if (ret < 0) {
        // 创建失败
        return -1;
    } else if (ret == 0) {
        // 也就是说，子线程确实不会进入到这里
        arr[0] = 40;
        return 0; // 子线程
    } else {
        arr[0] = 20;
        sleep(1);
        return cnt;
    }
}

#if 1

int main(void)
{
    thrd_create();
    printf("arr[0]=%d\n", arr[0]);
    printf("arr[1]=%d\n", arr[1]);
    return 0;
}

#endif
