#include <stdio.h>
#include <sys/types.h> 
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <sys/mman.h> 
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <limits.h>
#include <string.h>

typedef struct {
	char *path;
	char *word;
	int occurences;
} est;

int main (int argc, char *argv[]){
	int fd,i,r,k;
	int DATA_SIZE = 10*sizeof(est);
	est *s1;
	pid_t pid;

	fd = shm_open("/shmex13", O_CREAT|O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
	ftruncate(fd,DATA_SIZE);
	s1 = (est*)mmap(NULL,DATA_SIZE,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
	
	char *ficheiros[10] = {"./ficheiro01.txt", "./ficheiro02.txt", "./ficheiro03.txt", "./ficheiro04.txt", "./ficheiro05.txt",
	"./ficheiro06.txt", "./ficheiro07.txt", "./ficheiro08.txt", "./ficheiro09.txt", "./ficheiro10.txt"};
	char *palavra = "Pedro";
	
	for(k=0;k<10;k++){
		est *filho = (s1 + (k*sizeof(est)));
		filho->path = *(ficheiros+k);
		filho->word = palavra;
	}
	
	for(i=0;i<10;i++){
		pid = fork();
	
		if(pid==0){
			est* filho = s1 + (i * sizeof(est));
			FILE* file = fopen(filho->path, "r");
			if(file == NULL){
				printf("Erro na leitura do ficheiro.\n");
				return 0;
			}
			char str[ strlen(filho->word) ];
			while(!feof(file)){
			fscanf(file, "%s", str);
				if(strcmp(filho->word, str) == 0){
					filho->occurences++;
				}
			}
			exit(0);
		}
	}
	
	for(i=0;i<10;i++){
		wait(NULL);
	}
	
	for (i = 0; i < 10; i++){
		est *filho = s1 + (i * sizeof(est));
		printf("Ficheiro%d - %s | Palavra (%s) encontrada %d vezes.\n", i+1, filho->path, filho->word, filho->occurences);
	}
	
	r = munmap(s1, DATA_SIZE); 		/* disconnects */
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

