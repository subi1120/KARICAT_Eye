
#ifndef _RANSAC_PLANE_H_
#define _RANSAC_PLANE_H_

#include <math.h>

struct sPoint {
	double x, y, z;

	sPoint (double x_=0, double y_=0, double z_=0) : x(x_), y(y_), z(z_) { }  
};

struct sPlane {
	double a, b, c, d;	
	void plane (sPoint &p0, sPoint &p1, sPoint &p2)
	{
		
		double x1 = p1.x - p0.x;
		double y1 = p1.y - p0.y;
		double z1 = p1.z - p0.z;

		double x2 = p2.x - p0.x;
		double y2 = p2.y - p0.y;
		double z2 = p2.z - p0.z;

		double x3 = y1*z2 - z1*y2;
		double y3 = z1*x2 - x1*z2;
		double z3 = x1*y2 - y1*x2;

		double l = sqrt(x3*x3 + y3*y3 + z3*z3);

		a = x3/l;
		b = y3/l;
		c = z3/l;
		d = -(a*p0.x + b*p0.y + c*p0.z);
	}

	inline double length (sPoint &pt)
	{
		
		double k = (a*pt.x + b*pt.y + c*pt.z + d)/(a*a + b*b + c*c);
		double ka = k*a, kb = k*b, kc = k*c;
		return sqrt (ka*ka + kb*kb + kc*kc);
	}
};

extern double compute_distance (sPlane &plane, sPoint &point);
extern double ransac_plane_fitting (sPoint *data, int no_data, sPlane &model, double distance_threshold);

#endif
