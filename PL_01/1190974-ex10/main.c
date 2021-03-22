#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define ARRAY_SIZE 2000

int main ()
{
	int i, j, array[ARRAY_SIZE];
	pid_t p, waitId;
	int num = 10, status = 0;
	
	/* preencher vetor com números aleatórios */
	for(i = 0; i < ARRAY_SIZE; i++) {
		array[i] = rand() % 50;
	} 
	
	for(j = 0 ; j < 10; j++) {
		p = fork();
		if(p == 0) {
			/* procurar no vetor o número pedido num conjunto de 200 números */
			int minLim = 200 * j;
			int maxLim = 200 * j + 200;
			for(i = minLim; i < maxLim; i++) {
				if(array[i] == num) {
					/* devolver o índice relativo onde o número foi encontrado
					numa escala de 0 a 200 */
					exit(200 - (maxLim - i));
				}
			}
			exit(255);
		}
	} 
	
	while((waitId = wait(&status)) > 0) {
		if(WEXITSTATUS(status) == 255) {
			printf("Número não encontrado\n");
		} else {
			printf("Número encontrado entre 0 e %d\n", WEXITSTATUS(status));
		}
	}
	
	return 0;
} 
