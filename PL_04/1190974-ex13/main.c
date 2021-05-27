#include <stdio.h>
#include <sys/types.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mman.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <string.h>
#include <semaphore.h>

typedef struct{
	int index;
	int buffer[10];
} sharedStruct;

int main(){
	int i,j,fd,r, data_size = sizeof(sharedStruct);
	pid_t pid;
	
	sharedStruct* s1;
	
	fd = shm_open("/shmex13", O_CREAT|O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
	ftruncate(fd,data_size);
	s1 = (sharedStruct*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
	
	sem_t* sem1;
	sem_t* sem2;
	sem_t* sem3;
	if ((sem1 = sem_open("semex13Access", O_CREAT|O_EXCL, 0644, 1)) == SEM_FAILED) {
		perror("Erro a criar o semaforo.\n");
		exit(0);
	}	
	if ((sem2 = sem_open("semex13Control", O_CREAT|O_EXCL, 0644, 10)) == SEM_FAILED) {
		perror("Erro a criar o semaforo.\n");
		exit(0);
	}	
	
	if((sem3 = sem_open("semex13Pai",O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
		perror("Erro a criar o semaforo.\n");
		exit(0);
	}
	
	for(i=0;i<2;i++){
		pid = fork();
		
		if(pid==0){
			while(s1->index < 30){
				sem_wait(sem2);
				sem_wait(sem1);
			
				if(s1->index < 30) { 
					printf("Filho escreveu %d\n", s1->index);
					
					s1->buffer[s1->index % 10] = s1->index;
					
					s1->index++;
				}
				
				if(s1->index % 10 == 0){
					sem_post(sem3);
				}
				
				sem_post(sem1);
			}
			exit(0);
		}
	}
	
	while(s1->index < 30 ){
		sem_wait(sem3);
		 
		for(j=0;j<10;j++){
			printf("Pai leu %d\n", s1->buffer[j]);
		}
		
		for(j = 0; j < 10; j++) {
			sem_post(sem2);
		}
	}
	
	sem_unlink("semex13Access");
	sem_unlink("semex13Control");
	sem_unlink("semex13Pai");
	
	r = munmap(s1, data_size); 		/* disconnects */
	if (r < 0){ 					/* Check error */
		 exit(1);
	}
	r = close(fd); 					/* closes */
	if (r < 0){						/* Check error*/
		 exit(1); 
	}
	
	r = shm_unlink("/shmex13"); 		/* removes */
	if (r < 0) {					/* Check error */
		exit(1); 	
	}
	
	return 0;
}
