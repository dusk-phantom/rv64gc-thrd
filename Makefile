# RISC-V 64位交叉编译配置
TARGET = riscv64-linux-gnu
CC = clang --target=$(TARGET) --sysroot=$(RISCV_SYSROOT) -resource-dir $(CLANG_RESOURCE_DIR)
# 使用 RISC-V GCC 进行链接（避免 sysroot 路径问题）
LD = riscv64-unknown-linux-gnu-gcc
AR = llvm-ar
QEMU = qemu-riscv64 -L $(RISCV_SYSROOT)

CFLAGS += -I inc
CFLAGS += -g -O2

# IMPORTANT: compiling the fork.c should not use the callee saved regs
FFIXED = -ffixed-x9 -ffixed-x18 -ffixed-x19 -ffixed-x20 -ffixed-x21 -ffixed-x22 -ffixed-x23 -ffixed-x24 -ffixed-x25 -ffixed-x26 -ffixed-x27

lib: fork join clone son thrd
	$(AR) rcs libthrd.a build/fork.o build/clone.o build/thrd.o build/son.o build/join.o

thrd: fork join
	$(CC) lib/thrd.c -o build/thrd.o -c $(CFLAGS)

# fork 是不能使用一些 callee saved 的
fork: clone
	$(CC) -o build/fork.o -c lib/fork.c $(CFLAGS) $(FFIXED)

clone: son
	$(CC) -o build/clone.o -c lib/clone.c $(CFLAGS)

son:
	$(CC) lib/son.c -o build/son.o -c -I inc $(CFLAGS)

join:
	$(CC) lib/join.c -o build/join.o -c -g $(CFLAGS)

b: lib
	$(LD) test/b.c ./libthrd.a -I inc -o ./build/b -g
	$(QEMU) ./build/b
	
c: lib 
	$(LD) test/c.c ./libthrd.a -I inc -o ./build/c -g
	$(QEMU) ./build/c

d: lib 
	$(LD) test/d.c ./libthrd.a -I inc -o ./build/d -g
	$(QEMU) ./build/d

test1: lib
	$(LD) test/test1.c ./libthrd.a -I inc -o ./build/test1 -g
	$(QEMU) ./build/test1

test2: lib
	$(LD) test/test2.c ./libthrd.a -I inc -o ./build/test2 -g
	$(QEMU) ./build/test2

test3: lib
	$(LD) test/test3.c ./libthrd.a -I inc -o ./build/test3 -g
	$(QEMU) ./build/test3

test4: lib
	$(LD) test/test4.c ./libthrd.a -I inc -o ./build/test4 -g
	$(QEMU) ./build/test4

clean:
	rm -rf build/* ./libthrd.a
	