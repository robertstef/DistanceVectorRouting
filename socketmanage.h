/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT434
 * A3
 */ 

#ifndef SOCKMAN_H
#define SOCKMAN_H

#define MAXROUTERS 25 // max number of routers
#define NBR 0;  // flag to indicate neighbouring router
#define INCMG 1 // flag to indicate incoming router

/*
 * Initializes all necessary variables for socket
 * management. To be called when setting up a router.
 *
 * Input:
 * None
 *
 * Output:
 * 0 on success, -1 on failure
 */
int sockman_init(void);

/*
 * Logs newly created socket in list of sockets that
 * carry outgoing info to neighbouring routers, or
 * in the list of socket used to receive info from
 * other routers.
 *
 * Input:
 * int sockfd: socket file descriptor
 * int flag: NBR to indicate we are reconnecting to a
 *           neighbouring socket
 *
 *           INCMG to indicate we are getting a connection
 *           from another router looking to send us 
 *           information
 *
 * Output:
 * 0 on success, -1 on failure
 */
int log_socket(int sockfd, int flag);

/*
 * Removes and closes all inactive sockets in the
 * array of active sockets.
 *
 * Input:
 * None
 *
 * Output:
 * 0 on success, -1 on failure
 */
int remove_inactive(void);

#endif // SOCKMAN_H
