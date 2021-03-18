#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(void) {
	
    int status1, status2;
    pid_t p1 = fork();
    waitpid(p1,&status1,0);
    
    if(p1 > 0){
        pid_t p2 = fork();
        waitpid(p2,&status2,0);
        if(p2 > 0){
            if(WIFEXITED(status1) && WIFEXITED(status2)){
                printf("filho 1 retorna valor: %d\n", WEXITSTATUS(status1));
                printf("filho 2 retorna valor: %d\n", WEXITSTATUS(status2));
            }
        } else {
            sleep(2);
            exit(2);
        }
    } else {
        sleep(1);
        exit(1);
    }
    return 0;
}
