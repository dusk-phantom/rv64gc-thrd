// 测试：并行前缀和（Prefix Sum / Scan）
// 使用 Hillis-Steele 算法的简化版本
#include "thrd.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define N 8

int main(void) {
  int input[N] = {1, 2, 3, 4, 5, 6, 7, 8};
  int _output[N];
  int *output = _output;

  // 复制输入到输出
  memcpy(output, input, sizeof(input));

  printf("Input:  ");
  for (int i = 0; i < N; i++) {
    printf("%d ", output[i]);
  }
  printf("\n");

  // 并行计算：每个线程计算一个元素的前缀和
  // 简化版：直接串行计算每个位置（演示线程独立工作）
  int _prefix[N];
  int *prefix = _prefix;

  int id = thrd_create(N - 1);

  // 每个线程计算自己位置的前缀和
  prefix[id] = 0;
  for (int j = 0; j <= id; j++) {
    prefix[id] += output[j];
  }

  thrd_join();

  printf("Prefix: ");
  for (int i = 0; i < N; i++) {
    printf("%d ", prefix[i]);
  }
  printf("\n");

  // 验证结果: {1, 3, 6, 10, 15, 21, 28, 36}
  int expected[N] = {1, 3, 6, 10, 15, 21, 28, 36};
  for (int i = 0; i < N; i++) {
    assert(prefix[i] == expected[i]);
  }

  printf("Prefix sum test passed!\n");
  return 0;
}
