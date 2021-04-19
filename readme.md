**<ins>Distance Vector Routing Protocol</ins>**

This project is an implementation of a distance vector routing protocol.
Running the program will execute a simulation of the protocol.
The simulation can run a total of 26 routers, named A through Z where each router
can have a maximum of two neighbouring routers. Every two seconds the simulation will
print the router table for the local router displaying the distance to each known
router in the network and the name of the next hop required to reach the router for
that entry of the router table.

**<ins>Execution Instructions</ins>**

To run the program execute the following command:

`router routerName myPort theirPort1 [theirPort2]`

- `routerName`: A capital letter from A to Z
- `myPort`: the port this router is listening on for connections
- `theirPort1`: the port a router we wish to send to is listening on
- `theirPort2`: the port of a second router we wish to send to - OPTIONAL

To run multiple routers, run this program from multiple terminals, configuring the ports to give the desired topology.

**Notes**:
1. This program was developed and tested on a Linux machine. It intended to be run on a Linux machine and has not 
   been tested with other operating systems.
   <br></br>
2. If the following error message is printed on startup please restart the 
   program in the terminal the message appeared in. 
    <br></br>
   `connect_nbrs(): did not receive neighbours router name`
   <br></br> 
   This means the neighbouring router did not successfully send us their router name and thus their entry will 
   not be stored in our router table.
   

