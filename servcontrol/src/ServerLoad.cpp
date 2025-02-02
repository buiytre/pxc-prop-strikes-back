#include <ServerLoad.h>
#include <Control.h>

#include <iostream>

void ServerLoadRcvd::exec(Connection* c) const throw(){
	
	//Inserir la carrega a la carrega del server que toca i actualitzar la mascara on toca també
	std::cout << "Recibiendo mensaje del server: " << idServer << std::endl; //debug
	std::cout <<  "server: " << idServer << " zona: "  << idZone << " carga: " << zoneLoad << " faltan: " << remainingZones << std::endl; //debug
	std::list<Server*>::iterator it;
	int i;
	for(it=Control::instance().servers.begin();it!=Control::instance().servers.end();it++) {
		if ((*it)->id == idServer) { 
			for(i = 0; i < (*it)->load.distribution.size(); ++i){
				if((*it)->load.distribution.at(i)->zone == idZone){
					Control::instance().recievedMutex.lock();
					(*it)->load.distribution.at(i)->load = zoneLoad;
					(*it)->load.totalLoad += zoneLoad;
					Control::instance().recievedMutex.unlock();	
				}
			}
			(*it)->printServer(); //debug
			if(remainingZones == 0){
				Control::instance().recievedMutex.lock();
				//std::cout <<  "Resultat de la mascara: " << Control::instance().recievedConnectionMask << "\n";
				//std::cout << idServer << std::endl;
				Control::instance().recievedConnectionMask = Control::instance().recievedConnectionMask & ~(1 << idServer);
				//std::cout << "Resultat del desplaçament: " << ~(1 << idServer) << std::endl;
				//std::cout << "Resultat de la mascara canviada: " << Control::instance().recievedConnectionMask << "\n";
				Control::instance().recievedMutex.unlock();
				return;	
			} 			
		}		
	}
}

ServerLoadRcvd::~ServerLoadRcvd(){
	
}

