//
// BluringBrush.h
//
// The header file for Blurring Brush.
//

#ifndef BLURINGBRUSH_H
#define BLURINGBRUSH_H

#include "ImpBrush.h"

class BluringBrush : public ImpBrush {
public:
	BluringBrush(ImpressionistDoc* pDoc = NULL, char* name = NULL);

	void BrushBegin(const Point source, const Point target);
	void BrushMove(const Point source, const Point target);
	void BrushEnd(const Point source, const Point target);
	char* BrushName(void);

	int BrushInit(void*) { return 1; }

	//void EdgeClipMove(const Point source, const Point target,const int size,const int angle);
};

#endif