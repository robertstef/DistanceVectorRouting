/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT434
 * A3
 */

#ifndef ROUTERTABLE_H
#define ROUTERTABLE_H

#include "socketmanage.h"

typedef struct router_table
{
    char my_name; // name of the router associated with this table
    int dist[MAXROUTERS]; // Dx(y) for the network
    char next_hop[MAXROUTERS]; // next_hopx(y) for the network
    char names[MAXROUTERS]; // names of each router
} ROUTER_T;

extern ROUTER_T rtable;

/*
 * Initializes a router table when the router is started.
 *
 * Input:
 * char n: name of this router
 *
 * Output:
 * 0 on success, -1 on failure.
 */ 
int rtable_init(char n);

/*
 * Prints the current router table
 */ 
void print_rtable(void);

#endif // ROUTERTABLE_H
