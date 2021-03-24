#define ARRAY_SIZE 1000
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main ()
{
	int status;
	int numbers[ARRAY_SIZE]; /* array to lookup */
	int n; /* the number to find */
	int k;
	int i;
	int aux=0;
	
	time_t t; /* needed to init. the random number generator (RNG)*/

	/* intializes RNG (srand():stdlib.h; time(): time.h) */
	srand ((unsigned) time (&t));

	/* initialize array with random numbers (rand(): stdlib.h) */
	for (i = 0; i < ARRAY_SIZE; i++){
		numbers[i] = rand () % 50;
	}
	
	/* initialize n */
	n = 30;
	
	pid_t p;
	
	for (k = 0; k<10;k++){
		p=fork();
		if(p==0){
			for(i=k*200;i<200*k+200;i++){
				if(numbers[i]==n){
					exit(aux);
				}
				aux++;
			}
			exit(255);
		}
	}
	
	for(i=0;i<10;i++){
		wait(&status);;
		if(WEXITSTATUS(status) == 255) {
			printf("Número não encontrado\n");
		} else {
			printf("Número encontrado na posição %d\n", WEXITSTATUS(status));
		}
	}
	
	
	return 0 ;
}
