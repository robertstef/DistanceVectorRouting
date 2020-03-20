/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT434
 * A3
 */

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

int log_socket(int sockfd);

void connect_nbrs(void);

#endif // SMAN_H
