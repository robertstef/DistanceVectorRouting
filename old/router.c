#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stddef.h>
#include <poll.h>
#include <fcntl.h>
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
    RTABLE in_table;
    char ack[2];
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
            printf("Connect neighbours\n");
            connect_nbrs();
            printf("Print rtable\n");
            print_rtable();
            printf("Send rtable\n");
            send_rtable();
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
                    printf("Accpeting new connection\n");
                    newfd = accept(sockfd, &their_addr, &addr_len);

                    if ( newfd == -1 )
                    {
                        perror("accept");
                        exit(EXIT_FAILURE);
                    }


                    printf("Sending router name\n");
                    // send router name to new connection
                    rv = send_tcp(argv[1], newfd, sizeof(char));
                    if (rv == -1)
                        break;

                    if ((rv = fcntl(newfd, F_SETFL, O_NONBLOCK)) < 0)
                        fprintf(stderr, "log_socket(): fcntl error, could not set "
                                "socket to non-blocking\n");
                    
                    sleep(1);

                    rv = recv(newfd, ack, 2, 0);
                    if (rv == -1)
                    {
                        printf("Did not receive ack\n");
                        close(newfd);
                        break;
                    }

                    if (strcmp(ack, "OK") == 0)
                    {
                        // log new connection
                        pollinfo = log_socket(newfd);
                        printf("Got connection on sockfd %d\n", newfd);
                    }
                    else
                    {
                        printf("Ack was incorrect\n");
                        close(newfd);
                    }
                    read(LISTENER, &rv, sizeof(int));
                }
                // incoming router table
                else
                {
                    printf("Receiving router table\n");
                    read(pollinfo->pfds[i].fd, &in_table, sizeof(RTABLE));
                    //rv = recv(pollinfo->pfds[i].fd, &in_table, sizeof(RTABLE), 0);
                    //rv = recv_tcp(&in_table, pollinfo->pfds[i].fd, 
                    //        sizeof(RTABLE));

                    if (rv == -1)
                        break;
                    printf("Updating router table\n");
                    update_rtable(in_table);
                }
            }
        }
    }

    return 0;
}
