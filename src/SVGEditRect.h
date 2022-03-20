#pragma once
#include "SVGEditBase.h"
class CSVGEditRect : public CSVGEditBase
{
public:
	CSVGEditRect(CSVGDocument *pDocument);
	~CSVGEditRect();
	virtual void OnLButtonDown(const POINT& point);
	virtual void OnLButtonUp(const POINT& point);
	virtual void OnMouseMove(const POINT& point);
private:
	void EndTo(const POINT& point);
};

