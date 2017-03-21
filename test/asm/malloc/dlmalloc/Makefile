all:1.o malloc.o
	g++ -o 1 1.o malloc.o -ggdb3

malloc.o:
	g++ -o malloc.o -c malloc.cpp -ggdb3

1.o:
	g++ -o 1.o -c 1.cpp -ggdb3

clean:
	rm 1.o malloc.o 1

