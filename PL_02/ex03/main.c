#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(){

	int fd[2];
	int status;
	char string []= "Hello World!";
	char gdb []= "Goodbye!";
	char string1[255],string2[255];
	
	pid_t pid, p;
	
	if(pipe(fd) == -1){
		perror("Pipe failed");
		return 1;
	}
	
	pid = fork();
	
	if(pid>0){
		close(fd[0]);
		write(fd[1],&string,sizeof(string));
		write(fd[1],&gdb,sizeof(gdb));
		close(fd[1]);
		
		p = waitpid(pid,&status,0);
		if(WIFEXITED(status)){
			printf("Parent: child %d returned %d\n", p, WEXITSTATUS(status));
		}
	}else{
		close(fd[1]);
		read(fd[0],&string1,sizeof(string));
		read(fd[0],&string2,sizeof(gdb));
		printf("\nString1: %s, String2: %s\n",string1, string2);
		close(fd[0]);
		exit(1);
	}


	return 0;
}
