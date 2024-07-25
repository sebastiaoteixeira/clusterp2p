#include "node.h"
#include "../logger.h"
#include <argp.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define DEFAULT_HOST "0.0.0.0"
#define DEFAULT_PORT 12345
#define DEFAULT_MAX_CONNECTIONS 10

static char doc[] = "ClusterP2P Node Listener";
static char args_doc[] = "";

static struct argp_option options[] = {
    { "host", 'h', "HOST", 0, "Host to listen on" },
    { "port", 'p', "PORT", 0, "Port to listen on" },
    { "connect", 'c', "TO", 0, "Nodes to connect to" },
    { "max-connections", 'm', "MAX_CONNECTIONS", 0, "Maximum number of connections" },
    { "log-file", 'l', "LOG_FILE", 0, "Log file" },
    { "log-level", 'v', "LOG_LEVEL", 0, "Log level" },
    { 0 }
};

struct arguments {
    char *host;
    int port;
    char *connect;
    int max_connections;
    char *log_file;
    unsigned char log_level;
};

static error_t parse_opt(int key, char *arg, struct argp_state *state) {
    struct arguments *arguments = state->input;

    switch (key) {
        case 'h':
            arguments->host = arg;
            break;
        case 'p':
            arguments->port = atoi(arg);
            break;
        case 'c':
            arguments->connect = arg;
            break;
        case 'm':
            arguments->max_connections = atoi(arg);
            break;
        case 'l':
            arguments->log_file = arg;
            break;
        case 'v':
            arguments->log_level = atoi(arg);
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }

    return 0;
}

int main(int argc, char *argv[]) {
    // Parse arguments

    struct argp argp = { options, parse_opt, args_doc, doc };
    struct arguments arguments = { DEFAULT_HOST, DEFAULT_PORT, NULL, DEFAULT_MAX_CONNECTIONS, NULL, LOG_INFO };

    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    if (arguments.log_file != NULL) {
        FILE *log_file = fopen(arguments.log_file, "w");
        if (log_file == NULL) {
            perror("fopen");
            return -1;
        }

        set_log_file(log_file);
    }

    info("Starting node listener");
    
    int rs = init_node_listener(arguments.host, arguments.port, arguments.max_connections);
    if (rs == -1) {
        return -1;
    }
    
    // parse connect
    if (arguments.connect != NULL) {

        struct sockaddr_in to_connect;
        socklen_t to_connect_len = sizeof(to_connect);
        to_connect.sin_family = AF_INET;

        char *host = strtok(arguments.connect, ":");
        char *port = strtok(NULL, ":");
        to_connect.sin_addr.s_addr = inet_addr(host);
        to_connect.sin_port = htons(atoi(port));

        rs = node_connect((struct sockaddr *)&to_connect, to_connect_len);
        if (rs == -1) {
            perror("connect");
            return -1;
        }
    }

    info("Entering loop");
    loop_node_listener();
    
    return 0;
}