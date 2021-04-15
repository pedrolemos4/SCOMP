#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
	
	int i;
    int status;
    pid_t p1[4];
    int valor;
    
    for (i = 0; i < 4; i++) {
        p1[i] = fork();
        if (p1[i] == 0) {
            printf("pid do filho: %d, pid do pai: %d.\n", getpid(), getppid());
            exit(i+1);
        }
    }
    
	 for(i=0;i<4;i++){
		if(p1[i] % 2 == 0 ){
			printf("À espera do pid par: (%d)\n", p1[i]);
			waitpid(p1[i],&status,0);
			if(WIFEXITED(status)){
                printf("pid par: %d foi executado.\n", p1[i]);
            }
            valor = WEXITSTATUS(status);
            printf("Ordem de criação do filho: %d\n",valor);
		}	
	}

    printf("This is the end.\n");
    return 0;
}
