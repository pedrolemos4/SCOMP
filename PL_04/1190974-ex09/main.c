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

void buy_chips(){
	printf("Buy chips\n");
}

void buy_beer(){
	printf("Buy beer\n");
}

void eat_and_drink(){
	printf("Eat and drink\n");
}

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
	
	pid=fork();	
	if(pid==0){
		sem_wait(sem_p1);
		buy_chips();
		sem_getvalue(sem_p1,&i);
		if(i==0){
			sem_post(sem_p2);
		}
		sem_wait(sem_p2);
		eat_and_drink();
		sem_post(sem_p2);
		exit(0);
	}else{
		sem_wait(sem_p1);
		buy_beer();
		sem_getvalue(sem_p1,&i);
		if(i==0){
			sem_post(sem_p2);
		}
		sem_wait(sem_p2);
		eat_and_drink();
		sem_post(sem_p2);
	}
	
	sem_unlink("sem_p1");
	sem_unlink("sem_p2");
	
	return 0;
}

