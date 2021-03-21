#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define ARRAY_SIZE 2000

int main ()
{
	int numbers[ARRAY_SIZE], status,i,k,valor=0,index=255;
	for (i = 0; i < ARRAY_SIZE; i++)
		numbers[i] = rand () % 10000;
	int n = numbers[623];	 
	
	for(k=0;k<10;k++){
		pid_t p ;
		p=fork();
	
		if(p==0){
			for(i=valor;i<valor+200;i++){
				if(n==numbers[i]){
					index=i;
				}
			}
		valor=i;
		}else{
			exit(1);
		}
	}
	
	for(i=0;i<10;i++){
		wait(&status);
	}
	
	printf("Index= %d.\n",index);
	
	return 0;
} 
