#include "shared.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

// Função para inicializar a memória compartilhada
int init_shared_data(const char *name, shared_data_t **shared_data) {
    int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return -1;
    }

    if (ftruncate(shm_fd, sizeof(shared_data_t)) == -1) {
        perror("ftruncate");
        close(shm_fd);
        return -1;
    }

    *shared_data = mmap(0, sizeof(shared_data_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (*shared_data == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        return -1;
    }

    close(shm_fd);
    return 0;
}

// Função para limpar a memória compartilhada
void cleanup_shared_data(const char *name, shared_data_t *shared_data) {
    munmap(shared_data, sizeof(shared_data_t));
    //shm_unlink(name);
}
