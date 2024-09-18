#include "protocol.h"

void create_join_request(join_request* req) {
    req->type = JOIN_REQUEST;
    return req;
}

join_response* create_join_response(join_response* res, node_id_t peer_id, node_id_t suggested_id) {
    res->type = JOIN_RESPONSE;
    res->peer_id = peer_id;
    res->suggested_id = suggested_id;
    return res;
}

join_accept* create_join_accept(join_accept* acc, node_id_t peer_id) {
    acc->type = 3;
    acc->peer_id = peer_id;
    return acc;
}
