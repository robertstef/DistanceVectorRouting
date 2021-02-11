

**<ins>Distance Vector Routing Protocol</ins>**

This project is an implementation of a distance vector routing protocol.

**<ins>Execution Instructions</ins>**

To run the program execute the following command:

`router routerName myPort theirPort1 [theirPort2]`

​	- `routerName`: A caplital letter from A to Z

- `myPort`: the port this router is listening on for connections
- `theirPort1`: the port a router we wish to send to is listenting on
- `theirPort2`: the port of a second router we wish to send to - OPTIONAL

To run multiple routers, run this program from multiple terminals, configuring the ports to give the desired topology.

*Note*:

If the following error message is printed on startup please restart the program. This means the neighbouring router did not successfully send us their router name and thus their entry will not be stored in our router table.

`connect_nbrs(): did not receive neighbours router name`

