#pragma once
#include "SVGElementShape.h"
class CSVGElementGTemp : public CSVGElementShape
{
public:
	CSVGElementGTemp(CSVGDocument* ownerDoc);
	virtual ~CSVGElementGTemp();
	virtual void  calBBox();
	virtual bool  IsPtInShape(float fx, int fy);
	virtual bool  IsShapeInRect(CSVGRect rectf);
	virtual void  Move(int deltax, int deltay, const CSVGMatrix *screenmat);
	virtual void  SizeTo(int nptIndex, int nx, int ny, const CSVGMatrix *screenmat);
	virtual void  Rotate(float angle, float cx, float cy);
	void          addChildOnly(CSVGElement *pChild);
	void          removeChildOnly(CSVGElement *pChild);
	void          removeAllChildOnly();
	void          matrixToChild();
private:
	CSVGMatrix m_lstMat;
};

