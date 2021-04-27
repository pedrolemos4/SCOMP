#define ARRAY_SIZE 1000
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>


int main(int argc, char *argv[]){

	int i,k,readN;
	int globalMax=0;
	int c = ARRAY_SIZE/10;
	int vec1[ARRAY_SIZE];
	int fd[2];
	pid_t p;
	
	if(pipe(fd)==-1){
		perror("Pipe failed");
		return 1;
	}

	time_t t; /* needed to init. the random number generator (RNG)*/

	/* intializes RNG (srand():stdlib.h; time(): time.h) */
	srand ((unsigned) time (&t));

	for (i = 0; i < ARRAY_SIZE; i++){
		vec1[i] = rand () % 5000 + 1;
	}
	
	for(k=0;k<10;k++){
		p=fork();
		
		if(p==0){
			int max = 0;
			for(i=k*c;i<k*c + c; i++){
				if(vec1[i]>max){
					max = vec1[i];
				}
			}
			printf("Max na %d iteração: %d\n",k,max);
			close(fd[0]);
			write(fd[1],&max,sizeof(max));
			close(fd[1]);
			exit(0);
		}
	}
	
	close(fd[1]);
	for(i=0;i<10;i++){
		read(fd[0], &readN, sizeof(readN));
		if(readN>globalMax){
			globalMax=readN;
		}
	}
	close(fd[0]);
	
	printf("O máximo global é: %d\n",globalMax);
	
	return 0;
}
