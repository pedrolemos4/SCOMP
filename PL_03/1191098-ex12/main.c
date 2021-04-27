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


#define STR_SIZE 50
#define NR_DISC 10

struct aluno{
	int numero;
	char nome[STR_SIZE];
	int disciplinas[NR_DISC];
	int canRead;
	int canWrite;
};

int main (int argc, char *argv[]){
	int fd,r, data_size = sizeof(struct aluno);
	pid_t pid;
	int i,k;
	int soma=0;
	int media;
	struct aluno *s1;
	
	fd = shm_open("/shmex12", O_CREAT|O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
	ftruncate(fd,data_size);
	s1 = (struct aluno*)mmap(NULL,data_size,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
	
	s1->canRead = 0;
	s1->canWrite = 1;
	pid = fork();
	if(pid>0){
		while(s1->canWrite==0);
		s1->canWrite=0;	
		printf("Introduza o número do aluno:");
		scanf("%d",&(s1->numero));
		printf("\nIntroduza o nome do aluno:");
		scanf("%s",s1->nome);
		int nota;
		for(i=0;i<NR_DISC;i++){
			printf("\nIntroduza nota do aluno:\n");
			scanf("%d", &nota);
			s1->disciplinas[i] = nota;
		}
		s1->canRead=1;
		wait(NULL);
	} else {
		while(s1->canRead==0);
		s1->canRead=0;
		int notas[NR_DISC];
		int max=0;
		int min = INT_MAX;
		for(k=0;k<NR_DISC;k++){
			notas[k] = s1->disciplinas[k];
			if(notas[k]>max){
				max=notas[k];
			}
			if(notas[k]<min){
				min = notas[k];
			}
			soma +=notas[k];
		}
		media =soma/NR_DISC;
		
		printf("Nota mais alta: %d\n",max);
		printf("Nota mais baixa: %d\n",min);
		printf("Média: %d\n",media);
		s1->canWrite=1;
		exit(0);
	}
	
	r = munmap(s1, data_size); 		/* disconnects */
	if (r < 0){ 					/* Check error */
		 exit(1);
	}
	r = close(fd); 					/* closes */
	if (r < 0){						/* Check error*/
		 exit(1); 
	}
	
	r = shm_unlink("/shmex12"); 		/* removes */
	if (r < 0) {					/* Check error */
		exit(1); 	
	}
	
	return 0;
}
		
			
