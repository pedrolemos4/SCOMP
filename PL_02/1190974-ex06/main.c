#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[]){

	int fd[2];
	int i, k, tmp = 0, read1, total = 0;
	int vec1[1000];
	int vec2[1000];
	int duz = 200;
	
	pid_t pid;
	
	if(pipe(fd) == -1){
		perror("Pipe failed");
		return 1;
	}
	
	time_t t; /* needed to init. the random number generator (RNG)*/

	/* intializes RNG (srand():stdlib.h; time(): time.h) */
	srand ((unsigned) time (&t));

	for (i = 0; i < 1000; i++){
		vec1[i] = rand () % 50;
		vec2[i] = rand () % 50;
	}
	
	for(i=0;i<5;i++){
		pid = fork();
	
		if(pid==0){
			for(k=i*duz;k<i*duz+duz;k++){
				tmp += vec1[k] + vec2[k];
			}
			close(fd[0]);
			write(fd[1],&tmp,sizeof(tmp));
			close(fd[1]);
			exit(0);
		}
	}
	
	for(k=0;k<5;k++){
		close(fd[1]);
		read(fd[0],&read1,sizeof(read1));
		total += read1;
		close(fd[0]);
	}
	
	printf("Total : %d\n",total);
	
	return 0;
}
