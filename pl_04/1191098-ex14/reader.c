#include <stdio.h>
#include <sys/types.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mman.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <semaphore.h>
#include "estrutura.h"

int main(){
	int fdReader,fdString,r,s;
	int data_sizeReader = sizeof(readers);
	int data_sizeString = 100;

	readers* r1;
	
	fdString = shm_open("/shmEx14String", O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	fdReader = shm_open("/shmEx14Reader", O_EXCL|O_RDWR, S_IRUSR|S_IWUSR);
	
	ftruncate(fdReader,data_sizeReader);
	ftruncate(fdString,data_sizeString);
	
	r1 = (readers*)mmap(NULL,data_sizeReader,PROT_READ|PROT_WRITE,
		MAP_SHARED,fdReader,0);
	
	char* string = (char*) mmap(NULL,data_sizeString,PROT_READ|PROT_WRITE,
		MAP_SHARED,fdString,0);
		
	sem_t* semWriter;
	sem_t* semReader;
	
	if ((semWriter = sem_open("sem_ex14_writer", O_CREAT, 0644, 1)) == SEM_FAILED || (semReader = sem_open("sem_ex14_reader", O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("Erro a criar o semaforo.\n");
		exit(0);
	}
	
	sem_wait(semReader);
	r1->nr_readers++;
	if(r1->nr_readers == 1){
		printf("Ainda existem escritores, aguardando...\n");
		sem_wait(semWriter);
	}
	sem_post(semReader);
	
	printf("String: %s\nReaders: %d\n", string, r1->nr_readers);
	sleep(15);

	sem_wait(semReader);
	r1->nr_readers--;
	if(r1->nr_readers == 0){
		sem_post(semWriter);
	}
	sem_post(semReader);
	
	r = munmap(r1, data_sizeReader); 		/* disconnects */
	
	if (r < 0){ 					/* Check error */
		exit(1);
	}
	r = close(fdReader); 					/* closes */
	
	if (r < 0){						/* Check error*/
		exit(1); 
	}
	
	s = munmap(string,data_sizeString);
	
	if (s < 0 ){
		exit(1);
	}
	s = close(fdString);
	
	if (s < 0){
		exit(1);
	}
	return 0;
}
