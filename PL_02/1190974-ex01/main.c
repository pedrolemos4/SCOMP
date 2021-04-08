#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(){

	int fd[2],aux;
	pid_t pid, pid_pai;
	
	if(pipe(fd) == -1){
		perror("Pipe failed");
		return 1;
	}
	
	pid = fork();
	
	if(pid>0){
		pid_pai = getppid();
		printf("O pid do pai no pai= %d.\n",pid_pai);
		close(fd[0]);
		write(fd[1],&pid_pai,sizeof(pid));
		close(fd[1]);
		wait(NULL);
	}else{
		close(fd[1]);
		while((pid=read(fd[0],&aux,80))!=0){
			printf("Após ler do pipe= %d. \n",aux);
		}
		close(fd[0]);
	}


	return 0;
}
