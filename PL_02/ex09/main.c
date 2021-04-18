#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

#define ARRAY_SIZE 50000

typedef struct{
 int customer_code;
 int product_code;
 int quantity;
} sale;

int main(int argc, char *argv[]){

	pid_t p;
	int numProcesses = 10;
	int numPipes = 10;
	int fd[numPipes * 2];
	sale sales[ARRAY_SIZE];
	int products[ARRAY_SIZE];
	int c = ARRAY_SIZE/10;
	int k,i,j,readN,status;
	int count =0;
	
	time_t t; /* needed to init. the random number generator (RNG)*/

	/* intializes RNG (srand():stdlib.h; time(): time.h) */
	srand ((unsigned) time (&t));

	for (i = 0; i < ARRAY_SIZE; i++){
		sales[i].customer_code = rand () % 500;
		sales[i].product_code = rand () % 500;
		sales[i].quantity = rand() % 50;
	}
	
	for(i = 0; i < numPipes; i++) {
		if(pipe(fd + i * 2) == -1) {
			perror("Pipe failed\n");
			return 1;
		}
	}
	
	for(k=0;k<numProcesses;k++){
		p=fork();
		if(p==0){
			close(*(fd + k * 2)); /* fechar extremidade de leitura */
			for(i=k*c;i<k*c + c; i++){
				if(sales[i].quantity>20){
					write(*(fd + k * 2 + 1), &sales[i].product_code, 4);	/* enviar código para o pai */
				}
			}
			close(*(fd + k * 2 + 1));
			exit(0);
		}
	}
	
	for (j = 0; j < 10; j++) {
		close(*(fd+j*2+1));		
		while((read(*(fd + j*2), &readN, 4))>0){
			products[count] = readN;
			count++;
		}
			
		close(*(fd + j *2));
	}
	
	for(i=0;i<10;i++){
		wait(&status);
	}
	
	printf("Produtos vendidos mais de 20 vezes:\n");
	for(i = 0; i < count; i++) {
		printf("Código Final: %d\n", products[i]);
	}
	
	return 0;
}
