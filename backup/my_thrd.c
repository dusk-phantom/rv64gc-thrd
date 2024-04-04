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

#include "thrd.h"

int dummy()
{
    tids[5] = 23;
    tmp_mem[10] = 'a';
    return 0;
}

int _thrd_create(long stackSize)
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
    posix_memalign(&stack, 16, STACK_SIZE);
    if (stack == NULL) {
        // perror("posix_memalign stack error\n");
        return -1;
    }
    char* stack_top = (char*)stack + STACK_SIZE; // 指向栈顶

    int flags
        = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD | CLONE_SYSVSEM | CLONE_CHILD_CLEARTID
        | CLONE_CHILD_SETTID | CLONE_PARENT_SETTID;
    // 就直接认为这个 clone 会成功吧，如果失败的话，也就是 ret < 0 ，fatal 错误

    int cnt = ++tids[0];
    int ret = clone(dummy, stack_top, flags, NULL, &tids[cnt], NULL, NULL);

    return cnt;
}

int thrd_fork_d()
{

    return 0;
}
