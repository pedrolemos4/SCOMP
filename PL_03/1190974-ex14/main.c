#include <stdio.h>
#include <sys/types.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mman.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <limits.h>
#include <string.h>

#define STR_SIZE 50

typedef struct{
	int circularBuffer[10];
	int read;
	int write;
}values;

int main (int argc, char *argv[]){
	int DATA_SIZE = sizeof(values);
	values *s1;
	pid_t pid;
	int fd,i,k,j,r,number=0;

	fd = shm_open("/shmex14", O_CREAT|O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
	ftruncate(fd,DATA_SIZE);
	s1 = (values*)mmap(NULL,DATA_SIZE,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
	
	s1->read=0;
	s1->write=1;
	
	pid=fork();
	
	//produtor
	if(pid>0){
		for(i=0;i<3;i++){
			while(s1->write==0);
			for(j=0;j<10;j++){
				s1->circularBuffer[j]=number;
				number++;
				printf("%d\n",number);
			}
			s1->read=1;
			s1->write=0;
			printf("i: %d\n",i);
		}
	//consumidor
	}else{
		for(k=0;k<3;k++){
			while(s1->read==0);
			for(j=0;j<10;j++){
				printf("O valor %d da %d estrutura é %d \n",j,k,s1->circularBuffer[j]);
			}
			s1->write=1;
			s1->read=0;
		}
		exit(0);
	}
	
	r = munmap(s1, DATA_SIZE); 		/* disconnects */
	if (r < 0){ 					/* Check error */
		 exit(1);
	}
	r = close(fd); 					/* closes */
	if (r < 0){						/* Check error*/
		 exit(1); 
	}
	
	r = shm_unlink("/shmex14"); 		/* removes */
	if (r < 0) {					/* Check error */
		exit(1); 	
	}
	return 0;
}

