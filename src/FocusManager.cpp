
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
#include "FocusManager.h"



CFocusManager::CFocusManager()
{
}


CFocusManager::~CFocusManager()
{
	ClearAll();
}

CFocusForElement* CFocusManager::AddElement(CSVGElement *pElement, QColor clr)
{
	for (int i = 0; i < m_vcFocusElement.size(); ++i)
	{
		if (m_vcFocusElement[i]->GetElement() == pElement)
		{
			m_vcFocusElement[i]->ResetFocus();
			return m_vcFocusElement[i];
		}
	}
	CFocusForElement *pFocusElement = new CFocusForElement(pElement, clr);
	m_vcFocusElement.push_back(pFocusElement);
	pFocusElement->ResetFocus();
	return pFocusElement;
}

CFocusForElement* CFocusManager::ResetFocus(CSVGElement *pElement, int nIndex)
{
	for (int i = 0; i < m_vcFocusElement.size(); ++i)
	{
		if (m_vcFocusElement[i]->GetElement() == pElement)
		{
			m_vcFocusElement[i]->ResetFocus(nIndex);
			return m_vcFocusElement[i];
		}
	}
	CFocusForElement *pFocusElement = new CFocusForElement(pElement, Qt::green);
	m_vcFocusElement.push_back(pFocusElement);
	pFocusElement->ResetFocus(nIndex);
	return pFocusElement;
}

void CFocusManager::ResetAllFocus()
{
	for (int i = 0; i < m_vcFocusElement.size(); ++i)
	{
		m_vcFocusElement[i]->ResetFocus();
	}
}

void CFocusManager::RemoveElement(CSVGElement *pElement)
{
	if (!pElement)
		return;
	for (int i = 0; i < m_vcFocusElement.size(); ++i)
	{
		if (m_vcFocusElement[i]->GetElement() == pElement)
		{
			delete m_vcFocusElement[i];
			m_vcFocusElement.remove(i);
			break;
		}
	}
}

void CFocusManager::ClearAll()
{
	for (int i = 0; i < m_vcFocusElement.size(); ++i)
	{
		delete m_vcFocusElement[i];
	}
	m_vcFocusElement.clear();
}

QVector<CFocusForElement*> CFocusManager::GetFocusElement()
{
	return m_vcFocusElement;
}

FocusPointInfo CFocusManager::GetElementFocusPointInfo(CSVGElement *pElement, const CSVGPointF &pt)
{
	for (int i = 0; i < m_vcFocusElement.size(); ++i)
	{
		if (m_vcFocusElement[i]->GetElement() == pElement)
		{
			QVector<FocusPointInfo>vcKeyPoints = m_vcFocusElement[i]->GetFocusPoints();
			for (int j = 0; j < vcKeyPoints.size(); ++j)
			{
				if (std::fabs(vcKeyPoints[j].point.x - pt.x) <= 5 && std::fabs(vcKeyPoints[j].point.y - pt.y) <= 5)
				{
					return vcKeyPoints[j];
				}
			}
		}
	}
	FocusPointInfo retInfo;
	return retInfo;
}

FlashLineInfo * CFocusManager::GetFlashFocus()
{
	for (int i = 0; i < m_vcFocusElement.size(); ++i)
	{
		if (m_vcFocusElement[i]->GetFlashFocus())
		{
			return m_vcFocusElement[i]->GetFlashFocus();
		}
	}
	return NULL;
}
