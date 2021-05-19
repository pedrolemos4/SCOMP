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
	int i;
	sem_t *semChild3_2;
	sem_t *semChild1;
	sem_t *semChild2_1;
	sem_t *semChild2_2;
	sem_t *semChild3_1;
	
	if ((semChild3_2 = sem_open("semex7Child3_2", O_CREAT | O_EXCL, 0644,0)) == SEM_FAILED) {
		perror("Error in sem_open()");
		exit(1);
	}
	
	
	if ((semChild1 = sem_open("semex7Child1", O_CREAT | O_EXCL, 0644,0)) == SEM_FAILED) {
		perror("Error in sem_open()");
		exit(1);
	}
	
	if ((semChild2_1 = sem_open("semex7Child2_1", O_CREAT | O_EXCL, 0644,0)) == SEM_FAILED) {
		perror("Error in sem_open()");
		exit(1);
	}
	
	if ((semChild2_2 = sem_open("semex7Child2_2", O_CREAT | O_EXCL, 0644,0)) == SEM_FAILED) {
		perror("Error in sem_open()");
		exit(1);
	}
	
	if ((semChild3_1 = sem_open("semex7Child3_1", O_CREAT | O_EXCL, 0644,0)) == SEM_FAILED) {
		perror("Error in sem_open()");
		exit(1);
	}

	pid_t p1;
	pid_t p2;
	pid_t p3;
	
	p1=fork();
	if(p1==0){
		printf("Processo 1\n");
		//sem_wait(semChild1_1);
		
		printf("Sistemas ");
		
		sem_post(semChild2_1);
		
		sem_wait(semChild1);
		
		printf("a ");
		sem_post(semChild2_2);
		exit(0);
	} else {
		p2= fork();
		
		if(p2==0){
			printf("Processo 2\n");
			sem_wait(semChild2_1);
			
			printf("de ");
			
			sem_post(semChild3_1);
			
			sem_wait(semChild2_2);
			
			printf("melhor ");
			
			sem_post(semChild3_2);
			exit(0);
		} else{
			//p3=fork();
			
			//if(p3==0){
			printf("Processo3\n");
				sem_wait(semChild3_1);
				
				printf("Computadores -");
				
				sem_post(semChild1);
				sem_wait(semChild3_2);
				
				printf("disciplina!\n");
				//exit(0);
			//}
		}
	}
	
	sem_unlink("semex7Child3_2");
	sem_unlink("semex7Child1");
	sem_unlink("semex7Child2_1");
	sem_unlink("semex7Child2_2");
	sem_unlink("semex7Child3_1");
	return 0;
	
}
