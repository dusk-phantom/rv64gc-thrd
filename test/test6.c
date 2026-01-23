// 测试：并行查找最大值
#include "thrd.h"
#include <assert.h>
#include <stdio.h>

#define N 16
#define NUM_THREADS 4

int main(void) {
  int arr[N] = {3, 14, 15, 9, 26, 53, 58, 97, 93, 23, 84, 62, 64, 33, 83, 27};

  int _local_max[NUM_THREADS];
  int *local_max = _local_max;

  int id = thrd_create(NUM_THREADS - 1);

  // 每个线程找自己区域的最大值
  int chunk = N / NUM_THREADS;
  int start = id * chunk;
  int end = start + chunk;

  local_max[id] = arr[start];
  for (int i = start + 1; i < end; i++) {
    if (arr[i] > local_max[id]) {
      local_max[id] = arr[i];
    }
  }

  thrd_join();

  // 串行找全局最大值
  int global_max = local_max[0];
  for (int i = 1; i < NUM_THREADS; i++) {
    printf("local_max[%d] = %d\n", i, local_max[i]);
    if (local_max[i] > global_max) {
      global_max = local_max[i];
    }
  }

  printf("global max = %d\n", global_max);
  assert(global_max == 97);

  return 0;
}
