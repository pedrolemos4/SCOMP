#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main()
{
	int i;
	int status;
	int valor;
	pid_t p[4];

	for (i = 0; i < 4; i++) {
		p[i] = fork();
		if (p[i] == 0) {
			exit(i+1);
		}
	}
	for(i=0;i<4;i++){
		if(p[i] % 2 == 0 ){
			printf("À espera do pid par: (%d)\n", p[i]);
			waitpid(p[i],&status,0);
			if(WIFEXITED(status)){
                printf("pid par: %d foi executado.\n", p[i]);
            }
            valor = WEXITSTATUS(status);
            printf("Ordem de criação do filho: %d\n",valor);
		}	
	}
	
	printf("This is the end.\n");
	return 0;
} 
