#define ARRAY_SIZE 1000
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

int main ()
{
	int status;
	int i;
	int k;
	int valor=0;
	
	for(k=0;k<10;k++){
		pid_t p ;
		p=fork();
	
		if(p==0){
			for(i=valor;i<valor+100;i++){
				printf("%d. %d \n",k,i);
			}
		valor=i;
		}else{
			exit(1);
		}
	}
	
	for(i=0;i<10;i++){
		wait(&status);
	}
	
	/* A)
	 * Não é garantido que o output esteja sempre ordenado uma vez que a
	 * sua ordenação tem a ver com os escalonamentos realizados.
	 * */
	
	return 0;
} 
