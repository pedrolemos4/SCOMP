#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
	int status,i;
	int i1 = spawn_childs(6);

	if(i1 > 0) {
		exit(i1 * 2);
	} else if(i1 == 0) {
		for(i = 0; i < 6; i++) {
			pid_t pid = wait(&status);
			if(WIFEXITED(status)) {
				printf("PID: %d Exit value: %d \n", pid, WEXITSTATUS(status));
			}
		}
	}
	return 0;
}


int spawn_childs(int n) {
	pid_t pid;
	int k;
	for(k = 0 ; k < n; k++) {
		pid = fork();
		if(pid==0){
			return k+1;
		}
	}
	return 0;
}

