#include <sys/mman.h>
#include <sys/stat.h> 		/* For constants "mode" */
#include <fcntl.h>			/* For constante O_* */
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>


#define NAME_SIZE 80

typedef struct {
	 int idade;
	 char nome[NAME_SIZE];
} shared_struct;

int main (int argc, char *argv[]){
	int fd, data_size = sizeof(shared_struct);
	shared_struct *s1;
	fd = shm_open("/shmex1", O_CREAT|O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
	ftruncate(fd,data_size);
	s1 = (shared_struct*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
	printf("Introduza o nome do aluno:\n");
	fgets(s1->nome, NAME_SIZE, stdin);
	printf("Introduza a idade do aluno:\n");
	scanf("%d",&(s1->idade));
	return 0;
}
