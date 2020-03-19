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
#include "routertable.h"

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
    (void) new_table;
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
