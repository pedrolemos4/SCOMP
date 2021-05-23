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

	fd = shm_open("/shmex12", O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
		
	ftruncate(fd,data_size);
	
	b1 = (bilhetes*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
	
	sem_t* sem1;
	sem_t* sem2;
	sem_t* sem3;
	if ((sem1 = sem_open("semex12Seller", O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("Erro a criar o semaforo.\n");
		exit(0);
	}	
	if ((sem2 = sem_open("semex12Cliente", O_EXCL, 0644, 1)) == SEM_FAILED) {
		perror("Erro a criar o semaforo.\n");
		exit(0);
	}	
	if ((sem3 = sem_open("semex12FinalSeller", O_EXCL, 0644, 0)) == SEM_FAILED) {
		perror("Erro a criar o semaforo.\n");
		exit(0);
	}
	
	for(i=0;i<5;i++){
		printf("Início da compra%d.\n",i);
		sem_wait(sem2);
		bilhete = b1->index;
		printf("Cliente: Bilhete numero %d adquirido.\n", bilhete);
		sem_post(sem1);
		sem_wait(sem3);
		srand(time(NULL));
		int x = rand() %10;
		printf("Cliente: A espera %d segundos.\n", x);
		sleep(x);
		/* Terminar o processo */
		sem_post(sem1);
	}
	
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
	
	
	
	
