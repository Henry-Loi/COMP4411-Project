#ifndef INCLUDED_CURVE_EVALUATOR_H
#define INCLUDED_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)

#include "Curve.h"

//using namespace std;

class CurveEvaluator
{
public:
	virtual ~CurveEvaluator(void);
	virtual void evaluateCurve(const std::vector<Point>& ptvCtrlPts,
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap, const float in_tension, const int continuous) const = 0;
	static float s_fFlatnessEpsilon;
	static int s_iSegCount;
	float tension = 0.0;
};


#endif