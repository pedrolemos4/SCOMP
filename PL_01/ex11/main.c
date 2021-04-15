#define ARRAY_SIZE 1000
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define ARRAY_NUM_LIMIT 255

int main(){

	int i, array[ARRAY_SIZE],result[ARRAY_SIZE];
	int c = ARRAY_SIZE/5;
	int k ;
	int status;
	int max;
	for(i = 0; i < ARRAY_SIZE; i++) {
		array[i] = rand() % ARRAY_NUM_LIMIT+1; //rand() % range + 1
	}
	pid_t p;
	
	for(k=0;k<5;k++){
		p=fork();
		if(p==0){
			max =0;
			for(i=k*c;i<k*c + c; i++){
				if(array[i]>max){
					max=array[i];
				}
			}
			exit(max);
		}
	}
	
	for(i=0;i<5;i++){
		pid_t pid = wait(&status);
		if(WIFEXITED(status)){
			printf("O processo %d devolveu o valor máximo: %d\n\n", pid, WEXITSTATUS(status));
		}
		if(WEXITSTATUS(status) > max){
				max = WEXITSTATUS(status);
		}
	}
	pid_t pid1 = fork();
	if(pid1 == 0){
		for(i= 0; i < ARRAY_SIZE/2; i++){
			result[i]=((int) array[i]/max)*100;
			printf("Indice: %d --- Array: %d --- Result: %d\n", i, array[i], result[i]);
		}
	}
	else{
		wait(&status);
		for(i= ARRAY_SIZE/2; i < ARRAY_SIZE; i++){
			result[i]=((int) array[i]/max)*100;
			printf("Indice: %d --- Array: %d --- Result: %d\n", i, array[i], result[i]);
		}
	}
	
	return 0 ;
}
