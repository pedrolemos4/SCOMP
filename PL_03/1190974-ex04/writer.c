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


#define ARRAY_SIZE 100


int main (int argc, char *argv[]){
	int fd, r, i;
	char array[ARRAY_SIZE];
	char *s1;
	
	for(i=0;i<ARRAY_SIZE;i++){
		array[i] = (char) 65 + ( rand() % 26);
	}
	
	fd = shm_open("/shmex4", O_CREAT|O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
	ftruncate(fd, ARRAY_SIZE);
	s1 = (char*)mmap(NULL, ARRAY_SIZE , PROT_READ|PROT_WRITE,
		MAP_SHARED, fd, 0);
	
	for(i=0;i<ARRAY_SIZE;i++){
		*(s1+i)=array[i];
	}
	
	r = munmap(s1, ARRAY_SIZE); 		
	if (r < 0){ 					
		 exit(1);
	}
	r = close(fd); 		
	if (r < 0){			
		 exit(1); 
	}
	return 0;
}
	
	
	
