#define ARRAY_SIZE 1000
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>


int main(int argc, char *argv[]){

	int i,k,readN;
	int tmp=0;
	int soma=0;
	int c = ARRAY_SIZE/5;
	int vec1[ARRAY_SIZE];
	int vec2[ARRAY_SIZE];
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
		vec1[i] = rand () % 50;
		vec2[i] = rand () % 50;
	}
	
	for(k=0;k<5;k++){
		p=fork();
		
		if(p==0){
			for(i=k*c;i<k*c + c; i++){
				tmp += vec1[i] + vec2[i];
			}
			close(fd[0]);
			write(fd[1],&tmp,sizeof(tmp));
			close(fd[1]);
			exit(0);
		}
	}
	
	for(i=0;i<5;i++){
		close(fd[1]);
		read(fd[0], &readN, sizeof(readN));
		soma += readN;
	}
	
	close(fd[0]);
	printf("Resultado final: %d\n", soma);
	printf("A função read apenas é executada quando existe informação no descritor,\
	daí não ser necessária a utilzação da função wait para efeitos de sincronização.\n");
	
	
	return 0;
	
}
