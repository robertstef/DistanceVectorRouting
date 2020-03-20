/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT434
 * A3
 */

#include <stdio.h>
#include <stdlib.h>
#include <poll.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "socketsetup.h"
#include "sman_redo.h"
#include "userinput.h"
#include "list.h"

#define BACKLOG 10

int main(int argc, char *argv[])
{
    int rv, sockfd, newfd, pollrv;
    struct pollfd pfd;
    char *buf[10];
    socklen_t addr_len;
    struct sockaddr their_addr;
    struct addrinfo hints;

    // verify command line input
    if ( (rv = checkinput(argc, argv)) == -1)
    {
        fprintf(stderr, "Usage: router name myport theirport1 [theirport2]\n");
        exit(EXIT_FAILURE);
    }

    // setup socket manager
    if ((rv = sockman_init(argc, argv)) == -1) exit(EXIT_FAILURE);
    
    // setup passive socket
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if ( (sockfd = set_passive_tcp(&hints, argv[2])) == -1 )
        exit(EXIT_FAILURE);
    
    // set up poll fd
    pfd.fd = sockfd;
    pfd.events = POLLIN;

    // listen for incoming connections
    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while (1)
    {
        pollrv = poll(&pfd, 1, 2000);

        if (pollrv == -1)
        {
            fprintf(stderr, "poll(): poll failed - exiting program\n");
            exit(EXIT_FAILURE);
        }
        else if (pollrv == 0)
        {
            connect_nbrs();
            // timeout
        }
        // we got something
        else if (pfd.revents & POLLIN)
        {
            // accept new connection
            addr_len = sizeof(their_addr);
            newfd = accept(sockfd, &their_addr, &addr_len);

            if (newfd == -1)
            {
                perror("accept");
                exit(EXIT_FAILURE);
            }
            
            // send our name
            rv = send_tcp(argv[1], newfd, sizeof(char));
            if (rv == -1)
            {
                fprintf(stderr, "Unable to send router name to "
                        "connecting router\n");
                close(newfd);
                continue;
            }

            // set newfd to nonblocking
            if ((rv = fcntl(newfd, F_SETFL, O_NONBLOCK)) < 0)
            {
                fprintf(stderr, "main(): fcntl error, could not "
                        "set socket to non-blocking\n");
                close(newfd);
            }

            // get response
            sleep(2);
            rv = recv(newfd, buf, 10, 0);
            //rv = recv_tcp(buf,  newfd, 10);
            if (rv == -1)
            {
                fprintf(stderr, "Did not receive ack\n");
                close(newfd);
                continue;
            }

            rv = log_socket(newfd);
            if (rv == -1) continue;

            printf("Created new connection\n");

            // dummy recv to clear event queue
            recv(sockfd, buf, 0, 0);
        }

    }
    return 0;
}
