#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>

#define NUMBER_CARS 4

int main (int argc, char *argv[]){
	int valor, valor1, valor2, amount;
	sem_t *sem_vip, *sem_special, *sem_normal, *sem_amount;
	pid_t pid;
	
	if((sem_amount= sem_open("sem_amount",O_CREAT|O_EXCL,0644,300)) == SEM_FAILED){
		perror("Error in sem_open()");
		exit(1);
	}
	
	if((sem_vip= sem_open("sem_vip",O_CREAT|O_EXCL,0644,250)) == SEM_FAILED){
		perror("Error in sem_open()");
		exit(1);
	}
	
	if((sem_special= sem_open("sem_special",O_CREAT|O_EXCL,0644,60)) == SEM_FAILED){
		perror("Error in sem_open()");
		exit(1);
	}
	
	if((sem_normal= sem_open("sem_normal",O_CREAT|O_EXCL,0644,10)) == SEM_FAILED){
		perror("Error in sem_open()");
		exit(1);
	}
	
	pid=fork();	
	if(pid==0){
		sem_getvalue(sem_amount,&amount);
		while(amount>0){
			sem_getvalue(sem_vip, &valor);
			if(valor>0){
				printf("Visitante VIP %d\n",valor);
				sem_wait(sem_vip);
				sem_wait(sem_amount);
			}else{
				sem_getvalue(sem_special, &valor1);
				if(valor1>0){
					printf("Visitante SPECIAL %d\n",valor1);
					sem_wait(sem_special);
					sem_wait(sem_amount);
					sem_post(sem_amount);	
				}else{
					sem_getvalue(sem_normal, &valor2);
					if(valor2>0){
						printf("Visitante NORMAL %d\n",valor2);
						sem_wait(sem_normal);
						sem_wait(sem_amount);
						sem_post(sem_amount);	
					}else{
						printf("%d lugares livres\n",300-amount);
						exit(0);
					}
				}
			}
		}
		
		exit(0);
	}
	
	
	sem_unlink("sem_vip");
	sem_unlink("sem_special");
	sem_unlink("sem_amount");
	sem_unlink("sem_normal");
	
	return 0;
}

