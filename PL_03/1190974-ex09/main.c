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

#define ARRAY_SIZE 1000
#define HUNDRED 100
#define DATA_SIZE 10*4

int main (int argc, char *argv[]){
	int fd,i,r,k,max;
	int *s1;
	int array[ARRAY_SIZE];
	pid_t pid;

	fd = shm_open("/shmex9", O_CREAT|O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
	ftruncate(fd,DATA_SIZE);
	s1 = (int*)mmap(NULL,DATA_SIZE,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);

	for(i=0;i<ARRAY_SIZE;i++){
		array[i] =  rand() % 1000;
	}
	
	for(k=0;k<10;k++){
		pid = fork();
		if(pid==0){
			max=0;
			for(r=0;r<HUNDRED;r++){
				if(max<array[k*HUNDRED+r]){
					max=array[k*HUNDRED+r];
				}
			}
			printf("Máximo da iteração %d é %d\n",k,max);
			*(s1+k)=max;
			exit(0);
		}
	}
	
	for(k=0; k < 10; k++){
		wait(NULL);
	}
	
	max=0;
	for(i=0;i<10;i++){
		if(*(s1+i)>max){
			max = *(s1+i);
		}
	}
	printf("Máximo total é : %d\n",max);
	
	r = munmap(s1, DATA_SIZE); 	/* disconnects */
	if (r < 0){ 					/* Check error */
		 exit(1);
	}
	r = close(fd); 					/* closes */
	if (r < 0){						/* Check error*/
		 exit(1); 
	}
	
	r = shm_unlink("/shmex9"); 		/* removes */
	if (r < 0) {					/* Check error */
		exit(1); 	
	}
	return 0;
}

