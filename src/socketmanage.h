#ifndef SMAN_H
#define SMAN_H

#define MAXROUTERS 26
#define PORTSZ 5

typedef struct nbr_info
{
    char name;
    int sockfd;
    char port[PORTSZ];
} NBR_INFO;

extern NBR_INFO nbrs[2];
extern int NUM_NBRS; 

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
 * Sets the inputted fd to non-blocking and adds it
 * to the list of sockets we are receiving router
 * tables from.
 *
 * Input:
 * int sockfd: socket file descriptor
 *
 * Output:
 * 0 on success, -1 on failure
 */ 
int log_socket(int sockfd);

/*
 * Creates a connection with neighbouring sockets
 * and logs their information in a struct nbr_info.
 * Also checks to see if neighbours have been disconnected,
 * if yes, will try to restablish the connection.
 *
 * Input:
 * None
 *
 * Output:
 * None
 */ 
void connect_nbrs(void);

/*
 * Checks each incoming socket for a router table.
 * If on is there, we update our current table.
 */ 
void recv_tables(void);

int is_connected(NBR_INFO nbr);

#endif // SMAN_H
