#ifndef ROUTER_H
#define ROUTER_H

#include "routing_protocol.h"
#include "../../identity.h"

// Routing table
typedef struct {
    int socket;
    int8_t hops; // If < 0, the route is invalid
    time_t timestamp;
} route_entry;

void init_router();
route_entry* get_random_route();
int next_hop(int id);
void update_route(route_update* update, int socket);

#endif // ROUTER_H