#ifndef INCLUDED_CATMULLROM_CURVE_EVALUATOR_H
#define INCLUDED_CATMULLROM_CURVE_EVALUATOR_H


#pragma warning(disable : 4786)  

#include "CurveEvaluator.h"

using namespace std;

class CatmullromCurveEvaluator : public CurveEvaluator
{
public:
	CatmullromCurveEvaluator() {
		tension = 2.0;
	}
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;
	Mat4f basis = Mat4f(
		-1.0, 3.0, -3.0, 1.0,
		2.0, -5.0, 4.0, -1.0,
		-1.0, 0.0, 1.0, 0.0,
		0.0, 2.0, 0.0, 0.0) / 2.0;
};

#endif