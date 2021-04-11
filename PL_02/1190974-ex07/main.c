#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[]){

	int fd[10]; //2 descritores para cada pipe
	int i, k, read1, total[1000];
	int vec1[1000];
	int vec2[1000];
	int DUZ = 200;
	
	pid_t pid;
	
	for(i=0;i<5;i++){
		if(pipe(fd+i*2) == -1){
			perror("Pipe failed");
			return 1;
		}
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
			close(*(fd+i*2));
			for(k=i*DUZ;k<i*DUZ+DUZ;k++){
				int tmp = vec1[k] + vec2[k];
				write(*(fd+i*2+1),&tmp,sizeof(int));
			}
			close(*(fd+i*2+1));
			exit(0);
		}
	}
	
	for(k=0;k<5;k++){
		close(*(fd+k*2+1));
		for(i=0;i<DUZ;i++){
			read(*(fd+k*2),&read1,sizeof(read1));
			total[(DUZ*k)+i] = read1;
		}
		close(*(fd+k*2));
	}
	
	for (i = 0; i < 10; i++){
		printf("Final[%d] = %d\n", i, total[i]);
    }
	
	return 0;
}
