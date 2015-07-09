/*
 * UDPTranceiver.h
 *
 *  Created on: Sep 4, 2014
 *      Author: turtlebot
 */

#ifndef UDPTRANCEIVER_H_
#define UDPTRANCEIVER_H_
#include <string>
#include <kai/kai.h>
#include "Interface.h"



using namespace std;
class UDPTranceiver: public kaiSocket, public MsgTransmitter {
public:
	UDPTranceiver(unsigned short port, const string& remote_iddr, unsigned short remote_port);

	virtual ~UDPTranceiver();

	virtual void onMessage(kaiMsg& msg);
	virtual void sendMessage(const kaiMsg& msg);
	void registerMsgHandler(MsgHandler* handler);

private:

	sockaddr_in m_remoteAddr;
	MsgHandler* m_msgHandler;

};

#endif /* UDPTRANCEIVER_H_ */
