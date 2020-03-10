#include <stdio.h>
#include <stdlib.h>
#include "userinput.h"

int main(int argc, char *argv[])
{
    int rv;

    if ( (rv = checkinput(argc, argv)) == -1)
    {
        fprintf(stderr, "Usage: name myport theirport1...\n");
        exit(EXIT_FAILURE);
    }
    printf("Input OK!\n");
    return 0;
}
