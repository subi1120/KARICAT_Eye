/*
 * CameraSensor.h
 *
 *  Created on: Nov 6, 2014
 *      Author: turtlebot
 */

#ifndef CAMERASENSOR_H_
#define CAMERASENSOR_H_

#include "XtionProCamera.h"
#include <opencv2/opencv.hpp>

namespace MPEGLib
{
	class MPEGEncoder;
}
class UDPTranceiver;

class CameraSensor : public VideoModule::FrameCallback
{
public:
	CameraSensor();
	virtual ~CameraSensor();

	static const int RGB_CAMERA = 4;
	static const int DEPTH_CAMERA = 1;
	static const int FRAME_SIZE320X240 = 1;
	static const int FRAME_SIZE640X480 = 2;


	virtual void onNewFrame(char* pFrame, int width, int height, int type);
	bool init(int type, int frameType);

	const cv::Mat& getRGBImage() const { return m_rgbImage; }
	const cv::Mat& getDepthImage() const {return m_depthImage; }
	const cv::Mat& getBGRImage() const { return m_bgrImage; }

	void setROI(const cv::RotatedRect& rect);

	void enableSendToGC();
	void disableSendToGC();

private:

	XtionProCamera m_xtionPro;
	int m_frameWidth , m_frameHeight;
	int m_type;

	cv::Mat m_rgbImage;
	cv::Mat m_bgrImage;
	cv::Mat m_depthImage;

	FILE* 					m_pRGBFile;
	FILE* 					m_pDepthFile;

	MPEGLib::MPEGEncoder*	m_encoder;
	UDPTranceiver* 			m_tranceiver;
	FILE* 					m_pf;
	bool					m_bSetROI;
	cv::RotatedRect				m_rectROI;

	bool m_bSendImgToGC;




};

#endif /* CAMERASENSOR_H_ */
