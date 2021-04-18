#include <unistd.h>//function sleep
#include <stdio.h> //function printf

#define omega(m, a, i, n) m##a##i##n
#define fun2 omega(m,a,i,n)

/*
 * It was used a argumented macro to "run" the programm and avoid multiple definition of `main'.
 */
int fun2() {
    printf("Hello from TEST2\n");
    sleep(9);
    printf("End TEST2\n");
    return 0;
}

