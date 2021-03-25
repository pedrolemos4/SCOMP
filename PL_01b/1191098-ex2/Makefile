main2: main2.o
	gcc main2.o -o main2

main2.o: main2.c 
	gcc -g -Wall -c main2.c -o main2.o

run: main2
	./main2
	
clean:
	rm *.o main2
