#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define ARRAY_SIZE 1000 

int main(void) {
	
 int numbers[ARRAY_SIZE]; 
 int status;
 int n; 
 int amountp=0;
 int amountf=0;
 time_t t; 
 int i;

 srand ((unsigned) time (&t));
 
 for (i = 0; i < ARRAY_SIZE; i++)
 numbers[i] = rand () % 10000;

 n = rand () % 10000; 

    pid_t p;
    p=fork();
    
    if(p==0){
		for(i=0;i<500;i++){
			if(numbers[i]==n){
				printf("a\n");
				amountf++;
			}
		}
		exit(1);
	}else{
		for(i=500;i<1000;i++){
			if(numbers[i]==n){
				printf("b\n");
				amountp++;
			}
		}
	}
    wait(&status);
    printf("N: %d\n",n);
    int total = amountp + amountf;
    printf("Total: %d\n",total);
    
    return 0;
}
