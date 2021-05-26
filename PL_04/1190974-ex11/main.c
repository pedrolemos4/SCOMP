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
	int i, valor;
	sem_t *sem_p1, *sem_p2, *sem_p3, *sem_p4;
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
	
	if((sem_p4 = sem_open("sem_p4",O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
		perror("Error in sem_open4()");
		exit(1);
	}
	
	pid=fork();	
	if(pid==0){
		sem_getvalue(sem_p4, &valor);
		while(valor<200){
			sem_wait(sem_p1);
			printf("Entra/Sai na porta 1\n");
			sem_post(sem_p4);
		}
		exit(0);
	}else{
		pid1=fork();
		if(pid1==0){
			sem_getvalue(sem_p4, &valor);
			while(valor<200){
				sem_wait(sem_p2);
				printf("Entra/Sai na porta 2\n");
				sem_post(sem_p4);
			}
			exit(0);
		}else{
			pid2=fork();
			if(pid2==0){
				sem_getvalue(sem_p4, &valor);
				while(valor<200){
					sem_wait(sem_p3);
					printf("Entra/Sai na porta 3\n");
					sem_post(sem_p4);
				}
				exit(0);
			}else{
				for(i=0;i<200;i++){
					if(i%3==0){
						sem_post(sem_p1);
						sem_post(sem_p2);
						sem_post(sem_p3);
					}
				}
			}
		}
	}
	
	sem_unlink("sem_p1");
	sem_unlink("sem_p2");
	sem_unlink("sem_p3");
	sem_unlink("sem_p4");
	
	return 0;
}

