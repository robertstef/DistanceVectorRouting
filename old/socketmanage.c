#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <fcntl.h>
#include <errno.h>
#include "list.h"
#include "socketmanage.h"
#include "socketsetup.h"
#include "routertable.h"

#define MAXARGS 5 // max number of cmdline arguments
#define MAXBUF 100 // max buffer size


NBR_INFO *neighbours; // sockets to communicate with neighbours
int num_nbrs;    // number of neighbouring routers

POLLINFO pollinfo; // array of incoming sockets we are polling
int nxt_avail; // next open spot in pollinfo

char hostname[MAXBUF]; // hostname of local machine

int sockman_init(int argc, char *argv[])
{
    int rv;

    // get how many neighbours we have
    if (argc == MAXARGS)
        num_nbrs = 2;
    else
        num_nbrs = 1;
    
    neighbours = malloc(sizeof(NBR_INFO) * num_nbrs);
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
        // the 4th and maybe 5th index of argv
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

int connect_nbrs_helper(NBR_INFO nbr)
{   
    int newfd, rv;
    struct addrinfo hints;

    // setup hints
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    newfd = set_active_tcp(&hints, nbr.port, hostname);

    // neighbor not available
    if (newfd == -1)
    {
        fprintf(stderr, "Unable to connect with router listening "
                "on port %s\n\n", nbr.port);
        return -1;
    }
    // got connection
    else
        nbr.sockfd = newfd;

    return 0;
}

void connect_nbrs(void)
{
    int rv, newfd;
    char r_name;
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

            // neighbor not available
            if (newfd == -1)
                fprintf(stderr, "Unable to connect with router listening "
                        "on port %s\n\n", nbr.port);
            // got connection
            else
            {
                if ((rv = fcntl(newfd, F_SETFL, O_NONBLOCK)) < 0)
                    fprintf(stderr, "connect_nbrs(): fcntl error, could not "
                            "set socket to non-blocking\n");

                sleep(1);

                // wait for neighbor to send us their router name
                rv = recv(newfd, &r_name, 1, 0);

                if (rv == 1)
                {
                    rv = send(newfd, "OK", 2, 0);
                    if (rv == -1)
                    {
                        printf("connect_nbrs(): send error\n");
                        break;
                    }
                    else
                    {
                        nbr.sockfd = newfd;
                        nbr.name = r_name;
                        add_neighbour(nbr.name);
                        printf("Connected to router %c\n", nbr.name);
                    }

                }
                else
                {
                    printf("Didn't get router name\n");
                    close(newfd);
                    break;
                }
            }
        }
        // else check conection status - reconnect if necessary
        else { 
        }
    }
}

        /*
        if (nbr.sockfd != 0)
        {
            rv = write(nbr.sockfd, "", 0);
            if ((rv == -1) && (errno == EPIPE))
            {
                rv = connect_nbrs_helper(nbr);
                if (rv ==  -1) break;
            }

        }
        else
        {
            rv = connect_nbrs_helper(nbr);
            if (rv == -1) break;

            // its a new connection - get their name

            // wait for neighbor to send us their router name
            printf("Receiving nbrs router name\n");
            rv = recv_tcp(&nbr.name, nbr.sockfd, sizeof(char));
            if (rv == -1)
                break;

            add_neighbour(nbr.name);
            printf("Connected to router %c\n", nbr.name);
        }
        */
            /*
            // check connection status of each neighbour
            for (int n = 0; n < num_nbrs; n++)
            {
                rv = write(neighbours[n].sockfd, "", 0);
                if ((rv == -1) && (errno == EPIPE))
                {

                }
            }
            */
