#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(int argc, char *argv[]){

	int fd[2];
	pid_t p;
	int c,i=0;
	
	char str[2000];
	char string1[2000];
	
	FILE *fp;
    char *filename = "ficheiro.txt";
    
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
		
	} else {
		close(fd[1]);
		read(fd[0],&string1,sizeof(str));
		printf("Filho leu %s.\n",string1);
		close(fd[0]);
	}

	return 0;
}
