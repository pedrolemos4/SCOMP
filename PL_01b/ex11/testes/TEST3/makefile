#a simple makefile
prog_test: main.o
	gcc main.o -o prog_test


main.o: main.c
	gcc -g -Wall -c main.c -o main.o

clean:
	rm -f *.o prog_test

run: prog_test
	./prog_test
