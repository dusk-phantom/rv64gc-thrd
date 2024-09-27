// 测试多次使用创建并行区域的情况

#include <stdio.h>
#include <assert.h>

#include "thrd.h"

void test_parrallel_region() {
  int a[4][3] = {
      {1, 2, 3},
      {4, 5, 6},
      {7, 8, 9},
      {10, 11, 12},
  };
  int tmp[4] = {0, 0, 0, 0};
  int *sum = tmp;

  // 创建并行区域
  // 创建3个线程 (然后包括主线程就有4个线程了)
  int id = thrd_create(3);
  // 每个线程执行对应 二维数组的一个维度的求和
  for (int i = 0; i < 3; i++) {
    sum[id] += a[id][i];
  }
  // 等待所有线程执行完毕
  thrd_join();

  // 累加所有线程的求和结果
  int total = 0;
  for (int i = 0; i < 4; i++) {
    printf("sum[%d]=%d\n", i, sum[i]);
    total += sum[i];
  }
  printf("total=%d\n", total);
  assert(total == 78);
}

int main() {
  test_parrallel_region();
  return 0;
}