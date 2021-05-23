#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <semaphore.h>

#define LOTACAO 200

int main(){
	int i, entrou, saiu, capacidadeRestante;
	sem_t *sem_porta1, *sem_porta2, *sem_porta3, *sem_lotacao;
	
	if((sem_porta1 = sem_open("semex11Porta1", O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}
	if((sem_porta2 = sem_open("semex11Porta2", O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}
	
	
	if((sem_porta3 = sem_open("semex11Porta3", O_CREAT, 0644, 1)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}
	
	if((sem_lotacao = sem_open("semex11Lotacao", O_CREAT, 0644, 200)) == SEM_FAILED) {
		perror("Failed sem_open\n");
		return 1;
	}
	
	for(i=0;i<LOTACAO;i++){
		pid_t p = fork();
		
		if(p==0){
			
			entrou = 0;
			
			while(entrou ==0){
				if(sem_trywait(sem_porta1)!=0){
					sem_wait(sem_lotacao);
					entrou =1;
					
					sem_getvalue(sem_lotacao,&capacidadeRestante);
					printf("Passageiro entrou pela porta 1 - Capacidade Restante %d\n", capacidadeRestante);						
					
					sem_post(sem_porta1);
					break;
				}
				if(sem_trywait(sem_porta2)!=0){
					sem_wait(sem_lotacao);
					entrou = 1;
					
					sem_getvalue(sem_lotacao,&capacidadeRestante);
					printf("Passageiro entrou pela porta 2 - Capacidade Restante %d\n", capacidadeRestante);						
					
					sem_post(sem_porta2);
					break;
				}
				if(sem_trywait(sem_porta3)!=0){
					sem_wait(sem_lotacao);
					
					entrou = 1;
					
					sem_getvalue(sem_lotacao,&capacidadeRestante);
					printf("Passageiro entrou pela porta 3 - Capacidade Restante %d\n", capacidadeRestante);						
					
					sem_post(sem_porta3);
					break;
				}
			}
			
			saiu = 0;
			while(saiu==0){
				if(sem_trywait(sem_porta1)!=0){
					sem_post(sem_lotacao);
					saiu=1;
					
					sem_getvalue(sem_lotacao,&capacidadeRestante);
					printf("Passageiro saiu pela porta 1 - Capacidade Restante %d\n", capacidadeRestante);						
					
					sem_post(sem_porta1);
					break;
				}
				 
				if(sem_trywait(sem_porta2)!=0){
					sem_post(sem_lotacao);
					
					saiu = 1;
					sem_getvalue(sem_lotacao,&capacidadeRestante);
					printf("Passageiro saiu pela porta 2 - Capacidade Restante %d\n", capacidadeRestante);
					
					sem_post(sem_porta2);
					break;
				}
				
				if(sem_trywait(sem_porta3)!=0){
					sem_post(sem_lotacao);
					
					saiu = 1;
					sem_getvalue(sem_lotacao,&capacidadeRestante);
					
					printf("Passageiro saiu pela porta 3 - Capacidade Restante %d\n", capacidadeRestante);
					
					sem_post(sem_porta3);
					break;
				}
			}
			exit(0);
		}
	}
	
	for(i = 0 ; i< LOTACAO;i++){
		wait(NULL);
	}
	
	sem_unlink("semex11Porta1");
	sem_unlink("semex11Porta2");
	sem_unlink("semex11Porta3");
	sem_unlink("semex11Lotacao");

	return 0;
}
