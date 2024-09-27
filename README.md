# README

## warning

不能使用 gcc, 要使用 clang, 可能是 gcc 对栈有一定的保护机制, 没法通过 \_\_builtin 来知道栈帧的位置(使用 gcc 会段错误)

## 用途

用于实现自动并行化

## Quick Start

```bash
# 请看 test/test4.c 和 makefile
make test4
```

## 未定义行为

```c
int _arr[10] = { 0 };
int* arr = _arr;

int ret1 = thrd_create(1);
if (ret1 == 0) { // 主线程
	arr[ret1] = 2;
	int ret2 = thrd_create(1);
	if (ret2 == 0) { // 主线程
		arr[ret2] = 5;
	} else {
		arr[ret2] = 7;
	}

} else { // 子线程 tid = 1
	arr[ret1] = 3;
	int ret2 = thrd_create(1);
	if (ret2 == 1) { // 子线程
		arr[ret2] = 11;
	} else {
		arr[ret2] = 13;
	}
}

thrd_join();
```

这是未定义行为。解释一下：thrd_join 会根据 当前的线程数量决定是否阻塞，
并且我们这里还会有 tid 的复用（因为 tid 是根据 线程数量 确定的）。
因此会出现一种情况，子线程 tid = 1 执行完了 --> thrd_join() ，cnt--，
然后导致 主线程的 if 分支中 thrd_create 创建出来的 tid=1，导致数据被复写了。

为什么会发生未定义：不能保证 子线程 thrd_join 与 父线程再次 thrd_create 的次序

## 示例

- [ ] b 不通过（错误示范）
- [ ] c 不通过（同上）
- [ ] d 不通过（未定义行为，错误示范）

- [x] thrd1 通过
- [x] thrd2 通过（范例）
- [x] thrd3 通过（范例）
- [x] thrd4 通过（范例）

## TODO

- [x] 简化 thrd_create 的实现, 其实只需要 clone 指向的函数用 asm 实现就行了
- [x] 不使用记录线程 id 的数组, 打算使用 tp(x4) 来记录线程的地址, 这是符合语义的
