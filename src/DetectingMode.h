/*
 * DetectingMode.h
 *
 *  Created on: Nov 6, 2014
 *      Author: turtlebot
 */

#ifndef DETECTINGMODE_H_
#define DETECTINGMODE_H_

#include "Mode.h"

class CameraSensor;
class MyFeatureDetector;

class DetectingMode: public Mode {
public:
	DetectingMode(CameraSensor* camera, MsgTransmitter* tr);
	virtual ~DetectingMode();

	virtual void handleMessage(kaiMsg& msg);

	virtual void enter(void* param);
	virtual int process();
	virtual void leave(void* param);


private:
	MyFeatureDetector* m_detector;
	CameraSensor* m_camera;
};

#endif /* DETECTINGMODE_H_ */
