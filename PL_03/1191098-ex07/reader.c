#include <sys/mman.h>
#include <sys/stat.h> 		/* For constants "mode" */
#include <fcntl.h>			/* For constante O_* */
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 10

int main (int argc, char *argv[]){
	
	int *ptr;
	int i;
	int fd,r, data_size = 4*ARRAY_SIZE;
	
	fd = shm_open("/shmex7",O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd,data_size);
	ptr = (int*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
	int soma=0;
	int media;
	
	for(i=0;i<ARRAY_SIZE;i++){
		printf("%d\n",*(ptr+i));
		printf("==================\n");
		soma += *(ptr+i);
	}
	media = soma/ARRAY_SIZE;
	printf("A média é: %d\n",media);
		
	r = munmap(ptr, data_size); 		/* disconnects */
	if (r < 0){ 					/* Check error */
		 exit(1);
	}
	r = close(fd); 					/* closes */
	if (r < 0){						/* Check error*/
		 exit(1); 
	}
	
	r = shm_unlink("/shmex7"); 		/* removes */
	if (r < 0) {					/* Check error */
		exit(1); 	
	}
	return 0;
}


	
