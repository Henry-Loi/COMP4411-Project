#include "CatmullromCurveEvaluator.h"
#include <assert.h>
#include <iostream>
#define SEGMENT 10



void CatmullromCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	ptvEvaluatedCurvePts.clear();
	vector<Point> ctrlPt;
	
	int iCtrlPtCount = ptvCtrlPts.size();



	//if (bWrap) {
	//	// if wrapping is on, interpolate the y value at xmin and
	//	// xmax so that the slopes of the lines adjacent to the
	//	// wraparound are equal.


	//}
	//else {
		// if wrapping is off, make the first and last segments of
		// the curve horizontal
		ctrlPt.push_back(Point(0, ptvCtrlPts[0].y));
		ctrlPt.insert(ctrlPt.begin()+1, ptvCtrlPts.begin(), ptvCtrlPts.end());
		ctrlPt.push_back(Point(fAniLength, ptvCtrlPts[iCtrlPtCount-1].y));

	//}
	
	//draw curve by points
	ptvEvaluatedCurvePts.push_back(Point(0, ctrlPt[0].y));

	for (size_t cnt = 0; cnt + 3 < ctrlPt.size(); ++cnt)
	{
		Vec4f param_x(ctrlPt[cnt].x, ctrlPt[cnt + 1].x,
			ctrlPt[cnt + 2].x, ctrlPt[cnt + 3].x);
		Vec4f param_y(ctrlPt[cnt].y, ctrlPt[cnt + 1].y,
			ctrlPt[cnt + 2].y, ctrlPt[cnt + 3].y);

		for (int i = 0; i <= SEGMENT; ++i)
		{
			double t = i / (double)SEGMENT;

			Vec4f param_time(t * t * t, t * t, t, 1);



			Point eval_point(param_time * (basis * param_x), param_time *( basis * param_y));
			// 
			// avoid wave curve occurs
			if (ptvEvaluatedCurvePts.empty() || eval_point.x > ptvEvaluatedCurvePts.back().x)
			{
				ptvEvaluatedCurvePts.push_back(eval_point);
			}
		}
	}

	ptvEvaluatedCurvePts.push_back(Point(fAniLength, ctrlPt[ctrlPt.size() - 1].y));


}
