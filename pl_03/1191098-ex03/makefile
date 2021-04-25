# ex03/Makefile
SOURCES = main.c
OBJFILES = main.o
EXEC = ex03

.SUFFIXES: .c .o

.c.o:
	gcc -Wall -g -c $<
	
${EXEC}: ${OBJFILES}
	gcc -Wall -g -o ${EXEC} ${OBJFILES} -lm -lrt
	
${OBJFILES}: ${SOURCES}

run: ${EXEC}
	./${EXEC}
	
clean:
	rm -f ${OBJFILES} ${EXEC}
