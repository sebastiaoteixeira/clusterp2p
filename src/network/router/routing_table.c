#include <stdint.h>
#include <time.h>
#include <stdlib.h>
#include "router_table.h"

static route_entry route_table[MAX_IDS];

void init_router()
{
    for (int i = 0; i < MAX_IDS; i++) {
        route_table[i].socket = -1;
        route_table[i].hops = -1;
        route_table[i].timestamp = 0;
    }
}

route_entry* get_random_route()
{
    unsigned short i = rand() % MAX_IDS;;
    do {
        i = (i + 1) % MAX_IDS;
    } while (route_table[i].hops < 0);
    return &route_table[i];
}

int next_hop(int id)
{
    return route_table[id].socket;
}

void _update_route(node_id_t target_id, int socket, int8_t hops)
{
    route_table[target_id].socket = socket;
    route_table[target_id].hops = hops;
    route_table[target_id].timestamp = time(NULL);
}

void update_route(route_update* update, int socket)
{
    // If timestamp is older than 30 seconds, update the route
    if (time(NULL) - route_table[update->target_id].timestamp > 30) {
        _update_route(update->target_id, socket, update->hops);
        return;
    }

    // If the update is from the next_hop itself, update the route
    if (socket == route_table[update->target_id].socket) {
        _update_route(update->target_id, socket, update->hops);
        return;
    }

    // If the update is from a node with a shorter path, update the route
    if (update->hops < route_table[update->target_id].hops) {
        _update_route(update->target_id, socket, update->hops);
    }
}