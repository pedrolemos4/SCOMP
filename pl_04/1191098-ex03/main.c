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

typedef struct {
	int posicao;
	char frases[50][80];
} sharedStruct;

int main() {
	int fd,i,r, data_size = sizeof(sharedStruct);
	
	sharedStruct *s1;
	sem_t *sem;
	
	
	fd = shm_open("/shmex3", O_CREAT|O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
	ftruncate(fd,data_size);
	
	s1 = (sharedStruct*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
		
	
	if ((sem = sem_open("semex3", O_CREAT | O_EXCL, 0644,1)) == SEM_FAILED) {
		perror("Error in sem_open()");
		exit(1);
	}

	pid_t p;
	for(i=0;i<20;i++){
		p = fork();
		if(p == -1) {
			perror("Failed fork\n");
			exit(1);
		} else if(p == 0) {
			srand(time(NULL));
			rand();
			int pausa = rand() % (5 + (1 - 1));
			
			sem_wait(sem);
	
			sprintf(s1->frases[s1->posicao], "I'm the Father - with PID %d", getpid());
	
			s1->posicao++; 
			
			sleep(pausa);
		
			sem_post(sem);
			exit(0);
		} else {
			wait(NULL);
		}
	}
	
	sem_wait(sem);
	for(i = 0; i < 20; i++) {
		printf("%s\n", s1->frases[i]);
	}	
	sem_post(sem);
	r = munmap(s1, data_size); 		/* disconnects */
	if (r < 0){ 					/* Check error */
		 exit(1);
	}
	r = close(fd); 					/* closes */
	if (r < 0){						/* Check error*/
		 exit(1); 
	}
	
	r = shm_unlink("/shmex3"); 		/* removes */
	if (r < 0) {					/* Check error */
		exit(1); 	
	}
	
	sem_unlink("semex3");
	return 0;
}
