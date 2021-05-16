#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>


int main (int argc, char *argv[]){
	int number[200];
	int i,k;
	sem_t *sem_semafro,*sem_semafro1;
	pid_t pid;
	
	sem_semafro=sem_open("sem_semafro",O_CREAT|O_EXCL,0644,1);
	sem_semafro1=sem_open("sem_semafro1",O_CREAT|O_EXCL,0644,1);
	
	FILE* file;
	FILE *fp;
	
	for(i=0;i<8;i++){
		pid=fork();		
		if(pid==0){
			sem_wait(sem_semafro);
			file = fopen("./Numbers.txt", "r");
			for(k=0;k<200;k++){
				fscanf(file, "%d%*c", number+k);
			}
			fclose(file);
			
			sem_post(sem_semafro);
			
			sem_wait(sem_semafro1);
			fp = fopen("Output.txt","a");
			for(k=0;k<200;k++){
				fprintf(fp,"%d \n",*(number+k));
			}
			
			fclose(fp);
			sem_post(sem_semafro1);
			exit(0);
		}
	}
	
	for(k=0;k<8;k++){
		wait(NULL);
	}
	sem_unlink("sem_semafro");
	sem_unlink("sem_semafro1");
	
	return 0;
}

