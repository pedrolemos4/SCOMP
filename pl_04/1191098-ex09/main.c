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

#define NPROC 2

typedef struct {
	int nproc_at_barrier;
} sharedStruct;

void buy_chips(){
	printf("Buy chips\n");
}

void buy_beer(){
	printf("Buy beer\n");
}

void eat_and_drink(){
	printf("Eat and Drink\n");
}

int main(){

	int fd, r, data_size = sizeof(sharedStruct);
	
	sharedStruct *s1;
	
	fd = shm_open("/shmex9", O_CREAT|O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
	ftruncate(fd,data_size);
	
	s1 = (sharedStruct*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
	
	sem_t *sem_nproc;
	sem_t *sem_barrier;
	
	if ((sem_nproc = sem_open("semex9_nproc", O_CREAT | O_EXCL, 0644,1)) == SEM_FAILED) {
		perror("Error in sem_open()");
		exit(1);
	}
	
	
	if ((sem_barrier = sem_open("semex9_barrier", O_CREAT | O_EXCL, 0644,0)) == SEM_FAILED) {
		perror("Error in sem_open()");
		exit(1);
	}
	//s1->nproc_at_barrier = 0;
	pid_t p ;
	
	p = fork();
	
	if(p>0){
		buy_chips();
		sem_wait(sem_nproc);
		s1->nproc_at_barrier++;
		sem_post(sem_nproc);
		if (s1->nproc_at_barrier == NPROC){
			sem_post(sem_barrier);
		}
		sem_wait(sem_barrier);
		sem_post(sem_barrier);
		eat_and_drink();
	} else {
		buy_beer();
		sem_wait(sem_nproc);
		s1->nproc_at_barrier++;
		sem_post(sem_nproc);
		if (s1->nproc_at_barrier == NPROC){
			sem_post(sem_barrier);
		}
		sem_wait(sem_barrier);
		sem_post(sem_barrier);
		eat_and_drink();
		exit(0);
	}
	
	r = munmap(s1, data_size); 		/* disconnects */
	if (r < 0){ 					/* Check error */
		 exit(1);
	}
	r = close(fd); 					/* closes */
	if (r < 0){						/* Check error*/
		 exit(1); 
	}
	
	r = shm_unlink("/shmex9"); 		/* removes */
	if (r < 0) {					/* Check error */
		exit(1); 	
	}
	
	sem_unlink("semex9_nproc");
	sem_unlink("semex9_barrier");
	
	return 0;
}
