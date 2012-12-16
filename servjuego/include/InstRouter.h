#ifndef _INST_ROUTER_H_
#define _INST_ROUTER_H_

#include <Datagram.h>

//SENT

//RECEIVED

class AddRouterRcvd : public Datagram<AddRouterRcvd> {
	int mRouterId;
public:
	AddRouterRcvd();
	~AddRouterRcvd();
	
	void exec(Connection* c) throw();
};

#endif
