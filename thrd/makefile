# 指定编译器
CC=gcc
# 编译时添加的选项，-g 添加调试信息
CFLAGS=-g

# 目标程序名
TARGET=build/thrd

# 默认目标
all: $(TARGET)

# src 列表
SRCS=main.c thrd_create.s  thrd.c mtx.c

# 编译规则
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# 清理编译生成的文件
clean:
	rm -f $(TARGET)

asm:
	$(CC) -S my_thrd.c

# 运行程序的目标
run: $(TARGET)
	./$(TARGET)

# 使用GDB调试的目标
debug: $(TARGET)
	gdb -ex "b main" -ex "tui enable" -ex "layout regs" -ex "run" $(TARGET)
