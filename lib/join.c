#include "thrd.h"
#include "ctx.h"

void thrd_join() {

  if (gettid() == getpid()) {
  retry:
    __sync_synchronize();
    if (live > 1) {
      goto retry;
    }

    // for (int i = 0; i < MAX_THREAD_NUM; i++) {
    //     if (stack_alloc[i] != 0) {
    //         free((void*)stack_alloc[i]);
    //     }
    // }
    return;
  } else {
    __sync_fetch_and_sub(&live, 1);
    __sync_synchronize();
    while (1) { // 子线程
      // fprintf(stderr, "before\n");
      syscall(SYS_exit, 0);
    }
  }
}