#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h> 
#include <time.h>
#include <semaphore.h>
#include "estrutura.h"

int main(){
	
	sem_unlink("sem_ex14_writer");
	sem_unlink("sem_ex14_reader");
	shm_unlink("/shmEx14String");
	shm_unlink("/shmEx14Reader");
	int fdString,fdReader,r,s;
	int data_sizeReader = sizeof(readers);
	int data_sizeString = 100;
	readers *r1;

	fdString = shm_open("/shmEx14String", O_CREAT|O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
	
	fdReader = shm_open("/shmEx14Reader", O_CREAT|O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
	
		
	if (fdString == -1 || fdReader == -1){
		perror("Erro a abrir a memória partilhada.\n");
		exit(0);
	}
	
	if(ftruncate (fdReader, data_sizeReader) == -1 || ftruncate (fdString, data_sizeString) == -1){
		perror("Erro a definir o tamanho da memória partilhada.\n");
		exit(0);
	}
	
	r1 = (readers*) mmap(NULL, data_sizeReader, PROT_READ|PROT_WRITE, MAP_SHARED, fdReader, 0);
	
	char* string = (char*) mmap(NULL, data_sizeString, PROT_READ|PROT_WRITE, MAP_SHARED, fdString, 0);
	
	sem_t* semWriter;
	sem_t* semReader;
	
	if ((semWriter = sem_open("sem_ex14_writer", O_CREAT, 0644, 1)) == SEM_FAILED || (semReader = sem_open("sem_ex14_reader", O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("Erro a criar o semaforo.\n");
		exit(0);
	}
	
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
	
	printf("Sucesso\n");
	return 0;
}
