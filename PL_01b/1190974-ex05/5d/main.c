#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void handle_SIGQUIT (int signo, siginfo_t *sinfo, void *context){
	 write(1,"I won’t let the process end with CTRL-\!”\n",50);
 }

void handle_SIGINT (int signo, siginfo_t *sinfo, void *context){
	write(1,"I won’t let the process end with CTRL-C!”\n",50);
 }


int main() {

	for(;;){

		struct sigaction act;
		memset(&act, 0, sizeof(struct sigaction));
		sigemptyset(&act.sa_mask); /* No signals blocked */
		act.sa_sigaction = handle_SIGINT;
		sigaction(SIGINT, &act, NULL);
		
		struct sigaction act1;
		memset(&act1, 0, sizeof(struct sigaction));
		sigemptyset(&act1.sa_mask); /* No signals blocked */
		act1.sa_sigaction = handle_SIGQUIT;
		sigaction(SIGQUIT, &act1, NULL);
	}
	return 0;
}
