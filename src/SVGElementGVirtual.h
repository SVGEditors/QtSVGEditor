#pragma once
//虚拟组合图形,用于在编辑时多选时可以同时缩放选中的图形
#include "SVGElementShape.h"
class CSVGElementGVirtual : public CSVGElementShape
{
public:
	CSVGElementGVirtual(CSVGDocument* ownerDoc);
	virtual ~CSVGElementGVirtual();
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

