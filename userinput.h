#ifndef USERIN_H
#define USERIN_H

/*
 * Checks if the input string is a valid port
 * number in the range 30000 - 40000.
 *
 * Input:
 * char *port: a string representing a port number
 *
 * Output:
 * 0 on success, -1 on failure
 */ 
int valid_port(char *port);

/*
 * Checks if the input string is a single
 * capital letter from A-Z
 *
 * Input:
 * char *name: name of router
 *
 * Output:
 * 0 on success, -1 on failure
 */
int valid_name(char *name);

/*
 * Checks if the command line arguments
 * are of the following form:
 *
 * router name myport theirport1 ...
 *
 * Where:
 * name is a single uppcase ascii character
 * myport is an integer in range 30000-40000
 * theirport 1..n is an integer in range 30000-40000
 *
 * Input:
 * int count: argument count
 * char *args[]: argument vector
 *
 * Output:
 * 0 on success, -1 on failure
 */ 
int checkinput(int count, char *args[]);

#endif // USERIN_H
