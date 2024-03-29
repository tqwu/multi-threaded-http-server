#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "connection.h"
#include "asgn4_helper_funcs.h"

// Function to get command-line arguments: returns a valid port number
int getArgs(int argc, char *argv[]) {

    // Checking for correct number of command line arguments
    if (argc != 2 && argc != 4) {
        fprintf(stderr, "Usage: ./httpserver [-t threads] <port>\n");
        exit(1);
    }

    char *endptrport;
    long port;
    // long threads = 4; // Default number of threads is 4

    if (argc == 2) {
        port = strtol(argv[1], &endptrport, 10);
    } else {
        int opt;
        char *endptrthreads;
        while ((opt = getopt(argc, argv, "t:")) != -1) {
            switch (opt) {
            case 't':
                strtol(optarg, &endptrthreads, 10);
                // FIXME: what should be outputted for an invalid formatting of threads?
                if (*endptrthreads != '\0') {
                    fprintf(stderr, "Invalid number of threads\n");
                    exit(1);
                }
                break;
            case '?': fprintf(stderr, "Usage: ./httpserver [-t threads] <port>\n"); exit(1);
            }
        }
        port = strtol(argv[3], &endptrport, 10);
    }

    // Checking for a valid port number
    if (*endptrport != '\0' || port < 1 || port > 65535) {
        fprintf(stderr, "Invalid Port\n");
        exit(1);
    }

    // Storing number of threads and port number
    // c->threads = threads;
    // c->port = (int) port;

    return (int) port;
}

// Function to create, initialize, and return a listener socket for the port
Listener_Socket bindSocket(int port) {
    Listener_Socket sock;
    int bind = listener_init(&sock, port);

    // If there was an error getting the connection
    if (bind == -1) {
        fprintf(stderr, "Invalid Port\n");
        exit(1);
    }
    return sock;
}

// Main function
int main(int argc, char *argv[]) {

    // Parse command-line arguments
    // Connection *c = malloc(sizeof(Connection));
    int port = getArgs(argc, argv);

    // Create and initialize listener socket for the port
    Listener_Socket sock = bindSocket(port);

    while (1) {

        // Listen and accept incoming connection - only returns when a new connection is accepted
        acceptConnection(sock);

        // Processing connection
        processConnection();

        // Close socket to end connection with client
        closeConnection();
    }

    // free(c);
    return 0;
}
