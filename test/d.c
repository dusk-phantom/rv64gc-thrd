#include "thrd.h"

int main(void)
{
    int _arr[10] = { 11, 13, 17, 19, 23, 0 };

    int* arr = _arr;

    int ret1 = thrd_create(1);

    int ret2 = thrd_create(1);

    arr[ret1] = arr[ret1] * 5; // arr[1] 在这里 * 5^3

    arr[ret2] = arr[ret2] * 7; // arr[1] 在这里 * 7

    thrd_join();

    for (int i = 0; i < 10; i++) {
        printf("%d\n", arr[i]);
    }
}