all:
	gcc -S hello.c -o hello.s
	as --gstabs hello.s -o hello.o
	ld hello.o -o hello
