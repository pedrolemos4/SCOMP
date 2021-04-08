#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

typedef struct{
	int n;
	char string[255];
} struct0;

int main(int argc, char *argv[]){
	
	struct0 s1;
	struct0 s2;
	
	int fd[2];
	
	pid_t pid;
	
	if(pipe(fd) == -1){
		perror("Pipe failed");
		return 1;
	}
	
	pid = fork();
	
	if(pid>0){
		printf("Insira a string:\n");
		fgets(s1.string,255,stdin);
		printf("Introduza um número:\n");
		scanf("%d", &(s1.n));
		close(fd[0]);
		write(fd[1],&s1,sizeof(s1));
		close(fd[1]);
		wait(NULL);
	}else{
		close(fd[1]);
		read(fd[0],&s2,sizeof(s1));
		printf("\nString %s\n Integer %d\n",s2.string, s2.n);
		close(fd[0]);
	}


	return 0;
}
