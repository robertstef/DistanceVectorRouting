/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT434
 * A3
 */ 

#ifndef SOCKMAN_H
#define SOCKMAN_H

#include <poll.h>

#define MAXROUTERS 26

typedef struct poll_info
{
    int fdcount;
    struct pollfd pfds[MAXROUTERS];
} POLLINFO;

/*
 * Initializes all necessary variables for socket
 * management. To be called when setting up a router.
 *
 * Input:
 * int argc: argument count
 * char *argv[]: argument vector
 *
 * Output:
 * 0 on success, -1 on failure
 */
int sockman_init(int argc, char *argv[]);

/*
 * Logs newly created socket in list of sockets that
 * receive incoming information from routers and returns
 * the updated array of pollfds. If there is no room
 * for a new socket, a pointer to the unchaged array will
 * be printed and a message will be printed to stderr.
 *
 * Input:
 * int sockfd: socket file descriptor
 *
 * Output:
 * POLLINFO with updated polling info
 */
POLLINFO *log_socket(int sockfd);

/*
 * Attempts to establish or re-establish a conneciton
 * between the local router and its neighbouring routers.
 *
 * Input:
 * void
 *
 * Output:
 * void
 *
 * Note - if a connection is unable to be established, a
 * message will be printed to stderr, however the program
 * will continue to run.
 */
void connect_nbrs(void);

/*
 * Checks if the connection with the provided
 * file descriptor is still active.
 *
 * Input:
 * int sockfd: connection to check
 *
 * Output:
 * 1 if still active, 0 if not
 */ 
int is_active(int sockfd);

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
