## 用途

用于实现自动并行化

## 实现线程库技术路线

- c 代码嵌入 riscv 汇编代码编译得到完全.s 的 risc 代码 / 完全直接用 riscv 汇编代码实现
- 使用 clone 系统调用
- 使用读写屏障相关 riscv 指令

实现了锁，锁通过 futex 实现的。

线程创建还有些操蛋的地方：

几个问题：

1. 可能在外部访问库中的 tids、tmp_mem，当然我也没有公开这种访问方式
2. 不要用 switch，用了 switch 一定要 break，用 if-else ，这是我真的觉得操蛋的地方，可以看看 test/test_b.c 与 test/test_c.c
