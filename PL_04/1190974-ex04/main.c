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
	int i;
	char string[50][80];
}est;

int main (int argc, char *argv[]){
	int i,k,resposta,dataSize=sizeof(est);
	sem_t *sem;
	pid_t pid;
	
	int fdShm = shm_open("/shmex03", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	
	if(fdShm == -1){
		perror("Failed shm_open\n");
		return 1;
	}
	
	if(ftruncate(fdShm, dataSize) == -1){
		perror("Failed ftruncate\n");
		return 1;
	}
	
	if((sem = sem_open("sem",O_CREAT|O_EXCL,0644,1)) == SEM_FAILED){
		perror("Error in sem_open()");
		exit(1);
	}
	
	est *ptr = (est*) mmap(NULL, dataSize, PROT_READ |PROT_WRITE, MAP_SHARED, fdShm, 0);
	ptr->i=0;
	for(i=0;i<8;i++){
		pid=fork();		
		if(pid==0){
			sem_wait(sem);
			
			sprintf(ptr->string[ptr->i], "I'm the Father - with PID %d", getpid());
			ptr->i++;
			
			int aux = rand() % 5;
			
			sleep(aux);
			sem_post(sem);
			exit(0);
		}
	}
	
	for(k=0;k<8;k++){
		wait(NULL);
	}
	
	sem_wait(sem);
	for(i=0;i<8;i++){
		printf("Char: %s \n",ptr->string[i]);
	}
	sem_post(sem);
	
	do{
		printf("Pretende remover uma string da memoria? (1=Sim, 0=Nao)\n");
		scanf("%d",&resposta);
		if(resposta==1){
			struct timespec ts;
			
			if (clock_gettime(CLOCK_REALTIME, &ts) == -1){
				perror("Erro no temporizador de memoria partilhada.\n");
				exit(0);
			}
			ts.tv_sec += 12;
			
			if((i = sem_timedwait(sem, &ts)) == -1){
				perror("Timeout a aceder a memoria partilhada.\n");
				exit(0);
			}else{
				strcpy(ptr->string[ptr->i-1], "");
				ptr->i--;
				sem_post(sem);
			}
		}
	}while(resposta!=0);
	
	if(munmap(ptr, dataSize) == -1){
		perror("Failed munmap\n");
		return 1;
	}
	
	if(close(fdShm) == -1){
		perror("Failed close\n");
		return 1;
	}
	
	if(shm_unlink("/shmex03") == -1) {
		perror("Failed shm_unlink\n");
		return 1;
	}
	
	sem_unlink("sem");
	
	return 0;
}

