CFLAGS += -I inc
CFLAGS += -g

# fork 是不能使用一些 callee saved 的
fork: clone
	clang -o build/fork.o -c lib/fork.c $(CFLAGS)

clone: son
	clang -o build/clone.o -c lib/clone.c $(CFLAGS) -ffixed-x9 -ffixed-x18 -ffixed-x19 -ffixed-x20 -ffixed-x21 -ffixed-x22 -ffixed-x23 -ffixed-x24 -ffixed-x25 -ffixed-x26 -ffixed-x27

thrd: fork
	clang lib/thrd.c -o build/thrd.o -c $(CFLAGS)

son:
	clang lib/son.c -o build/son.o -c $(CFLAGS)

run: thrd
	clang src/main.c build/fork.o build/clone.o build/thrd.o build/son.o -o build/main $(CFLAGS)
	./build/main
	
clean:
	rm -rf build/*
	