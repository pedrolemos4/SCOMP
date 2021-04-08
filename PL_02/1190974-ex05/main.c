#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(){

	int up[2];
	int down[2];
	int i;
	char string[255], string1[255], string2[255];
	
	pid_t pid;
	
	if(pipe(up) == -1){
		perror("Pipe failed");
		return 1;
	}
	
	if(pipe(down) == -1){
		perror("Pipe1 failed");
		return 1;
	}
	
	pid = fork();
	
	if(pid>0){
		close(up[1]);
		read(up[0],&string1,sizeof(string));
		
		for (i = 0; string1[i]!='\0'; i++) {
			if(string1[i] >= 'a' && string1[i] <= 'z') {
				string1[i] = string1[i] - 32;
			} else if(string1[i] >= 'A' && string1[i] <= 'Z') {
				string1[i] = string1[i] + 32;
			}
		}
		
		close(up[0]);
		
		close(down[0]);
		write(down[1],&string1,sizeof(string1));
		close(down[1]);
		
	}else{
		printf("Insira a string:\n");
		fgets(string,255,stdin);
		
		close(up[0]);
		write(up[1],&string,sizeof(string));
		close(up[1]);
		
		close(down[1]);
		read(down[0],&string2,sizeof(string1));
		printf("String final: %s\n",string2);
		close(down[0]);
	}


	return 0;
}
