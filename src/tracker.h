
#ifndef _TRACKER_H_
#define _TRACKER_H_

#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

class CameraSensor;

class FeatureTracker
{
public:
	FeatureTracker();
	~FeatureTracker();

	static const int FRAMESIZE_640X480 = 1;
	static const int FRAMESIZE_320X240 = 2;

	void init(int vmin, int vmax, int smin, int frameType);

	bool setInitialROI(const Rect& rect, const Mat& img);
	bool track(const Mat& img, float& angle, Rect& targetRect, Point& center);




private:

	int m_vmin;
	int m_vmax;
	int m_smin;

	Mat m_hist;
	Rect m_trackWindow;

	int m_hsize;
	float m_hranges[2];
	const float* m_hRange;

	float m_fx, m_fy, m_cx, m_cy, m_u, m_v;

};

class MyFeatureDetector
{
public:
	MyFeatureDetector();
	~MyFeatureDetector();

	bool init(const string& cascadename);
	bool detect(vector<Rect>& ROI, const Mat& img, double scale);

private:
	CascadeClassifier m_cascade;
};


#endif


