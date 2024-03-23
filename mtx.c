#define _GNU_SOURCE

#include <err.h>
#include <errno.h>
#include <linux/futex.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

#define CAS_strong(p_obj, p_expected, desired) __atomic_compare_exchange_n(p_obj, p_expected, desired, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)

static int
futex(uint32_t* uaddr, int futex_op, uint32_t val,
    const struct timespec* timeout, uint32_t* uaddr2, uint32_t val3)
{
    return syscall(SYS_futex, uaddr, futex_op, val,
        timeout, uaddr2, val3);
}

/* Acquire the futex pointed to by 'futexp': wait for its value to
   become 1, and then set the value to 0. */

void mtx_lock(uint32_t* futexp)
{
    long s;
    const uint32_t one = 1;

    /* atomic_compare_exchange_strong(ptr, oldval, newval)
       atomically performs the equivalent of:

           if (*ptr == *oldval)
               *ptr = newval;

           返回 true ， 如果 *ptr == *oldval
       It returns true if the test yielded true and *ptr was updated. */

    while (1) {

        /* Is the futex available? */
        // 如果 *futexp==1 ， 那么直接返回，并将 futexp 设置为0 ，相当于是设置 mutex flag，设置为上锁状态
        // if (atomic_compare_exchange_strong(futexp, &one, 0)) {
        if (CAS_strong(futexp, &one, 0)) {
            break; /* Yes */
        }

        /* Futex is not available; wait. */

        // 当 *futexp==0 的时候陷入等待
        s = futex(futexp, FUTEX_WAIT, 0, NULL, NULL, 0);
        if (s == -1 && errno != EAGAIN) {
            err(EXIT_FAILURE, "futex-FUTEX_WAIT");
        }
    }
}

/* Release the futex pointed to by 'futexp': if the futex currently
   has the value 0, set its value to 1 and then wake any futex waiters,
   so that if the peer is blocked in fwait(), it can proceed. */

void mtx_unlock(uint32_t* futexp)
{
    long s;
    const uint32_t zero = 0;

    /* atomic_compare_exchange_strong() was described
       in comments above. */

    // 如果已经 *futexp==0 ，相当于是 上锁状态，这个时候 通知一个线程解锁
    if (CAS_strong(futexp, &zero, 1)) {
        s = futex(futexp, FUTEX_WAKE, 1, NULL, NULL, 0);
        if (s == -1) {
            err(EXIT_FAILURE, "futex-FUTEX_WAKE");
        }
    }
}