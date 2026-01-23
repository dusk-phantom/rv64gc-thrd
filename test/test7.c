// 测试：多个顺序并行区域
#include "thrd.h"
#include <assert.h>
#include <stdio.h>

#define N 4

int main(void) {
  int _arr[N];
  int *arr = _arr;

  // 第一个并行区域：初始化
  int id1 = thrd_create(N - 1);
  arr[id1] = id1 + 1; // arr = {1, 2, 3, 4}
  thrd_join();

  printf("After init: ");
  for (int i = 0; i < N; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");

  // 第二个并行区域：平方
  int id2 = thrd_create(N - 1);
  arr[id2] = arr[id2] * arr[id2]; // arr = {1, 4, 9, 16}
  thrd_join();

  printf("After square: ");
  for (int i = 0; i < N; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");

  // 第三个并行区域：加 10
  int id3 = thrd_create(N - 1);
  arr[id3] = arr[id3] + 10; // arr = {11, 14, 19, 26}
  thrd_join();

  printf("After add 10: ");
  for (int i = 0; i < N; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");

  // 验证结果
  assert(arr[0] == 11);
  assert(arr[1] == 14);
  assert(arr[2] == 19);
  assert(arr[3] == 26);

  printf("All assertions passed!\n");
  return 0;
}
