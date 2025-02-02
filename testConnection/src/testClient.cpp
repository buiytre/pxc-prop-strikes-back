#include <string>
#include "Datagram.h"
#include "Connection.h"
#include <TestClasses.h>
#include <iostream>

int main(int argc, char** argv){
        //Esto se hace al principio del main (inicialiación de la conexión)
        //TestProfile es herenccia de TransferableProfile para decirle 
        TransferableFactory::instance().setProfile(new TestProfile());
        TransferableFactory::instance().setProtocol("0.1a");
        std::string ip(argv[1]);
        std::string port(argv[2]);
        printf("MainClient(): 1\n");
        fflush(stdout);
	 Connection* n = new TCPConnectionSecurity("ValidClient.pem", "dh1024.pem");
        while(!n->connect(ip, port)) { 
                std::cout << "No se ha podido conectar. Se volvera a intentar en unos momentos.. "<< std::endl; 
                sleep(3);
        }
        printf("MainClient(): objeto Conexion creado\n");
        fflush(stdout);
        printf("MainClient(): Connected\n");
        fflush(stdout);


        while (1){
                if (n->isLinkOnline()){                 
                        TestTransferableSent2* sent2 = NULL;
                        sent2 = new TestTransferableSent2();
                
                        printf("MainClient(): Sending2...\n");
                        fflush(stdout);
                        n->send(*sent2);
                        printf("MainClient(): sent2\n");
                        fflush(stdout);
                        delete sent2;

                        TestTransferableSent7* sent7 = NULL;
                        sent7 = new TestTransferableSent7();
                        printf("MainClient(): Sending7...\n");
                        fflush(stdout);
                        n->send(*sent7);
                        printf("MainClient(): sent7\n");
                        fflush(stdout);
                        delete sent7;
                }else{
                        std::cout << "Conexion perdida, Reconectando... "<< std::endl;
                        if(n->connect(ip, port)) {
                                std::cout << "Conexion Restablecida "<< std::endl; 
                        }else{
                                std::cout << "No se ha podido conectar. Se volvera a intentar en unos momentos.. "<< std::endl; 
                                sleep(3);
                        }
                }
        }
}


