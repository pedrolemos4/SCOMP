#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SIZE_BUFFER 10
#define TIMES_TOTAL 3

int main(int argc, char *argv[]) {
	int i, j;
	int fd2[TIMES_TOTAL][2];
	int vec[SIZE_BUFFER];
	int readVec[SIZE_BUFFER];
	pid_t pid;
	for(i = 0; i < TIMES_TOTAL; i++){
		if (pipe(fd2[i]) == -1){
			perror("Pipe failed");
			return 1;
		}
	}

	pid = fork();
	if(pid == -1){
		perror("Fork falhou");
		exit(1);
	}

	if(pid > 0){ // PRODUTOR
		for(i = 0; i < TIMES_TOTAL; i++){
			close(fd2[i][0]);
			for(j = 0; j < SIZE_BUFFER; j++){
				vec[j] = i*10 + j ;
			}
			write(fd2[i][1], &vec, sizeof(vec));
			close(fd2[i][1]);
		}
		wait(NULL);
	} 

	if(pid == 0){
		for(i = 0; i < TIMES_TOTAL; i++){
			close(fd2[i][1]);
			if(read(fd2[i][0], &readVec, sizeof(readVec)) > 0){
				for(j = 0; j < SIZE_BUFFER; j++){
					printf("CONSUMIDOR\t=  \t%d\n", readVec[j]);
				}
			}
			close(fd2[i][0]);
		}
		exit(0);
	}	
	
	return 0;
}

