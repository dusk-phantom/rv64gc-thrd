CFLAGS += -I inc
CFLAGS += -g

# IMPORTANT: compiling the fork.c should not use the callee saved regs
FFIXED = -ffixed-x9 -ffixed-x18 -ffixed-x19 -ffixed-x20 -ffixed-x21 -ffixed-x22 -ffixed-x23 -ffixed-x24 -ffixed-x25 -ffixed-x26 -ffixed-x27

thrd: fork join
	clang lib/thrd.c -o build/thrd.o -c $(CFLAGS)

# fork 是不能使用一些 callee saved 的
fork: clone
	clang -o build/fork.o -c lib/fork.c $(CFLAGS) $(FFIXED)

clone: son
	clang -o build/clone.o -c lib/clone.c $(CFLAGS)

son:
	clang lib/son.c -o build/son.o -c -I inc $(CFLAGS)

join:
	clang lib/join.c -o build/join.o -c -g $(CFLAGS)

b: thrd 
	clang test/b.c build/fork.o build/clone.o build/thrd.o build/son.o build/join.o -o build/main $(CFLAGS) -o ./build/b
	./build/b
	
c: thrd 
	clang test/c.c build/fork.o build/clone.o build/thrd.o build/son.o build/join.o -o build/main $(CFLAGS) -o ./build/c
	./build/c

c: thrd 
	clang test/d.c build/fork.o build/clone.o build/thrd.o build/son.o build/join.o -o build/main $(CFLAGS) -o ./build/d
	./build/d

test1:
	clang test/test1.c build/fork.o build/clone.o build/thrd.o build/son.o build/join.o -o build/main $(CFLAGS) -o ./build/test1
	./build/test1

test2:
	clang test/test2.c build/fork.o build/clone.o build/thrd.o build/son.o build/join.o -o build/main $(CFLAGS) -o ./build/test2
	./build/test2

test3:
	clang test/test3.c build/fork.o build/clone.o build/thrd.o build/son.o build/join.o -o build/main $(CFLAGS) -o ./build/test3
	./build/test3

test4:
	clang test/test4.c build/fork.o build/clone.o build/thrd.o build/son.o build/join.o -o build/main $(CFLAGS) -o ./build/test4
	./build/test4
	
gdb: build/main
	gdb build/main
	
clean:
	rm -rf build/*
	