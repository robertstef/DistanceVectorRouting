/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT434
 * A3
 */

#include "list.h"
#include "socketmanage.h"

LIST *sockets; // list of active sockets

int sockman_init(void)
{
    sockets = listCreate();
    if (sockets == NULL)
        return -1;
    return 0;
}

int log_socket(int sockfd)
{
    // make sure we have room for new connection
    if (cur >= MAXROUTERS)
    {
        printf("log_socket(): cannot accomodate new connection," 
                "buffer is full\n");
        return -1;
    }

    // record active sockfd
    sockets[cur] = sockfd;
    cur++;
    return 0;
}

void shuffle(void)
{
    
}

int remove_inactive(void)
{
    return 0;
}
