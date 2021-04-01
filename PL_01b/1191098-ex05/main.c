#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

void handle_INT(int signo, siginfo_t *sinfo, void *context)
    {
    write(1,"\n“I won’t let the process end with CTRL-C!”\n",50);
    }

void handle_QUIT(int signo, siginfo_t *sinfo, void *context)
    {
    write(1,"\n“I won’t let the process end by pressing CTRL-\!”\n",50); 
    }

int main() {
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask); /* No signals blocked */
    act.sa_handler = handle_INT;
    sigaction(SIGINT, &act, NULL);
    
    struct sigaction act1;
    memset(&act1,0,sizeof(struct sigaction));
    sigemptyset(&act1.sa_mask);
    act1.sa_handler=handle_QUIT;
    sigaction(SIGQUIT,&act1,NULL);

	for(;;){
		printf("I Like Signal\n");
		sleep(1);
	}
	return 0;
}
