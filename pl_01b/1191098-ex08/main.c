#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

volatile sig_atomic_t CHILD;

void handle_CHLD(int signo,siginfo_t *sinfo, void *context){
	printf("SIGCHLD has been captured\n");
	CHILD++;
	printf("%d\n",CHILD);
	sleep(1);
}

int main(int argc, char *argv[]){
	struct sigaction act;
	memset(&act, 0, sizeof(struct sigaction));
    sigemptyset(&act.sa_mask); /* No signals blocked */
    act.sa_sigaction = handle_CHLD;
    act.sa_flags = SA_SIGINFO;
    act.sa_flags = SA_NOCLDSTOP;
    act.sa_flags = SA_NOCLDWAIT;
    sigaction(SIGCHLD, &act, NULL);

	int status;
	pid_t p;
	int i;
	int k=0;
	for(i = 0; i<5; i++){
		p=fork();
		/*if(p>0){
			while(CHILD!=5){
				pause();
			}
		} else*/
		if(p==0){
			int low = i*200;
			int max =(i+5)*200;
			for(k = low;k<=max;k++){
				printf("%d\n",k);
			}
			sleep(5);
			exit(1);
			
		} 
		/*else{
			exit(1);
		}*/
		
	}
	
	while(CHILD!=5){
		pause();
		printf("Child= %d\n", CHILD);
	}
		
	for(i =0; i<5;i++){
		wait(&status);
	}


	return 0;
}
