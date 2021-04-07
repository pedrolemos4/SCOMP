#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]){

	int fd[2];
	pid_t p;
	int status;
	
	char string[] = "Hello World\n";
	char string1[] = "Goodbye!\n";
	char string2[255];
	char string3[255];
	
	if(pipe(fd)==-1){
		perror("Pipe failed");
		return 1;
	}
	
	p=fork();
	
	if(p>0){
		close(fd[0]);
		write(fd[1],&string,sizeof(string));
		write(fd[1],&string1,sizeof(string1));
		close(fd[1]);
		
		pid_t pid = wait(&status);
		if(WIFEXITED(status)) {
			printf("PID: %d, exit value: %d\n", pid, WEXITSTATUS(status));
		}	
		
	} else {
		close(fd[1]);

		read(fd[0],&string2,sizeof(string));
		read(fd[0],&string3,sizeof(string1));
		
		printf("\n%s\n",string2);
		printf("%s\n",string3);
		close(fd[0]);
		sleep(50);
		exit(1);
	}

	return 0;
}
