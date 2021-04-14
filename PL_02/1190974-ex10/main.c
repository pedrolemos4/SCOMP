#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[]){

	int fd[2],fd1[2];
	pid_t pid;
	int numberF, numberC, childAprova, childBet, x, credit=20, newCredit;
	
	if(pipe(fd) == -1){
		perror("Pipe failed");
		return 1;
	}
	
	if(pipe(fd1) == -1){
		perror("Pipe failed");
		return 1;
	}
	
	time_t t; /* needed to init. the random number generator (RNG)*/

	/* intializes RNG (srand():stdlib.h; time(): time.h) */
	srand ((unsigned) time (&t));
	
	pid = fork();
	
	if(pid>0){
		close(fd[0]);
		close(fd1[1]);
		while(credit>0){
			numberF = (rand() % (5-1+1))+1;
			printf("Número pai : %d \n",numberF);
			if(credit>0){
				x=1;
			}else{
				x=0;
			}
			write(fd[1],&x,4);
			
			read(fd1[0],&childBet,4);
			if(childBet == numberF){
				credit += 10;
			}else{
				credit -= 5;
			}
			write(fd[1],&credit,4);
		}
		close(fd[1]);
		close(fd1[0]);
		
	}else{
		close(fd[1]);
		close(fd1[0]);
		while(credit!=0){
			read(fd[0],&childAprova,4);
			if(childAprova==0){
				close(fd[0]);
				close(fd1[1]);
				exit(0);
			}
			numberC = (rand() % (5-1+1))+1;
			printf("Número filho : %d \n",numberC);
			write(fd1[1],&numberC,4);
			
			read(fd[0],&newCredit,4);
			printf("New Credit: %d \n", newCredit);
		}
		close(fd1[1]);
		close(fd[0]);
	}
	
	
	return 0;
}
