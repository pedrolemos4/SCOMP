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
	int i;
	int status;
	
	if(pipe(fd)==-1){
		perror("Pipe failed");
		return 1;
	}

	
	for(i=0;i<10;i++){
		p = fork();
		
		if (p==0){
			close(fd[1]);
			read(fd[0],&str1,sizeof(str));
			
			printf("\nPid: %d\nWinning message: %s\nRound number: %d\n",
			getpid(),str1.mensagem,str1.n);
			close(fd[0]);
			
			exit(str1.n);
		}
	}
	
	for( i=0;i<10;i++){
			sleep(2);
			strcpy(str.mensagem, "Win");
			str.n = i;
			close(fd[0]);
			write(fd[1],&str,sizeof(str));
	}
	close(fd[1]);

	for(i=0; i<10; i++){
		pid_t pid = wait(&status);
		if(WIFEXITED(status)) {
			printf("PID: %d, exit value: %d\n", pid, WEXITSTATUS(status));
		}	
	}
	return 0;
}
