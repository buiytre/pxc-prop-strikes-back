# pxc-prop-strikes-back
Project for the subject PXC from UPC:  Client - Server structure for a game distributed in multiple servers

This is a project made for the PXC subject from UPC that simulates an arquitecture of multiple Servers and clients.

Every Server controls a zone in the game and the client can "navigate" between this zones and be redirected to the server that it belongs.
---
//LOGIN sControl sJugador
pxc-prop-strikes-back/servlogin$ bin/Login  3457 3450

//ServerGame
pxc-prop-strikes-back/servjuego/bin$ ./gameServer 3458 34588
pxc-prop-strikes-back/servjuego/bin$ ./gameServer 3459 34599
pxc-prop-strikes-back/servjuego/bin$ ./gameServer 3460 ????
pxc-prop-strikes-back/servjuego/bin$ ./gameServer 3461 ????
pxc-prop-strikes-back/servjuego/bin$ ./gameServer 3462 ????

//control
pxc-prop-strikes-back/servcontrol$ ./Control 5

//servrouter
pxc-prop-strikes-back/servrouter/bin$ ./router 3455 3456

//cliente
pxc-prop-strikes-back/cliente/bin$ ./client 127.0.0.1 3450 user1 user1
---
De 150 a 200: instrucciones entre router y servidor de juegos
150: AddRouterSend - AddRouterRcvd
155: EndZoneTransferSend - EndZoneTransferRcvd

De 200 a 250: instrucciones entre cliente y router
200: ConnectClientSend - ConnectClientRcvd

De 500 en adelante: Instrucciones entre cliente de juego y servidor de juego pasando por router, y sentido inverso 
500: SetUserStatusSend - SetUserStatusRcvd
501: SetZoneSend - SetZoneRcvd
502: SetDestinationSend - SetDestinationRcvd
