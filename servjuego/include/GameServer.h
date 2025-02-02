#ifndef _GAME_SERVER_H_
#define _GAME_SERVER_H_

#include <map>
#include <thread>
#include <string>

#include <Zone.h>
#include <Singleton.h>
#include <Connection.h>

class GameServer : public Singleton<GameServer> {
public:
	class ControlListener : public ConnectionCallback {
		friend class GameServer;
		ControlListener();
		~ControlListener();
	public:
		virtual void callbackFunction(Transferable* received, Connection* c) throw();
	};
	
	class RouterListener : public ConnectionCallback {
		friend class GameServer;
		RouterListener();
		~RouterListener();
	public:
		virtual void callbackFunction(Transferable* received, Connection* c) throw();
	};
	
	ControlListener* CONTROL_LISTENER;
	RouterListener* ROUTER_LISTENER;
	
private:
	
	int mServerId;
	std::map<int, ZoneHandler*> mZones;
	std::map<int, int> mClients; //Map client-zone
	
	std::vector<Connection*> mRouters;
	
	std::mutex mZonesMutex;
	std::mutex mClientsMutex;
	std::mutex mRoutersMutex;

protected:
	bool hasZone(int zoneId) const;

	friend class Singleton<GameServer>;
	GameServer();
	
public:
	virtual ~GameServer();
	
	
	void serverId(int id);
	int serverId() const;
	
	void addClient(int clientId, int clientZone);
	void delClient(int clientId);

	//Start managing a zone from DB
	void startZone(int zoneId);
	
	//Totally stop managing a zone, destroying all its related objects
	void stopZone(int zoneId, bool force = false);
	
	//Totally stop all zones, set GameServer object ready for destruction
	void stopAll();
	
	//Receiving order from balance to set a zone as detachable for given server
	void markForDetach(int zoneId, int idServer);
	
	//Receiving order from balance to connect with given server to retrieve the zone
	void receiveZone(int zoneId, Connection* oldServer);
	
	//Start cloning a zone to a new server, sending its current Zone data to the new server
	//And sending zone's events to the new server untill all data is copied.
	void detachZone(int zoneId, Connection* newServer);

	void queueInstruction(int idZone, Instruction* ins, Connection* c);
	
	
	void addRouter(Connection* c);
	//Start collecting zone loads and sending then back to balance 
	void SendZoneLoads(Connection*);
	
	void sendToClients(Transferable*);
};



#endif
