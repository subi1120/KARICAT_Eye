/*
 * UDPTranceiver.cpp

 *
 *  Created on: Sep 4, 2014
 *      Author: turtlebot
 */
#include "Interface.h"
#include "UDPTranceiver.h"

char buffer[1000];

UDPTranceiver::UDPTranceiver(unsigned short port, const string& remote_iddr, unsigned short remote_port) : m_msgHandler(NULL)
{
	kaiSocket::create(false, kaiON_MESSAGE);
	bind(port);
//	connect(remote_iddr.c_str(), remote_port);
	setBlockingMode(kaiNON_BLOCKING_SOCKET);
	// TODO Auto-generated constructor stub


	m_remoteAddr.sin_port = htons(remote_port);
	m_remoteAddr.sin_addr.s_addr = inet_addr(remote_iddr.c_str());
	m_remoteAddr.sin_family = AF_INET;
}

UDPTranceiver::~UDPTranceiver() {
	// TODO Auto-generated destructor stub
	close();
}


void UDPTranceiver::onMessage(kaiMsg& msg)
{
	if (m_msgHandler)
		m_msgHandler->handleMessage(msg);
}

void UDPTranceiver::sendMessage(const kaiMsg& msg)
{
	sendTo(msg, &m_remoteAddr);
}

void UDPTranceiver::registerMsgHandler(MsgHandler* handler)
{
	m_msgHandler = handler;
}



