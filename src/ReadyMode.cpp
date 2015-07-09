/*
 * ReadyMode.cpp
 *
 *  Created on: Nov 6, 2014
 *      Author: turtlebot
 */
#include "MsgDef.h"
#include "ReadyMode.h"

ReadyMode::ReadyMode(MsgTransmitter* tr) : Mode(tr), m_bSetChannel(false)
{
	// TODO Auto-generated constructor stub
	m_modeType = eModeType_Ready;

}

ReadyMode::~ReadyMode() {
	// TODO Auto-generated destructor stub
}


void ReadyMode::enter(void* param)
{
	m_bSetChannel = false;

	kaiMsg sendmsg;
	char buffer[10];
	sendmsg.setBuffer(buffer);

	sendmsg.begin();
	sendmsg.id(REQ_KARIKAT_EYE_REGISTER);
	sendmsg.end();

	m_msgTransmitter->sendMessage(sendmsg);

	printf("ready mode\n");

}

void ReadyMode::handleMessage(kaiMsg& msg)
{
	switch(msg.id())
	{
	case NOTI_KARIKAT_ON:
	{
		kaiMsg sendmsg;
		char buffer[10];
		sendmsg.setBuffer(buffer);

		sendmsg.begin();
		sendmsg.id(REQ_KARIKAT_EYE_REGISTER);
		sendmsg.end();

		m_msgTransmitter->sendMessage(sendmsg);
	}
	break;

	case ANS_KARIKAT_EYE_REGISTER:
	{
		m_bSetChannel = true;
	}
	break;
	}

}


int ReadyMode::process()
{
	int nextmode = eModeType_Ready;
	if (m_bSetChannel)
		nextmode = eModeType_Detecting;

	return nextmode;
}

void ReadyMode::leave(void* param)
{

}
