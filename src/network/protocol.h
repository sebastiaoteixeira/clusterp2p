#ifndef PROTOCOL_H
#define PROTOCOL_H

// This is a simple protocol to establish connections between nodes

#include <stdint.h>
#include "../identity.h"

// Types enum
typedef enum {
    ROUTE_UPDATE = 0,
    JOIN_REQUEST = 1,
    JOIN_RESPONSE = 2,
    JOIN_ACCEPT = 3,
} msg_type;

// JOIN_REQUEST
typedef struct {
    msg_type type; // Type = 1
} join_request;

// JOIN_RESPONSE
typedef struct {
    msg_type type; // Type = 2
    node_id_t peer_id;
    node_id_t suggested_id;
} join_response;

// JOIN_CONFIRMATION
typedef struct {
    msg_type type; // Type = 3
    node_id_t peer_id;
} join_accept;


// Message Header (common to all operational messages)
typedef struct {
    msg_type type;
    node_id_t sender_id;
    node_id_t receiver_id;
} msg_header;
    

// lenght enum
typedef enum {
    JOIN_REQUEST_LEN = sizeof(join_request),
    JOIN_RESPONSE_LEN = sizeof(join_response),
    JOIN_ACCEPT_LEN = sizeof(join_accept)
} msg_len;

#endif // PROTOCOL_H