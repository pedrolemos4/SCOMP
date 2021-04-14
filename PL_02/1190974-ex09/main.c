#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

#define ARRAY_SIZE 50000
#define AMOUNT 5000

typedef struct{
 int customer_code;
 int product_code;
 int quantity;
} salesRec;


int main(int argc, char *argv[]){

	int fd[20]; //10*2 for 10 childs
	int arraySales[ARRAY_SIZE], i, k, code_prod, count = 0;
	
	pid_t pid;
	
	salesRec sales[ARRAY_SIZE];
	
	for(i=0;i<10;i++){
		if(pipe(fd+i*2) == -1){
			perror("Pipe failed");
			return 1;
		}
	}
	
	time_t t; /* needed to init. the random number generator (RNG)*/

	/* intializes RNG (srand():stdlib.h; time(): time.h) */
	srand ((unsigned) time (&t));

	for (i = 0; i < ARRAY_SIZE; i++){
		sales[i].customer_code = rand () % 500;
		sales[i].product_code = rand () % 500;
		sales[i].quantity = rand() % 50;
	}
	
	for(i=0;i<10;i++){
		pid = fork();
		if(pid==0){
			for(k=i*AMOUNT;k<i*AMOUNT+AMOUNT;k++){
				if(sales[k].quantity>20){
					close(fd[i*2]);
					write(fd[i*2+1],&(sales[k].product_code),sizeof(sales[k].product_code));
				}
				close(fd[i*2+1]);
			}
			exit(0);
		}
	}
	
	for(i=0;i<10;i++){
		close(fd[i*2+1]);
		while(read(fd[i*2], &code_prod, sizeof(sales[k].product_code))>0){
			arraySales[count]=code_prod;
			count++;
		}
		close(fd[i*2]);
	}
	
	for(i=0;i<count;i++){
		waitpid(pid,NULL,0);
		printf("Sales: %d \n",arraySales[i]);
	}
	
	return 0;
}
