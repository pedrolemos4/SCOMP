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

#define ARRAY_SIZE 1000

int main (int argc, char *argv[]){
	int array[ARRAY_SIZE];
	int fd,r, data_size = sizeof(sizeof(int)*10);
	int i,j,k;
	int c = ARRAY_SIZE/10;
	//int max=0;
	int status;
	
	int *s1;
	pid_t pid;
	
	srand (time(NULL));

	fd = shm_open("/shmex9", O_CREAT|O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
	ftruncate(fd,data_size);
	s1 = (int*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
		
	for (i = 0; i < ARRAY_SIZE; i++){
		array[i] = rand () % 500 +1 ;
		printf("Array[%d]: %d\n",i,array[i]);
	}
	
	for(i=0; i < 10; i++){
		pid = fork();
		if(pid==0){
			int max = 0;
			for(j=i*c;j<i*c + c;j++){
				if(array[j]>max){
					max=array[j];
				}
			}
			printf("Max na %d iteração: %d\n",i,max);
			*(s1+i)=max;
			exit(0);
		} 
	}
	
	for(k=0; k < 10; k++){
		wait(&status);
	}
	
	int globalMax=0;
	for(k=0; k<10; k++){
		printf("Valor do array: %d\n",*(s1+k));
		if(*(s1+k)>globalMax){
			globalMax = *(s1+k);
		}
	}
	
	printf("GlobalMax: %d\n",globalMax);
	
	r = munmap(s1, data_size); 		/* disconnects */
	if (r < 0){ 					/* Check error */
		 exit(1);
	}
	r = close(fd); 					/* closes */
	if (r < 0){						/* Check error*/
		 exit(1); 
	}
	
	r = shm_unlink("/shmex9"); 		/* removes */
	if (r < 0) {					/* Check error */
		exit(1); 	
	}
	
	return 0;
}
	
	
	
