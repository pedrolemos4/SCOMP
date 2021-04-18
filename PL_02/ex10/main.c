#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <time.h>

int main(int argc, char *argv[]){

	int up[2];
	int down[2];
	pid_t p;
	int creditos = 20;
	int betFilho,nCreditos;
	int x =1;
	int betPai;
	
	time_t t; /* needed to init. the random number generator (RNG)*/

	/* intializes RNG (srand():stdlib.h; time(): time.h) */
	srand ((unsigned) time (&t));

	if(pipe(up)==-1){
		perror("Pipe failed");
		return 1;
	}
	
	if(pipe(down)==-1){
		perror("Pipe failed");
		return 1;
	}
	
	p=fork();
	
	if(p>0){
		close(down[0]);
		close(up[1]);
		while(creditos >0){
			betPai= (rand() %(5 - 1 + 1)) + 1;
			printf("BetPai: %d\n", betPai);
			rand();
		
			if(creditos>0){
				x=1;
			} else { 
				x=0;
			}
	
			write(down[1],&x,4);
			read(up[0],&betFilho,4);
			printf("Aposta: %d, Resultado %d\n", betFilho, betPai);
			if(betFilho==betPai){
				creditos += 10;
			} else {
				creditos -= 5;
			}
		
			write(down[1],&creditos,4);
		}
		close(down[1]);
		close(up[0]);
	} else {
		close(down[1]);
		close(up[0]);
		
		while(creditos!=0){
			read(down[0],&x,4);
			if(x==1){
				betFilho = (rand() %(5 - 1 + 1)) + 1;
				printf("BetFilho: %d\n", betFilho);
			} else {
				close(up[1]);
				close(down[0]);
				exit(0);
			}
				
			write(up[1],&betFilho,4);
	
			read(down[0],&nCreditos,4);
			printf("Créditos filho: %d\n",nCreditos);
			}
		}
		close(down[0]);
		close(up[1]);
		return 0;
	}
