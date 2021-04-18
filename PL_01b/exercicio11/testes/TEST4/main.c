
#include <unistd.h>//function sleep
#include <stdio.h> //function printf

#define begin(m, a, i, n) m##a##i##n
#define fun4 begin(m,a,i,n)

/*
 * It was used a argumented macro to "run" the programm and avoid multiple definition of `main'.
 */
int fun4() {
    printf("Hello from TEST4\n");
    sleep(3);
    printf("End TEST4\n");
    return 0;
}

