# note 与 记录了一些可能会用在 线程创建之中的东西

## pthread_create 的一些想法

```c
    unsigned flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND
        | CLONE_THREAD | CLONE_SYSVSEM | CLONE_SETTLS
        | CLONE_PARENT_SETTID | CLONE_CHILD_CLEARTID | CLONE_DETACHED（已经没有 detached 了）;
```

上面这些参数中，clone_thread + clone_vm + clone_sighand 是一定需要的（这在 man page 中写的一清二楚）

那么就还剩下：

clone_fs + clone_files + clone_sysvsem + clone_settls + clone_parent_settid + clone_child_cleartid

其中，clone_fs + clone_files + clone_sysvsem 是可以理解的：共享文件系统 + 共享 fd 表 + 共享信号灯。
当然我们已经 通过 futex 实现了 mtx，所以 共享信号灯可能是多余的。

剩下有：clone_settls + clone_parent_settid + clone_child_cleartid

### CLONE_SETTLS (since Linux 2.5.32)

The TLS (Thread Local Storage) descriptor is set to tls.

The interpretation of tls and the resulting effect is
architecture dependent. On x86, tls is interpreted as a
`struct user_desc *` (see set_thread_area(2)). On x86-64 it
is the new value to be set for the %fs base register (see
the ARCH_SET_FS argument to arch_prctl(2)). On
architectures with a dedicated TLS register, it is the new
value of that register.

Use of this flag requires detailed knowledge and generally
it should not be used except in libraries implementing
threading.

然后我查了一下 https://www.man7.org/linux/man-pages/man2/set_thread_area.2.html ，
看样子， riscv 是没有 set_thread_area 这个功能了（或者说 riscv 有更好的处理方式）

关于 riscv 的 tls 看这篇： https://maskray.me/blog/2021-02-14-all-about-thread-local-storage

下面是一个使用 pthread 库实现线程局部存储（TLS）的完整示例。在这个例子中，我们将创建几个线程，每个线程都有自己的线程局部存储（TLS）数据。
每个线程将修改其 TLS 数据，并将其打印出来以显示每个线程都有其独立的存储。

这个例子分为以下几个步骤：

1. 创建一个 TLS 键。
2. 每个线程将创建自己的 TLS 数据。
3. 修改并打印 TLS 数据以证明它是线程局部的。
4. 清理资源。

```c
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// 线程局部存储的键
pthread_key_t tlsKey;

// 线程的运行函数
void* threadFunc(void* arg) {
    // 为这个线程的TLS数据分配内存
    int* myData = (int*)malloc(sizeof(int));
    *myData = 0; // 初始值

    // 将数据与键关联
    pthread_setspecific(tlsKey, myData);

    // 修改TLS数据
    *myData = (int)arg;

    // 打印TLS数据
    printf("Thread %d: TLS data = %d\n", (int)arg, *myData);

    // 退出前不需要清理，因为有一个destructor函数
    return NULL;
}

// 当线程结束时调用的函数，用于释放TLS数据
void destructor(void* ptr) {
    free(ptr);
}

int main(void) {
    // 创建TLS键，并提供销毁函数
    pthread_key_create(&tlsKey, destructor);

    const int NUM_THREADS = 4;
    pthread_t threads[NUM_THREADS];

    // 创建线程
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, threadFunc, (void*)(i + 1));
    }

    // 等待所有线程完成
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // 清理TLS键
    pthread_key_delete(tlsKey);

    return 0;
}
```

这个程序展示了以下几个重要的概念：

- 使用`pthread_key_create`创建一个线程局部存储键，并提供一个销毁函数`destructor`，用于在线程结束时自动清理分配的内存。
- 在每个线程的运行函数`threadFunc`中，分配 TLS 数据并使用`pthread_setspecific`将数据与键关联。
- 使用`pthread_getspecific`获取线程局部存储的数据是不必要的，因为我们直接操作了指针`myData`。但在其他情况下，你可能需要通过`pthread_getspecific`来访问 TLS 数据。
- 程序结束前，使用`pthread_key_delete`删除 TLS 键，虽然在这个例子中，当`main`函数返回时程序就结束了，所有的资源（包括 TLS 键）都会被自动清理。

请确保在支持 pthread 的环境中编译和运行此代码，例如使用 gcc 或 clang，并链接 pthread 库（例如，在 Linux 上使用`gcc example.c -lpthread -o example`命令）。

### CLONE_PARENT_SETTID

- CLONE_PARENT_SETTID (since Linux 2.5.49)

  Store the child thread ID at the location pointed to by
  parent_tid (clone()) or cl_args.parent_tid (clone3()) in
  the parent's memory. (In Linux 2.5.32-2.5.48 there was a
  flag CLONE_SETTID that did this.) The store operation
  completes before the clone call returns control to user
  space.

`CLONE_PARENT_SETTID` 是 Linux 系统调用 `clone()` 的一个标志，用于在创建新线程或进程时的特殊行为配置中。当使用 `clone()` 创建一个新的进程或线程时，如果指定了 `CLONE_PARENT_SETTID` 标志，那么新创建的线程或进程的线程 ID（TID）会被写入到调用 `clone()` 时指定的父进程地址空间中的一个整数变量。

#### `CLONE_PARENT_SETTID` 详解

- **目的**: 允许父进程获取新创建线程或进程的线程 ID。
- **使用场景**: 这对于某些需要追踪或管理子线程 ID 的应用程序来说非常有用。
- **行为**: 在 `clone()` 调用中指定 `CLONE_PARENT_SETTID` 标志，并提供一个指向父进程地址空间中整数变量的指针，新的线程 ID 将被写入该地址。

