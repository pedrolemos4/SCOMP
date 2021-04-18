#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]){
	
	char string[255];
	char string2[255];
	int n,x;
	
	int fd[2];
	pid_t p;
	
	if(pipe(fd)==-1){
		perror("Pipe failed");
		return 1;
	}
	
	p=fork();
	
	if(p>0){
		close(fd[0]);
		printf("Insira a string:\n");
		fgets(string,255,stdin);
		printf("Introduza um número:\n");
		scanf("%d", &n);	
		write(fd[1],&string,sizeof(string));
		write(fd[1],&n,sizeof(n));
		close(fd[1]);
		wait(NULL);
	} else {
		close(fd[1]);
		read(fd[0],&string2,sizeof(string));
		read(fd[0],&x,sizeof(n));

		printf("\nO filho leu a string: %s\nO filho leu o número: %d\n", string2, x);
		close(fd[0]);
		
	}
	
	return 0;
}
