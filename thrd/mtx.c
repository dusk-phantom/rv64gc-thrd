#define _GNU_SOURCE

#include <err.h>
#include <linux/futex.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/syscall.h>
#include <unistd.h>

#include "fhq.h"
#include "my_thrd.h"

#define CAS_strong(p_obj, p_expected, desired) __atomic_compare_exchange_n(p_obj, p_expected, desired, 0, __ATOMIC_SEQ_CST, __ATOMIC_SEQ_CST)
#define ATOMIC_STORE_strong(p_obj, val) __atomic_store_n(p_obj, val, __ATOMIC_SEQ_CST)

inline static int futex(uint32_t* uaddr, int futex_op, uint32_t val, const struct timespec* timeout, uint32_t* uaddr2, uint32_t val3)
{
    return syscall(SYS_futex, uaddr, futex_op, val, timeout, uaddr2, val3);
}

bool _try_lock(uint32_t* futexp)
{
    uint32_t expected = 0;
    // 使用原子比较并交换操作尝试设置互斥锁的值为1
    return CAS_strong(futexp, &expected, 1);
}

void _lock(uint32_t* futexp)
{
    // 如果直接尝试获取锁失败，则进入等待
    while (!_try_lock(futexp)) {
        // syscall(SYS_futex, futex_val, FUTEX_WAIT, 1, NULL, NULL, 0);
        futex(futexp, FUTEX_WAIT, 1, NULL, NULL, 0);
    }
}

static void _unlock(uint32_t* futexp)
{
    ATOMIC_STORE_strong(futexp, 0);
    futex(futexp, FUTEX_WAKE, 1, NULL, NULL, 0);
}

/**
 * @brief 锁列表
 *
 */
static Node* _root = NULL;

int mtx_create()
{
    /* 局部 static 状态 */
    static int _cnt = 1; // 锁的编号 从 1 开始

    uint32_t* futexp = malloc(sizeof(uint32_t));
    // *futex = 1; // 设置为有效
    ATOMIC_STORE_strong(futexp, 0);
    _root = insert(_root, _cnt, futexp);

    return _cnt++;
}

// 对指定编号的互斥锁在 共享区 上锁
void mtx_lock(int mtx)
{
    Node* node = search(_root, mtx);
    if (node == NULL) {
        abort();
    }
    _lock(node->value);
}

// 对指定编号的互斥锁在 共享区 解锁
void mtx_unlock(int mtx)
{
    Node* node = search(_root, mtx);
    if (node == NULL) {
        abort();
    }
    _unlock(node->value);
}

void destroy_mtxs()
{
    freeTree(_root);
}
