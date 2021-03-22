#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define ARRAY_SIZE 1000

int main ()
{
	int numbers[ARRAY_SIZE], max, i, k, valor=0;
	for(i = 0 ; i < ARRAY_SIZE ; i++){
		numbers[i] = rand() % 256;
	}
	
	for(k=0;k<5;k++){
		pid_t p ;
		p=fork();
	
		if(p==0){
			for(i=valor;i<valor+200;i++){
				if(numbers[i]>max){
					max = numbers[i];
				}
			}
		valor=i;
		exit(max);
		}
	}
	
	for(i=0;i<5;i++){
		int status;
		pid_t p1 = wait(&status);
		if(WEXITSTATUS(status)){
			printf("Processo %d retornou o valor máximo de %d.\n",p1,WEXITSTATUS(status));
		}
		
	}
	
	return 0;
} 
