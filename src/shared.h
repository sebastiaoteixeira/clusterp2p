#ifndef SHARED_H
#define SHARED_H

#include <stdlib.h>

// Funções para inicializar e acessar a memória compartilhada
int init_shared_data(const char *name, void **shared_data, size_t size);
void dettach_shared_data(void *shared_data);
void cleanup_shared_data(const char *name);

#endif // SHARED_H
