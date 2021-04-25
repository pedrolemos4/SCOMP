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


#define STRING_SIZE 80
#define ARRAY_SIZE 10000
#define STRING "ISEP – SCOMP 2020"

typedef struct {
	 int number;
	 int canRead;
	 int canWrite;
	 char stringIsep[STRING_SIZE];
} sharedStruct;

int main (int argc, char *argv[]){
	int fd,r, data_size = sizeof(sharedStruct);
	int i,k,numRead;
	char stringRead [STRING_SIZE];
	sharedStruct *s1;
	sharedStruct str1;
	clock_t startShm, endShm, startPipe, endPipe;
	
	sharedStruct arrayStruct[ARRAY_SIZE];
	int fdPipe[2];
	pid_t pid;
	
	
	for(i = 0; i < ARRAY_SIZE; i++) {
		arrayStruct[i].number = rand() % 10;
		strcpy(arrayStruct[i].stringIsep,STRING);
	}
	

	fd = shm_open("/shmex3", O_CREAT|O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
	ftruncate(fd,data_size);
	s1 = (sharedStruct*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
	
	s1->canRead = 0;
	s1->canWrite = 1;
	pid = fork();
	if(pid>0){
		srand((unsigned) time(NULL));
		startShm = clock();
		for(i=0;i<ARRAY_SIZE;i++){
			while(s1->canWrite==0);
			s1->canWrite=0;
			s1->number=arrayStruct[i].number;
			strcpy(s1->stringIsep,arrayStruct[i].stringIsep);
			s1->canRead=1;
		}
		wait(NULL);
	} else {
		for(k=0;k<ARRAY_SIZE;k++){
			while(s1->canRead==0);
			s1->canRead=0;
			numRead = s1->number;
			strcpy(stringRead,s1->stringIsep);
			s1->canWrite = 1;
		}
		exit(0);
	}
	endShm = clock();
	
	float timeShm = (float) (endShm - startShm) / CLOCKS_PER_SEC;
	printf("Tempo de transferência usando a memória partilhada = %f segundos\n", timeShm);
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
	
	
	if(pipe(fdPipe)==-1){
		perror("Pipe failed");
		return 1;
	}
	pid = fork();
	if(pid>0){
		close(fdPipe[0]);
		startPipe = clock();
		for(i=0; i < ARRAY_SIZE; i++){
			write(fdPipe[1],&(arrayStruct[i]),sizeof(sharedStruct));
		}
		close(fdPipe[1]);
		wait(NULL);
	} else {
		close(fdPipe[1]);
		for(k=0; k<ARRAY_SIZE; k++){
			read(fdPipe[0],&str1,sizeof(sharedStruct));
		}
		close(fdPipe[0]);
		exit(0);
	}
	endPipe = clock();
	
	float timePipe = (float) (endPipe - startPipe) / CLOCKS_PER_SEC;
	printf("Tempo de transferência usando o pipe = %f segundos\n", timePipe);
	return 0;
}

