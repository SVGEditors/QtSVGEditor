/**
*
* @license MIT
*
* @copyright: 2022 LinJi
*
* @technical support: www.svgsvg.cn
*
* @email: 93681992@qq.com
*
* @module: QtSVGEditor
*
* 版权申明:
*
* 本源码开源基于MIT协议。
*
* 该软件及其相关文档对所有人免费，可以任意处置，包括使用，复制，修改，合并，发表，分发，再授权，或者销售。
*
* 唯一的限制是需要保留我们的许可申明。
*
*/
#pragma once
#include "SVGElement.h"

class FocusPointInfo
{
public:
	CSVGPointF  point;
	QColor      clr;
	int         nIndex;
	int         nCtrlIndex;
	CSVGElement *pElement;
	FocusPointInfo()
	{
		nIndex = -1;
		nCtrlIndex = -1;
		pElement = NULL;
		clr = Qt::green;
	}
	FocusPointInfo(const CSVGPointF &pt, int ni, int nc, CSVGElement*pE, QColor c)
	{
		point = pt;
		nIndex = ni;
		nCtrlIndex = nc;
		pElement = pE;
		clr = c;
	}
};

class FocusLineInfo
{
public:
	CSVGLineF m_FocusLine;
	int       m_nLineWidth;
	QColor    m_clr;
	Qt::PenStyle m_ps;
	FocusLineInfo()
	{
		m_nLineWidth = 1;
	}
	FocusLineInfo(const CSVGLineF &line, const QColor &clr)
	{
		m_FocusLine = line;
		m_clr = clr;
		m_nLineWidth = 1;
		m_ps = Qt::DashLine;
	}
};

class FlashLineInfo
{
public:
	CSVGLineF  m_FocusLine1;
	int        m_nLineWidth1;
	QColor     m_clr1;
	CSVGLineF  m_FocusLine2;
	int        m_nLineWidth2;
	QColor     m_clr2;
	FlashLineInfo()
	{
		m_nLineWidth1 = 2;
		m_nLineWidth2 = 1;
		m_clr1 = QColor(0, 0, 0, 255);
		m_clr2 = QColor(255, 255, 255, 255);
		m_FocusLine1 = CSVGLineF(CSVGPointF(0, 0), CSVGPointF(0, 0));
		m_FocusLine2 = CSVGLineF(CSVGPointF(0, 0), CSVGPointF(0, 0));
	}
};

class CFocusForElement
{
public:
	CFocusForElement(CSVGElement *pElement, QColor clr);
	~CFocusForElement();
public:
	CSVGElement *GetElement();
	void ResetFocus(int nIndex = -1);
	QVector<FocusPointInfo>GetFocusPoints();
	QVector<FocusLineInfo> GetFocusLines();
	FlashLineInfo *GetFlashFocus();
private:
	CSVGElement    *        m_pElement;
	QColor                  m_clr;
	QVector<FocusPointInfo> m_vcFocusKeyPoints;
	QVector<FocusLineInfo>  m_vcFocusLine;
	FlashLineInfo  *        m_FlashLine;
};
