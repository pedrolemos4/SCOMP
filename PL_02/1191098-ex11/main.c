#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[]){

	pid_t p;
	int numProcesses = 5;
	int numPipes = 6;
	int fd[numPipes * 2];
	int k,i,num=0;
	int numFilho,readN,bigN;

	time_t t;
	srand ((unsigned) time (&t));

	for(i = 0; i < numPipes; i++) {
		if(pipe(fd + i * 2) == -1) {
			perror("Pipe failed\n");
			return 1;
		}
	}
	
	for(k=0;k<numProcesses;k++){
		p=fork();
		if(p==0){
			close(*(fd + k * 2 + 1)); 
			srand((unsigned)time(&t) ^ getpid());
			numFilho = (rand() %(500)) + 1;
			printf("PID: %d = %d\n", getpid(), numFilho);
			read(*(fd + k * 2),&readN,4);
			close(*(fd + k * 2)); 
			if(readN >= numFilho){
				num = readN;
			} else {
				num = numFilho;
			}
			
			close(*(fd + (k+1) * 2));
			write(*(fd + (k+1) * 2 + 1),&num,4);
			close(*(fd + (k+1) * 2 + 1));
			exit(0);
		}
	}
		
	close(fd[0]);
	srand((unsigned)time(&t) ^ getpid());
	int randomNumber = (rand () % 500) + 1;
	printf("PAI Random number: %d, pid: %d\n", randomNumber, getpid());
	write(fd[1], &randomNumber, sizeof(randomNumber));
	close(fd[1]);
			
	close(*(fd + 5*2 +1));
	read(*(fd + 5*2), &bigN, 4);
	printf("\nGreatest random number: %d\n", bigN);
	close(*(fd + 5*2));
	
	return 0;
}
