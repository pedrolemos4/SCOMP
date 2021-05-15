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

#define STRING_SIZE 80

typedef struct {
	 char *path;
	 char *word;
	 int occurrences;
} sharedStruct;

int main (int argc, char *argv[]){
	int fd,r, data_size = 10 *sizeof(sharedStruct);
	int i;
	sharedStruct *s1;
	pid_t p;
	
	fd = shm_open("/shmex13", O_CREAT|O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
	ftruncate(fd,data_size);
	s1 = (sharedStruct*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
		
	char ficheiros[10] = {"files/ficheiro01.txt", "files/ficheiro02.txt", "files/ficheiro03.txt", "files/ficheiro04.txt", "files/ficheiro05.txt",
	"files/ficheiro06.txt", "files/ficheiro07.txt", "files/ficheiro08.txt", "files/ficheiro09.txt", "files/ficheiro10.txt"};
	char *palavra = "Tiago";
	
	for(i=0;i<10;i++){
		sharedStruct *filho = (s1 + (i*sizeof(sharedStruct)));
		filho->path = *(ficheiros + i);
		filho->word = palavra;
	}
	
	for(i=0;i<10;i++){
		p = fork();
		if(p==0){
			sharedStruct* filho = s1 + (i * sizeof(sharedStruct));
			FILE* file = fopen(filho->path, "r");
			if(file == NULL){
				printf("Erro na leitura do ficheiro.\n");
				return 0;
			}
			char str[ strlen(filho->word) ];
			while(!feof(file)){
				fscanf(file, "%s", str);
				if(strcmp(filho->word, str) == 0)
					filho->occurrences++;
			}
			exit(0);
		}
	}
	
	for(i=0;i<10;i++){
		wait(NULL);
	}
	
	
	for (i = 0; i < 10; i++){
		sharedStruct  *filho = s1 + (i * sizeof(sharedStruct));
		printf("Ficheiro%d - %s | Palavra (%s) encontrada %d vezes.\n", i+1, filho->path, filho->word, filho->occurrences);
	}
	
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
