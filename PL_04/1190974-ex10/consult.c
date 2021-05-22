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
	int i, k, number, dataSize=sizeof(est);
	char name[80];
	
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
	
	printf("Insira o seu nome\n");
	scanf("%s",name);
	printf("Insira o seu numero:\n");
	scanf("%d",&number);
	
	i=ptr->index;
	
	for(k=0;k<i;k++){
		if(ptr->number[k]==number && strcmp(ptr->name[k],name)==0){
			printf("Info: Name: %s Number: %d Address: %s \n",ptr->name[k], ptr->number[k], ptr->address[k]);
		}
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
