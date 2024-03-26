#define _GNU_SOURCE
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <linux/sched.h> /* Definition of struct clone_args */
#include <sched.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "thrd.h"

/**
 * @brief 返回我们自己的 tid
 *
 * @return int
 */
#if 0
int _get_tid(void)
{
    pid_t pid = gettid(); // 得到当前线程真正的 tid
    int cnt = tids[0];
    int i = cnt;
    while (i > 0) {
        if (tids[i] == pid) {
            return i;
        }
        i--;
    }

	return -1;
}
#else
int _get_tid(void)
{
    pid_t pid = gettid();
    for (int i = tids[tids[0]]; i > 0; i--) {
        if (tids[i] == pid) {
            return i;
        }
    }
    return -1;
}
#endif

int thrd_join(void)
{
    pid_t tid = _get_tid();
    printf("%d\n", tid);
    while (1) {
        syscall(SYS_exit, 0);
    }
    return tid;
}
