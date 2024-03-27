# 定义编译器和汇编器
CC = gcc
AS = $(CC)

# 定义编译和链接选项
CFLAGS = -Wall -fPIC -Iinc
LDFLAGS = -shared

# 定义源代码和构建目录
SRC_DIR = src
BUILD_DIR = build

# 自动找到源代码目录中的源文件
SRCS = $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/*.s)

# 将源文件列表转换为目标目录中的对象文件列表
OBJS = $(patsubst $(SRC_DIR)/%,$(BUILD_DIR)/%,$(SRCS:.c=.o))
OBJS := $(OBJS:.s=.o)

# 目标库的名称
TARGET_LIB = $(BUILD_DIR)/libthrd.so

# 默认目标
build: $(TARGET_LIB)

# 规则：如何从源文件生成目标库
$(TARGET_LIB): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

# 规则：如何从C源文件生成对象文件
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

# 规则：如何从汇编源文件生成对象文件
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s
	@mkdir -p $(@D)
	$(AS) $(CFLAGS) -c $< -o $@

# 清理编译生成的文件
clean:
	rm -rf $(BUILD_DIR)

# 防止make误解释文件名
.PHONY: all clean
