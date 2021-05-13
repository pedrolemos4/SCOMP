#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>


int main(int argc, char *argv[]){
	sem_t *semRead,*semWrite;
	int i,j;
	int array[200];
	FILE* numbers;
	FILE* output;

	if ((semRead = sem_open("semRead", O_CREAT | O_EXCL, 0644,1)) == SEM_FAILED) {
		perror("Error in sem_open()");
		exit(1);
	}
	
	if ((semWrite = sem_open("semWrite", O_CREAT | O_EXCL, 0644,1)) == SEM_FAILED) {
		perror("Error in sem_open()");
		exit(1);
	}
	
	pid_t p;
	for(i=0;i<8;i++){
		p = fork();
		if(p==0){
			sem_wait(semRead);
			numbers = fopen("Numbers.txt", "r");
			for(j = 0; j < 200; j++) {
				fscanf(numbers, "%d%*c", array + j);
			}
			
			fclose(numbers);
			sem_post(semRead);

			sem_wait(semWrite);
			output = fopen("Output.txt", "a");
            for(j = 0; j < 200; j++) {
                fprintf(output, "%d \n", *(array + j));
			}
			
			fclose(output); 
			sem_post(semWrite);
			exit(0);
		}
	}
	
	
	for(i=0;i<8;i++){
		wait(NULL);
	}
	sem_unlink("semRead");
	sem_unlink("semWrite");
	/*FILE* final = fopen("Output.txt","r");
	int result;
	fscanf(final,"%d",&(result));
	while(result!=EOF){
		printf("%d",result);
		fscanf(final,"%d",&(result));
	}
	fclose(final);*/
	return 0;	
}

