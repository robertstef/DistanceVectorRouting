/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT434
 * A3
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "socketsetup.h"
#include "socketmanage.h"

#define MAXBUF 100

int create_connections(int argc, char *argv[])
{
    struct addrinfo hints;
    char hostname[MAXBUF];
    int rv, newfd;

    // setup hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    // get hostname of local machine
    memset(hostname, 0, MAXBUF);
    if ((rv = gethostname(hostname, MAXBUF)) == -1)
    {
        fprintf(stderr, "create_connections(): unable to get hostname"
                "of local machine\n");
        return -1;
    }

    // attempt to connect with routers on ports specified
    // by the user
    for (int i = 3; i < argc; i++)
    {
        newfd = set_active_tcp(&hints, argv[i], hostname);

        // connection unsuccessful
        if ( newfd == -1)
            fprintf(stderr, "create_connections(): unable to create"
                    "TCP connections with router listening on port"
                    " %s\n", argv[i]);
        // connection successful
        else
        {
            // log the socket in our list of sockfds
            if ((rv = log_socket(newfd, 1)) == -1)
                return -1;
        }
    }
    return 0;
}
