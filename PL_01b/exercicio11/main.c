#include <stdio.h>//function printf
#include <stdlib.h>//function exit
#include <string.h>//function memset;strcpy;
#include <unistd.h>//function fork;execvp;sleep;
#include <signal.h>//function sigemptyset;sigaction;kill
#include <sys/wait.h>//function wait

/*
 * Struct for the command that was given.
 */
typedef struct {
    char cmd[100];
    int tempo;
} comando;

volatile sig_atomic_t time_exceeded = 0;

//Enters in this handler if the child process is terminated
void handle_signal(int signal) {
    time_exceeded = 1;
}

int main() {

    struct sigaction act;
    memset(&act, 0, sizeof(struct sigaction));//cleans the struct
    sigemptyset(&act.sa_mask); //no signals blocked
    act.sa_handler = handle_signal; //defines the signal handler
    sigaction(SIGCHLD, &act, NULL);//SIGCHILD is now captured and handled in the handle_signal function


/*
 * Creates the list of commands that will be executed.
 */
    comando list_of_commands[4];
    strcpy(list_of_commands[0].cmd, "./testes/TEST1/prog_test");
    list_of_commands[0].tempo = 10;
    strcpy(list_of_commands[1].cmd, "./testes/TEST2/prog_test");
    list_of_commands[1].tempo = 20;
    strcpy(list_of_commands[2].cmd, "./testes/TEST3/prog_test");
    list_of_commands[2].tempo = 5;
    strcpy(list_of_commands[3].cmd, "./testes/TEST4/prog_test");
    list_of_commands[3].tempo = 8;


    pid_t pid;
    int i;
    /*
     * Loop that alloww the execution of all comands in the list of commands.
     */
    for (i = 0; i < (sizeof(list_of_commands) / sizeof(comando)); i++) {

        pid = fork();
        //if the fork fails
        if (pid < 0) {
            printf("Error in fork");
            exit(1);
        }
        /*
         * Child process is responsible for execute the different commands that are defined in the list.
         */
        if (pid == 0) {
            char *arguments[] = {list_of_commands[i].cmd, NULL}; //comand arguments
            execvp(arguments[0],
                   arguments);//replaces the child process with the program given by the path (char *arguments)
            exit(1); //terminates the child process

        } else {
            int time_counter = 0;//counts the time

            /*
             * Loops while the time to execute the process is not overpast and the time counter is inferior to the time defined to 
             * the command be able to execute and finish.The time counter is incremented.
             */
            while (!time_exceeded && time_counter < list_of_commands[i].tempo) {
                sleep(1);//sleeps for a second, that is the time that is added to time_counter for every interaction
                if (!time_exceeded) time_counter++;
            }
            //if the time counter is higher than the time for the execution that sends a SIGTERM to child process. Since 
            //the signal SIGCHILD is been handled the time_exceed will change to the number defined in the handler function.
            if (time_counter >= list_of_commands[i].tempo) {
                kill(pid, SIGTERM);
                printf("The command %s didn't end in its allowed time!\n", list_of_commands[i].cmd);

            }

            wait(NULL);//wait for the child process to terminate
            time_exceeded = 0;
        }
    }
    return 0;
}

