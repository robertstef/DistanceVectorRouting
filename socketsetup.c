/*
 * Robert Stefanyshin
 * ras867
 * 11137532
 * CMPT434
 * A2
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "socketsetup.h"

#define BACKLOG 10 // number of pending connection queue can hold

int set_passive_tcp(struct addrinfo *hints, char *port)
{
    int rv;
    int sockfd;
    struct addrinfo *p, *servinfo;
    int yes = 1;

    if ((rv = getaddrinfo(NULL, port, hints, &servinfo)) != 0) {
        fprintf(stderr, "set_passive_tcp() getaddrinfo: %s\n", 
                gai_strerror(rv));
        return -1;
    }

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                        p->ai_protocol)) == -1) {
            perror("set_passive_tcp(): socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                    sizeof(int)) == -1) {
            perror("set_passive_tcp(): setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("set_passive_tcp(): bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
		fprintf(stderr, "set_passive_tcp(): failed to bind\n");
        return -1;
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("set_passive_tcp(): listen");
        return -1;
	}

    return sockfd;
}

int set_active_tcp(struct addrinfo *hints, char *port, char *name)
{
    int rv, sockfd;
    struct addrinfo *p, *servinfo;

    if ((rv = getaddrinfo(name, port, hints, &servinfo)) != 0) {
		fprintf(stderr, "set_active_tcp(): getaddrinfo: %s\n", 
                gai_strerror(rv));
		return -1;
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("set_active_tcp(): socket");
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			perror("set_active_tcp(): connect");
			close(sockfd);
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "set_active_tcp(): failed to connect\n");
		return -1;
	}

    freeaddrinfo(servinfo);

    return sockfd;
}

int set_passive_udp(struct addrinfo *hints, char *port)
{
    int rv, sockfd;
    struct addrinfo *p, *servinfo;

    if ((rv = getaddrinfo(NULL, port, hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return -1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("set_passive_udp(): socket");
			continue;
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("set_passive_udp(): bind");
			continue;
		}

		break;
	}

	if (p == NULL) {
		fprintf(stderr, "set_passive_udp(): failed to bind socket\n");
		return -1;
	}

	freeaddrinfo(servinfo);

    return sockfd;
}

SOCK_INFO *set_active_udp(struct addrinfo *hints, char *port, char *name)
{
    int rv, sockfd;
    struct addrinfo *p, *servinfo;
    SOCK_INFO *result;

    if ( (result = malloc(sizeof(SOCK_INFO))) == NULL )
    {
        fprintf(stderr, "set_active_udp(): malloc error\n");
        return NULL;
    }

    if ((rv = getaddrinfo(name, port, hints, &servinfo)) != 0) {
        fprintf(stderr, "set_active_udp(): getaddrinfo: %s\n", 
                gai_strerror(rv));
        return NULL;
    }

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                        p->ai_protocol)) == -1) {
            perror("set_active_udp(): socket()");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "set_active_udp(): failed to create socket\n");
        return NULL;
    }

    // socket successfully created, package result and return
    result->addr = *p->ai_addr;
    result->addr_len = p->ai_addrlen;
    result->sockfd = sockfd;

    return result;
}

int send_tcp(void *pkt, int sockfd, int bufsize)
{
    int rv;

    while ( (rv = send(sockfd, pkt, bufsize, 0)) < bufsize )
    {
        if ( rv == -1 )
        {
            perror("send_tcp(): send");
            return -1;
        }
    }
    return 0;
}

int send_udp(void *pkt, int bufsize, SOCK_INFO *info)
{
   int rv;

   while ( (rv = sendto(info->sockfd, pkt, bufsize, 0, &info->addr, 
                   info->addr_len)) < bufsize )
   {
       if ( rv == -1 )
       {
           perror("send_udp(): sendto");
           return -1;
       }
   }
   return 0;
}

int recv_tcp(void *pkt, int sockfd, int bufsize)
{
    int rv;

    while ( (rv = recv(sockfd, pkt, bufsize, 0)) < bufsize )
    {
        if ( rv == -1 )
        {
            perror("recv_tcp(): recv");
            return -1;
        }
    }
    return 0;
}

int recv_udp(void *pkt, int bufsize, SOCK_INFO *info)
{
    int rv;

    while ( (rv = recvfrom(info->sockfd, pkt, bufsize, 0,
                    &info->addr, &info->addr_len)) < bufsize )
    {
        if ( rv == -1 )
        {
            perror("recv_udp(): recvfrom");
            return -1;
        }
    }
    return 0;
}

