/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT434
 * A3
 */

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include "list.h"
#include "socketmanage.h"
#include "socketsetup.h"

#define MAXARGS 5 // max number of cmdline arguments
#define PORTSZ 5  // a port number must have 5 digits
#define MAXBUF 100 // max buffer size

/* Info for neighbouring routers */
typedef struct nbr_info
{
    int sockfd;
    char port[PORTSZ];
} NBR_INFO;

NBR_INFO *neighbours; // sockets to communicate with neighbours
int num_nbrs;    // number of neighbouring routers

POLLINFO pollinfo;
int nxt_avail;

char hostname[MAXBUF]; // hostname of local machine

int sockman_init(int argc, char *argv[])
{
    int rv;

    // get how many neighbours we have
    if (argc == MAXARGS)
        num_nbrs = 2;
    else
        num_nbrs = 1;
    
    neighbours = malloc(sizeof(int) * num_nbrs);
    if (neighbours == NULL)
    {
        fprintf(stderr, "sockman_init(): malloc error\n");
        return -1;
    }

    // store port number of neighbours
    for (int i = 0; i < num_nbrs; i++)
    {
        neighbours[i].sockfd = 0;

        // use idx argv[i+3] bc we need to access
        // the 4nd and maybe 5th index of argv
        // depending how many neighbours we have
        strcpy(neighbours[i].port, argv[i + 3]);
    }

    // get hostname of local machine
    memset(hostname, 0, MAXBUF);
    if ((rv = gethostname(hostname, MAXBUF)) == -1)
    {
        fprintf(stderr, "sockman_init(): unable to get hostname"
                "of local machine\n");
        return -1;
    }

    return 0;
}

POLLINFO *log_socket(int sockfd)
{
    int rv;

    if (nxt_avail < MAXROUTERS)
    {
        // set socket to non-blocking
        if ((rv = fcntl(sockfd, F_SETFL, O_NONBLOCK)) < 0)
            fprintf(stderr, "log_socket(): fcntl error, could not set "
                    "socket to non-blocking\n");

        pollinfo.pfds[nxt_avail].fd = sockfd;
        pollinfo.pfds[nxt_avail].events = POLLIN;
        pollinfo.fdcount++;

        nxt_avail++;
    }
    else
        fprintf(stderr, "log_socket(): cannot accomodate new connection - "
                "buffer is full\n");

    return &pollinfo;
}

void connect_nbrs(void)
{
    int newfd, rv;
    struct addrinfo hints;

    // setup hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    for (int i = 0; i < num_nbrs; i++)
    {
        NBR_INFO nbr = neighbours[i];

        // if sockfd is 0 automatically create connection
        if (nbr.sockfd == 0)
        {
            newfd = set_active_tcp(&hints, nbr.port, hostname);
            if (newfd == -1)
                fprintf(stderr, "connect_nbrs(): unable to create "
                        "TCP connections with router listening on port"
                        " %s\n", nbr.port);

        }
        // else check conection status - reconnect if necessary
        else{ 
            // do stuff 
        }
    }
}

