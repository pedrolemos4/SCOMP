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

#define TOTAL_AMOUNT 1000000

int main (int argc, char *argv[]){
	int fd,i,r,k,numRead,numRead1;
	int DATA_SIZE = sizeof(int);
	int *s1;
	pid_t pid;

	fd = shm_open("/shmex6", O_CREAT|O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
	ftruncate(fd,DATA_SIZE);
	s1 = (int*)mmap(NULL,DATA_SIZE,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
	
	*s1 = 100;
	
	pid = fork();
	
	if(pid>0){
		for(i=0;i<TOTAL_AMOUNT;i++){
			numRead=*s1;
			numRead++;
			*s1=numRead;
		}
		for(i=0;i<TOTAL_AMOUNT;i++){
			numRead1=*s1;
			numRead1--;
			*s1=numRead1;
		}
		wait(NULL);
	} else {
		for(k=0;k<TOTAL_AMOUNT;k++){
			numRead=*s1;
			numRead++;
			*s1=numRead;
		}
		for(k=0;k<TOTAL_AMOUNT;k++){
			numRead1=*s1;
			numRead1--;
			*s1=numRead1;
		}
		exit(0);
	}
	
	printf("Valor = %d\n",*s1);
	
	r = munmap(s1, DATA_SIZE); 	/* disconnects */
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
	return 0;
}

