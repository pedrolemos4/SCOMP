#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h> 
#include <time.h>
#include <semaphore.h>
#include "bilhetes.h"


int main(){
	int fd, r, data_size =sizeof(bilhetes);
	int bilhete,i;
	bilhetes *b1;
	
	sem_unlink("semex12Seller");
	sem_unlink("semex12Cliente");
	sem_unlink("semex12FinalSeller");
	
	fd = shm_open("/shmex12", O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
		
	ftruncate(fd,data_size);
	
	b1 = (bilhetes*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
	
	sem_t* sem1;
	sem_t* sem2;
	sem_t* sem3;
	if ((sem1 = sem_open("semex12Seller", O_CREAT|O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("Erro a criar o semaforo.\n");
		exit(0);
	}	
	if ((sem2 = sem_open("semex12Cliente", O_CREAT|O_EXCL, 0644, 1)) == SEM_FAILED) {
		perror("Erro a criar o semaforo.\n");
		exit(0);
	}	
	if ((sem3 = sem_open("semex12FinalSeller", O_CREAT|O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("Erro a criar o semaforo.\n");
		exit(0);
	}
	
	while(b1->index < 6){
		printf("Início da venda.\n");
		sem_wait(sem1);
		printf("Comprador\n");
		b1->index++;
		sem_post(sem3);
		sem_wait(sem1);
		printf("Vendedor: Cliente %d terminou.\n", b1->index -1);
		sem_post(sem2);
	}
	
	sem_unlink("semex12Seller");
	sem_unlink("semex12Cliente");
	sem_unlink("semex12FinalSeller");
	
	r = munmap(b1, data_size); 		/* disconnects */
	if (r < 0){ 					/* Check error */
		 exit(1);
	}
	r = close(fd); 					/* closes */
	if (r < 0){						/* Check error*/
		 exit(1); 
	}
	
	return 0;
}
