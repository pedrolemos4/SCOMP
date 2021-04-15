#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <stdio.h>

struct sigaction act;
volatile sig_atomic_t i = 1;
    void signals_set(sigset_t * set)
{
    sigprocmask(SIG_SETMASK, NULL, set);
    while (i<=65)  { 
        if (sigismember(set, i) == 1){
            char c[27];
            sprintf(c, "O sinal %d está bloqueado.\n", i);
            write(STDOUT_FILENO, c, 28);
        } else{
            char c[31];
            sprintf(c, "O sinal %d não está bloqueado.\n", i);
            write(STDOUT_FILENO, c, 34);
        }
        ++i;
    }
}

void handle_signal(int sign, siginfo_t *sinfo, void *context){
    signals_set(&act.sa_mask);
}

int main(){
    memset(&act, 0, sizeof(struct sigaction));
    sigfillset(&act.sa_mask);
    act.sa_sigaction = handle_signal;
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);
    raise(SIGUSR1); 
    return 0;
}