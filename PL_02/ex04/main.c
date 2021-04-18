#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define MAXCHAR 2000

int main(int argc, char *argv[]){

	int i=0,c;
	int fd[2];
	pid_t p;
	
	FILE *fp;
    char str[MAXCHAR];
    char str1[MAXCHAR];
    char *filename = "teste.txt";
    
    fp = fopen(filename, "r");
    
    if (fp == NULL){
        printf("Could not open file %s",filename);
        return 1;
    }
    
    while(1) {
			c = fgetc(fp);
			if(feof(fp)) {
				break;
			}
			str[i] = c;
			i++;
		}
    
    fclose(fp);
	

	if(pipe(fd)==-1){
		perror("Pipe failed");
		return 1;
	}
	
	p=fork();
	
	if(p>0){
		close(fd[0]);
		write(fd[1],&str,sizeof(str));
		close(fd[1]);
		wait(NULL);
	} else {
		close(fd[1]);
		read(fd[0],&str1,sizeof(str));
		printf("Filho leu: \n%s\n", str1);
		close(fd[0]);
	}
	
	return 0;
}
