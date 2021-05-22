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
	sem_t *array[8];
	char *nomes[8]={"1","2","3","4","5","6","7","8"};
	pid_t pid;
	
	for(i=0;i<8;i++){
		if((array[i] = sem_open(*(nomes+i),O_CREAT|O_EXCL,0644,0)) == SEM_FAILED){
			perror("Error in sem_open()");
			exit(1);
		}
	}
	
	FILE* file;
	FILE *fp;
	
	sem_post(array[0]);
	for(i=0;i<8;i++){
		pid=fork();		
		if(pid==0){
			sem_wait(array[i]);
			printf("Ordem: %d\n",i);
			file = fopen("./Numbers.txt", "r");
			for(k=0;k<200;k++){
				fscanf(file, "%d%*c", number+k);
			}
			fclose(file);
			
			fp = fopen("Output.txt","a");
			for(k=0;k<200;k++){
				fprintf(fp,"%d \n",*(number+k));
			}
			
			fclose(fp);
			sem_post(array[i+1]);
			exit(0);
		}
	}
	
	for(k=0;k<8;k++){
		wait(NULL);
	}
	for(i=0;i<8;i++){
		sem_unlink(nomes[i]);
	}
	
	return 0;
}

