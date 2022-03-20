#pragma once
#include "ExportMarco.h"
#include "SVGToolShape.h"
class SVGCONTROL_API CSVGToolSelect : public CSVGToolShape
{
public:
	CSVGToolSelect(CSVGDocument *pDocument);
	~CSVGToolSelect();
	virtual unsigned short GetToolType() { return TOOLTYPE_SELECT; };
	virtual void OnLButtonDown(const POINT &point);
	virtual void OnLButtonUp(const POINT &point);
	virtual void OnMouseMove(const POINT &point);
	virtual void OnLButtonDblClk(const POINT& point);
	void SetmouseType(unsigned short type);//0����仯,1��������
private:
	void  ShowHandleCursor();
	mousetype    m_mousetype;
	SelectMode   m_selectMode;
	bool         m_bCanMove;
	POINT        m_ptDown;
	int          m_nHandle;//������갴��ʱѡ�е��ĸ��ǵ�
	CSVGElementShape *m_pCurrentShape;
};


