To run the program execute the following command:

router routerName myPort theirPort1 [theirPort2]

routerName: A caplital letter from A to Z
myPort: the port this router is listening on for connections
theirPort1: the port a router we wish to send to is listenting on
theirPort2: OPTIONAL - the port of a second router we wish to send to

To run multiple routers, run this program from multiple terminals, configuring
the ports to give the desired topology.


NOTE:

If the error message:

connect_nbrs(): did not receive neighbours router name

is printed to the screen on startup, please restart the program. This means the
neighbouring router did not successfully send us their router name
and thus their entry will not be stored in our router table.
