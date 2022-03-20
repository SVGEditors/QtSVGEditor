#pragma once
#include "BasetypeDef.h"
#include "SVGLine.h"

class CSVGElementShape;
class CSVGFocusShape
{
public:
	CSVGFocusShape(CSVGElementShape *pShape);
	~CSVGFocusShape();
	void SetFillColor(COLORREF clr);
	void SetStrokeColor(COLORREF clr);
	void SetStrokeWidth(float fwidth);
	void DrawShape(Graphics *pGraphic,const CSVGMatrix *screenmat);
	void AddPoint(float x,float y);
	void AddRoteateHandle(float x, float y);
	void AddLine(float x1,float y1,float x2,float y2);
	void Clear();
	CSVGRect GetBBox();
public:
	COLORREF      m_strokeclr;	 //������ɫ
	COLORREF      m_fillclr;	 //�����ɫ
	COLORREF      m_frameclr;	 //�߿���ɫ
	float  	      m_strokewidth; //�߿�
	std::vector<CSVGPoint> m_vcPoint;
	std::vector<CSVGLine>  m_vcLine;
	CSVGPoint   *m_pRotateHandle;
private:
	void resetPath(const CSVGMatrix * mat);
	GraphicsPath      m_Linepath;
	GraphicsPath      m_Pointpath;
	CSVGElementShape *m_pShape;
};

