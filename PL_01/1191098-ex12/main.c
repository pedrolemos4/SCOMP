#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
	int status;
	int i;
	int index = spawn_childs(6);

	if(index > 0) {
		exit(index * 2);
	} else if(index == 0) {
		for(i = 0; i < 6; i++) {
			pid_t p = wait(&status);
			if(WIFEXITED(status)) {
				printf("PID: %d, exit value: %d\n", p, WEXITSTATUS(status));
			}
		}
	}
	return 0;
}


int spawn_childs(int n) {
	pid_t p;
	int j;
	for(j = 0 ; j < n; j++) {
		p = fork();
		if(p==0){
			return j+1;
		}
	}
	return 0;
}

