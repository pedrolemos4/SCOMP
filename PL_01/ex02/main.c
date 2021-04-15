#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int main(void) {
    pid_t p;
    int status;
    printf("I'm..\n");
    
    p = fork();
    if(p == 0) {
        printf("I'll never join you!\n");
        exit(0);
    }
    wait(&status);
    printf("the..\n");
    p = fork();
    if(p == 0) {
        printf("I'll never join you!\n");
        exit(0);
    }
    wait(&status);
    printf("father!\n");
    
    p = fork();
    if(p == 0) {
        printf("I'll never join you!\n");
        exit(0);
    }
    wait(&status);
    return 0;
}
