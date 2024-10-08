// FIXME 这种也是未定义行为

#include "thrd.h"

int main(void)
{

    int s0_sp_difference;
    __asm__ volatile(
        "sub %0, s0, sp\n" // 计算s0与sp的差值
        : "=r"(s0_sp_difference) // 输出到变量
        :
        : "memory");
    printf("s0 - sp = %d\n", s0_sp_difference);

    int _arr[2];
    int* arr = _arr;
    // fork();

    int sum = 0;

    int mtx1 = mtx_create();

    int ret1 = thrd_create(1);
    if (ret1 == -1) { // 错误
        return -1;
    } else if (ret1 == 0) { // 主线程

        arr[0] = 20;

        for (int i = 0; i < 100; i++) {
            mtx_lock(mtx1);
            sum++;
            mtx_unlock(mtx1);
        }

        int ret2 = thrd_create(1);
        if (ret2 == -1) { // 错误
            return -1;
        } else if (ret2 == 0) { // 主线程
            for (int i = 0; i < 100; i++) {
                mtx_lock(mtx1);
                sum++;
                mtx_unlock(mtx1);
            }
        } else {
            // 另一个子线程
            for (int i = 0; i < 100; i++) {
                mtx_lock(mtx1);
                sum++;
                mtx_unlock(mtx1);
            }
        }

    } else { // 子线程 tid = 1

        arr[1] = 100;

        for (int i = 0; i < 100; i++) {
            mtx_lock(mtx1);
            sum++;
            mtx_unlock(mtx1);
        }

        int ret2 = thrd_create(1);
        if (ret2 == -1) { // 错误
            return -1;
        } else if (ret2 == 1) { // 子线程
            for (int i = 0; i < 100; i++) {
                mtx_lock(mtx1);
                sum++;
                mtx_unlock(mtx1);
            }
        } else {
            // 理论上不会执行这一段
            for (int i = 0; i < 100; i++) {
                mtx_lock(mtx1);
                sum++;
                mtx_unlock(mtx1);
            }
        }
    }

    thrd_join();

    printf("father thread pid=%d\n", getpid());

    printf("%d\n", arr[1] + arr[0]);
    printf("sum=%d\n", sum);
}