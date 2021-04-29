#include <sys/mman.h>
#include <sys/stat.h> 		/* For constants "mode" */
#include <fcntl.h>			/* For constante O_* */
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	int code;
	int price;
	char desc[80];
} stru;

int main(){
	int fd;
	stru *s;
	fd = shm_open("/shmex2",O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd,sizeof(stru));
	s = (stru*)mmap(NULL,sizeof(stru),PROT_READ,MAP_SHARED,fd,0);
	printf("Descricao é: %s \n",s->desc);
	printf("Codigo é: %d \n",s->code);
	printf("Preco é: %d \n",s->price);
	return 0;
}
