#include <sys/mman.h>
#include <sys/stat.h> 		/* For constants "mode" */
#include <fcntl.h>			/* For constante O_* */
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

typedef struct {
	int number;
	char name[80];
} stru;

int main(){
	int fd;
	stru *s;
	fd = shm_open("/shmex1",O_RDWR,S_IRUSR|S_IWUSR);
	ftruncate(fd,sizeof(stru));
	s = (stru*)mmap(NULL,sizeof(stru),PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
	printf("Nome do aluno é: %s \n",s->name);
	printf("Numero do aluno é: %d \n",s->number);
	return 0;
}
