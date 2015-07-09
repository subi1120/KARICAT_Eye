/*
 * ModeManager.cpp
 *
 *  Created on: Nov 6, 2014
 *      Author: turtlebot
 */

#include "MsgDef.h"
#include "Interface.h"
#include "tracker.h"
#include "UDPTranceiver.h"
#include "ReadyMode.h"
#include "DetectingMode.h"
#include "TrackingMode.h"
#include "CameraSensor.h"
#include "ModeManager.h"

Rect targetROI;

ModeManager::ModeManager() : m_curMode(NULL), m_featureDetector(NULL), m_featureTracker(NULL), m_cameraSensor(NULL), m_tranceiver(NULL)
{
	// TODO Auto-generated constructor stub
	m_modes[Mode::eModeType_Ready] = NULL;
	m_modes[Mode::eModeType_Detecting] = NULL;
	m_modes[Mode::eModeType_Tracking] = NULL;

}

ModeManager::~ModeManager() {
	// TODO Auto-generated destructor stub

	if (m_featureDetector)
		delete m_featureDetector;

	if (m_curMode)
	{
		delete[] m_modes;
	}

	if (m_featureTracker)
		delete m_featureTracker;

	if (m_cameraSensor)
		delete m_cameraSensor;

	if (m_tranceiver)
		delete m_tranceiver;

}


void ModeManager::init()
{
	m_tranceiver = new UDPTranceiver(7150, "192.168.0.5", 7250);
	m_tranceiver->registerMsgHandler(this);

	m_cameraSensor = new CameraSensor();
	m_cameraSensor->init(CameraSensor::RGB_CAMERA | CameraSensor::DEPTH_CAMERA, CameraSensor::FRAME_SIZE320X240);


	m_modes[Mode::eModeType_Ready] = new ReadyMode(m_tranceiver);
	m_modes[Mode::eModeType_Detecting] = new DetectingMode(m_cameraSensor, m_tranceiver);
	m_modes[Mode::eModeType_Tracking] = new TrackingMode(m_cameraSensor, m_tranceiver);

	printf("here\n");

	m_curMode = m_modes[Mode::eModeType_Ready];
	m_curMode->enter(NULL);

}

void ModeManager::handleMessage(kaiMsg& msg)
{

	switch(msg.id())
	{
	case CMD_START_SEND_IMG:
		m_cameraSensor->enableSendToGC();
		break;

	case CMD_STOP_SEND_IMG:
		m_cameraSensor->disableSendToGC();
		break;
	}

	m_curMode->handleMessage(msg);


}

void ModeManager::process()
{
	int res =1;

	while(res > 0)
	{
		res = m_tranceiver->recv();
	}

	int nextMode = m_curMode->process();
	if (m_curMode->type() != nextMode)
	{
		changeMode(nextMode);
	}

}

void ModeManager::changeMode(int nextMode)
{
	m_curMode->leave(NULL);
	m_curMode = m_modes[nextMode];
	m_curMode->enter(&targetROI);
}
