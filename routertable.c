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
#include "socketsetup.h"
#include "routertable.h"
#include "sman_redo.h"

#define S_ALPH 65 // ascii code for A
#define INF 2147483647 // value for infinity

RTABLE rtable; // router table for this router

int init_rtable(char n)
{
    rtable.my_name = n;

    for (int i = 0; i < MAXROUTERS; i++)
    {
        // set our distance to 0
        if ((i + S_ALPH) == n)
            rtable.dist[i] = 0;
        else
            rtable.dist[i] = INF;
        rtable.next_hop[i] = '-';
        rtable.names[i] = i + S_ALPH; 
    }
    return 0;
}

void print_rtable(void)
{
    printf("| Name | D_%c(y) | Next_hop_%c(y) |\n", 
            rtable.my_name, rtable.my_name);
    printf("---------------------------------\n");
    for (int i = 0; i < MAXROUTERS; i++)
    {
        // assume if distance is INF we don't know about the router
        if (rtable.dist[i] != INF)
            printf("| %c    | %d      | %c             |\n", rtable.names[i], 
                rtable.dist[i], rtable.next_hop[i]);
    }
    printf("\n\n");
}

int update_rtable(RTABLE new_table)
{
    int i = 0;
    int is_nbr = 0;
    int dist_est[2]; // distance estmate for path we're calculating
    char nxt_hop[2]; // corresponding next hop router for the distance
                     // estimate
                     // NOTE: since we only have max two outgoing routers
                     //       there are at most 2 distance estimates for
                     //       each table entry

    while (rtable.names[i] != rtable.my_name) i++;

    // our router is not in incoming table - do nothing
    if (new_table.dist[i] == INF) return 0;

    // calc Dx(y) for all routers y
    for (int i  = 0; i < MAXROUTERS; i++)
    {
        // our router - don't change
        if (rtable.names[i] == rtable.my_name) continue;

        // our router to neighbour - don't change
        for (int n = 0; n < NUM_NBRS; n++)
            if (rtable.names[i] == nbrs[i].name) is_nbr = 1;
        
        if (is_nbr) continue;

        for (int n = 0; n < NUM_NBRS; n++)
        {
            dist_est[n] = 1 + new_table.dist[i];
            nxt_hop[n] = nbrs[n].name;
        }

        // get minimum estimate
        if (dist_est[0] <= dist_est[1])
        {
            rtable.dist[i] = dist_est[0];
            rtable.next_hop[i] = nxt_hop[0];
        }
        else
        {
            rtable.dist[i] = dist_est[1];
            rtable.next_hop[i] = nxt_hop[1];
        }
    }



    return 0; 
}

void add_neighbour(char name)
{
    // find neighbour in router table
    for (int i = 0; i < MAXROUTERS; i++)
    {
        if (rtable.names[i] == name)
        {
            rtable.names[i] = name;
            rtable.dist[i] = 1;
            rtable.next_hop[i] = name;
            break;
        }
    }
}

void send_rtable(void)
{
    int rv;

    for (int n = 0; n < NUM_NBRS; n++)
    {
        //rv = send_tcp(&rtable, neighbours[n].sockfd, sizeof(RTABLE));
        rv = send(nbrs[n].sockfd, &rtable, sizeof(RTABLE), 0);
        if (rv == -1)
            fprintf(stderr, "Unable to send router table to router %c\n\n", 
                    nbrs[n].name);
    }
}
