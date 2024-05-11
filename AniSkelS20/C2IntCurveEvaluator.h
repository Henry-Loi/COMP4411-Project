#pragma once
#ifndef _C2INTERPOLATING_H_
#define _C2INTERPOLATING_H_
#include "curveevaluator.h"

class C2InterpolatingCurveEvaluator : public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& controlPoints,
		std::vector<Point>& evaluatedPoints,
		const float& animationLength,
		const bool& bWrap
		, const float in_tension, const int continuous) const;

};

#endif