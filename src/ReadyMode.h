/*
 * ReadyMode.h
 *
 *  Created on: Nov 6, 2014
 *      Author: turtlebot
 */

#ifndef READYMODE_H_
#define READYMODE_H_

#include "Mode.h"

class ReadyMode: public Mode {
public:
	ReadyMode(MsgTransmitter* tr);
	virtual ~ReadyMode();

	virtual void handleMessage(kaiMsg& msg);

	virtual void enter(void* param);
	virtual int process();
	virtual void leave(void* param);

private:
	bool m_bSetChannel;
};

#endif /* READYMODE_H_ */
