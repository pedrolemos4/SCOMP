#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void handle_USR1(int signo, siginfo_t *sinfo, void *context)
{
	/* Don’t use printf: it is not safe!!! */
	/* See man sigaction for list of safe functions. */
	printf("“SIGUSR1 has been captured and was sent by the process with PID %d\n",
	sinfo->si_pid);
}


int main(int argc, char *argv[]){
	
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask); /* No signals blocked */
	act.sa_sigaction = handle_USR1;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, NULL);

	pid_t p;
	int status;
	p=fork();
	
	if(p>0){
		printf("Hello I am Father\n");
		sleep(1);
        kill(p, SIGUSR1);
        wait(&status);
    }else{		
		printf("Hello I am Son\n");
		pause();
	}
	
	return 0;
}
