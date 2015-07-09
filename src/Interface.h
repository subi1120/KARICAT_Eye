#ifndef _INTERFACES_H_
#define _INTERFACES_H_


class kaiMsg;

class MsgHandler
{
public:
	MsgHandler(){}
	virtual ~MsgHandler() {}

	virtual void handleMessage(kaiMsg& msg) = 0;
};



class MsgTransmitter
{
public:
	MsgTransmitter(){}
	virtual ~MsgTransmitter()  {}

	virtual void sendMessage(const kaiMsg& msg) = 0;


};

#endif
