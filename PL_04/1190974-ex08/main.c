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
	int i;
	sem_t *sem_p1, *sem_p2;
	pid_t pid;
	
	
	if((sem_p1 = sem_open("sem_p1",O_CREAT|O_EXCL,0644,2)) == SEM_FAILED){
		perror("Error in sem_open1()");
		exit(1);
	}
	
	if((sem_p2 = sem_open("sem_p2",O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
		perror("Error in sem_open2()");
		exit(1);
	}
	
	//Process1
	pid=fork();	
	if(pid==0){
		int j=0;
		while(j<5){
			sem_wait(sem_p1);
			printf("S");
			fflush(stdout);
			sem_getvalue(sem_p1,&i);
			if(i>0){
				sem_post(sem_p2);
			}else{
				sem_post(sem_p2);
				sem_post(sem_p2);
			}
			j++;
		}
		exit(0);
	}else{
		//Process2
		int k=0;
		while(k<5){
			sem_wait(sem_p2);
			printf("C");
			fflush(stdout);
			sem_getvalue(sem_p2,&i);
			if(i>0){
				sem_post(sem_p1);
			}else{
				sem_post(sem_p1);
				sem_post(sem_p1);
			}
			k++;
		}
	}
	printf("\n");
	
	sem_unlink("sem_p1");
	sem_unlink("sem_p2");
	
	return 0;
}

