#pragma once
#include "SVGEditBase.h"
class CSVGEditZoom : public CSVGEditBase
{
public:
	CSVGEditZoom(CSVGDocument *pDocument, int nType);
	~CSVGEditZoom();
	virtual void OnLButtonDown(const POINT& point);
	virtual void OnLButtonUp(const POINT& point);
	virtual void OnRButtonDown(const POINT &point);
	virtual void OnRButtonUp(const POINT &point);
	virtual void OnMouseMove(const POINT &point);
private:
	int m_nType;
};

