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
#include "list.h"
#include "socketmanage.h"

LIST *neighbours; // sockets to communicate with neighbours
                  // used to send out our router table
LIST *in_sockets; // active sockets we are receiving from

int sockman_init(void)
{
    sockets = listCreate();
    if (sockets == NULL)
        return -1;

    neighbours = listCreate();
    if (sockets == NULL)
        return -1;
    return 0;
}

int log_socket(int sockfd, int flag)
{
    int *sockptr, rv;

    // store the new sockfd
    if ((sockptr = malloc(sizeof(int))) == NULL)
    {
        printf("log_socket(): malloc error\n");
        return -1;
    }

    *sockptr = sockfd;
    
    // new socket is an incoming socket
    if (flag == INCMG)
    {
        // make sure we have room for new connection
        if (size(in_sockets) > MAXROUTERS)
        {
            fprintf(stderr, "log_socket(): cannot accomodate new connection," 
                    "buffer is full\n");
            return -1;
        }
        if ((rv = append(in_sockets, sockptr)) == -1)
            return -1;

        return 0;
    }
    // socket is for a neighbour
    else if (flag == NBR)
    {
        if ((rv = append(neighbours, sockptr)) == -1)
            return -1;

        return 0;
    }
    // invalid flag
    else
    {
        *sockptr = 0;
        free(sockptr);
        fprintf(stderr, "log_socket(): invalid flag\n");
        return -1;
    }
}

int remove_inactive(void)
{
    return 0;
}
