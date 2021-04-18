#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>



void handle_ALRM(int signo) {
    write(1, "Too slow!\n", 11);
    exit(0);
}

int main(int argc, char *argv[]){
    struct sigaction act;
    int contador = 0;
    int i = 0;
    sigset_t mask;
    memset(&act, 0, sizeof(struct sigaction));
    act.sa_handler = handle_ALRM; 
    sigaction(SIGALRM, &act, NULL);
    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    char palavra[50];
    printf("Tem 10 segundos para introduzir uma string!\n");
    alarm(10);
    fgets(palavra,255,stdin);
    if(strlen(palavra) > 0){
        sigprocmask(SIG_BLOCK, &mask, 0);
		while (contador < (strlen(palavra)-1)) {
			contador++;
			i++;
		}
		printf("A string possui %d caracteres.\n", contador);
		sleep(20);
		printf("Successful execution!\n");
    }
    return 0;
}
