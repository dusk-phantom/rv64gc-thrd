CFLAGS += -I inc
CFLAGS += -g -Ofast

# IMPORTANT: compiling the fork.c should not use the callee saved regs
FFIXED = -ffixed-x9 -ffixed-x18 -ffixed-x19 -ffixed-x20 -ffixed-x21 -ffixed-x22 -ffixed-x23 -ffixed-x24 -ffixed-x25 -ffixed-x26 -ffixed-x27

lib: fork join clone son thrd
	ar rcs libthrd.a build/fork.o build/clone.o build/thrd.o build/son.o build/join.o

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

b: lib
	clang test/b.c ./libthrd.a -I inc -o ./build/b -g
	./build/b
	
c: lib 
	clang test/c.c ./libthrd.a -I inc -o ./build/c -g
	./build/c

d: lib 
	clang test/d.c ./libthrd.a -I inc -o ./build/d -g
	./build/d

test1: lib
	clang test/test1.c ./libthrd.a -I inc -o ./build/test1 -g
	./build/test1

test2: lib
	clang test/test2.c ./libthrd.a -I inc -o ./build/test2 -g
	./build/test2

test3: lib
	clang test/test3.c ./libthrd.a -I inc -o ./build/test3 -g
	./build/test3

test4: lib
	clang test/test4.c ./libthrd.a -I inc -o ./build/test4 -g
	./build/test4

clean:
	rm -rf build/* ./libthrd.a
	