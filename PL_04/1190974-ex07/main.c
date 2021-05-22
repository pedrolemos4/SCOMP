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
	sem_t *sem_p3, *sem_p1, *sem_p2;
	pid_t pid, pid1, pid2;
	
	
	if((sem_p1 = sem_open("sem_p1",O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
		perror("Error in sem_open1()");
		exit(1);
	}
	
	if((sem_p2 = sem_open("sem_p2",O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
		perror("Error in sem_open2()");
		exit(1);
	}
	
	if((sem_p3 = sem_open("sem_p3",O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
		perror("Error in sem_open3()");
		exit(1);
	}
	
	//Process1
	pid=fork();	
	if(pid==0){
		printf("Sistemas ");
		fflush(stdout);
		sem_post(sem_p2);
		sem_wait(sem_p1);
		printf("a ");
		fflush(stdout);
		sem_post(sem_p2);
		exit(0);
	}else{
		//Process2
		pid1=fork();	
		}if(pid1==0){
			sem_wait(sem_p2);
			printf("de ");
			fflush(stdout);
			sem_post(sem_p3);
			sem_wait(sem_p2);
			printf("melhor ");
			fflush(stdout);
			sem_post(sem_p3);
			exit(0);
		}else{
			//Process3
			pid2=fork();	
			if(pid2==0){
				sem_wait(sem_p3);
				printf("Computadores - ");
				fflush(stdout);
				sem_post(sem_p1);
				sem_wait(sem_p3);
				printf("disciplina! ");
				fflush(stdout);
				exit(0);
		}
	}
	
	
	sem_unlink("sem_p1");
	sem_unlink("sem_p2");
	sem_unlink("sem_p3");
	
	return 0;
}

