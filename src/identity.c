#include "identity.h"
#include "shared.h"
#include <stdlib.h>

#define ID_ARRAY_SIZE (MAX_IDS >> 3)

static char *id_array = NULL;

int init_id_array() {
    int rs = init_shared_data("/clusterp2p>id_array", (void **) &id_array, ID_ARRAY_SIZE);
    if (rs == -1) {
        return -1;
    }

    for (int i = 0; i < ID_ARRAY_SIZE; i++) {
        id_array[i] = 0;
    }

    return 0;
}

int get_new_id() {
    for (int i = 0; i < MAX_IDS >> 3; i++) {
        if (id_array[i] != 0xFF) {
            for (int j = 0; j < 8; j++) {
                if ((id_array[i] & (1 << j)) == 0) {
                    id_array[i] |= 1 << j;
                    return i * 8 + j;
                }
            }
        }
    }

    return -1;
}