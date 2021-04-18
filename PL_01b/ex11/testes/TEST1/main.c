#include <stdio.h> //function printf
#include <unistd.h> //function sleep

#define tetra(m, a, i, n) m##a##i##n
#define fun tetra(m,a,i,n)

/*
 * It was used a argumented macro to "run" the programm and avoid multiple definition of `main'.
 */
int fun() {

    printf("Hello from TEST1\n");
    sleep(5);
    printf("End test1\n");
    return 0;
}

