#include <sys/mman.h>
#include <sys/stat.h> 		/* For constants "mode" */
#include <fcntl.h>			/* For constante O_* */
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#define DESCRIPTION_SIZE 80

typedef struct {
	 int product_code;
	 int price;
	 char description[DESCRIPTION_SIZE];
} shared_market;

int main (int argc, char *argv[]){
	int fd,r, data_size = sizeof(shared_market);
	shared_market *s1;
	fd = shm_open("/shmex2",O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd,data_size);
	s1 = (shared_market*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
	printf("O código do produto é: %d\n",s1->product_code);
	printf("A descrição do produto é: %s\n", s1->description);
	printf("O preço do produto é: %d\n", s1->price);
	
	r = munmap(s1, data_size); 		/* disconnects */
	if (r < 0){ 					/* Check error */
		 exit(1);
	}
	r = close(fd); 					/* closes */
	if (r < 0){						/* Check error*/
		 exit(1); 
	}
	
	r = shm_unlink("/shmex2"); 		/* removes */
	if (r < 0) {					/* Check error */
		exit(1); 	
	}
	return 0;
}

