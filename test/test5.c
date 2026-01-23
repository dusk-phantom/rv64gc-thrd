// 测试：并行向量点积
#include "thrd.h"
#include <assert.h>
#include <stdio.h>

#define N 8

int main(void) {
  int a[N] = {1, 2, 3, 4, 5, 6, 7, 8};
  int b[N] = {8, 7, 6, 5, 4, 3, 2, 1};

  // 每个线程计算两个元素的乘积
  int _partial[4];
  int *partial = _partial;

  int id = thrd_create(3);

  // 每个线程处理 2 个元素
  int start = id * 2;
  partial[id] = a[start] * b[start] + a[start + 1] * b[start + 1];

  thrd_join();

  // 串行累加
  int dot = 0;
  for (int i = 0; i < 4; i++) {
    printf("partial[%d] = %d\n", i, partial[i]);
    dot += partial[i];
  }

  printf("dot product = %d\n", dot);
  // 1*8 + 2*7 + 3*6 + 4*5 + 5*4 + 6*3 + 7*2 + 8*1 = 8+14+18+20+20+18+14+8 = 120
  assert(dot == 120);

  return 0;
}
