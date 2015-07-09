/*
 * ModeManager.h
 *
 *  Created on: Nov 6, 2014
 *      Author: turtlebot
 */

#ifndef MODEMANAGER_H_
#define MODEMANAGER_H_

#include "Mode.h"
#include "Interface.h"
#include "UDPTranceiver.h"

class Mode;
class FeatureTracker;
class MyFeatureDetector;
class CameraSensor;

class ModeManager : MsgHandler {
public:
	ModeManager();
	virtual ~ModeManager();



	void init();
	void process();
	virtual void handleMessage(kaiMsg& msg);
	void changeMode(int nextMode);

private:

	Mode* m_modes[Mode::eModeType_Num];
	Mode* m_curMode;
	MyFeatureDetector* 	m_featureDetector;
	FeatureTracker*		m_featureTracker;
	CameraSensor*		m_cameraSensor;

	UDPTranceiver* 		m_tranceiver;

};

#endif /* MODEMANAGER_H_ */
