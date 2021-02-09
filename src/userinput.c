#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include "userinput.h"

int valid_port(char *port)
{
    long int rv;
    char *ptr;

    if ( (rv = strtol(port, &ptr, 10)) == -1 )
        return -1;

    // port must be in range 30000-40000
    if ( (rv < 30000) || (rv > 40000) )
        return -1;

    return 0;
}

int valid_name(char *name)
{
    char c = name[0];

    // Make sure we only have 1 char
    if ( strlen(name) != 1 )
        return -1;

    // Upper case ascii chars lie in range 65 - 91
    else if ( (c < 65) || (c > 91) )
        return -1;

    return 0;
}

int checkinput(int count, char *args[])
{
    int rv;
    
    // count must be: 3 < count < 6
    if ( count < 4 || count > 5 )
        return -1;

    // NAME
    if ( (rv = valid_name(args[1])) == -1 )
    {
        fprintf(stderr, "name must be a single letter A-Z\n");
        return -1;
    }

    // PORT
    for (int i = 2; i < count; i++)
    {
        if ( (rv = valid_port(args[i])) == -1 )
        {
            fprintf(stderr, "port must be an integer in range 30000-40000\n");
            return -1;
        }
    }

    return 0;
}
    
