#include "bezierCurveEvaluator.h"
#include <assert.h>
#include <iostream>
#define SEGMENT 30



void BezierCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap, const float in_tension, const int continuous) const
{
	ptvEvaluatedCurvePts.clear();
	vector<Point> ctrlPt;
	vector<Point> tempEvalPt;
	int iCtrlPtCount = ptvCtrlPts.size();
	int wrapInd = 0;
	float gradient = (ptvCtrlPts[0].y - ptvCtrlPts[iCtrlPtCount - 1].y) / (fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x + ptvCtrlPts[0].x);
	float y1, y2;
	if (!bWrap) {

		wrapInd = 0;
		y1 = ptvCtrlPts[0].y;
		y2 = ptvCtrlPts[ptvCtrlPts.size() - 1].y;
	}
	else {

		ctrlPt.push_back(Point(fAniLength + ptvCtrlPts[0].x, ptvCtrlPts[0].y));
	}
	ctrlPt.insert(ctrlPt.begin() + wrapInd, ptvCtrlPts.begin(), ptvCtrlPts.end());

	//draw curve by points
	if (!bWrap) {
		ptvEvaluatedCurvePts.push_back(Point(0, y1));

	}
	for (size_t cnt = 0; cnt + 3 < ctrlPt.size(); cnt+=3)
	{
		Vec4f param_x(ctrlPt[cnt].x, ctrlPt[cnt + 1].x,
			ctrlPt[cnt + 2].x, ctrlPt[cnt + 3].x);
		Vec4f param_y(ctrlPt[cnt].y, ctrlPt[cnt + 1].y,
			ctrlPt[cnt + 2].y, ctrlPt[cnt + 3].y);
		cout << "ctrlPt[cnt].x" << ctrlPt[cnt].x << " ctrlPt[cnt + 1].x" << ctrlPt[cnt + 1].x << "  " << ctrlPt[cnt + 2].x << "  " << ctrlPt[cnt + 3].x << endl;
		for (int i = 0; i <= SEGMENT; ++i)
		{
			double t = i / (double)SEGMENT;
			double wrap_X = 0;
			int wrap_count = 0;
			Vec4f param_time(t * t * t, t * t, t, 1);
			Point eval_point(param_time * (basis * param_x), param_time * (basis * param_y));
			// avoid wave curve occurs
			if (ptvEvaluatedCurvePts.empty() || eval_point.x > (ptvEvaluatedCurvePts.back().x))
			{
				if (float(eval_point.x) >= -0.5 && (float)eval_point.x <= fAniLength + 0.5)//not working
					ptvEvaluatedCurvePts.push_back(eval_point);
			}
			if (eval_point.x > fAniLength && eval_point.x>fAniLength+wrap_X)
			{
				ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin()+wrap_count, Point(eval_point.x-fAniLength,eval_point.y));
				++wrap_count;
			}
		}
	}
	if((ctrlPt.size()-1)%3>0)
		for (size_t cnt = 0; cnt < (ctrlPt.size()-1) % 3; cnt++) {
			Point eval_point(ctrlPt[(ctrlPt.size()-1) / 3 * 3 + cnt+1].x, ctrlPt[(ctrlPt.size()-1) / 3 * 3 + cnt+1].y);
			float gradient = (ptvEvaluatedCurvePts[0].y - ptvEvaluatedCurvePts[ptvEvaluatedCurvePts.size()-1].y) / (fAniLength - ptvEvaluatedCurvePts[ptvEvaluatedCurvePts.size() - 1].x + ptvEvaluatedCurvePts[0].x);
			if (ptvEvaluatedCurvePts.empty() || eval_point.x > (ptvEvaluatedCurvePts.back().x))
			{
				if (float(eval_point.x) >= -0.5 && (float)eval_point.x <= fAniLength + 0.5)//not working
					ptvEvaluatedCurvePts.push_back(eval_point);
			}
			if (eval_point.x > fAniLength)
			{
				
				ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin() , Point(0, ptvEvaluatedCurvePts[0].y - gradient * ptvEvaluatedCurvePts[0].x));
				ptvEvaluatedCurvePts.push_back(Point(fAniLength, ptvEvaluatedCurvePts[ptvEvaluatedCurvePts.size() - 1].y + gradient * (fAniLength - ptvEvaluatedCurvePts[ptvEvaluatedCurvePts.size() - 1].x)));
				cout << "added front" <<ctrlPt.size()<< endl;

			}
		}
			
	if (!bWrap) {

		ptvEvaluatedCurvePts.push_back(Point(fAniLength, y2));

	}
	//ptvEvaluatedCurvePts.insert(ptvEvaluatedCurvePts.begin()+wrapInd-2, tempEvalPt.begin(), tempEvalPt.end());

}
