#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]){
	
	char string[255];
	char string2[255];
	char string3[255];
	int i;
	int up[2];
	int down[2];
	pid_t p;
	
	if(pipe(up)==-1){
		perror("Pipe failed");
		return 1;
	}
	
	if(pipe(down)==-1){
		perror("Pipe failed");
		return 1;
	}
	
	p=fork();
	
	if(p>0){
		close(down[0]);
		close(up[1]);
		
		read(up[0],&string2,sizeof(string));
		for (i = 0; string2[i]!='\0'; i++) {
			if(string2[i] >= 'a' && string2[i] <= 'z') {
				string2[i] = string2[i] - 32;
			} else if(string2[i] >= 'A' && string2[i] <= 'Z') {
				string2[i] = string2[i] + 32;
			}
		}
		write(down[1],&string2,sizeof(string2));
		close(down[1]);
		close(up[0]);
	
	
	} else {
		close(down[1]);
		close(up[0]);
		printf("Insira a string:\n");
		fgets(string,255,stdin);
		write(up[1],&string,sizeof(string));
		read(down[0],&string3,sizeof(string2));
		printf("\nO cliente leu a string convertida: %s\n", string3);

		close(down[0]);
		close(up[1]);
			
	}
	
	return 0;
}
