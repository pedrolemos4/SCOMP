#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


int main(void){
	int x = 1;
	pid_t p = fork(); 	
	if(p == 0){
		x = x+1;
		printf("1. x = %d\n", x);
	} else {
		x = x-1;
		printf("2. x = %d\n", x);
	}
	printf("3. %d; x = %d\n", p, x);
	return 0;
}
