`router.c`

This source file contains the main code to run the router. This file
calls all initialization functions and sets up all necessary parameters.
The code in this file polls the passive socket for new connections.
When a new connection is requested, the connection is accepted, the
name of the router is send to the neighbour, and an ack is awaited.
Once the connection has been acknowledged, the new receiving socket
is added to the list of other receiving sockets.

On a timeout from poll, occurring every 2 seconds, the router table
will be printed the stout and connections to neighbouring routers will
be checked. If neighbouring routers have disconnected, the program will
attempt to re-establish the lost connection. Lastly, our router table 
will be sent to all outgoing neighbouring routers.

\
`socketmanage.[ch]`

These files contain all necessary code to manage socket file descriptors
for both outgoing and incoming connections. 

This file performs 3 primary functions, first is to log incoming connections.
Each socket will be set to non blocking so the program can run asynchronously.
It will then be added to a list of incoming sockets.

The primary function is this manages the outgoing socket connections with
our neighbours specified in the command line arguments. To test if a socket
is active we will attempt to send 0 bytes to the socket. If the send is
unsuccessful errno will checked for the EPIPE error, indicating the socket
is broken. If this is the case our router table will be updated to reflect
the dropped connections. Next we will try to restablish the conection.
On success, the connection will be updated is the corresponding struct
nbr_info.

Lastly, this file received router tables from each of our incoming sockets.
The function iterates over the linked list of incoming sockets and checks
for a router table, if one is there, we update our local router table
accordingly.

\
`routertable.[ch]`

These files contain all necessary code to manage and update the router table.

A router table is defined in the struct RTABLE and uses three parallel arrays
to keep track of information. The arrays contain distance for Dx(y) where x
is our router and y is every other router in the network. Next_hopx(y) for
each next hop router and the names of every potential router in the network
from A to Z.

The router table is updated in the follow way. If our router is not in the
incoming table then we do not process it. If it is, calculate Dx(y) for
all nodes y. Firstly we calculate the new distance estimate for the current
y. If it is a shorted path than the one in our table, update our table.
Lastly we check all the next hops in our table. If the next hop we upated
to a router than is not our neighbour in the previous step, we discard
that change because it means we do not have a path to that router.

Additionally, this file contains auxillary functions that add outgoing
neighbouring routers to our table and that send our table to all neighbouring
routers.

Lastly, if a dropped router has been detected, the function dropped_rtable
iterates through our table and removes the path to that neighbour as well
as removes any paths that had that neighbour as the next hop.


Bugs:

The program does not always correctly diagnose when and propagate the changes
when a router has been dropped. Although the router dropped_rtable() functions
as wanted, there are issues with the changes showing up throughout the
network. Generally, a dropped router is recongized when only two or three
routers belong to the network, but as the network grows, changes fail
to propagate. I have managed to get the changes to propage in 1 or 2 test
scenarios, however, the results are not consistent.

Note, the program is able to handle routers dropping and rejoining the
network. The issues reside with updating the router tables after a
router has dropped.

Additionally, the program stuggles with the following scenario. Consider
a graph with the following edges:

A -> B
B -> A

B -> C
C -> B

C -> D
D -> C

D -> A

The program will correctly update the router table for nodes B, C, and D.
However, the program will fail to identify the path from A to D. After much
time spent considering the problem, I have been unable to figure out a solution.



