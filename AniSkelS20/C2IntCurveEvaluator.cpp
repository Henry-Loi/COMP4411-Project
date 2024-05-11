#include "C2IntCurveEvaluator.h"
#include "mat.h"
#include "vec.h"
#include "modelerapp.h"

#define SAMPLE 150


void C2InterpolatingCurveEvaluator::evaluateCurve(const std::vector<Point>& controlPoints,
	std::vector<Point>& evaluatedPoints,
	const float& animationLength,
	const bool& bWrap, const float in_tension, const int continuous) const
{
	evaluatedPoints.clear();

	vector<Point> curve;
	curve.insert(curve.end(), controlPoints.begin(), controlPoints.end());
	curve.push_back(Point(curve[0].x + animationLength, curve[0].y));


	//think of each cubic segment as a hermite curve, for which we get to set the position and deriavative of the endpoints
	//solve the derivative and find D
	//input D to get the bezier control points and draw the final spline

	vector<float> D(curve.size(), 0.0);

	int N = controlPoints.size();
	if (bWrap == false)
		N = N - 1;



	//vector<vector<float>> coeff;
	//vector<float> deltaX(curve.size(), 0.0);
	//vector<float> deltaY(curve.size(), 0.0);
	//for (int i = 0; i < curve.size(); i++) {
	//	vector<float>temp(curve.size(), 0.0);
	//	if (i == 0) {
	//		temp[0] = 2.0;
	//		temp[1] = 1.0;
	//		coeff.push_back(temp);
	//		continue;
	//	}
	//	if (i + 1 == curve.size()) {
	//		temp[i-1] = 1.0;
	//		temp[i] = 2.0;
	//		coeff.push_back(temp);
	//		break;
	//	}
	//	cout << "Coeff_i:" << i << " " << curve.size() << endl;
	//	temp[i-1] = 1.0;
	//	temp[i ] = 4.0;
	//	temp[i + 1] = 1.0;
	//	coeff.push_back(temp);
	//}
	//for (int i = 0; i < curve.size(); i++) {//byrow
	//	if (i == 0) {
	//		deltaX[0] = 3*(curve[1].x - curve[0].x);
	//		deltaY[0] = 3*(curve[1].y - curve[0].y);
	//		continue;
	//	}
	//	if(i+1 == curve.size())
	//	{
	//		deltaX[i] = 3* (curve[i].x - curve[i-1].x);
	//		deltaY[i] = 3 * (curve[i].y - curve[i-1].y);
	//		continue;
	//	}
	//	cout << "i:" << i << " "<<curve.size()<<endl;
	//	deltaX[i] = 3 * (curve[i+1].x - curve[i-1].x);
	//	deltaY[i] = 3 * (curve[i+1].y - curve[i-1].y);
	//	
	//}
	//cout << "Matrix" << endl;
	//for (int k = 0; k < curve.size() ; k++)
	//{
	//	for (int j = 0; j < curve.size() ; j++) {
	//		cout << coeff[k][j]<<" ";
	//	}
	//	cout <<"i:"<<k<< endl;
	//}
	////GET UPPER TRIANGLE
	//for (int i = 1; i < curve.size(); i++) {
	//	cout << "UpperTria_i:" << i << " " << curve.size() << endl;
	//	float multiple = 0.0;
	//	multiple = coeff[i][i - 1] / coeff[i - 1][i - 1];
	//	deltaX[i] -= deltaX[i - 1] * multiple;
	//	deltaY[i] -= deltaY[i - 1] * multiple;
	//	if (i + 1 == curve.size()) {
	//		for (int j = 0; j < 2; j++) {
	//			coeff[i][i + j - 1] -= coeff[i - 1][i + j - 1] * multiple;
	//		}
	//		continue;
	//	}
	//	for (int j = 0; j < 3; j++) {
	//		coeff[i][i+j - 1] -= coeff[i - 1][i+j - 1] * multiple;
	//	}
	//}
	//cout << "Matrix" << endl;
	//for (int k = 0; k < curve.size(); k++)
	//{
	//	for (int j = 0; j < curve.size(); j++) {
	//		cout << coeff[k][j] << " ";
	//	}
	//	cout << "i:" << k << endl;
	//}


	//for (int i = curve.size() - 1; i >= 0; i--) {
	//	if (i == curve.size() - 1) {
	//		deltaX[i] = deltaX[i] / coeff[i][i];
	//		deltaY[i] = deltaY[i] / coeff[i][i];
	//		continue;
	//	}
	//	cout << "Cut_i:" << i << " " << curve.size() << endl;
	//	deltaX[i] = (deltaX[i] - coeff[i][i+1]*deltaX[i + 1]) / coeff[i][i];
	//	deltaY[i] = (deltaX[i] - coeff[i][i + 1] * deltaY[i + 1]) / coeff[i][i];
	//}
	//for (int i = 0; i < curve.size()-1; i++)
	//{
	//	Mat4d M
	//	(2.0, -2.0, 1.0, 1.0,
	//		-3.0, 3.0, -2.0, -1.0,
	//		0.0, 0.0, 1.0, 0.0,
	//		1.0, 0.0, 0.0, 0.0);

	//	Vec4d vecX = M * Vec4d(
	//		curve[i].x,
	//		curve[i + 1].x,
	//		deltaX[i],
	//		deltaX[i + 1]
	//	);
	//	Vec4d vecY = M * Vec4d(
	//		curve[i].y,
	//		curve[i + 1].y,
	//		deltaY[i],
	//		deltaY[i + 1]
	//	);

	//	for (int j = 0; j < SAMPLE; ++j)
	//	{
	//		double t = j / (double)SAMPLE;
	//		Vec4d param_time(t * t * t, t * t, t, 1);

	//		Point evalPoint(param_time * (M * vecX), param_time * (M * vecY));
	//		//double interval = j / (double)SAMPLE;
	//		//Vec4d T(interval * interval * interval, interval * interval, interval, 1);
	//		//float length_x = curve[i + 1].x - curve[i].x;
	//		//if (evalPoint.x < 0)
	//		//	evalPoint.x +=animationLength;
	//		evaluatedPoints.push_back(evalPoint);
	//	}
	//}




	vector<float> coef(curve.size(), 0.0);
	vector<float> delta(curve.size(), 0.0);

	//CONSTRUCT COEFFICIENT




	coef[0] = 0.5f;

	for (int i = 1; i < N; ++i)
		coef[i] = 1.0f / (4.0f - coef[i - 1]);

	coef[N] = 1.0 / (2.0f - coef[N - 1 - 1]);

	//CONSTRUCT PI-PI-1
	delta[0] = 1.5 * (curve[1].y - curve[0].y);
	for (int i = 1; i < N; i++)
		delta[i] = coef[i] * (3 * (curve[i + 1].y - curve[i - 1].y) - delta[i - 1]);

	delta[N] = coef[N] * (3 * (curve[N].y - curve[N - 1].y) - delta[N]);

	//CONSTRUXT D0-DM
	D[N] = delta[N];
	for (int i = N - 1; i >= 0; i--)
		D[i] = delta[i] - coef[i] * D[i + 1];


	for (int i = 0; i < N; i++)
	{
		Mat4d M
		(2.0, -2.0, 1.0, 1.0,
			-3.0, 3.0, -2.0, -1.0,
			0.0, 0.0, 1.0, 0.0,
			1.0, 0.0, 0.0, 0.0);

		Vec4d vec = M * Vec4d(
			curve[i].y,
			curve[i + 1].y,
			D[i],
			D[i + 1]
		);

		for (int j = 0; j < SAMPLE; ++j)
		{
			double interval = j / (double)SAMPLE;
			Vec4d T(interval * interval * interval, interval * interval, interval, 1);
			float length_x = curve[i + 1].x - curve[i].x;
			if (length_x < 0)
				length_x = length_x + animationLength;
			float py = T * vec;
			float px = curve[i].x + interval * length_x;
			px = fmod(px, animationLength);
			evaluatedPoints.push_back(Point(px, py));
		}
	}


	if (bWrap == false)
	{
		evaluatedPoints.push_back(Point(0, controlPoints[0].y));
		evaluatedPoints.push_back(Point(animationLength, controlPoints[controlPoints.size()-1].y));
	}
}