#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#define NUM_CHILDS 50

int simula1() {
    srand((unsigned) time(NULL));
    return rand() % 2; // Retorna 0 ou 1
}

void simula2() {
    printf("simula2() executed\n");
}

volatile sig_atomic_t n_execs = 0;
volatile sig_atomic_t n_success = 0;

void handle_success(int signo, siginfo_t *sinfo, void *context) {
    n_execs++;
    n_success++;
}

void handle_insuccess(int signo, siginfo_t *sinfo, void *context) {
    n_execs++;
}

void handle_mid_exec(int signo, siginfo_t *sinfo, void *context) {
    simula2();
    _exit(0);
}

int main() {
    // Handler do signal de sucesso (SIGURS1 - pai)
    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));
    sigfillset(&act.sa_mask);
    act.sa_sigaction = handle_success;
    act.sa_flags = SA_SIGINFO | SA_NODEFER;
    sigaction(SIGUSR1, &act, NULL);
    // Handler do signal de insucesso (SIGURS2 - pai)
    struct sigaction act2;
    memset(&act2, 0, sizeof(struct sigaction));
    sigfillset(&act2.sa_mask);
    act2.sa_sigaction = handle_insuccess;
    act2.sa_flags = SA_SIGINFO | SA_NODEFER;
    sigaction(SIGUSR2, &act2, NULL);

    int i;
    pid_t p[NUM_CHILDS];
    for (i = 0; i < NUM_CHILDS; i++) {
        p[i] = fork();
        if (p[i] < 0) {
			return 1; // Erro ao fazer fork
		}
        if (p[i] == 0) {
            // Handler de signal (SIGUSR1 - child)
            // Este signal pode ser enviado pelo pai quando atinge 25 processos terminados e houve algum sucesso
            sigemptyset(&act.sa_mask);
            act.sa_sigaction = handle_mid_exec;
            sigaction(SIGUSR1, &act, NULL);

            if (simula1()){
                kill(getppid(), SIGUSR1); // Sucesso
            } else{ 
                kill(getppid(), SIGUSR2); // Insucesso
            }
            
            pause();
            simula2();

            exit(0);
        }
        sleep(1);
    }

    while (n_execs < 25); // Esperar que 25 processos tenham chegado à pausa

    for (i = 0; i < 25; i++) {
        // Avisa os childs em pausa para avançar para simula2()
        kill(p[i], SIGUSR2);
        wait(NULL);
    }

    if (n_success == 0) {
        printf("Inefficient algorithm!\n");
        for (i = 25; i < NUM_CHILDS; i++) {
            kill(p[i], SIGKILL); // Termina execução dos restantes filhos
         }
    } else {
        for (i = 25; i < NUM_CHILDS; i++) {
            kill(p[i], SIGUSR1); // Sinal para avançar instantaneamente para simula2()
            wait(NULL);
        }
    }
	return 0;
}
