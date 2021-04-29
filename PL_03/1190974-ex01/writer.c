#include <sys/mman.h>
#include <sys/stat.h> 		/* For constants "mode" */
#include <fcntl.h>			/* For constante O_* */
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>


#define NAME_SIZE 80

typedef struct {
	 int number;
	 char name[NAME_SIZE];
} stru;

int main(int argc, char *argv[]){
	int fd;
	stru *s;
	fd = shm_open("/shmex1", O_CREAT|O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
	ftruncate(fd,sizeof(stru));
	s = (stru*)mmap(NULL,sizeof(stru),PROT_WRITE|PROT_READ,MAP_SHARED,fd,0);
	printf("Introduza o nome do aluno:\n");
	fgets(s->name, 80, stdin);
	printf("Introduza a idade do aluno:\n");
	scanf("%d",&(s->number));
	return 0;
}
