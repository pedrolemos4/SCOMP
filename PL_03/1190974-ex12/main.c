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
	int write;
	int read;
};

int main (int argc, char *argv[]){
	int fd,i,r,nota,max,min,avg,amount;
	int DATA_SIZE = sizeof(struct aluno);
	struct aluno *s1;
	pid_t pid;

	fd = shm_open("/shmex12", O_CREAT|O_EXCL|O_RDWR,
		S_IRUSR|S_IWUSR);
	ftruncate(fd,DATA_SIZE);
	s1 = (struct aluno*)mmap(NULL,DATA_SIZE,PROT_READ|PROT_WRITE,
		MAP_SHARED,fd,0);
	
	s1->write=1;
	s1->read=0;
	
	pid = fork();
	if(pid>0){
		while(s1->write==0);
		printf("Insira o seu numero: \n");
		scanf("%d",&(s1->numero));
		printf("Insira o seu nome: \n");
		scanf("%s",s1->nome);
		for(i=0;i<NR_DISC;i++){
			printf("Insira as suas notas: \n");
			scanf("%d",&nota);
			s1->disciplinas[i] = nota;
		}
		s1->read=1;
		s1->write=0;
	}else{
		while(s1->read==0);
		max=0;
		min=10000;
		amount=0;
		for(i=0;i<NR_DISC;i++){
			if(max<(s1->disciplinas[i])){
				max = s1->disciplinas[i];
			}else if(min>(s1->disciplinas[i])){
				min = s1->disciplinas[i];
			}
			amount += s1->disciplinas[i];
		}
		avg = amount/NR_DISC;
		
		printf("Nota mais alta: %d\n",max);
		printf("Nota mais baixa: %d\n",min);
		printf("Média: %d\n",avg);
		s1->read=1;
		s1->write=0;
		exit(0);
	}
	
	r = munmap(s1, DATA_SIZE); 	/* disconnects */
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

