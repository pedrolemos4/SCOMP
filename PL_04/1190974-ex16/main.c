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
	int i, k;
	sem_t *sem_west, *sem_east;
	pid_t pid;
	
	if((sem_east= sem_open("sem_east",O_CREAT|O_EXCL,0644,1)) == SEM_FAILED){
		perror("Error in sem_open()");
		exit(1);
	}
	
	if((sem_west= sem_open("sem_west",O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
		perror("Error in sem_open()");
		exit(1);
	}
	
	pid=fork();	
	if(pid==0){
		for(i=0;i<NUMBER_CARS;i++){
			sem_wait(sem_west);
			printf("Car %d coming from east is on the bridge, wait\n",i);
			sem_post(sem_east);
		}
		exit(0);
	}else{
		for(k=0;k<NUMBER_CARS;k++){
			sem_wait(sem_east);
			printf("Car %d coming from west is on the bridge, wait\n",k);
			sem_post(sem_west);
		}
		wait(NULL);
	}
	
	sem_unlink("sem_east");
	sem_unlink("sem_west");
	
	return 0;
}

