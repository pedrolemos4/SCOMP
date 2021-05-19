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
	pid_t p;
	
	
	sem_t *sem1;
	sem_t *sem2;
	
	if ((sem1 = sem_open("semex8_1", O_CREAT | O_EXCL, 0644,2)) == SEM_FAILED) {
		perror("Error in sem_open()");
		exit(1);
	}
	
	
	if ((sem2 = sem_open("semex8_2", O_CREAT | O_EXCL, 0644,2)) == SEM_FAILED) {
		perror("Error in sem_open()");
		exit(1);
	}
	
	p = fork();
	
	if(p==0){
		while(1){
			 sem_wait(sem1);
			 printf("S\n");
			 sleep(2);
			 int count;
			 sem_getvalue(sem2,&count);
			 if(count == 0){
				 sem_post(sem2);
				 sem_post(sem2);
			 } else if(count ==1){
				 sem_post(sem2);
			 }
		 }
		 exit(0);
	 }
	 while(1){
		 sem_wait(sem2);
		 printf("C\n");
		 sleep(2);
		 int count;
		 sem_getvalue(sem1,&count);
		 if(count ==0){
			 sem_post(sem1);
			 sem_post(sem1);
		 } else if(count == 1){
			 sem_post(sem1);
		 }
	 }
	 
	 sem_unlink("semex8_1");
	 sem_unlink("semex8_2");
	 return 0;
 }
