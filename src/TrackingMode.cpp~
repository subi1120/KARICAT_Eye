/*
 * TrackingMode.cpp
 *
 *  Created on: Nov 6, 2014
 *      Author: turtlebot
 */
#include "tracker.h"
#include "MsgDef.h"
#include "CameraSensor.h"
#include "RANSAC_Tracker.h"
#include "TrackingMode.h"


TrackingMode::TrackingMode(CameraSensor* camera, MsgTransmitter* tr) : Mode(tr), m_camera(camera)
{
	// TODO Auto-generated constructor stub

//	m_tracker = new FeatureTracker();
//	m_tracker->init(vmin, vmax, smin, FeatureTracker::FRAMESIZE_320X240);

	m_tracker = NULL;
	m_modeType = eModeType_Tracking;
	m_RANSACTracker = new RANSAC_Tracker;

}

TrackingMode::~TrackingMode() {
	// TODO Auto-generated destructor stub
	if (m_tracker)
		delete m_tracker;
	if (m_RANSACTracker)
		delete m_RANSACTracker;
}


void TrackingMode::enter(void* param)
{
/*	const Mat& img = m_camera->getBGRImage();
	Rect* targetROI = reinterpret_cast<Rect*>(param);

	m_tracker->setInitialROI(cvRect(targetROI->x , targetROI->y, targetROI->width, targetROI->height), img);*/
	m_RANSACTracker->init(320,240);

	printf("tracking mode.. \n");

}

void TrackingMode::handleMessage(kaiMsg& msg)
{

}


int TrackingMode::process()
{
	int nextmode = eModeType_Tracking;

/*	const Mat& img = m_camera->getBGRImage();
	if (!img.empty())
	{
		Rect targetRect;
		float angle;
		Point center;
		bool bTargeting = true;

		try
		{
			m_tracker->track(img, angle, targetRect, center);
		}
		catch (cv::Exception& ex)
		{
			bTargeting = false;
		}

		if (targetRect.size().width == 0 )
		{
			bTargeting = false;
		}

		if (bTargeting)
		{
			m_camera->setROI(targetRect);
			unsigned short distance = 0;
			const Mat& depth = m_camera->getDepthImage();
			if (!depth.empty())
			{
				distance = depth.at<unsigned short>(center);
			}

			kaiMsg sendmsg;
			char buffer[20];

		//	printf("angle : %f\n", angle);

			short _angle = 100*angle;
			sendmsg.setBuffer(buffer);
			sendmsg.begin();
			sendmsg<<_angle << distance;
			sendmsg.id(NOTI_TARGET_TRACKING_INFO);
			sendmsg.end();

			m_msgTransmitter->sendMessage(sendmsg);
		}
		else
		{
			nextmode = eModeType_Detecting;
		}
	//	printf("tracking..\n");

	}*/
	
	const Mat& depth = m_camera->getDepthImage();
	Mat copyDepth;
	depth.copyTo(copyDepth);
	
	m_RANSACTracker->track(copyDepth);

	kaiMsg sendmsg;
	char buffer[20];

		//	printf("angle : %f\n", angle);
	float angle = 0;
	unsigned short distance = 0;
	m_RANSACTracker->getTargetVector(distance, angle);
	m_camera->setROI(m_RANSACTracker->getBoundingBox());
	short _angle = 100*angle;
	sendmsg.setBuffer(buffer);
	sendmsg.begin();
	sendmsg<<_angle << distance;
	sendmsg.id(NOTI_TARGET_TRACKING_INFO);
	sendmsg.end();

	m_msgTransmitter->sendMessage(sendmsg);

	
	return nextmode;
}

void TrackingMode::leave(void* param)
{

}
