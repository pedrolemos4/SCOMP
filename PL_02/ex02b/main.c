#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define BUFFER_SIZE 80

typedef struct{
	int n;
	char mensagem[BUFFER_SIZE];
} struct1;

int main(int argc, char *argv[]){
	
	struct1 str;
	struct1 str1;
	
	int fd[2];
	pid_t p;
	
	if(pipe(fd)==-1){
		perror("Pipe failed");
		return 1;
	}
	
	p=fork();
	
	if(p>0){
		close(fd[0]);
		/* Introduzir o inteiro e a string */
		printf("Introduza uma string:\n");
		fgets(str.mensagem, BUFFER_SIZE, stdin);
		printf("Introduza um número:\n");
		scanf("%d", &(str.n));
		
		write(fd[1],&str,sizeof(str));
		close(fd[1]);
		wait(NULL);
	} else {
		close(fd[1]);
		
		read(fd[0],&str1,sizeof(str));
		
		printf("\nO filho leu a string: %s\nO filho leu o número: %d\n",
		str1.mensagem, str1.n);
		
		close(fd[0]);
	}


	return 0;
}
