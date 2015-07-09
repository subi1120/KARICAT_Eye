
#include "RANSAC_Tracker.h"


#define Z_MIN		 700		// 50cm
#define Z_MAX		 2500		// 5m

#define C_x 322.34 / 2  // principal point x
#define C_y 241.71 / 2  // principal point y 
#define f_x 534.97 / 2  // focal length x
#define f_y 535.40 / 2  // focal length y 

int RotationMat[3][3] = { 1, 0, 0, 0, 0, 1, 0, -1, 0 };


struct sElement {
	short	x, y, z;	// 측정값의 x,y,z 위치, 단위: cm
	//unsigned short	r;			// range points
};

bool IsNormalVectorVertical(sElement &xl, sElement &xr, sElement &yl, sElement&yr)
{
	// Cross product 연산 수행:
	// (x3, y3, z3) = Cross (xl-xr, yl-yr);

	double x1 = xl.x - xr.x;
	double y1 = xl.y - xr.y;
	double z1 = xl.z - xr.z;

	double x2 = yl.x - yr.x;
	double y2 = yl.y - yr.y;
	double z2 = yl.z - yr.z;

	double x3 = y1*z2 - z1*y2;
	double y3 = z1*x2 - x1*z2;
	double z3 = x1*y2 - y1*x2;

	// x, y 방향에 비해 z방향의 크기가 5배 이상 큰지 비교
	return fabs(z3) > 5 * sqrt(x3*x3 + y3*y3);
}



sElement pcd[320][240] = {};
int width = 320;
int height = 240;

sPoint *points = NULL;

RANSAC_Tracker::RANSAC_Tracker()
{
}


RANSAC_Tracker::~RANSAC_Tracker()
{
	if (points)
		free(points);

	cvReleaseImage(&m_dimg);

}

void RANSAC_Tracker::init(int width, int height)
{
	Size size(width, height);


	m_dimg = cvCreateImage(size, IPL_DEPTH_8U, 1);
	points = (sPoint*)malloc(sizeof(sPoint)*width*height);


}

void RANSAC_Tracker::track(const Mat& depthMat)
{



	const Mat& copyDepth = depthMat;
	/*depthMat.copyTo(copyDepth);

	flip(copyDepth, copyDepth, 1);*/

	cv::Size depthSize = depthMat.size();
	int pixelNum = depthSize.width*depthSize.height;

	unsigned short* depthData = (unsigned short*)(depthMat.data);

	int width = depthSize.width;
	int height = depthSize.height;

	for (int i = 0; i < depthSize.width; i++)
	{
		for (int j = 0; j < depthSize.height; j++)
		{
			pcd[i][j].x = (i + 1 - C_x)*copyDepth.at<unsigned short>(j, i) / f_x;
			pcd[i][j].y = (j + 1 - C_y)*copyDepth.at<unsigned short>(j, i) / f_y;
			pcd[i][j].z = copyDepth.at<unsigned short>(j, i);
		}
	}

	int point_count = 0;
	

	//for (int i = 1; i < 320 - 1; i++){
	//for (int j = 1; j < 240 - 1; j++){
	for (int i = 1; i < width - 1; i++){
		for (int j = 1; j < height - 1; j++){
			sElement &pt = pcd[i][j];
			if (Z_MIN <= pt.z && pt.z <= Z_MAX) {
				if (rand() % 10 == 0) { // 모든 점을 처리하지 않고 10개 중 1개 정도의 확률로 후보군들을 선택한다.
					//if (IsNormalVectorVertical(pcd[i + 1][j], pcd[i - 1][j], pcd[i][j + 1], pcd[i][j - 1])) {
					points[point_count++] = sPoint(pt.x, pt.y, pt.z);
					//}
				}
			}
		}
	}

	// RANSAC 알고리즘으로 지면과 가장 부합하는 평면을 찾는다.
	sPlane model;
	double threshold = 30;	// cm
	double ret = ransac_plane_fitting(points, point_count, model, threshold);


	//IplImage* r_dimg = cvCreateImage(size, IPL_DEPTH_8U, 1);
	cvSet(m_dimg, cvScalar(0));



	//for (int i = 0; i < 320; i++) {
	//for (int j = 0; j < 240; j++) {
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			sElement &pt = pcd[i][j];
			if (Z_MIN <= pt.z && pt.z <= Z_MAX) {

				// 거리 r로부터 색을 결정: 센서에서 멀리 있을 수록 어둡다.
				//unsigned short v = pt.z;
				int v = 255 - (int)(255 * ((double)pt.z / Z_MAX));
				v = std::max(0, v);
				v = std::min(255, v);
				
				sPoint temp(pt.x, pt.y, pt.z);
				double l = model.length(temp);
				if (l < threshold) continue;

				CvScalar pixel;

				pixel.val[0] = v;
				//zero.val[0] = 0;

				cvSet2D(m_dimg, j, i, pixel);
				//cvSet2D(r_dimg, j, i, zero);

			}
		}
	}


	vector<Vec4i> hierarchy;

	m_cntrDepth = cvarrToMat(m_dimg);
	//Mat resDepth = cvarrToMat(r_dimg);


	// Create a structuring element
	int erosion_size = 3;
	Mat element = getStructuringElement(cv::MORPH_CROSS, cv::Size(2 * erosion_size + 1, 2 * erosion_size + 1), cv::Point(erosion_size, erosion_size));

	// Apply erosion or dilation on the image
	erode(m_cntrDepth, m_cntrDepth, element);
	//dilate(cntrDepth, cntrDepth,element);

	m_contours.clear();
	findContours(m_cntrDepth, m_contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);


	for (unsigned int i = 0; i < m_contours.size(); i++)
	{
		double a = contourArea(m_contours[i], false);

		vector<Point> points;

		int min_area = 100;

		//if (a < min_area)
		//contours.erase(contours.begin() + i);


		if (a > min_area) // For 320x240 --> 200
		{
			drawContours(m_cntrDepth, m_contours, i, 255, 2, 3);


			Mat_<uchar>::iterator it = m_cntrDepth.begin<uchar>();
			Mat_<uchar>::iterator end = m_cntrDepth.end<uchar>();

			for (; it != end; ++it)
			if (*it)
				points.push_back(it.pos());

			m_bbox = minAreaRect(Mat(points));
		}
	}


