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
	
typedef struct {
	int ticket;
	int index;
}est;

int main (int argc, char *argv[]){
	
	sem_unlink("sem_seller");
	sem_unlink("sem_client");
	
	int dataSize=sizeof(est);
	sem_t *sem_client1, *sem_seller1;
	
	int fdShm = shm_open("/shmex12", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	
	if(fdShm == -1){
		perror("Failed shm_open\n");
		return 1;
	}
	
	if(ftruncate(fdShm, dataSize) == -1){
		perror("Failed ftruncate\n");
		return 1;
	}
	
	if((sem_seller1 = sem_open("sem_seller",O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
		perror("Error in sem_open1()");
		exit(1);
	}
	
	if((sem_client1 = sem_open("sem_client",O_CREAT|O_EXCL,0644,1)) == SEM_FAILED){
		perror("Error in sem_open1()");
		exit(1);
	}
	
	est *ptr = (est*) mmap(NULL, dataSize, PROT_READ |PROT_WRITE, MAP_SHARED, fdShm, 0);
	
	while(ptr->ticket<3){	//Apenas 8 bilhetes para vender
		sem_wait(sem_client1);
		ptr->ticket++;
		ptr->index++;
		sem_post(sem_seller1);
	}
	
	sem_unlink("sem_seller");
	sem_unlink("sem_client");
	
	if(munmap(ptr, dataSize) == -1){
		perror("Failed munmap\n");
		return 1;
	}
	if(close(fdShm) == -1){
		perror("Failed close\n");
		return 1;
	}
	
	if (shm_unlink("/shmex12") < 0) {					
		exit(1); 	
	}
	
	return 0;
}
