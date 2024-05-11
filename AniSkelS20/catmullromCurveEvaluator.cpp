#include "CatmullromCurveEvaluator.h"
#include <assert.h>
#include <iostream>
#define SEGMENT 30
using namespace std;

Mat4f calculateBasis(Mat4f mat1,Mat4f mat2, float tension) {
	Mat4f temp = mat2;
	int tension_ind[] = { 4,6,9,11 };
	for (int i : tension_ind)
		temp[i / 4][i % 4] /= tension;
	temp = mat1 * temp;
	return temp;
}

void CatmullromCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap, const float in_tension,const int continuous) const
{
	Mat4f basis = calculateBasis(basis_orig1,basis_orig2,in_tension);
	ptvEvaluatedCurvePts.clear();
	vector<Point> ctrlPt;
	//Mat4f basis = basis_orig1 * basis_orig2;
	int iCtrlPtCount = ptvCtrlPts.size();
	int wrapInd = 2;
	float gradient = (ptvCtrlPts[0].y - ptvCtrlPts[iCtrlPtCount - 1].y) / (fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x + ptvCtrlPts[0].x);
	float y1, y2;
	if (!bWrap) {
		ctrlPt.push_back(Point(0, ptvCtrlPts[0].y));
		ctrlPt.push_back(Point(fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y));
		wrapInd = 1;
		y1 = ctrlPt[0].y;
		y2 = ctrlPt[ctrlPt.size() - 1].y;
	}
	else {
		
		ctrlPt.push_back(Point(-1 * (fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x)-1, ptvCtrlPts[iCtrlPtCount - 1].y-gradient*1));
		ctrlPt.push_back(Point(-1* (fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x), ptvCtrlPts[iCtrlPtCount - 1].y));
		ctrlPt.push_back(Point(fAniLength+ ptvCtrlPts[0].x, ptvCtrlPts[0].y));
		ctrlPt.push_back(Point(fAniLength + ptvCtrlPts[0].x+1, ptvCtrlPts[0].y+gradient*1));
		y1 = ptvCtrlPts[0].y - gradient * ptvCtrlPts[0].x;
		y2 = ptvCtrlPts[iCtrlPtCount - 1].y + gradient * (fAniLength - ptvCtrlPts[iCtrlPtCount - 1].x);
	}
	ctrlPt.insert(ctrlPt.begin() + wrapInd, ptvCtrlPts.begin(), ptvCtrlPts.end());
	cout << "size" << ctrlPt.size() << endl;
	//draw curve by points
	if(!bWrap)
	ptvEvaluatedCurvePts.push_back(Point(0, y1));
	for (size_t cnt = 0; cnt + 3 < ctrlPt.size(); ++cnt)
	{
		Vec4f param_x(ctrlPt[cnt].x, ctrlPt[cnt + 1].x,
			ctrlPt[cnt + 2].x, ctrlPt[cnt + 3].x);
		cout << "ctrlPt[cnt].x" << ctrlPt[cnt].x << " ctrlPt[cnt + 1].x" << ctrlPt[cnt + 1].x << "  " << ctrlPt[cnt + 2].x << "  " << ctrlPt[cnt + 3].x << endl;
		Vec4f param_y(ctrlPt[cnt].y, ctrlPt[cnt + 1].y,
			ctrlPt[cnt + 2].y, ctrlPt[cnt + 3].y);
		cout << "ctrlPt[cnt].y" << ctrlPt[cnt].y << " ctrlPt[cnt + 1].y" << ctrlPt[cnt + 1].y << "  " << ctrlPt[cnt + 2].y << "  " << ctrlPt[cnt + 3].y << endl;

		for (int i = 0; i <= SEGMENT; ++i)
		{
			double t = i / (double)SEGMENT;

			Vec4f param_time(t * t * t, t * t, t, 1);


			Point eval_point(param_time * (basis * param_x), param_time *( basis * param_y));
			// avoid wave curve occurs
			cout << "added" << eval_point.x << " " << eval_point.y << endl;
			if (ptvEvaluatedCurvePts.empty() || eval_point.x > ptvEvaluatedCurvePts.back().x)
			{//not working

					ptvEvaluatedCurvePts.push_back(eval_point);
					
			}
		}
	}
	if (!bWrap)
	ptvEvaluatedCurvePts.push_back(Point(fAniLength, y2));
	cout << "size2" << ptvEvaluatedCurvePts.size() << endl;
	ctrlPt.clear();

}

