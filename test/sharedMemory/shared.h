#ifndef SHARED_H
#define SHARED_H

// Estrutura de dados compartilhada
typedef struct {
    int a;
    float b;
    void (*print_message)(); // Ponteiro para função
} shared_data_t;

// Funções para inicializar e acessar a memória compartilhada
int init_shared_data(const char *name, shared_data_t **shared_data);
void cleanup_shared_data(const char *name, shared_data_t *shared_data);

#endif // SHARED_H
