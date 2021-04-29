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

typedef struct {
	int first;
	int second;
} estrutura;

int main (int argc, char *argv[]){
	int i, fd, r, data_size = sizeof(estrutura);
	estrutura *s1, *s2;
	pid_t pid;

	fd = shm_open("/shmex5", O_CREAT|O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
	ftruncate(fd,data_size);
	s1 = (estrutura*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
		
	s1->first=8000;
	s1->second=200;
	
	pid = fork();
	
	if(pid>0){
		for(i=0;i<1000000;i++){
			s1->first+=1;
			s1->second-=1;
		}
		wait(NULL);
	}else{
		for(i=0;i<1000000;i++){
			s1->second+=1;
			s1->first-=1;
		}
		exit(0);
	}
	
	
	r = munmap(s1, data_size); 		/* disconnects */
	if (r < 0){ 					/* Check error */
		 exit(1);
	}
	r = close(fd); 					/* closes */
	if (r < 0){						/* Check error*/
		 exit(1); 
	}
	
	fd = shm_open("/shmex5",O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd,data_size);
	s2 = (estrutura*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
		
	printf("First (8000): %d Second (200): %d \n",s2->first, s2->second);
		
	r = munmap(s2, data_size); 		/* disconnects */
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

