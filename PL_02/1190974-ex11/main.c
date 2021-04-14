#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

#define MAIOR 500;

int main(int argc, char *argv[]){

	int fd[12];
	pid_t pid;
	int i, n, highestF, number, max, maior=0;
	
	for(i=0;i<5;i++){
		if(pipe(fd+i*2) == -1){
			perror("Pipe failed");
			return 1;
		}
	}
	
	time_t t;
	
	for(i=0;i<5;i++){
		pid = fork();
		 if(pid==0){
			 close(*(fd+i*2+1));
			 srand ((unsigned) time(&t)^ getpid());
			 n = (rand() % 500)+1;
			 printf("PID: %d Numero: %d \n",getpid(),n);
			 read(*(fd+i*2),&number,4);
			 close(*(fd+i*2));
			 if(number>=n){
				 maior=number;
			 }else{
				 maior=n;
			 }
			 close(*(fd+(i+1)*2));
			 write(*(fd+(i+1)*2+1), &maior, 4);
			 close(*(fd+(i+1)*2+1));
			 exit(0);
		 }
	}
	
	close(fd[0]);
	srand ((unsigned) time(&t)^ getpid());
	highestF = (rand() % 500)+1;
	printf("Pai numero: %d \n",highestF);
	write(fd[1],&highestF,sizeof(highestF));
	close(fd[1]);
	
	close(*(fd + 4*2 +1));
	read(*(fd + 4*2),&max,4);
	printf("Máximo: %d \n",max);
	close(*(fd + 4*2));
	
	return 0;
}
