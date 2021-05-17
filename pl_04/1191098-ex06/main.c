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


int main(){

	sem_t *semFilho;
	sem_t *semPai;

	if ((semFilho = sem_open("semex5Filho", O_CREAT | O_EXCL, 0644,1)) == SEM_FAILED) {
		perror("Error in sem_open()");
		exit(1);
	}
	
	if ((semPai = sem_open("semex5Pai", O_CREAT | O_EXCL, 0644,0)) == SEM_FAILED) {
		perror("Error in sem_open()");
		exit(1);
	}

	pid_t p;
		int i =0;

	p= fork();
	while(i<7){
		if(p>0){
			sem_wait(semPai);
		
			printf("I'm the father.\n");
			i++;
			sem_post(semFilho);
		
		} else{
		
			sem_wait(semFilho);
		
			printf("I'm the child.\n");
			i++;
			sem_post(semPai);
			//sem_post(semFilho);
		
		}
	}
	
	if(p==0){
		sem_wait(semFilho);
		printf("I'm the child.\n");
		sem_post(semPai);
		exit(0);
	}
	
	sem_unlink("semex5Filho");
	sem_unlink("semex5Pai");
		
	
	return 0;
}
