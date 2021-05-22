#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <time.h>
#include <semaphore.h>

typedef struct {
	int number[100];
	char name[100][80];
	char address[100][80];
	int index;
}est;

int main (int argc, char *argv[]){
	int i, dataSize=sizeof(est);
	
	int fdShm = shm_open("/shmex03", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	
	if(fdShm == -1){
		perror("Failed shm_open\n");
		return 1;
	}
	
	if(ftruncate(fdShm, dataSize) == -1){
		perror("Failed ftruncate\n");
		return 1;
	}

	est *ptr = (est*) mmap(NULL, dataSize, PROT_READ |PROT_WRITE, MAP_SHARED, fdShm, 0);
	
	int aux=ptr->index;
	
	for(i=0;i<aux;i++){
		printf("User number: %d = Name: %s Number: %d Address: %s \n",i,ptr->name[i], ptr->number[i], ptr->address[i]);
	}
	
	if(munmap(ptr, dataSize) == -1){
		perror("Failed munmap\n");
		return 1;
	}
	if(close(fdShm) == -1){
		perror("Failed close\n");
		return 1;
	}

	return 0;
}
