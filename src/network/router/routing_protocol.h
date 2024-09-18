#ifndef ROUTING_PROTOCOL_H
#define ROUTING_PROTOCOL_H

#include <time.h>
#include <stdint.h>
#include "../../identity.h"

#define ROUTE_UPDATE 0
#define SEND_UNICAST 8
#define SEND_BROADCAST 9

typedef uint8_t routing_msg_t; 

// ROUTE_UPDATE
// Update the routing table of a node
typedef struct {
    routing_msg_t type; // Type = 0
    node_id_t target_id;
    int8_t hops;
} route_update;

typedef struct {
    node_id_t id;
    unsigned short port;
} p2p_addr_t;

// SEND_UNICAST
// Send a message to a node
typedef struct {
    routing_msg_t type; // Type = 8
    p2p_addr_t source;
    p2p_addr_t destination;
    unsigned int length;
} send_to;

// SEND_BROADCAST
// Send a message to all nodes
typedef struct {
    routing_msg_t type; // Type = 9
    p2p_addr_t source;
    unsigned short port;
    unsigned int length;
} broadcast;

#endif // ROUTING_PROTOCOL_H