#include "thrd.h"

#include <stdio.h>

// int main() {

//   int _a[10] = {2, 3, 5};
//   int *a = _a;

//   int id = thrd_create(2);

//   // fprintf(stderr, "id=%d\n", id);

//   a[id] = a[id] * 2;

//   thrd_join();

//   fprintf(stdout, "%d, %d, %d\n", a[0], a[1], a[2]); // 2, 4, 6

//   return 0;
// }

// void func()
// {
//     thrd_create(1);
//     void* sp;
//     void* s0;
//     __asm__ volatile(
//         "mv %0, sp\n"
//         "mv %1, s0\n"
//         : "=r"(sp), "=r"(s0)
//         :
//         :);
//     printf("main: sp=%p, s0=%p\n", sp, s0);

//     sleep(1);
// }

int fib(int n) {
  if (n <= 1) {
    return n;
  }
  return fib(n - 1) + fib(n - 2);
}

int main(void) {
  // 并行,每个线程计算一个fib
  // int count[4] = { 14, 13, 12, 11 };
  int count[4] = {11, 12, 13, 14};
  int expects[4] = {fib(count[0]), fib(count[1]), fib(count[2]), fib(count[3])};
  int _results[4];
  volatile int *results = _results;

  int id = thrd_create(3);

  // 并行区域
  results[id] = fib(count[id]);

  // fprintf(stderr, "id=%d\n", id);

  // 等待所有线程执行完毕
  thrd_join();

  // 检查结果
  for (int i = 0; i < 4; i++) {
    printf("results[%d]=%d\n", i, results[i]);
    printf("expects[%d]=%d\n", i, expects[i]);
    if (results[i] != expects[i]) {
      printf("Error: results[%d] != expects[%d]\n", i, i);
      // exit(-1);
    }
  }
  return 0;
}

// int main()
// {

//     int _a[10] = { 1, 2, 3 };
//     int* a = _a;

//     int id = thrd_create(2);

//     a[id] = a[id] * 2;

//     thrd_join();

//     printf("%d, %d, %d\n", a[0], a[1], a[2]); // 2, 4, 6

//     return 0;
// }