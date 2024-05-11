#ifndef INCLUDED_BEZIER_CURVE_EVALUATOR_H
#define INCLUDED_BEZIER_CURVE_EVALUATOR_H


#pragma warning(disable : 4786)  

#include "CurveEvaluator.h"

using namespace std;

class BezierCurveEvaluator : public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap, const float in_tension, const int continuous) const;
	Mat4f basis = Mat4f(
		-1.0, 3.0, -3.0, 1.0,
		3.0, -6.0, 3.0, 0.0,
		-3.0, 3.0, 0.0, 0.0,
		1.0, 0.0, 0.0, 0.0);
};

#endif