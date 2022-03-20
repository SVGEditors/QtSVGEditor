#pragma once
#include "SVGEditBase.h"
class CSVGEditEllipse :	public CSVGEditBase
{
public:
	CSVGEditEllipse(CSVGDocument *pDocument,int nType);
	~CSVGEditEllipse();
	virtual void OnLButtonDown(const POINT& point);
	virtual void OnLButtonUp(const POINT& point);
	virtual void OnMouseMove(const POINT& point);
private:
	void EndTo(const POINT& point);
	int m_nType;
};

