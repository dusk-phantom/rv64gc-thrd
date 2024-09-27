#include "thrd.h"
#include "ctx.h"

tid_t thrd_create(uint64_t num) {
  uint64_t ra;
  __asm__ volatile("mv %0, ra\n" : "=r"(ra) : :);

  if (gettid() != getpid()) { // 判断是否是主线程
    // 子线程返回 tid
    uint64_t tp;
    __asm__ volatile("mv %0, tp\n" : "=r"(tp) : :);
    return tp; // 这个是返回的子线程的 tid
  }

  if (num == 0 || num > MAX_THREAD_NUM) { // 判断 num 是否合法
    return 0;                             // 返回主线程的 tid
  }

  /* ---------- 保存上下文 callee saved ---------- */
  static create_t crea;
  __asm__ volatile("mv %0, s1\n" : "=r"(crea.s1) : :);

  __asm__ volatile("mv %0, s2\n" : "=r"(crea.s2) : :);

  __asm__ volatile("mv %0, s3\n" : "=r"(crea.s3) : :);

  __asm__ volatile("mv %0, s4\n" : "=r"(crea.s4) : :);

  __asm__ volatile("mv %0, s5\n" : "=r"(crea.s5) : :);

  __asm__ volatile("mv %0, s6\n" : "=r"(crea.s6) : :);

  __asm__ volatile("mv %0, s7\n" : "=r"(crea.s7) : :);

  __asm__ volatile("mv %0, s8\n" : "=r"(crea.s8) : :);

  __asm__ volatile("mv %0, s8\n" : "=r"(crea.s8) : :);

  __asm__ volatile("mv %0, s9\n" : "=r"(crea.s9) : :);

  __asm__ volatile("mv %0, s10\n" : "=r"(crea.s10) : :);

  __asm__ volatile("mv %0, s11\n" : "=r"(crea.s11) : :);

  uint64_t main_sp =
      (uint64_t)__builtin_frame_address(0); // 0: self_s0 == main_sp
  uint64_t main_size =
      (uint64_t)__builtin_frame_address(1) - main_sp; // 1: main_s0

  live_son = num; // 初始化线程数

  /* ---------- 创建线程 ---------- */
  for (int i = 1; i <= num; i++) {
    __thrd_create(&crea, main_sp, main_size, ra,
                  i); // 子线程直接返回到 main 了
  }

  return 0;
}
