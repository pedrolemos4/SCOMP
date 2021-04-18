#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>

void handle_USR1(int signo) {
    write(1, "Tarefas completas!\n", 20);
    exit(0);
}

int main(int argc, char *argv[]){
    int n;
    sigset_t mask;
    sigset_t pending;
    time_t t;

    srand ((unsigned) time (&t));

    n = rand () % 5;

    struct sigaction act;

    memset(&act, 0, sizeof(struct sigaction));

    act.sa_handler = handle_USR1; 
    sigemptyset(&mask);

    sigaddset(&mask, SIGUSR1);

    sigprocmask(SIG_BLOCK, &mask, 0);

    sigaction(SIGUSR1, &act, NULL);

    pid_t pid = fork();
    if(pid > 0){
        printf("Task A\n");
        sleep(1);
        printf("Passou 1 segundo.\n");
        sleep(1);
        printf("Passou 1 segundo.\n");
        sleep(1);
        printf("Passou 1 segundo.\n");
        kill(pid, 10);
        wait(NULL);
        
    }else{
        int i;
        printf("Task B\n");
        for (i = 0; i < n; i++){
            sleep(1);
            printf("Passou 1 segundo.\n");
        } 
		sigpending(&pending);
        if(!sigismember(&pending, SIGUSR1)){
			printf("Task C\n");
            sigprocmask(SIG_UNBLOCK, &mask, 0);
        }else{
			pause();
			printf("Task C\n");
            sigprocmask(SIG_UNBLOCK, &mask, 0);
        }
        
    }
    return 0;
}
