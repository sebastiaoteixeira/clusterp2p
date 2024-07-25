typedef union
{
    int address;
    struct
    {
        u_int32_t empty;
        u_int16_t prev;
        u_int16_t next;
    } empty_conn;
} conn_elem_t;

static conn_elem_t *connections = NULL;
static conn_elem_t *empty_conn = NULL;
static sem_t *connections_sem = NULL;

// Add connection to connections array
int add_connection(int peer)
{
    
    if (empty_conn->empty_conn.empty != 0xFFFFFFFF)
    {
        return -1;
    }

    sem_wait(connections_sem);
    
    int prev_empty_conn_i = empty_conn->empty_conn.prev;
    int next_empty_conn_i = empty_conn->empty_conn.next;
    
    connections[prev_empty_conn_i].empty_conn.next = next_empty_conn_i;
    connections[next_empty_conn_i].empty_conn.prev = prev_empty_conn_i;    

    int conn_index = empty_conn - connections;
    empty_conn = connections + empty_conn->empty_conn.next;   

    connections[conn_index].address = peer;    

    sem_post(connections_sem);
    return conn_index;
}

// Pop connection from connections array
void pop_connection(int conn_index)
{
    sem_wait(connections_sem);

    int prev_empty_conn_i = empty_conn->empty_conn.prev;
    int next_empty_conn_i = empty_conn->empty_conn.next;

    connections[prev_empty_conn_i].empty_conn.next = conn_index;
    connections[next_empty_conn_i].empty_conn.prev = conn_index;

    connections[conn_index].empty_conn.prev = prev_empty_conn_i;
    connections[conn_index].empty_conn.next = next_empty_conn_i;

    empty_conn = connections + conn_index;

    sem_post(connections_sem);
}

// Remove connection from connections array
void remove_connection(int peer)
{
    sem_wait(connections_sem);

    for (int i = 0; i < MAX_CONNECTIONS; i++)
    {
        if (memcmp(&connections[i].address, &peer, sizeof(peer)) == 0)
        {
            pop_connection(i);
            break;
        }
    }

    sem_post(connections_sem);
}

// Get connection from connections array
int *get_connection(int conn_index)
{
    return connections[conn_index].empty_conn.empty == 0xFFFFFFFF ? NULL : &(connections[conn_index].address);
}

int connections_full()
{
    return empty_conn->empty_conn.empty == 0xFFFFFFFF;
}

void iterate_connections(void (*callback)(int))
{
    sem_wait(connections_sem);

    for (conn_elem_t* conn = connections; conn < connections + MAX_CONNECTIONS; conn++)
    {
        callback(conn->address);
    }

    sem_post(connections_sem);
}