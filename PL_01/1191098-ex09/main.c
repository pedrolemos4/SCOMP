
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
	pid_t p;
	int status;
	int i;
	int k;
	
	for (k = 0; k<10;k++){
		p=fork();
		if(p==0){
			for(i=(k*100)+1;i<=(k+1)*100;i++){
				printf("%d. %d\n",k,i);
			}
		} else {
			exit(1);
		}
	}
	
	
	for(i =0; i<10;i++){
		wait(&status);
	}
	
	return 0 ;
}
		
		
		
		
		
		
		
		
