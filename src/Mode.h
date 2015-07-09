/*
 * Mode.h
 *
 *  Created on: Nov 6, 2014
 *      Author: turtlebot
 */

#ifndef MODE_H_
#define MODE_H_
#include "kai/kai.h"
#include "Interface.h"

class Mode {
public:
	Mode(MsgTransmitter* tr);
	virtual ~Mode();

	enum eModeType
	{
		eModeType_Ready = 0,
		eModeType_Detecting,
		eModeType_Tracking,
		eModeType_Num
	};

	int type() {return m_modeType;}


	virtual void handleMessage(kaiMsg& msg) = 0;

	virtual void enter(void* param) = 0;
	virtual int process() = 0;
	virtual void leave(void* param) = 0;

protected:
	int m_modeType;
	MsgTransmitter* m_msgTransmitter;


};

#endif /* MODE_H_ */
