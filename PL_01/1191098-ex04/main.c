#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void) {
	int a=0, b, c, d;
	b = (int) fork();
	c = (int) getpid(); /* getpid(), getppid(): unistd.h*/
	d = (int) getppid();
	a = a + 5;
	printf("\na=%d, b=%d, c=%d, d=%d\n",a,b,c,d);
	return 0;
} 
