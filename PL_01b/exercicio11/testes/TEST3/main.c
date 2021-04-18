#include <unistd.h>//function sleep
#include <stdio.h> //function printf

#define beta(m, a, i, n) m##a##i##n
#define fun3 beta(m,a,i,n)

/*
 * It was used a argumented macro to "run" the programm and avoid multiple definition of `main'.
 */
int fun3() {
    printf("Hello from TEST3\n");
    sleep(15);
    printf("End TEST3\n");
    return 0;
}


