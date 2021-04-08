#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int USR1_COUNTER=0;

void handle_signal (int signo, siginfo_t *sinfo, void *context){
	printf("SIGUSR1 signal captured, USR1_COUNTER = %d. \n", USR1_COUNTER++);
	sleep(1);
}

int main() {
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	sigfillset(&act.sa_mask);
	act.sa_sigaction = handle_signal;
	sigaction(SIGUSR1, &act, NULL);
		
	for(;;){
		printf("I'm working!\n");
	}
	
	return 0;
}
