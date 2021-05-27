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

#define SIZE 12
#define SHOW_TIME 2

int main (int argc, char *argv[]){
	int i, valor;
	clock_t startShow, endShow;
	//sem_t *sem_array[SIZE];
	sem_t *sem_sala;
	//char name[SIZE] = {"sem_p1","sem_p2","sem_p3","sem_p4","sem_p5","sem_p6","sem_p7","sem_p8","sem_p9","sem_p10","sem_p11","sem_p12"};
	pid_t pid;
	
	/*for(i=0;i<SIZE;i++){
		if((*(sem_array+i) = sem_open(name[i],O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
			perror("Error in sem_open()");
			exit(1);
		}
	}*/
	
	if((sem_sala= sem_open("sem_sala",O_CREAT|O_EXCL,0644,5)) == SEM_FAILED){
		perror("Error in sem_open()");
		exit(1);
	}
	
	for(i=0;i<SIZE;i++){
		pid=fork();	
		if(pid==0){
			while(1){
				printf("1-Visitante %d espera\n",i);
				sem_wait(sem_sala);
				printf("2-Visitante %d entra\n",i);
				sleep(SHOW_TIME);
				sem_post(sem_sala);
				printf("3-Visitante %d saiu\n",i);
				exit(0);
			}
		}
	}
	
	/*for(i=0;i<SIZE;i++){
		sem_unlink(name[i]);
	}*/
	
	sem_unlink("sem_sala");
	
	return 0;
}

