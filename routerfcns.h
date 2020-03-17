/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT434
 * A3
 */ 

#ifndef ROUTERFCNS_H
#define ROUTERFCNS_H

/*
 * Attempts to create a TCP connection with the
 * router listening on the specified port numbers(s).
 * If a connection cannot be created, a message will
 * be printed to stdout. The socket file descriptors
 * of the newly created connections will be stored in
 * the array of active sockets, declared in routerfcns.c.
 *
 * Input:
 * int argc: argument count
 * char *argv[]: argument vector
 *
 * Output:
 * 0 on success, -1 on failure.
 *
 * Note: only returns failure if unable to get hostname of local
 *       machine. If socket cannot be created with router on specified 
 *       port, a message will be printed and the program will continue.
 */ 
int create_connections(int argc, char *argv[]);

#endif // ROUTERFCNS_H
