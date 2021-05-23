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
	int i;
	
	sharedStruct *s1;
	
	fd = shm_open("/shmex10", O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
		
	ftruncate(fd,data_size);
	
	s1 = (sharedStruct*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
	
	sem_t *sem;
	if ((sem = sem_open("semex10ConsultAll", O_EXCL, 0644, 1)) == SEM_FAILED) {
		perror("Erro a criar o semaforo.\n");
		exit(0);
	}
	
	sem_wait(sem);
	
	sleep(50);
	if(s1->posicao==0){
		printf("Não existem registos.\n");
	}
	
	for(i = 0 ; i < s1->posicao;i++){
		user* user = &(s1->users[i]);
		printf("Número do User: %d\nNome do User: %s\nMorada do User: %s\n",user->number,user->name,user->address);
	}
	
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
