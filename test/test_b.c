#include <assert.h>
#include <stdlib.h>

int thrd_fork(void);

int mtx_create(void);
void mtx_lock(int mtx);
void mtx_unlock(int mtx);

// TODO: 测试mtx
int main()
{
    int sum = 0;
    int id = thrd_fork();

    int mtx = mtx_create();

    switch (id) {
    case -1:
        // 错误情况
        abort();
    case 0:
        // 父线程
        for (int i = 0; i < 100; i++) {
            mtx_lock(mtx);
            sum++;
            mtx_unlock(mtx);
        }
    default:
        // 子线程
        for (int i = 0; i < 100; i++) {
            mtx_lock(mtx);
            sum++;
            mtx_unlock(mtx);
        }
    }

    assert(sum == 200);
}