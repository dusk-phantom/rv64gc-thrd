#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

int _thrd_create(int stackSiz3);

/* static inline __attribute__((always_inline)) */ int thrd_create(int num)
{
    // long s0_sp_difference;
    // __asm__ volatile(
    //     "sub %0, s0, sp\n" // 计算s0与sp的差值
    //     : "=r"(s0_sp_difference) // 输出到变量
    //     :
    //     : "memory");

    // int id = 0;
    // int* p_num = &num;
    // while (num > 0) {
    //     if (id == 0) {
    //         id = _thrd_create(s0_sp_difference);
    //         if (id == -1) {
    //             exit(-1);
    //         } else if (id == 0) {
    //             // 主线程
    //             num--;
    //         } else {
    //             while (*p_num > 1) {
    //                 __asm__ volatile("nop");
    //             }
    //             return id;
    //         }
    //     }
    // }
    // return id;

    if (syscall(SYS_gettid) != syscall(SYS_getpid)) {
    }
}