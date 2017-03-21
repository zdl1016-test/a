all:intrument.o
	g++ -O -g -finstrument-functions test.cpp instrument.o -o test
	./test
	pvtrace test
	#dot -Tjpg graph.dot -o graph.jpg
	ls

intrument.o:
	gcc -g -finstrument-functions -c instrument.c -o instrument.o

clean:
	rm instrument.o test trace.txt graph.dot
