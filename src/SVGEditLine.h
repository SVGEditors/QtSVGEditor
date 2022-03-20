#pragma once
#include "SVGEditBase.h"
class CSVGEditLine : public CSVGEditBase
{
public:
	CSVGEditLine(CSVGDocument *pDocument);
	~CSVGEditLine();
	virtual void OnLButtonDown(const POINT& point);
	virtual void OnLButtonUp(const POINT& point);
	virtual void OnMouseMove(const POINT& point);
private:
	void EndTo(const POINT& point);
	CSVGPoint m_pt1;
};

