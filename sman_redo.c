/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT434
 * A3
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include "socketsetup.h"
#include "sman_redo.h"
#include "list.h"

#define MAXARGS 5
#define MAXBUF 100

NBR_INFO nbrs[2];
int NUM_NBRS;

char hostname[MAXBUF];

LIST *connections;

char hostname[MAXBUF];

int sockman_init(int argc, char *argv[])
{
    int rv;

    // get how many neighbours we have
    if (argc == MAXARGS)
        NUM_NBRS = 2;
    else
        NUM_NBRS = 1;

    for (int n = 0; n < NUM_NBRS; n++)
    {
        strcpy(nbrs[n].port, argv[n + 3]);
        nbrs[n].sockfd = -1;
    }

    connections = listCreate();

    // get hostname
    memset(hostname, 0, MAXBUF);
    if ((rv = gethostname(hostname, MAXBUF)) == -1)
    {
        fprintf(stderr, "sockman_init(): unable to get hostname"
                "of local machine\n");
        return -1;
    }

    return 0;
}

int log_socket(int sockfd)
{
    int rv;
    int *store;

    // set to non-blocking
    if ((rv = fcntl(sockfd, F_SETFL, O_NONBLOCK)) < 0)
    {
        fprintf(stderr, "log_socket(): fcntl error, could not set "
                "socket to non-blocking\n");
        return -1;
    }
    
    store = malloc(sizeof(int));
    if (store == NULL)
    {
        fprintf(stderr, "log_socket(): malloc error\n");
        return -1;
    }
    
    // add to list
    append(connections, store);

    return 0;
}

void connect_nbrs(void)
{
    int rv, newfd, set;
    struct addrinfo hints;
    char buf[10];

    // setup hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    for (int n = 0; n < NUM_NBRS; n++)
    {
        // check if we have connection to current neighbour
        //rv = send(nbrs[n].sockfd, "", 0, 0);
        rv = read(nbrs[n].sockfd, buf, 10);

        // no - create connection
        if (errno == EBADF)
        {
            // create a socket
            newfd = set_active_tcp(&hints, nbrs[n].port, hostname);

            // set sock options for sigpipe
            //setsockopt(newfd, SOL_SOCKET, SO_SIGNOPIPE, (void *)&set, sizeof(int));

            if (newfd == -1)
            {
                fprintf(stderr, "Unable to connect with router listening "
                        "on port %s\n\n", nbrs[n].port);
                continue;
            }

            // set newfd to nonblocking
            if ((rv = fcntl(newfd, F_SETFL, O_NONBLOCK)) < 0)
            {
                fprintf(stderr, "connect_nbrs(): fcntl error, could not "
                        "set socket to non-blocking\n");
                close(newfd);
            }

            // get router name
            sleep(1);
            rv = recv(newfd, &nbrs[n].name, sizeof(char), 0);

            if (rv == -1)
            {
                fprintf(stderr, "connect_nbrs(): did not receive neighbours"
                        " router name\n");
                close(newfd);
                continue;
            }
            
            // ack connections
            rv = send_tcp("OK", newfd, 10);
            if (rv == -1) continue;

            // record the socket
            nbrs[n].sockfd = newfd;

            printf("Created new connection with neighbour %c\n", nbrs[n].name);
        }
    }
}
