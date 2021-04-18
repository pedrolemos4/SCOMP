#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <string.h>

int main(int argc, char *argv[]){

	int fd[2], pids[10];
	int i,status;
	
	pid_t pid;
	
	struct struct_s1{
		char string[10];
		int round;
	} s;
	struct struct_s1* ptr = &s;
	struct struct_s1* ptr1 = &s;
	
	if(pipe(fd) == -1){
		perror("Pipe failed");
		return 1;
	}
	
	for(i=0;i<10;i++){
		pid = fork();
	
		if(pid==0){
			close(fd[1]);	
			read(fd[0],ptr1,20);
			close(fd[0]);
			
			printf("PID: %d Winner message: %s Number: %d \n",getpid(),ptr1->string,ptr1->round);
			
			exit(ptr1->round);
		}else{
			//O pid do filho (>0) é retornado no processo pai, 
			//logo o pid do filho é adicionado aos pids no processo pai
			pids[i]=pid;
		}
	}
	
	
	for(i=0;i<10;i++){
		sleep(2);
		strcpy(ptr->string, "Win");
		ptr->round=i;	
		close(fd[0]);		
		write(fd[1],ptr,20);
	}
	close(fd[1]);
	
	for(i=0;i<10;i++){
		waitpid(pids[i],&status,0);
		if(WIFEXITED(status))printf("Exit value: %d PID: %d \n",WEXITSTATUS(status), pids[i]);
	}
	
	return 0;
}
