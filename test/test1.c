#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "thrd.h"

int main() {
  int _a[10] = {1, 2, 3};
  int *a = _a;

  int id = thrd_create(2);

  a[id] = a[id] * 2;

  thrd_join();

  printf("%d, %d, %d\n", a[0], a[1], a[2]); // 2, 4, 6

  return 0;
}