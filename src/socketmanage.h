#ifndef SMAN_H
#define SMAN_H

#define MAXROUTERS 26
#define PORTSZ 5

// Holds relevant info for a neighbouring router
typedef struct nbr_info {
    char name;         // router name
    int sockfd;        // socket fd we are connected on
    char port[PORTSZ]; // port we are connected on
} NBR_INFO;

extern NBR_INFO nbrs[2]; // the neighbouring routers (max 2)
extern int NUM_NBRS;     // current number of neighbouring routers

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
 * if yes, will try to re-establish the connection.
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
