#pragma once
#include "SVGEditBase.h"
class CSVGEditMove : public CSVGEditBase
{
public:
	CSVGEditMove(CSVGDocument *pDocument);
	~CSVGEditMove();
	virtual void OnLButtonDown(const POINT &point);
	virtual void OnMButtonDown(const POINT &point);
	virtual void OnLButtonUp(const POINT &point);
	virtual void OnMouseMove(const POINT &point);
	virtual void OnRButtonUp(const POINT &point);
};

