#define ARRAY_SIZE 1000

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>


int main(int argc, char *argv[]){
	
	int c = ARRAY_SIZE/5;
	int vec1[ARRAY_SIZE];
	int vec2[ARRAY_SIZE];
	int finalResult[ARRAY_SIZE];
	int numPipes=5;
	int fd[numPipes*2];
	int i,k,readN,j,a;
	
	time_t t; /* needed to init. the random number generator (RNG)*/

	/* intializes RNG (srand():stdlib.h; time(): time.h) */
	srand ((unsigned) time (&t));

	for (i = 0; i < ARRAY_SIZE; i++){
		vec1[i] = rand () % 50;
		vec2[i] = rand () % 50;
	}
	
	pid_t p;
	
	for(i = 0; i < numPipes; i++) {
		if(pipe(fd + i * 2) == -1) {
			perror("Pipe failed\n");
			return 1;
		}
	}
	

	for (i = 0; i < ARRAY_SIZE; i++){
		vec1[i] = rand () % 50;
		vec2[i] = rand () % 50;
	}
	
	for(k=0;k<5;k++){
		p=fork();
		if(p==0){
			close(*(fd + k * 2)); /* fechar extremidade de leitura */
			for(i=k*c;i<k*c + c; i++){
				int tmp = vec1[i] + vec2[i];
				if(i<10){
					printf("Tmp: %d\nI: %d\n",tmp,i);
				}
				write(*(fd + k * 2 + 1), &tmp, 4);	/* enviar número para o pai */
			}
			
			close(*(fd + k * 2 + 1)); /* fechar extremidade de escrita */
			exit(0);
		} 
	}
	
	for (j = 0; j < 5; j++) {
			close(*(fd+j*2+1));
			for(a = 0; a < 200; a++){
				read(*(fd + j*2), &readN, 4);
				finalResult[(j*200) + a] = readN;
			}
			close(*(fd + k *2));
		
	}

	for (i = 0; i < 10; i++){
		printf("Result[%d] = %d\n", i, finalResult[i]);
    }
	
	return 0;
}
