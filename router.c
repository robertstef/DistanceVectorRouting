/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT 434
 * A3
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <poll.h>
#include "userinput.h"
#include "socketsetup.h"
#include "routerfcns.h"
#include "socketmanage.h"
#include "routertable.h"

#define BACKLOG 10  // max number connection requests
#define MAXARG 5   // max allowable cmd line args

int main(int argc, char *argv[])
{
    int rv, pollrv, sockfd, newfd, tout;
    struct addrinfo hints;
    struct sockaddr their_addr;
    socklen_t addr_len;
    POLLINFO *pollinfo;
    int LISTENER;

    // verify command line input
    if ( (rv = checkinput(argc, argv)) == -1)
    {
        fprintf(stderr, "Usage: router name myport theirport1 [theirport2]\n");
        exit(EXIT_FAILURE);
    }

    // setup socket manager
    if ((rv = sockman_init(argc, argv)) == -1)
        exit(EXIT_FAILURE);

    // initialize router table
    if ((rv = init_rtable(*argv[1])) == -1)
        exit(EXIT_FAILURE);
    
    // try and connect to neighbouring routers
    connect_nbrs();

    // setup passive socket
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ( (sockfd = set_passive_tcp(&hints, argv[2])) == -1 )
        exit(EXIT_FAILURE);
    
    // record the listening socket
    LISTENER = sockfd;
    pollinfo = log_socket(sockfd);

    // listen for incoming connections
    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    tout = 5000;
    while (1)
    {
        pollrv = poll(pollinfo->pfds, pollinfo->fdcount, tout);

        // poll failed
        if ( pollrv == -1 )
        {
            fprintf(stderr, "poll(): poll failed - exiting program\n");
            exit(EXIT_FAILURE);
        }
        
        // timeout occurred
        if ( pollrv == 0 )
        {
            print_rtable();
            tout = 5000;
        }

        for (int i = 0; i < pollinfo->fdcount; i++)
        {
            if ( pollinfo->pfds[i].revents & POLLIN )
            {
                // listening socket
                if (pollinfo->pfds[i].fd == LISTENER)
                {
                    // accept new connection
                    addr_len = sizeof(their_addr);
                    newfd = accept(sockfd, &their_addr, &addr_len);

                    if ( newfd == -1 )
                    {
                        perror("accept");
                        exit(EXIT_FAILURE);
                    }

                    // log new connection
                    pollinfo = log_socket(newfd);

                    // send router name to new connection
                    rv = send_tcp(argv[1], newfd, sizeof(char));
                    if (rv == -1)
                        break;

                    printf("Got connection on sockfd %d\n", newfd);
                }
                // incoming router table
                else
                {
                    printf("Updating router table\n");
                    tout = 0;
                }
            }
        }
    }

    return 0;
}
