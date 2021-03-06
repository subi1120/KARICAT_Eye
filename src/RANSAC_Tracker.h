#ifndef _RANSAC_TRACKER_H_
#define _RANSAC_TRACKER_H_

#include <vector>
#include <opencv2/opencv.hpp>
#include "RANSAC_plane.h"


using namespace std;

using namespace cv;

class RANSAC_Tracker
{
public:
	RANSAC_Tracker();
	~RANSAC_Tracker();

	void init(int width, int height);
	void track(const Mat& depthMat);

	Mat& getFilteredFeature() ;
	RotatedRect& getBoundingBox() ;
	void getTargetVector(unsigned short& distance, float& yawAngle) ;

private:

	IplImage* m_dimg;
	vector<vector<Point> > m_contours;

	RotatedRect m_bbox, m_cbbox;

	Mat m_cntrDepth;

	float m_yawAngle;
	unsigned short m_distance;


};

#endif
