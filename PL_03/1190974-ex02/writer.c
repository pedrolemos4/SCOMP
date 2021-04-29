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

int main(int argc, char *argv[]){
	int fd;
	stru *s;
	fd = shm_open("/shmex2", O_CREAT|O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
	ftruncate(fd,sizeof(stru));
	s = (stru*)mmap(NULL,sizeof(stru),PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
	printf("Introduza a descricao:\n");
	fgets(s->desc, 80, stdin);
	printf("Introduza o preco:\n");
	scanf("%d",&(s->price));
	printf("Introduza o codigo:\n");
	scanf("%d",&(s->code));
	return 0;
}
