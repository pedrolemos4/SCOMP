#define ARRAY_SIZE 1000
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main ()
{
	int status;
	int f_half=0;
	int s_half=0;
	int numbers[ARRAY_SIZE]; /* array to lookup */
	int n; /* the number to find */
	time_t t; /* needed to init. the random number generator (RNG)
*/
	int i;

	/* intializes RNG (srand():stdlib.h; time(): time.h) */
	srand ((unsigned) time (&t));

	/* initialize array with random numbers (rand(): stdlib.h) */
	for (i = 0; i < ARRAY_SIZE; i++){
		numbers[i] = rand () % 10000;
	}

	/* initialize n */
	n = rand () % 10000;
	pid_t p ;
	p=fork();
	
	if(p>0){
		for(i=0; i <500; i++){
			if(numbers[i] == n){
				printf("a\n");
				f_half++;
			}
		}
	} else {
		for(i = 500 ; i < 1000; i++ ){
			if(numbers[i] == n){
				printf("b\n");
				s_half++;
			}
		}
		exit(1);
	}
	wait(&status);
	printf("N = %d\n",n);
	int total;
	total = f_half + s_half;
	printf("Total = %d\n", total);
		
	return 0;
} 
