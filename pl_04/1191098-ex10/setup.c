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
#include "estruturas.h"

int main(){
	
	int fd,r,data_size = sizeof(sharedStruct);
	sharedStruct *s1;
	
	sem_unlink("semex10Insert");
	sem_unlink("semex10Consult");
	sem_unlink("semex10ConsultALl");
	sem_t* sem1;
	sem_t* sem2;
	sem_t* sem3;
	if ((sem1 = sem_open("semex10Insert", O_CREAT|O_EXCL, 0644, 1)) == SEM_FAILED) {
		perror("Erro a criar o semaforo.\n");
		exit(0);
	}	
	if ((sem2 = sem_open("semex10Consult", O_CREAT|O_EXCL, 0644, 1)) == SEM_FAILED) {
		perror("Erro a criar o semaforo.\n");
		exit(0);
	}	
	if ((sem3 = sem_open("semex10ConsultAll", O_CREAT|O_EXCL, 0644, 1)) == SEM_FAILED) {
		perror("Erro a criar o semaforo.\n");
		exit(0);
	}	
	
	r = shm_unlink("/shmex10"); 		/* removes */
	
	fd = shm_open("/shmex10", O_CREAT|O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
		
	if (fd == -1){
		perror("Erro a abrir a memória partilhada.\n");
		exit(0);
	}
		
	ftruncate(fd,data_size);
	
	s1 = (sharedStruct*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
		
	r = munmap(s1, data_size); 		/* disconnects */
	
	if (r < 0){ 					/* Check error */
		exit(1);
	}
	r = close(fd); 					/* closes */
	
	if (r < 0){						/* Check error*/
		exit(1); 
	}
	
	printf("Sucesso\n");
	return 0;
}
