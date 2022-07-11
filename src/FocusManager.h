/**
*
* @license MIT
*
* @copyright: 2022 LJ
*
* @technical support: http://www.svgsvg.cn/support/tech/
*

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
#include "FocusForElement.h"

class CFocusManager
{
public:
	CFocusManager();
	~CFocusManager();
	CFocusForElement* AddElement(CSVGElement *pElement, QColor clr);
	CFocusForElement* ResetFocus(CSVGElement *pElement, int nIndex = -1);
	void ResetAllFocus();
	void RemoveElement(CSVGElement *pElement);
	void ClearAll();
	QVector<CFocusForElement*> GetFocusElement();
	FocusPointInfo GetElementFocusPointInfo(CSVGElement *pElement, const CSVGPointF &pt);
	FlashLineInfo *GetFlashFocus();
private:
	QVector<CFocusForElement*> m_vcFocusElement;
};
