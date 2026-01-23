# ============================================================
# RISC-V 64位交叉编译配置
# ============================================================

TARGET  := riscv64-linux-gnu
CC      := clang --target=$(TARGET) --sysroot=$(RISCV_SYSROOT) -resource-dir $(CLANG_RESOURCE_DIR)
LD      := riscv64-unknown-linux-gnu-gcc  # 使用 GCC 链接（避免 sysroot 路径问题）
AR      := llvm-ar
QEMU    := qemu-riscv64 -L $(RISCV_SYSROOT)

CFLAGS  := -I inc -g -O2

# fork.c 编译时禁止使用 callee-saved 寄存器（s1-s11）
FFIXED  := $(addprefix -ffixed-,x9 x18 x19 x20 x21 x22 x23 x24 x25 x26 x27)

# ============================================================
# 目录和文件
# ============================================================

BUILD_DIR := build
LIB_SRCS  := son clone fork join thrd
LIB_OBJS  := $(addprefix $(BUILD_DIR)/,$(addsuffix .o,$(LIB_SRCS)))
LIB_OUT   := $(BUILD_DIR)/libthrd.a

# ============================================================
# 主要目标
# ============================================================

.PHONY: lib clean all

all: lib

lib: $(LIB_OUT)

$(LIB_OUT): $(LIB_OBJS)
	$(AR) rcs $@ $^

# ============================================================
# 库对象文件编译规则
# ============================================================

$(BUILD_DIR)/%.o: lib/%.c | $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

# fork.c 需要特殊的编译选项
$(BUILD_DIR)/fork.o: lib/fork.c | $(BUILD_DIR)
	$(CC) -c $< -o $@ $(CFLAGS) $(FFIXED)

$(BUILD_DIR):
	mkdir -p $@

# ============================================================
# 测试目标
# ============================================================

TESTS := test1 test2 test3 test4 test5 test6 test7 test8 b c d

.PHONY: $(TESTS)

# 模式规则：编译并运行测试
$(TESTS): %: lib | $(BUILD_DIR)
	$(LD) test/$*.c $(LIB_OUT) -I inc -o $(BUILD_DIR)/$* -g
	$(QEMU) $(BUILD_DIR)/$*

# ============================================================
# 清理
# ============================================================

clean:
	rm -rf $(BUILD_DIR)
