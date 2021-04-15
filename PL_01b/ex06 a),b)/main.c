#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

volatile sig_atomic_t USR1_COUNTER=0;;

void handle_USR1(int signo, siginfo_t *sinfo, void *context){
	USR1_COUNTER++;
    printf("SIGUSR1 signalcaptured, USR1_COUNTER = %d\n",USR1_COUNTER);
	sleep(10);
}

int main(int argc, char *argv[]){
	struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    sigfillset(&act.sa_mask); /* All signals blocked */
    //sigemptyset(&act.sa_mask); /* No signals blocked */
    act.sa_sigaction = handle_USR1;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);

	for(;;){
		printf("I'm working!\n");
		sleep(1);
	}
	
	/*pid_t pid;
	pid=fork();
	
	if(pid>0){
		wait(NULL);
	}else {
		kill(getppid(),SIGUSR1);
		sleep(1);
		//kill(getppid(),SIGUSR2);
		kill(getppid(),SIGINT);
	}
	*/
	return 0;
}
