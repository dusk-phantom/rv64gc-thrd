// FIXME 这种操作是未定义行为

#include "thrd.h"
#include <stdio.h>

int main(void) {
  int _arr[10] = {0};
  int *arr = _arr;

  int ret1 = thrd_create(1);
  if (ret1 == -1) { // 错误
    return -1;
  } else if (ret1 == 0) { // 主线程
    arr[ret1] = 2;
    int ret2 = thrd_create(1);
    if (ret2 == -1) { // 错误
      return -1;
    } else if (ret2 == 0) { // 主线程
      arr[ret2] = 5;
    } else {
      arr[ret2] = 7;
    }

  } else { // 子线程 tid = 1
    arr[ret1] = 3;
    int ret2 = thrd_create(1);
    if (ret2 == -1) { // 错误
      return -1;
    } else if (ret2 == 1) { // 子线程
      arr[ret2] = 11;
    } else {
      arr[ret2] = 13;
    }
  }

  thrd_join();

  for (int i = 0; i < sizeof(_arr) / sizeof(int); i++) {
    printf("$%d = %d\n", i, arr[i]);
  }
}