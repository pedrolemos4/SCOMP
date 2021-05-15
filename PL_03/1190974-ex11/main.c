#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

#define ARRAY_SIZE 1000
#define HUNDRED 100

int main (int argc, char *argv[]){
	int aux,i,r,k,max,valor;
	int array[ARRAY_SIZE];
	pid_t pid;
	int fd[2];
	
	if(pipe(fd)==-1){
		perror("Pipe failed");
		return 1;
	}

	for(i=0;i<ARRAY_SIZE;i++){
		array[i] =  rand() % 1000;
	}
	
	for(k=0;k<10;k++){
		pid = fork();
		if(pid==0){
			max=0;
			for(r=0;r<HUNDRED;r++){
				if(max<array[k*HUNDRED+r]){
					max=array[k*HUNDRED+r];
				}
			}
			printf("Máximo da iteração %d é %d\n",k,max);
			close(fd[0]);
			write(fd[1],&max,sizeof(int));
			close(fd[1]);
			exit(0);
		}
	}
	
	for(k=0; k < 10; k++){
		wait(NULL);
	}
	
	max=0;
	close(fd[1]);
	for(i=0;i<10;i++){
		aux=read(fd[0],&valor,sizeof(int));
		if(valor>max){
			max = valor;
		}
	}
	printf("Máximo total é : %d\n",max);
	
	
	return 0;
}

