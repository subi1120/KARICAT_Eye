/*
 * DetectingMode.cpp
 *
 *  Created on: Nov 6, 2014
 *      Author: turtlebot
 */

#include "tracker.h"
#include "CameraSensor.h"
#include "DetectingMode.h"


extern Rect targetROI;

DetectingMode::DetectingMode(CameraSensor* camera, MsgTransmitter* tr) : Mode(tr), m_camera(camera)
{
//	m_detector = new MyFeatureDetector();
	m_detector = NULL;
	m_modeType = eModeType_Detecting;
	// TODO Auto-generated constructor stub

}

DetectingMode::~DetectingMode() {
	// TODO Auto-generated destructor stub
	if (m_detector)
		delete m_detector;
}



void DetectingMode::handleMessage(kaiMsg& msg)
{

}


void DetectingMode::enter(void* param)
{
//	m_detector->init("cs_cascade.xml");
	printf("detecting mode.. \n");
}

int DetectingMode::process()
{

	int nextmode = eModeType_Tracking;
/*	int nextmode = eModeType_Detecting;
	vector<Rect> targetROIs;

	bool bDetectFeature = false;

	const Mat& img = m_camera->getBGRImage();
*/
/*	if (!img.empty())
	{
		try
		{
			bDetectFeature = m_detector->detect(targetROIs,img,1.0);
		}
		catch (cv::Exception& ex)
		{
			cout << ex.msg;
		}
	}

	if (bDetectFeature)
	{
		nextmode = eModeType_Tracking;
		targetROI = targetROIs[0];
	}
*/
	return nextmode;
}

void DetectingMode::leave(void* param)
{

}
