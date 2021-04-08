#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void handle_SIGINT (int signo, siginfo_t *sinfo, void *context){
	printf("I won´t let the process end with CTRL-C!\n");
}


int main() {

	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	sigemptyset(&act.sa_mask); /* No signals blocked */
	act.sa_sigaction = handle_SIGINT;
	sigaction(SIGINT, &act, NULL);
	
	for(;;){
		printf("I Like Signal\n");
		sleep(1);
	}
	
	return 0;
}
