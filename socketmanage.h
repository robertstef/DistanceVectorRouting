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
 * Logs a newly created socket in the array of active
 * sockets. If, for some reason, the new socket is already
 * in the array, will indicate failure.
 *
 * Input:
 * int sockfd: socket file descriptor
 *
 * Output:
 * 0 on success, -1 on failure
 */
int log_socket(int sockfd);

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
