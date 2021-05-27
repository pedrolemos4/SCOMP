#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

#define STR_SIZE 50

typedef struct {
    char str[STR_SIZE];
    int n_writers;
    int n_readers;
} sh_data;

int main() {
    sem_t *sem, *nw, *nr, *read_access;
    if ((sem = sem_open("/sem14", O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    if ((nw = sem_open("/semnw", O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    if ((nr = sem_open("/semnr", O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }
    if ((read_access = sem_open("/semra", O_CREAT, 0644, 1)) == SEM_FAILED) {
        perror("Error in sem_open()");
        exit(1);
    }

    int fd, r, data_size = sizeof(sh_data), new_shm = 0;
    sh_data *data;
    // Criar shared memory
    if ((fd = shm_open("/shmex14", O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR)) >= 0) {
        new_shm = 1;
    } else {
        if ((fd = shm_open("/shmex14", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR)) < 0)
            return 1; // Erro ao criar memória partilhada
    }
    if (ftruncate(fd, data_size) < 0)
        return 1; // Erro a definir tamanho do espaço de memória
    if ((data = (sh_data *)mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == NULL)
        return 1; // Erro a mapear memória

    if (new_shm) {
        strcpy(data->str, "");
        data->n_writers = 0;
        data->n_readers = 0;
    }

    sem_wait(nr);
    data->n_readers++;
    sem_post(nr);

    sem_wait(read_access);
    sem_wait(nr);
    printf("%s\nReaders: %d\n", data->str, data->n_readers--);
    sem_post(nr);
    sem_post(read_access);
    
    // Fechar ligação à memória partilhada
    if ((r = munmap(data, data_size)) < 0) exit(1);
    if ((r = close(fd)) < 0) exit(1); // Erro ao fechar descritor
}
