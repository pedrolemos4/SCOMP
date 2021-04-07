#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]){

	int x;
	int fd[2];
	
	pid_t p;
	pid_t p1;
	
	if(pipe(fd)==-1){
		perror("Pipe failed");
		return 1;
	}
	
	p=fork();
	
	if(p>0){
		close(fd[0]);
		p1=getpid();
		printf("Pid do pai: %d\n",p1);
		write(fd[1],&p1,sizeof(p));
		close(fd[1]);
		wait(NULL);
	} else {
		close(fd[1]);
		while((p = read(fd[0],&x,sizeof(p))!=0)){
			printf("Consegui ler do pipe o pid do pai: %d\n",x);
		}		
		close(fd[0]);
	}
	
	return 0;
}
