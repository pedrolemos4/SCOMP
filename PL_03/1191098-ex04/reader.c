#include <sys/mman.h>
#include <sys/stat.h> 		/* For constants "mode" */
#include <fcntl.h>			/* For constante O_* */
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 100

int main (int argc, char *argv[]){
	int fd,r, data_size = ARRAY_SIZE;
	int i;
	char *c1;
	fd = shm_open("/shmex4",O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd,data_size);
	c1 = (char*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
	int soma=0;
	int media;
	for(i=0;i<ARRAY_SIZE;i++){
		printf("%c, ascii code: %d\n",*(c1+i),*(c1+i));
		printf("==================\n");
		soma += (int)*(c1+i);
	}
	media = soma/ARRAY_SIZE;
	printf("A média é: %d\n",media);
	
	r = munmap(c1, data_size); 		/* disconnects */
	if (r < 0){ 					/* Check error */
		 exit(1);
	}
	r = close(fd); 					/* closes */
	if (r < 0){						/* Check error*/
		 exit(1); 
	}
	
	r = shm_unlink("/shmex4"); 		/* removes */
	if (r < 0) {					/* Check error */
		exit(1); 	
	}
	return 0;
}

