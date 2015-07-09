#include <stdlib.h>
#include "RANSAC_plane.h"
#include "MatrixAlgebra.h"

bool find_in_samples (sPoint *samples, int no_samples, sPoint *data)
{
	for (int i=0; i<no_samples; ++i) {
		if (samples[i].x == data->x && samples[i].y == data->y && samples[i].z == data->z) {
			return true;
		}
	}
	return false;
}



void get_samples (sPoint *samples, int no_samples, sPoint *data, int no_data)
{
	// 데이터에서 중복되지 않게 N개의 무작위 셈플을 채취한다.

	for (int i=0; i<no_samples; ) {
		int j = rand()%no_data;
		
		if (!find_in_samples(samples, i, &data[j])) {
			samples[i] = data[j];
			++i;
		}
	}
}

bool compute_model_parameter(sPoint samples[], int no_samples, sPlane &model)
{
	if (no_samples < 3) {
		return false;
	}
	else if (no_samples==3) {
		model.plane (samples[0], samples[1], samples[2]);
		return true;
	}
	else {
		// 점이 4개 이상일 때는 least square 방법으로 평면의 방정식을 찾는다.
		// 평면의 방정식이 ax + bx + cz + d = 0 형태이기 때문에
		// (x1, y1, z1), (x2, y2, z2), (x3, y3, z3), ... 점들을 대입하여 행렬 형태로 정리하면 다음과 같은 모양이 된다:
		//   [ x1 y1 z1  1]    [ a ]   [ 0   ] 
		//   [ x2 y2 z2  1] *  [ b ] = [ 0   ]
		//   [ x3 y3 z3  1]    [ c ]   [ 0   ]
		//   [ ...       1]    [ d ]   [ ... ]

		dMatrix A(no_samples, 4);

		for (int i=0; i<no_samples; i++) {
			A(i, 0) = samples[i].x;
			A(i, 1) = samples[i].y;
			A(i, 2) = samples[i].z;
			A(i, 3) = 1.;
		}

		// 동차방정식 Ax=0 형태의 해를 SVD로 구한다.
		dMatrix V;
		dVector D;
		A.svd(V, D);

		// 여기서 해는 V의 마지막 열 벡터가 된다.
		model.a = V(0,3);
		model.b = V(1,3);
		model.c = V(2,3);
		model.d = V(3,3);
		return true;
	}
}




double model_verification (sPoint *inliers, int *no_inliers, sPlane &estimated_model, sPoint *data, int no_data, double distance_threshold)
{
	*no_inliers = 0;

	double cost = 0.;

	for(int i=0; i<no_data; i++){
		// 직선에 내린 수선의 길이를 계산한다.
		double distance = estimated_model.length (data[i]);
	
		// 예측된 모델에서 유효한 데이터인 경우, 유효한 데이터 집합에 더한다.
		if (distance < distance_threshold) {
			cost += 1.;

			inliers[*no_inliers] = data[i];
			++(*no_inliers);
		}
	}
	return cost;
}

double ransac_plane_fitting (sPoint *data, int no_data, sPlane &model, double distance_threshold)
{
	const int no_samples = 3;

	if (no_data < no_samples) {
		return 0.;
	}

	sPoint *samples = new sPoint[no_samples];

	int no_inliers = 0;
	sPoint *inliers = new sPoint[no_data];

	sPlane estimated_model;
	double max_cost = 0.;

	int max_iteration = (int)(1 + log(1. - 0.99)/log(1. - pow(0.5, no_samples)));

	for (int i = 0; i<max_iteration; i++) {
		// 1. hypothesis

		// 원본 데이터에서 임의로 N개의 셈플 데이터를 고른다.
		get_samples (samples, no_samples, data, no_data);

		// 이 데이터를 정상적인 데이터로 보고 모델 파라메터를 예측한다.
		compute_model_parameter (samples, no_samples, estimated_model);
		// if (!estimated_model.convert_std_form ()) { --i; continue; }

		// 2. Verification

		// 원본 데이터가 예측된 모델에 잘 맞는지 검사한다.
		double cost = model_verification (inliers, &no_inliers, estimated_model, data, no_data, distance_threshold);

		// 만일 예측된 모델이 잘 맞는다면, 이 모델에 대한 유효한 데이터로 새로운 모델을 구한다.
		if (max_cost < cost) {
			max_cost = cost;
	
			compute_model_parameter (inliers, no_inliers, model);
			// model.convert_std_form ();
		}
	}
	
	delete [] samples;
	delete [] inliers;

	return max_cost;
}
