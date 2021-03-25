#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int main() {
	 int opc;
	 int pid;
	 printf("DIZ O PID POR FAVOR\n");
		 scanf("%d", &pid);
	 pid_t p = pid;
	 do
	 {
		 printf("Stop - 1, Continue - 2, Kill - 3\n");
		 scanf("%d", &opc);
		 if (opc == 1){
			 kill(p,SIGSTOP);
		 }
		 if (opc == 2){
			 kill(p,SIGCONT);
		 }
		 if (opc == 3){
			 kill(p,SIGKILL);
		 }
		 
	 } while (1 != 0);
	 return 0;
}
