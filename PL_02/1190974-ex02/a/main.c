#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(){

	int fd[2];
	int n;
	int n1;
	char string[255];
	char string1[255];
	
	pid_t pid;
	
	if(pipe(fd) == -1){
		perror("Pipe failed");
		return 1;
	}
	
	pid = fork();
	
	if(pid>0){
		printf("Insira a string:\n");
		fgets(string,255,stdin);
		printf("Introduza um número:\n");
		scanf("%d", &n);
		close(fd[0]);
		write(fd[1],&string,sizeof(string));
		write(fd[1],&n,sizeof(n));
		close(fd[1]);
		wait(NULL);
	}else{
		close(fd[1]);
		read(fd[0],&string1,sizeof(string));
		read(fd[0],&n1,sizeof(n));
		printf("\nString %s\n Integer %d\n",string1, n1);
		close(fd[0]);
	}


	return 0;
}