//		Point2f vertices[4];
//		m_bbox.points(vertices);

	//	for (int i = 0; i < 4; ++i)
	//		line(m_cntrDepth, vertices[i], vertices[(i + 1) % 4], 255, 2, CV_AA);


		float Target_y = m_bbox.center.y - m_bbox.size.height / 3;
		float Target_x = m_bbox.center.x;

		//if (bbox.size.area() > 1000)


		if ((m_bbox.size.area() - m_cbbox.size.area()) < 500)
		{
				//bbox = cbbox;
			//	for (int i = 0; i < 4; ++i)
			//		line(copyImage, vertices[i], vertices[(i + 1) % 4], Scalar(0, 255, 0), 2, CV_AA);

			Target_y = m_bbox.center.y - m_bbox.size.height / 3;
			Target_x = m_bbox.center.x;

			//	circle(copyImage, Point(Target_x, Target_y), 1, Scalar(0, 255, 0), 1, 8);

		}
		else{
			Rect ccbbox = cvRect(m_cbbox.center.x - m_cbbox.size.width / 2, m_cbbox.center.y - m_cbbox.size.height / 2, m_cbbox.size.width, m_cbbox.size.height);
		//	rectangle(copyImage, ccbbox, Scalar(0, 0, 255), 2, CV_AA);
			//	circle(copyImage, Point(Target_x, Target_y), 1, Scalar(0, 0, 255), 1, 8);

		}
		m_cbbox = m_bbox;
	

		float u = (Target_x - C_x) / f_x;
		float v = (Target_y - C_y) / f_y;

		float camvec[1][3] = { u, v, 1 };

		float xw = RotationMat[0][0] * camvec[0][0] + RotationMat[0][1] * camvec[0][1] + RotationMat[0][2] * camvec[0][2];
		float yw = RotationMat[1][0] * camvec[0][0] + RotationMat[1][1] * camvec[0][1] + RotationMat[1][2] * camvec[0][2];
		float zw = RotationMat[2][0] * camvec[0][0] + RotationMat[2][1] * camvec[0][1] + RotationMat[2][2] * camvec[0][2];

		m_yawAngle = -atan2(xw, yw) * 180 / 3.141592;

		m_distance = copyDepth.at<unsigned short>(Point(Target_x, Target_y));

}


 Mat& RANSAC_Tracker::getFilteredFeature() 
{
	return m_cntrDepth;
}


RotatedRect& RANSAC_Tracker::getBoundingBox() 
{
	return m_bbox;
}

void RANSAC_Tracker::getTargetVector(unsigned short& distance, float& yawAngle) 
{
	distance = m_distance;
	yawAngle = m_yawAngle;
}



