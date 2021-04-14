#ifndef SOCKSETUP_H
#define SOCKSETUP_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

/*
 * Stores info for an active UDP socket.
 */
typedef struct sock_info
{
    struct sockaddr addr; // address information
    socklen_t addr_len;   // address size
    int sockfd;           // socket file descriptor we are sending
                          // or receiving over
} SOCK_INFO;

/*
 * Creates a passive socket on the local machine.
 *
 * Input:
 * struct addrinfo *hints: hints for getaddrinfo(), needs to have
 *                         all fields specified before passing into
 *                         this function
 * char *port: the port number we will be connecting to
 *
 * Output:
 * The socket file descriptor on success, -1 otherwise.
 */
int set_passive_tcp(struct addrinfo *hints, char *port);

/*
 * Creates an active TCP socket on the local machine.
 *
 * Input:
 * struct addrinfo *hints: hints for getaddrinfo(), needs to have all
 *                         fields specified before passing into
 *                         this function
 * char *port: port number host is listening on
 * char *name: name or IP address of the machine we are connecting to
 *
 * Output:
 * The socket file descriptor on success, -1 otherwise
 */
int set_active_tcp(struct addrinfo *hints, char *port, char *name);

/*
 * Creates a passive UDP socket on the local machine.
 *
 * Input:
 * struct addrinfo *hints: hints for getaddrinfo(), needs to have all
 *                         fields specified before passing into
 *                         this function
 * char *port: port number we want to bind to
 *
 * Output:
 * The socket file descriptor on success, -1 on failure
 */
int set_passive_udp(struct addrinfo *hints, char *port);

/*
 * Creates an active UDP socket on the local machine.
 *
 * Input:
 * struct addrinfo *hints: hints for getaddrinfo(), need to have all
 *                         fields specified before passing into
 *                         this function
 * char *port: port the server is listening on
 * char *name: servers machine name or IP address
 *
 * Output:
 * Pointer to a struct SOCK_INFO on success, NULL on failure
 */
SOCK_INFO *set_active_udp(struct addrinfo *hints, char *port, char *name);

/*
 * Sends a packet over a TCP connection.
 *
 * Input:
 * void *pkt: packet to be sent
 * int sockfd: socket file descriptor
 * int bufsize: size of packet
 *
 * Output:
 * 0 on success, -1 on failure
 */
int send_tcp(void *pkt, int sockfd, int bufsize);

/*
 * Sends a packet over a UDP connection.
 *
 * Input:
 * void *pkt: packet to be sent
 * int sockfd: size of packet
 * SOCK_INFO *info: SOCK_INFO with receivers address info and
 *                  the socket we are sending on
 *
 * Output:
 * 0 on success, -1 on failure
 */
int send_udp(void *pkt, int sockfd, SOCK_INFO *info);

/*
 * Receives a packet over a TCP connection.
 *
 * Input:
 * void *pkt: buffer to hold received packet
 * int sockfd: socket file descriptor
 * int bufsize: size of incoming packet
 *
 * Output:
 * 0 on success, -1 on failure
 */
int recv_tcp(void *pkt, int sockfd, int bufsize);

/*
 * Receives a packet over a UDP connection.
 *
 * Input:
 * void *pkt: buffer to hold received packet
 * int bufsize: size of packed to be received
 * SOCK_INFO *info: SOCK_INFO with the socket we are receiving on
 *                  and fields to hold the sender address information
 *
 * Output:
 * 0 on success, -1 on failure
 */
int recv_udp(void *pkt, int bufsize, SOCK_INFO *info);

#endif // SOCKSETUP_H
