#include "thrd.h"
#include "ctx.h"

// 只有主线程能进入这个函数, 子线程创建出来以后, 不用管这个函数, 直接返回到 main
tid_t __thrd_create(const create_t *crea, uint64_t main_sp, uint64_t main_size,
                    uint64_t main_ra,
                    tid_t tid /* 指定: 新创建出来的线程的 id */) {

  /* ---------- 分配栈 ---------- */
  void *lo;
  if (posix_memalign(&lo, 16, STACK_SIZE) != 0) {
    abort();
  }
  uint64_t hi = (uint64_t)lo + STACK_SIZE;

  /* ---------- main ---------- */
  memcpy((void *)(hi - main_size), (void *)(main_sp), main_size);

  args_vec[tid].crea = crea;
  args_vec[tid].thrd_create_next = main_ra;
  args_vec[tid].chld_thrd_s0 = hi; // data race
  args_vec[tid].chld_thrd_sp = hi - main_size;
  args_vec[tid].tid = tid;
  args_vec[tid].__alloc = lo;

  // 子线程直接返回到 main
  clone(son, (void *)hi, FLAGS, (void *)&args_vec[tid]);

  return 0; // 主线程返回 0
}
