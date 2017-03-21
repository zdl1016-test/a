INC=-I/home/nemo/code/big-test/lua-5.1/src
LIB=-L/home/nemo/code/big-test/lua-5.1/src -llua -ldl
OBJS=array.o
TARGET=array.so

all:$(TARGET)

$(TARGET):$(OBJS) Makefile
	g++ -ggdb3 -o $@ -fPIC -shared $(OBJS) $(LIB) 

%.o:%.c
	g++ -ggdb3 -c -o $@ $< $(INC) 

%.o:%.cpp
	g++ -ggdb3 -c -o $@ $< $(INC) 

clean:
	rm $(OBJS) 
	rm $(TARGET)