#### 示例

以下是一个简单的示例，展示了如何在使用 `clone()` 时设置 `CLONE_PARENT_SETTID` 标志，并捕获新创建的线程 ID。

```c
#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

// 定义子线程的栈大小
#define STACK_SIZE (1024 * 1024)

// 子线程执行的函数
int childFunc(void *arg) {
    printf("Inside child thread\n");
    return 0;
}

int main() {
    char *stack;                   // 子线程的栈
    char *stackTop;                // 栈顶
    pid_t tid;                     // 用于存储新线程的ID

    // 为子线程分配栈空间
    stack = malloc(STACK_SIZE);
    if (stack == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    stackTop = stack + STACK_SIZE;  // 计算栈顶地址

    // 创建新线程
    pid_t child_pid = clone(childFunc, stackTop, CLONE_PARENT_SETTID | SIGCHLD, NULL, &tid);
    if (child_pid == -1) {
        perror("clone");
        exit(EXIT_FAILURE);
    }

    printf("clone() created a child thread with PID %ld\n", (long)child_pid);
    printf("Child thread TID: %ld\n", (long)tid);

    // 等待子线程结束
    waitpid(child_pid, NULL, 0);

    free(stack); // 释放栈空间
    return 0;
}
```

（个人想法，gpt 给的这个代码不一定是正确的）
但是我确实可以通过：让 parent settid 的方式来获取 tid

在这个示例中：

- 使用 `malloc()` 分配了一块内存作为子线程的栈。
- 通过 `clone()` 创建一个新线程，同时指定 `CLONE_PARENT_SETTID` 标志，并将 `tid` 的地址作为参数传递，这样新线程的线程 ID 就会被写入 `tid` 指向的变量。
- 输出创建的子线程的 PID 和通过 `tid` 获取的线程 ID。
- 使用 `waitpid()` 等待子线程结束，然后释放之前分配的栈空间。

注意，这个示例仅用于演示 `CLONE_PARENT_SETTID` 的用法，并假设你有一定的 C 语言和 Linux 系统编程的基础知识。在实际应用中，需要根据具体需求调整使用方式。

### CLONE

`CLONE_CHILD_CLEARTID` 标志用于在 `clone3` 调用中指定，当子进程退出时，其线程 ID（TID）将被清除，并且在指定的地址处产生一个 futex 唤醒操作。

下面是使用 `clone3` 系统调用并设置 `CLONE_CHILD_CLEARTID` 标志的一个示例。这个示例演示了如何创建一个新的线程，并且当这个线程结束时，自动清除其线程 ID，并在指定的内存地址上触发一个 futex 唤醒操作。这可以用于同步，等待线程结束。

为了使用 `clone3`，你需要在你的系统上有较新的 Linux 内核支持。以下示例代码假设你具有适当的编程背景和环境设置：

```c
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <linux/sched.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <linux/futex.h>
#include <sys/time.h>
#include <errno.h>
#include <stdatomic.h>

#define STACK_SIZE (1024 * 1024) // 分配1MB堆栈

// 子线程执行的函数
int childFunc(void *arg) {
    printf("Child thread is running\n");
    return 0;
}

// clone3 系统调用包装函数
int clone3(struct clone_args *cl_args, size_t size) {
    return syscall(__NR_clone3, cl_args, size);
}

int main() {
    // 分配子线程堆栈空间
    void *child_stack = malloc(STACK_SIZE);
    if (!child_stack) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    // 设置子线程结束时清除TID的内存地址
    pid_t child_tid;
    atomic_int clear_tid;
    clear_tid = 0;

    // 准备 clone 参数
    struct clone_args cl_args = {
        .flags = CLONE_CHILD_CLEARTID | CLONE_CHILD_SETTID | SIGCHLD,
        .pidfd = 0,
        .child_tid = (uint64_t)&child_tid,
        .parent_tid = 0,
        .exit_signal = SIGCHLD,
        .stack = (uint64_t)child_stack,
        .stack_size = STACK_SIZE,
        .tls = 0,
        .set_tid = 0,
        .set_tid_size = 0,
        .cgroup = 0,
        .child_tid = (uint64_t)&clear_tid,
    };

    // 创建子线程
    int ret = clone3(&cl_args, sizeof(cl_args));
    if (ret == -1) {
        perror("clone3");
        free(child_stack);
        exit(EXIT_FAILURE);
    }

    printf("clone3() created a child thread with TID %ld\n", (long)child_tid);

    // 等待子线程结束
    while (atomic_load(&clear_tid) != 0) {
        pause(); // 等待futex唤醒
    }

    printf("Child thread has terminated\n");

    free(child_stack);
    return 0;
}
```

在这个例子中，`clone3` 被用于创建一个新的线程，`CLONE_CHILD_CLEARTID` 标志与 `child_tid` 地址一起被设置，
以便在子线程退出时自动清除存储在 `child_tid` 指向的内存位置的线程 ID，并触发一个 futex 唤醒操作。
父进程通过检查 `clear_tid` 的值来等待子线程结束，这个值在子线程退出时会被设置为 0。

请注意，为了运行上述代码，你的系统需要支持 `clone3` 系统调用，这通常意味着你需要一个相对较新的 Linux 内核。此外，示例代码可能需要根据你的具体环境进行调整。

### 综上

个人感觉有用的一些标志：

```c
unsigned flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND
        | CLONE_THREAD | CLONE_SYSVSEM | CLONE_PARENT_SETTID | CLONE_CHILD_CLEARTID
```
