#include "shared.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>


int init_shared_data(const char *__restrict name, void **__restrict shared_data, size_t size) {
    int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return -1;
    }

    if (ftruncate(shm_fd, size + sizeof(size_t)) == -1) {
        perror("ftruncate");
        close(shm_fd);
        return -1;
    }

    *shared_data = mmap(0, size + sizeof(size_t), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (*shared_data == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        return -1;
    }

    *((size_t *) *shared_data) = size;
    *shared_data += sizeof(size_t);

    close(shm_fd);
    return 0;
}

void dettach_shared_data(void *shared_data) {
    munmap(shared_data, *((size_t *) shared_data - sizeof(size_t)) + sizeof(size_t));
}

void cleanup_shared_data(const char *name) {
    shm_unlink(name);
}
