
#include "SVGToolSelect.h"
#include "SVGElementShape.h"
#include "resource.h"


CSVGToolSelect::CSVGToolSelect(CSVGDocument *pDocument)
	:CSVGToolShape(pDocument)
	,m_selectMode(SM_NONE)
	,m_bCanMove(false)
{
	m_ptDown.x = 0;
	m_ptDown.y = 0;
	m_nHandle = -1;
	m_pCurrentShape = NULL;
}

CSVGToolSelect::~CSVGToolSelect()
{
}

void CSVGToolSelect::SetmouseType(unsigned short type)
{
	m_mousetype = (mousetype)type;
}

void  CSVGToolSelect::ShowHandleCursor()
{
	LPCTSTR id = IDC_ARROW;
	switch (m_nHandle)
	{
	case LEFT_TOP:
	case RIGHT_BOTTOM:
	{
		id = IDC_SIZENWSE;//北西 南东 " ↘ "
		break;
	}
	case TOP_MID:
	case BOTTOM_MID:
	{
		id = IDC_SIZENS;  //北南  " ↓ "
		break;
	}
	case RIGHT_TOP:
	case LEFT_BOTTOM:
	{
		id = IDC_SIZENESW;//北东 南西 " ↗ "
		break;
	}
	case LEFT_MID:
	case RIGHT_MID:
	{
		id = IDC_SIZEWE; //西东 " ← "
		break;
	}
	default:
	{
		if (m_nHandle >= 0)
		{
			id = IDC_SIZEALL;
		}
	}
	}
	::SetCursor(::LoadCursor(NULL, id));
}

void CSVGToolSelect::OnLButtonDown(const POINT &point)
{
	CSVGToolShape::OnLButtonDown(point);
	ShowHandleCursor();
	m_selectMode = SM_NONE;
	std::vector<CSVGElementShape*>vcSelShape = m_pDocument->getSelectShapes();
	for (size_t i = 0; i < vcSelShape.size(); ++i)
	{
		CSVGElementShape *pShape = vcSelShape[i];
		if ((m_nHandle = pShape->HitTest(point.x, point.y, m_pDocument->getScreenMatrix())) >= 0)
		{
			ShowHandleCursor();
			m_pCurrentShape = pShape;
			m_selectMode = SM_SIZE;
			return;
		}
	}
	CSVGElementShape *pShape = m_pDocument->getShapeByPoint(point.x, point.y);
	if (pShape)
	{
		if (::GetKeyState(VK_SHIFT) >= 0)//Shift没被按着
		{
			if (!m_pDocument->IsSelectShape(pShape))
			{//原先的图形不是被选中的,就要取消所有的选中
				m_pDocument->unSelectAll();
			}
			if (!m_pDocument->IsSelectShape(pShape))
			{
				m_pDocument->addSelectShape(pShape);
			}
		}
		else
		{//Shift被按住,原先选中的要取消,没选中的就要选中;
			if (!m_pDocument->IsSelectShape(pShape))
			{
				m_pDocument->addSelectShape(pShape);
			}
			else
			{
				m_pDocument->unSelectShape(pShape);
			}
		}		
		m_selectMode = SM_MOVE;
		m_pDocument->Draw(NULL);
	}
	else
	{
		if (::GetKeyState(VK_SHIFT) >= 0)//Shift没被按着
		{
			m_pDocument->unSelectAll();
			m_selectMode = SM_RECTSEL;
		}
		m_pDocument->Draw(NULL);
	}
	m_nHandle = -1;
	m_pCurrentShape = NULL;
	m_ptDown.x = point.x;
	m_ptDown.y = point.y;
}

void CSVGToolSelect::OnLButtonUp(const POINT &point)
{
	m_pCurrentShape = NULL;
	if (m_selectMode == SM_RECTSEL)
	{
		m_pDocument->dragSelectShape();
		m_pDocument->clearDragFocus();
		m_pDocument->Draw(NULL);
	}
	m_selectMode = SM_NONE;
	CSVGToolShape::OnLButtonUp(point);
}

void CSVGToolSelect::OnMouseMove(const POINT &point)
{
	POINT delta;
	delta.x = point.x - m_lastPoint.x;
	delta.y = point.y - m_lastPoint.y;
	switch (m_selectMode)
	{
	case SM_SIZE:
	{
		if (m_pCurrentShape)
		{
			m_pCurrentShape->SizeTo(m_nHandle, point.x, point.y, m_pDocument->getScreenMatrix());
			m_pCurrentShape->resetFocus();
			m_pDocument->Draw(NULL);
			ShowHandleCursor();
		}
		break;
	}
	case SM_MOVE:
	{
		//只有偏离5个像素点以上才给移动		
		if (!m_bCanMove)
		{
			if (abs(point.x - m_ptDown.x) >= 5 || abs(point.y - m_ptDown.y) >= 5)
			{
				m_bCanMove = true;
				delta.x = point.x - m_ptDown.x;
				delta.y = point.y - m_ptDown.y;
			}
		}

		if (m_bCanMove)
		{
			std::vector<CSVGElementShape*> vcShapes = m_pDocument->getSelectShapes();
			for (size_t i = 0; i < vcShapes.size(); ++i)
			{
				vcShapes[i]->Move(delta.x, delta.y,m_pDocument->getScreenMatrix());
			}
			m_pDocument->Draw(NULL);
		}
		break;
	}
	case SM_NONE:
	{
		CSVGElementShape *pShape = m_pDocument->getShapeByPoint(point.x, point.y);
		if (pShape)
		{
			::SetCursor(::LoadCursor(ghInstance, MAKEINTRESOURCE(IDC_SELECT)));
		}
		else
		{
			::SetCursor(::LoadCursor(NULL, IDC_ARROW));
		}
		break;
	}
	case SM_RECTSEL://拉框多选
	{		
 		int x1 = m_downPoint.x <= point.x ? m_downPoint.x : point.x;
 		int y1 = m_downPoint.y <= point.y ? m_downPoint.y : point.y;
		int x2 = x1 + abs(point.x - m_downPoint.x);
		int y2 = y1 + abs(point.y - m_downPoint.y);
		m_pDocument->setDragFocus(x1, y1, x2, y2);
		m_pDocument->Draw(NULL);
		break;
	}
	}
	CSVGToolShape::OnMouseMove(point);
}

void CSVGToolSelect::OnLButtonDblClk(const POINT& point)
{

}
