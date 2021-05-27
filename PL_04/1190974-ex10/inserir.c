#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>
#include "estruturas.h"

int main(){
	
	int fd, r, data_size =sizeof(sharedStruct);
	int number;
	char name[50];
	char address[50];
	
	sharedStruct *s1;
	
	fd = shm_open("/shmex10", O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
		
	ftruncate(fd,data_size);
	
	s1 = (sharedStruct*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
	
	sem_t *sem;
	if ((sem = sem_open("semex10Insert", O_EXCL, 0644, 1)) == SEM_FAILED) {
		perror("Erro a criar o semaforo.\n");
		exit(0);
	}
	
	sem_wait(sem);
	user* u1 = &(s1->users[s1->posicao]);
	
	printf("Insira o número do utilizador:");
	scanf("%d",&number);
	getc(stdin); //Para remover o '\n' seguido na leitura do numero
	u1->number = number;
	
	printf("Insira o nome do utilizador:");
	fgets(name,50,stdin);
	strcpy(u1->name,name);
	
	printf("Insira a morada do utilizador:");
	fgets(address,50,stdin);
	strcpy(u1->address,address);
	s1->posicao++;
	
	sem_post(sem);
	
	r = munmap(s1, data_size); 		/* disconnects */
	if (r < 0){ 					/* Check error */
		 exit(1);
	}
	r = close(fd); 					/* closes */
	if (r < 0){						/* Check error*/
		 exit(1); 
	}
	
	return 0;
}

