# rv64gc-thrd

一个轻量级的 RISC-V 64 位用户级线程库，用于实现自动并行化。

## 特性

- 基于 Linux `clone` 系统调用实现真正的并行执行
- 简洁的 fork-join 并行模型
- 自动栈复制，子线程继承父线程的栈内容
- 零依赖，仅需 glibc

## 环境要求

- **编译器**: Clang（不支持 GCC，详见[注意事项](#注意事项)）
- **目标平台**: RISC-V 64-bit (rv64gc)
- **运行环境**: Linux / QEMU

### 使用 Nix（推荐）

```bash
# 进入开发环境
nix develop

# 构建库
make lib

# 运行测试
make test4
```

### 手动配置

需要配置以下环境变量：
- `RISCV_SYSROOT`: RISC-V sysroot 路径
- `CLANG_RESOURCE_DIR`: Clang 资源目录（包含 `stddef.h` 等）

## API

```c
#include "thrd.h"

// 创建 n 个子线程
// 返回值：主线程返回 0，子线程返回 1~n
tid_t thrd_create(uint64_t n);

// 等待所有子线程结束
void thrd_join(void);
```

## 快速示例

```c
#include "thrd.h"
#include <stdio.h>

int main() {
    int _result[4];
    int *result = _result;

    // 创建 3 个子线程（加上主线程共 4 个）
    int id = thrd_create(3);

    // 并行区域：每个线程执行自己的任务
    result[id] = id * id;

    // 等待所有线程完成
    thrd_join();

    // 输出结果
    for (int i = 0; i < 4; i++) {
        printf("result[%d] = %d\n", i, result[i]);
    }
    // result[0] = 0, result[1] = 1, result[2] = 4, result[3] = 9

    return 0;
}
```

## 测试用例

| 测试  | 描述                        | 状态 |
| ----- | --------------------------- | ---- |
| test1 | 基本并行数组操作            | ✅    |
| test2 | 多次并行区域 + 二维数组求和 | ✅    |
| test3 | 并行矩阵求和                | ✅    |
| test4 | 并行递归（斐波那契）        | ✅    |
| test5 | 并行向量点积                | ✅    |
| test6 | 并行查找最大值              | ✅    |
| test7 | 多个顺序并行区域            | ✅    |
| test8 | 并行前缀和                  | ✅    |
| b, d  | 错误示范 / 未定义行为       | ❌    |

运行测试：

```bash
make test1
make test2
# ...
```

## 注意事项

### 必须使用 Clang

GCC 对栈有保护机制，无法通过 `__builtin_frame_address` 正确获取栈帧位置，会导致段错误。

### 指针必须使用中间变量

```c
// ❌ 错误：直接使用数组名
int arr[4];
arr[id] = id;  // 可能出错

// ✅ 正确：通过指针访问
int _arr[4];
int *arr = _arr;
arr[id] = id;  // 正确
```

原因：子线程的栈是从父线程复制的，数组在栈上的地址需要通过指针间接访问才能正确映射。

### 避免嵌套并行

子线程内部不应调用 `thrd_create`，这会导致未定义行为：

```c
int id1 = thrd_create(1);
if (id1 != 0) {
    // ❌ 子线程内部创建新线程 - 未定义行为
    int id2 = thrd_create(1);
}
```

### 线程数量限制

最大支持 10 个线程（包括主线程），可在 `lib/ctx.h` 中修改 `MAX_THREAD_NUM`。

## 实现原理

1. `thrd_create` 保存当前 callee-saved 寄存器（s1-s11）和栈帧信息
2. 使用 `clone` 系统调用创建子线程，并复制父线程的栈内容
3. 子线程从 `thrd_create` 的返回点继续执行，返回其 tid
4. 主线程返回 0
5. `thrd_join` 使用 futex 等待所有子线程完成

### 特殊编译要求

`lib/fork.c` 必须使用 `-ffixed-x9 -ffixed-x18 ... -ffixed-x27` 编译，禁止编译器使用 callee-saved 寄存器，否则编译器生成的代码会破坏手动保存的上下文。

## 项目结构

```
rv64gc-thrd/
├── inc/
│   └── thrd.h          # 公共头文件
├── lib/
│   ├── ctx.h           # 内部数据结构
│   ├── fork.c          # thrd_create 实现
│   ├── clone.c         # clone 包装
│   ├── son.c           # 子线程入口
│   ├── join.c          # thrd_join 实现
│   └── thrd.c          # 辅助函数
├── test/
│   └── test*.c         # 测试用例
├── Makefile
├── flake.nix           # Nix 开发环境
└── README.md
```

## License

Apache License 2.0
