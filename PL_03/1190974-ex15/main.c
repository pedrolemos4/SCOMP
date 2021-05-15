#include <stdio.h>
#include <sys/types.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mman.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <limits.h>
#include <string.h>

int main (int argc, char *argv[]){
	pid_t pid;
	int s2[10];
	int circularBuffer[10];
	int fd[2],i,k,j,number;
	
	if(pipe(fd)){
		printf("Erro a criar o pipe.\n");
		return 0;
	}
	
	pid=fork();
	
	//CORRIGIR INCREMENTAÇÃO 
	
	//produtor
	if(pid>0){
		for(i=0;i<3;i++){
			number=i*10;
			for(j=0;j<10;j++){
				circularBuffer[j]=number;
				number++;
			}
			close(fd[0]);
			write(fd[1],&circularBuffer,sizeof(circularBuffer));
			close(fd[1]);
		}
	//consumidor
	}else{
		for(k=0;k<3;k++){
			close(fd[1]);
			read(fd[0],&s2,sizeof(circularBuffer));
			for(j=0;j<10;j++){
				printf("O valor %d da %d estrutura é %d \n",j,k,s2[j]);
			}
			close(fd[0]);
		}
		exit(0);
	}
	
	
	return 0;
}

