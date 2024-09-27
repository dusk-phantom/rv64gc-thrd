#include "thrd.h"
#include "ctx.h"

void thrd_join() {
  if (gettid() == getpid()) {
  retry:
    __sync_synchronize();
    if (live_son > 0) {
      goto retry;
    }
    for (int i = 0; i < MAX_THREAD_NUM; i++) {
      if (args_vec[i].crea != NULL) {
        free((void *)args_vec[i].__alloc);
        args_vec[i].crea = NULL;
      }
    }
    return;
  } else {
    __sync_fetch_and_sub(&live_son, 1);
    __sync_synchronize();
    // fprintf(stderr, "tp=%d\n", tp);
    while (1) { // 子线程
      __asm__ volatile("li a7, 93\n"
                       "ecall\n");
    }
  }
}