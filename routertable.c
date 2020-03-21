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
#define INF 2147483600 // value for infinity

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
    int dist_est;
    
    //printf("UPDATING FROM %c\n", new_table.my_name);
    while (rtable.names[i] != rtable.my_name) i++;

    // our router is not in incoming table - do nothing
    if (new_table.dist[i] == INF) return 0;

    // calc Dx(y) for all routers y
    for (int i  = 0; i < MAXROUTERS; i++)
    {
        // our router - don't change
        if (rtable.names[i] == rtable.my_name) continue;

        //if ((new_table.dist[i] == INF) ) continue;

        // if our table has their name
        dist_est = 1 + new_table.dist[i];
        if (dist_est < rtable.dist[i])
        {
            rtable.dist[i] = dist_est;
            rtable.next_hop[i] = new_table.my_name;
        }
    }

    // validate next_hops
    is_nbr = 0;
    for (int i = 0; i < MAXROUTERS; i++)
    {
        if (rtable.names[i] != rtable.my_name)
        {
            for (int n = 0; n < NUM_NBRS; n++)
            {
                if (rtable.next_hop[i] == nbrs[n].name)
                {
                    //rtable.dist[i] = INF;
                    //rtable.next_hop[i] = '-';
                    is_nbr = 1;
                }
            }
            if (is_nbr == 0)
            {
                rtable.dist[i] = INF;
                rtable.next_hop[i] = '-';
            }
            is_nbr = 0;
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
        if (nbrs[n].sockfd == -1) continue;

        rv = send(nbrs[n].sockfd, &rtable, sizeof(RTABLE), MSG_NOSIGNAL);
        if (rv == -1)
            fprintf(stderr, "Unable to send router table to router %c\n\n", 
                    nbrs[n].name);
    }
}

void dropped_rtable(char name)
{
    // remove direct path and paths that have dropped
    // router as next hop
    for (int n = 0; n < MAXROUTERS; n++)
    {
        if ((rtable.names[n] == name) || (rtable.next_hop[n] == name))
        {
            //printf("Removing %c from table\n", name);
            rtable.dist[n] = INF;
            rtable.next_hop[n] = '-';
        }
    }
}
