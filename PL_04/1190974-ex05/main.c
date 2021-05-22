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
	sem_t *sem;
	pid_t pid;
	
	if((sem = sem_open("sem",O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
		perror("Error in sem_open()");
		exit(1);
	}
	
	pid=fork();		
	if(pid==0){
		printf("I'm the child\n");
		sem_post(sem);
		exit(0);
	}
	
	sem_wait(sem);
	printf("I'm the father\n");
	sem_unlink("sem");
	
	return 0;
}

