#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

int nanosleep(const struct timespec *req, struct timespec *rem);

void handle_USR1(int signo, siginfo_t *sinfo, void *context){
	write(1,"Signal captured\n", 16);
	sleep(10);
}

int main(int argc, char *argv[]){
	
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
	sigfillset(&act.sa_mask); /* All signals blocked */
	act.sa_sigaction = handle_USR1;
	act.sa_flags = SA_SIGINFO;
	sigaction(SIGUSR1, &act, NULL);

	pid_t pid;
	int status;
	
	struct timespec delay;
	delay.tv_sec = 0;
	delay.tv_nsec = 10 * 1000000;
	
	pid = fork();
	
	if(pid == 0){
		kill(pid,SIGUSR1);
		nanosleep(&delay, NULL);
		kill(pid,SIGUSR2);
		nanosleep(&delay, NULL);
		kill(pid,SIGINT);
		nanosleep(&delay, NULL);
		
		
	} else {
		pause();
	}
	
	wait(&status);

	return 0;
}
