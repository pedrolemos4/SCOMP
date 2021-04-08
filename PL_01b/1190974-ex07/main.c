#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	
	sigset_t mask, pending;

	sigfillset(&mask);
	sigaddset(&mask,SIGUSR1);
	sigprocmask(SIG_BLOCK, NULL, 0);
	
	sigpending(&pending);
	if(sigismember(&pending, SIGUSR1)){
		printf("SIGUSR1 pending\n");
	}
	
	return 0;
}
