#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
	
	int i, j;
    int status;
    pid_t p1[4];
    
    for (i = 0; i < 4; i++) {
        p1[i] = fork();
        if (p1[i] == 0) {
            printf("pid do filho: %d, pid do pai: %d.\n", getpid(), getppid());
            exit(0);
        }
    }
    
	 for (j = 0; j < 4; j++) {
        if (p1[j] % 2 == 0) {
            printf("À espera do pid par: (%d)\n", p1[j]);
            waitpid(p1[j],&status,0);
            if(WIFEXITED(status)){
                printf("pid par: %d foi executado.\n", p1[j]);
            }
        }
    }

    printf("This is the end.\n");
    return 0;
}
