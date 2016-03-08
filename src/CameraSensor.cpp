/*
 * CameraSensor.cpp
 *
 *  Created on: Nov 6, 2014
 *      Author: turtlebot
 */

#include "MsgDef.h"
#include "UDPTranceiver.h"
#include "CameraSensor.h"
#include "MPEGCoder.h"
#include "zlib.h"


string colorFileName = "/home/turtlebot/data/tracking/color1.avi";
//string colorFileName = "color1.avi";
string depthFileName = "/home/turtlebot/data/tracking/depth1.cdd";


uint8_t encodedImgBuffer[100000];
char imgDataBuffer[100000];
uint8_t compressBuffer[100000];

CameraSensor::CameraSensor() : m_encoder(NULL), m_tranceiver(NULL), m_pf(NULL), m_bSendImgToGC(false), m_bSetROI(false)
								, m_pRGBFile(NULL), m_pDepthFile(NULL)
{
	// TODO Auto-generated constructor stub


}

CameraSensor::~CameraSensor() {
	// TODO Auto-generated destructor stub
	if (m_pf)
	{
		fclose(m_pf);
	}



	if (m_tranceiver)
	{
		delete m_tranceiver;
	}

	if (m_encoder)
	{
		delete m_encoder;
	}




	//m_videoWriter.release();
}

bool CameraSensor::init(int type, int frameType)
{
	m_type = type;
	bool bRes = false;

	if(frameType == FRAME_SIZE320X240)
	{
		m_frameWidth = 320;
		m_frameHeight = 240;
	}
	else if (frameType == FRAME_SIZE640X480)
	{
		m_frameWidth = 640;
		m_frameHeight = 480;
	}

	 int res = m_xtionPro.initialize(type, m_frameWidth, m_frameHeight);

	 if (res == 0)
	 {
		 m_xtionPro.start(m_type);

		 if (m_type & RGB_CAMERA)
			 m_xtionPro.registerFrameCallback(this, VideoModule::eFrameType_RGB);

		 if (m_type & DEPTH_CAMERA)
			 m_xtionPro.registerFrameCallback(this, VideoModule::eFrameType_Depth);

		 bRes = true;

		 m_encoder = new MPEGLib::MPEGEncoder();
	//	 m_encoder->initialize(1, 320, 240);



		 m_tranceiver = new UDPTranceiver(8150, "192.168.0.10", 6250);


	 }
	 else
	 {
		 printf("camera initialization error..");
	 }

	int a = 0;




	 return bRes;
}

void CameraSensor::enableSendToGC()
{
	if (!m_bSendImgToGC)
	{
		m_encoder->initialize(1, 320, 240);
    	// m_pRGBFile = fopen(colorFileName.c_str(), "wb");
	 //   m_pDepthFile = fopen(depthFileName.c_str(), "wb");

		m_bSendImgToGC = true;
	}
}

void CameraSensor::disableSendToGC()
{
	if (m_bSendImgToGC)
	{

	/*	if (m_pRGBFile)
		{
			uint8_t encode[] = {0,0,1,0xb7};
			fwrite(encode, 1, sizeof(encode), m_pRGBFile);
			fclose(m_pRGBFile);
			m_pRGBFile = NULL;
		}

		if (m_pDepthFile)
		{
			fclose(m_pDepthFile);
			m_pDepthFile = NULL;
		}
*/
		m_bSendImgToGC = false;
		m_bSetROI = false;
		m_encoder->end();
	}
}

void CameraSensor::setROI(const cv::RotatedRect& rect)
{
	m_bSetROI = true;
	m_rectROI = rect;
}


void CameraSensor::onNewFrame(char* pFrame, int width, int height, int type)
{
	if (type == VideoModule::eFrameType_RGB)
	{
		m_rgbImage = cv::Mat(cv::Size(width, height), CV_8UC3, pFrame);
		flip(m_rgbImage, m_rgbImage, 1);
		cvtColor(m_rgbImage, m_bgrImage, CV_RGB2BGR);

///		m_videoWriter << m_bgrImage;

		if (m_bSetROI)
		{
			//cv::rectangle(m_rgbImage, m_rectROI, cv::Scalar(255,0,0), 3, CV_AA);
			m_bSetROI = false;
			cv::Point2f vertices[4];
			m_rectROI.points(vertices);
			for(int i = 0; i < 4; i++)
				line(m_rgbImage, vertices[i], vertices[(i+1)%4], cv::Scalar(0,255,0), 2, CV_AA);
			
		}

		int size = 0;

		if (m_bSendImgToGC)
			size = m_encoder->EncodeImage(encodedImgBuffer, (const uint8_t*)pFrame);

		if (size > 0 )
		{
				unsigned char id = 0;
				kaiMsg msg;
				msg.setBuffer(imgDataBuffer);
				msg.begin();
				msg << id;
				msg.writeData(encodedImgBuffer, size);
				msg.id(MSG_ID_NC_NOTI_CAMERA_VALUE);

			//	fwrite(encodedImgBuffer, 1, size, m_pRGBFile);

				msg.end();
				m_tranceiver->sendMessage(msg);
		}

	}
	else if (type == VideoModule::eFrameType_Depth)
	{
		m_depthImage = cv::Mat(cv::Size(width, height), CV_16UC1, pFrame);
		flip(m_depthImage, m_depthImage, 1);

	/*	if (m_bSendImgToGC)
		{
			unsigned long bufferLen = 100000;
			compress(compressBuffer + sizeof(unsigned long), &bufferLen,  (const unsigned char*)pFrame, m_frameWidth*2*m_frameHeight );
			memcpy(compressBuffer, &bufferLen, sizeof(unsigned long));

			fwrite(compressBuffer, 1, bufferLen+sizeof(unsigned long), m_pDepthFile);
		}*/
	}


}
