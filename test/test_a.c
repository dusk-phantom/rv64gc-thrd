#include "../my_thrd.h"

int main()
{
    int a[2] = {1, 2};
    int id = thrd_create();
    a[id] = a[id] * 2;
    thrd_join();
    return a[0] + a[1];
}