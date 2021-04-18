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
    int count = 0;
    int i = 0;
    sigset_t mask;
    memset(&act, 0, sizeof(struct sigaction));
    act.sa_handler = handle_ALRM; 
    sigaction(SIGALRM, &act, NULL);
    sigemptyset(&mask);
    sigaddset(&mask, SIGALRM);
    char s[50];
    printf("Input the string!!! 10s!!!\n");
    pid_t new = fork();
    if(new != 0){
        int result = scanf("%s", s);
        if(result > 0){
            sigprocmask(SIG_BLOCK, &mask, 0);
            while (s[i] != 0) {
                count++;
                i++;
            }
        }
        printf("%d\n", count);
        sleep(20);
        printf("Successful execution!\n");
    }else{
        sleep(10);
        kill(getppid(),14);
    }
    return 0;
}
