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

#define ARRAY_SIZE 1000000
#define DATA_SIZE ARRAY_SIZE*4


int main (int argc, char *argv[]){
	int fd,i,r,k,numRead;
	int array [ARRAY_SIZE];
	int *s1, *s2;
	clock_t startShm, endShm, startPipe, endPipe;
	
	int fdPipe[2];
	pid_t pid;
	
	
	for(i = 0; i < ARRAY_SIZE; i++) {
		array[i] = rand() % 10;
	}
	

	fd = shm_open("/shmex6", O_CREAT|O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
	ftruncate(fd,DATA_SIZE);
	s1 = (int*)mmap(NULL,DATA_SIZE,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
	
	int canRead = 0;
	int canWrite = 1;
	pid = fork();
	if(pid>0){
		srand((unsigned) time(NULL));
		startShm = clock();
		for(i=0;i<ARRAY_SIZE;i++){
			while(canWrite==0);
			canWrite=0;
			*(s1+i)=array[i];
			canRead=1;
		}
		wait(NULL);
	} else {
		for(k=0;k<ARRAY_SIZE;k++){
			while(canRead==0);
			canRead=0;
			numRead = *(s1+i);
			canWrite = 1;
		}
		exit(0);
	}
	endShm = clock();
	
	float timeShm = (float) (endShm - startShm) / CLOCKS_PER_SEC;
	printf("Tempo de transferência usando a memória partilhada = %f segundos\n", timeShm);
	r = munmap(s1, DATA_SIZE); 		/* disconnects */
	if (r < 0){ 					/* Check error */
		 exit(1);
	}
	r = close(fd); 					/* closes */
	if (r < 0){						/* Check error*/
		 exit(1); 
	}
	
	r = shm_unlink("/shmex6"); 		/* removes */
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
			write(fdPipe[1],&(array[i]),sizeof(DATA_SIZE));
		}
		close(fdPipe[1]);
		wait(NULL);
	} else {
		close(fdPipe[1]);
		for(k=0; k<ARRAY_SIZE; k++){
			read(fdPipe[0],&s2,sizeof(DATA_SIZE));
		}
		close(fdPipe[0]);
		exit(0);
	}
	endPipe = clock();
	
	float timePipe = (float) (endPipe - startPipe) / CLOCKS_PER_SEC;
	printf("Tempo de transferência usando o pipe = %f segundos\n", timePipe);
	return 0;
}

