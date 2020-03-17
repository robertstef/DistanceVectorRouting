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

ROUTER_T rtable; // router table for this router

int rtable_init(char n)
{
    rtable.my_name = n;
    
    // fill dist with INF
    // fill next_hop with '-'
    // fill name column with A-Z
    for (int i = 0; i < MAXROUTERS; i++)
    {
        rtable.dist[i] = INF;
        rtable.next_hop[i] = '-';
        rtable.names[i] = i + S_ALPH; 
    }
    return 0;
}

void print_rtable(void)
{
    printf("| Name | D%c(y) | Next_hop%c(y) |\n", 
            rtable.my_name, rtable.my_name);
    printf("---------------------------------\n");
    for (int i = 0; i < MAXROUTERS; i++)
    {
        if (rtable.dist[i] == INF)
            printf("| %c    | INF   | %c            |\n", rtable.names[i], 
                    rtable.next_hop[i]);
        else
            printf("| %c   | %d    | %c            |\n", rtable.names[i], 
                rtable.dist[i], rtable.next_hop[i]);
    }
}
