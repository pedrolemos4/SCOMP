#include <sys/mman.h>
#include <sys/stat.h> 		/* For constants "mode" */
#include <fcntl.h>			/* For constante O_* */
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define ARRAY_SIZE 100
int main (int argc, char *argv[]){
	
	char *ptr;
	int fd,i,r,data_size = ARRAY_SIZE;
	
	srand(time(NULL)); 
	fd = shm_open("/shmex4", O_CREAT|O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
	ftruncate(fd,data_size);
	ptr = (char*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
	for(i=0;i<ARRAY_SIZE;i++){
		*(ptr+i) = (rand()%(90-65))+65;
		printf("%c, ascii code: %d\n",*(ptr+i),*(ptr+i));
		printf("=================\n");
		
	}
	r = munmap(ptr, data_size); 		/* disconnects */
	if (r < 0){ 					/* Check error */
		 exit(1);
	}
	r = close(fd); 		/* closes */
	if (r < 0){			/* Check error*/
		 exit(1); 
	}
	return 0;
}
	
