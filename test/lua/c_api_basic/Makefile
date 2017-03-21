INC=-I/home/nemo/code/big-test/lua-5.1/src
LIB=-L/home/nemo/code/big-test/lua-5.1/src -llua -ldl
all: 1

1:1.o
	g++ -ggdb3 -o $@ 1.o $(LIB) 

1.o:1.cpp
	g++ -ggdb3 -c -o $@ $< $(INC) 

clean:
	rm *.o 
	rm 1

