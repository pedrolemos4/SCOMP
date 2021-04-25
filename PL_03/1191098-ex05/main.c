#include <stdio.h>
#include <sys/types.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mman.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define COUNT 1000000

typedef struct{
	int number1;
	int number2;
} sharedStruct;

int main (int argc, char *argv[]){
	int fd,r, data_size = sizeof(sharedStruct);
	int i,j;
	sharedStruct *s1;
	pid_t pid;
	
	fd = shm_open("/shmex5", O_CREAT|O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
	ftruncate(fd,data_size);
	s1 = (sharedStruct*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
	s1->number1=8000;
	s1->number2=200;
	
	pid=fork();
	if(pid>0){
		for(i=0;i<COUNT;i++){
			s1->number1 += 1;
			s1->number2 -= 1;
		}
		printf("Processo Pai: Número1 = %d\nNúmero2: %d\n", (s1->number1),s1->number2);
	}else {
		for(j=0;j<COUNT;j++){
			s1->number1 -=1;
			s1->number2 +=1;
		}
		printf("Processo filho: Número1 = %d\nNúmero2: %d\n", (s1->number1),s1->number2);

		exit(0);
	}
	
	wait(NULL);
	
	printf("Number 1: %d\nNumber 2: %d\n",s1->number1,s1->number2);
	
	r = munmap(s1, data_size); 		/* disconnects */
	if (r < 0){ 					/* Check error */
		 exit(1);
	}
	r = close(fd); 					/* closes */
	if (r < 0){						/* Check error*/
		 exit(1); 
	}
	
	r = shm_unlink("/shmex5"); 		/* removes */
	if (r < 0) {					/* Check error */
		exit(1); 	
	}
	
	return 0;
}
	
