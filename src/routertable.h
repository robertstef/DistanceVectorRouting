#ifndef ROUTERTABLE_H
#define ROUTERTABLE_H

#include "socketmanage.h"

// Defines a router table
typedef struct router_table {
    char my_name;              // name of the router associated with this table
    int dist[MAXROUTERS];      // Dx(y) for the network
    char next_hop[MAXROUTERS]; // next_hopx(y) for the network
    char names[MAXROUTERS];    // names of each router
} RTABLE;

// lets socket manager have access
// to router table for sending packets
extern RTABLE rtable;

/*
 * Initializes a router table when the router is started.
 *
 * Input:
 * char n: name of this router
 *
 * Output:
 * 0 on success, -1 on failure.
 */ 
int init_rtable(char n);

/*
 * Prints the current router table
 */ 
void print_rtable(void);

/*
 * Updates the router using the router table
 * we received from an incoming router.
 *
 * Input:
 * RTABLE new_table: router table from a neighbouring router
 *
 * Output:
 * 0 on success, -1 on failure
 */
int update_rtable(RTABLE new_table);

/*
 * Adds a newly connected outgoing router to the
 * router table. If an entry for this router
 * already exists, it will be overwritten since
 * a direct connection is always more efficient.
 * Otherwise a new entry will be added.
 *
 * Input:
 * char name: name of router we are connected to
 *
 * Output:
 * None
 */ 
void add_neighbour(char name);

/*
 * Sends router table to neighbouring routers.
 *
 * Input:
 * None
 *
 * Output:
 * 0 on success, -1 on failure.
 */ 
void send_rtable(void);

/*
 * Updates the router table for
 * a dropped connection.
 *
 * Input:
 * char name: name of dropped router
 */ 
void dropped_rtable(char name);

#endif // ROUTERTABLE_H
