#include <sys/mman.h>
#include <sys/stat.h> 		/* For constants "mode" */
#include <fcntl.h>			/* For constante O_* */
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 100
#define DATA_SIZE ARRAY_SIZE*4

int main (int argc, char *argv[]){
	int fd, i, r, amount=0;
	char *s1;
	int aux;
	fd = shm_open("/shmex4",O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd,DATA_SIZE);
	s1 = (char*)mmap(NULL,DATA_SIZE,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
	
	for(i=0;i<ARRAY_SIZE;i++){
		aux = (int) *(s1+i);
		amount = amount + aux;
		printf("Char %c \n",*(s1+i));
	}
	
	printf("Average: %d \n",amount/ARRAY_SIZE);
	
	r = munmap(s1, DATA_SIZE); 		
	if (r < 0){ 					
		 exit(1);
	}
	r = close(fd); 					
	if (r < 0){						
		 exit(1); 
	}
	
	r = shm_unlink("/shmex4"); 		
	if (r < 0) {					
		exit(1); 	
	}
	return 0;
}

