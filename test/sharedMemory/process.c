#include "shared.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>

// Função que será armazenada na memória compartilhada
void hello_world() {
    printf("Hello, World!\n");
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <process_id>\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *name = "shared_memory_example";
    shared_data_t *shared_data;

    if (init_shared_data(name, &shared_data) != 0) {
        fprintf(stderr, "Failed to initialize shared data\n");
        return EXIT_FAILURE;
    }

    int process_id = atoi(argv[1]);

    if (process_id == 1) {
        // Processo 1: Define a função e atualiza os dados compartilhados
        shared_data->a = 10;
        shared_data->b = 20.5;
        shared_data->print_message = hello_world;
        printf("Process 1: Updated shared data: a = %d, b = %.2f\n", shared_data->a, shared_data->b);
        printf("Process 1: Stored function pointer for message printing.\n");
        while (1) {
            sleep(8);
            shared_data->a++;
            shared_data->b += 0.9;
            printf("Process 1: Updated shared data: a = %d, b = %.2f, print_message = %p\n", shared_data->a, shared_data->b, shared_data->print_message);
        }
    } else if (process_id == 2) {
        // Processo 2: Executa a função armazenada na memória compartilhada
        if (shared_data->print_message != NULL) {
            printf("Process 2: Executing stored function:\n");
            //shared_data->print_message();
        } else {
            printf("Process 2: Function pointer is NULL.\n");
        }
        printf("Process 2: Shared data: a = %d, b = %.2f\n", shared_data->a, shared_data->b);
    }

    cleanup_shared_data(name, shared_data);
    return EXIT_SUCCESS;
}
