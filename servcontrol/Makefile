Done: Control
	echo "DONE"

Control: Connection.o ConnectionException.o TCPConnection.o TCPConnectionSecurity.o Transferable.o TransferableFactory.o TransferableProfile.o ServerLoad.o NewUser.o DisconnectUser.o Server.o ControlProfile.o IPServer.o Control.o
	g++ ConnectionException.o Connection.o TCPConnection.o TCPConnectionSecurity.o TransferableFactory.o Transferable.o TransferableProfile.o ServerLoad.o NewUser.o DisconnectUser.o Server.o ControlProfile.o IPServer.o Control.o -o Control -lssl -lcrypto -pthread -std=c++0x `mysql_config --libs` -lmysqlpp

dummy: dummy.o
	g++ dummy.o `mysql_config --libs` -lmysqlpp -o dummy

dummy.o: ./dummy.cpp
	g++ -Wall -pedantic -Wno-long-long -O2 `mysql_config --include` -I/usr/include/mysql++ -c dummy.cpp

DisconnectUser.o: ./src/DisconnectUser.cpp
	g++ -c ./src/DisconnectUser.cpp -I./ -I../conexion/include -I./include -std=c++0x -pedantic -Wno-long-long -O2 `mysql_config --include` -I/usr/include/mysql++

NewUser.o: ./src/NewUser.cpp
	g++ -c ./src/NewUser.cpp -I./ -I../conexion/include -I./include -std=c++0x -pedantic -Wno-long-long -O2 `mysql_config --include` -I/usr/include/mysql++

ServerLoad.o: ./src/ServerLoad.cpp
	g++ -c ./src/ServerLoad.cpp -I./ -I../conexion/include -I./include -std=c++0x -pedantic -Wno-long-long -O2 `mysql_config --include` -I/usr/include/mysql++

Server.o: ./src/Server.cpp
	g++ -c ./src/Server.cpp -I./ -I../conexion/include -I./include -std=c++0x

ConnectionManager.o: ../conexion/source/ConnectionManager.cpp
	g++ -c ../conexion/source/ConnectionManager.cpp -I../conexion/include -std=c++0x
Connection.o: ../conexion/source/Connection.cpp
	g++ -c ../conexion/source/Connection.cpp -I../conexion/include/ -std=c++0x
ConnectionException.o: ../conexion/source/ConnectionException.cpp
	g++ -c ../conexion/source/ConnectionException.cpp -I../conexion/include/ -std=c++0x
TCPConnection.o: ../conexion/source/TCPConnection.cpp
	g++ -c ../conexion/source/TCPConnection.cpp -I../conexion/include/ -std=c++0x
TCPConnectionSecurity.o: ../conexion/source/TCPConnectionSecurity.cpp
	g++ -c ../conexion/source/TCPConnectionSecurity.cpp -I../conexion/include/ -std=c++0x
Transferable.o: ../conexion/source/Transferable.cpp
	g++ -c ../conexion/source/Transferable.cpp -I../conexion/include/ -std=c++0x
TransferableFactory.o: ../conexion/source/TransferableFactory.cpp
	g++ -c ../conexion/source/TransferableFactory.cpp -I../conexion/include/ -std=c++0x
TransferableProfile.o: ../conexion/source/TransferableProfile.cpp
	g++ -c ../conexion/source/TransferableProfile.cpp -I../conexion/include/ -std=c++0x
IPServer.o: ./src/IPServer.cpp
	g++ -c ./src/IPServer.cpp -I./ -I../conexion/include -I./include -std=c++0x -pedantic -Wno-long-long -O2 `mysql_config --include` -I/usr/include/mysql++
Control.o: ./src/Control.cpp
	g++ -c ./src/Control.cpp -I./ -I../conexion/include -I./include -std=c++0x -pedantic -Wno-long-long -O2 `mysql_config --include` -I/usr/include/mysql++



ControlProfile.o: ./src/ControlProfile.cpp
	g++ -c ./src/ControlProfile.cpp -I../conexion/include -I./include -std=c++0x -pedantic -Wno-long-long -O2 `mysql_config --include` -I/usr/include/mysql++

clean:
	rm *.o
	rm Control	
