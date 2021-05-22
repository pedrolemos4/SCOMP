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

int main (int argc, char *argv[]){
	int i=0;
	sem_t *sem_filho, *sem_pai;
	pid_t pid;
	
	
	if((sem_filho = sem_open("sem_filho",O_CREAT|O_EXCL,0644,1)) == SEM_FAILED){
		perror("Error in sem_open()");
		exit(1);
	}
	
	if((sem_pai = sem_open("sem_pai",O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
		perror("Error in sem_open()");
		exit(1);
	}
	
	pid=fork();	
	while(i<7){	
		if(pid==0){
			sem_wait(sem_filho);
			printf("I'm the child \n");
			sem_post(sem_pai);
			i++;
		}else{
			sem_wait(sem_pai);
			printf("I'm the father \n");
			sem_post(sem_filho);
			i++;
		}
	}
	if(pid==0){
		exit(0);
	}
	
	sem_unlink("sem_filho");
	sem_unlink("sem_Pai");
	
	return 0;
}

