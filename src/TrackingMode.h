/*
 * TrackingMode.h
 *
 *  Created on: Nov 6, 2014
 *      Author: turtlebot
 */

#ifndef TRACKINGMODE_H_
#define TRACKINGMODE_H_

#include "Mode.h"

class FeatureTracker;
class CameraSensor;
class RANSAC_Tracker;

class TrackingMode: public Mode {
public:
	TrackingMode(CameraSensor* camera, MsgTransmitter* tr);
	virtual ~TrackingMode();

	virtual void handleMessage(kaiMsg& msg);
	virtual void enter(void* param);
		virtual int process();
		virtual void leave(void* param);

private:
	FeatureTracker* m_tracker;
	CameraSensor* m_camera;
	RANSAC_Tracker* m_RANSACTracker;
};

#endif /* TRACKINGMODE_H_ */
