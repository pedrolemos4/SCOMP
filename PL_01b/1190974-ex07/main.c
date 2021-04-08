#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <sys/types.h>

void handle_SIGUSR1 (int signo, siginfo_t *sinfo, void *context){
	
	sigset_t mask, pending;
	
	sigfillset(&mask);
	sigaddset(&mask,SIGUSR1);
	sigprocmask(SIG_BLOCK, NULL, 0);
	
	sigpending(&pending);
	if(sigismember(&pending, SIGUSR1)){
		printf("SIGUSR1 pending\n");
	}
	
	for(;;){
		printf("handle_SIGUSR1\n");
		sleep(1);
	}
	
	sigprocmask(SIG_UNBLOCK, &mask, 0);
}

int main() {
	
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask); 
	act.sa_sigaction = handle_SIGUSR1;
	sigaction(SIGUSR1, &act, NULL);
	
	for(;;){
		printf("Waiting for SIGUSR1...\n");
		sleep(1);
	}
	
	return 0;
}
