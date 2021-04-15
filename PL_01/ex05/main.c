#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
	pid_t p;
	int status;
	int valor;
	
	int i;
	for(i=0;i<2;i++){
		p = fork();
		if(p<=0){
			sleep(i+1);
			exit(i+1);
		}
	}
	
	for(i=0;i<2;i++){
		wait(&status);
		valor = WEXITSTATUS(status);
		printf("%d\n",valor);
	}
	return 0;
}
