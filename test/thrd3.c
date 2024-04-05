#include <assert.h>
#include <memory.h>
#include <stdio.h>

#include "thrd.h"

// 矩阵并行计算过程1, 并行求和
void matrix_sum(int* matrix, int col, int row, int* sum)
{
    // 创建暂存列和 的数组
    int* sums = (int*)malloc(sizeof(int) * col);
    memset(sums, 0, sizeof(int) * col);

    //  创建col-1个线程, 那么总共 就有 col 个线程
    int tid = thrd_create(col - 1);
    // 求和
    for (int j = 0; j < row; j++) {
        sums[tid] += matrix[tid * row + j];
    }
    // 等待所有线程执行完毕
    thrd_join();

    // 累加所有线程的求和结果
    for (int i = 0; i < col; i++) {
        *sum += sums[i];
    }
    free(sums);
}

int main()
{
    // 定义个3行4列的矩阵
    int matrix[3][4] = {
        { 1, 2, 3, 4 },
        { 5, 6, 7, 8 },
        { 9, 10, 11, 12 }
    };
    // 定义一个求和结果
    int sum = 0;
    // 并行求和
    matrix_sum((int*)matrix, 3, 4, &sum);
    // 打印结果
    printf("sum=%d\n", sum);
    assert(sum == 78);
    return 0;
}