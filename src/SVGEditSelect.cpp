
#include "SVGEditSelect.h"
#include "SVGElementShape.h"
#include "SVGDocument.h"
#include "resource.h"
#include "SVGElementPath.h"
#include "ActionBase.h"
#include "SVGElementText.h"

CSVGEditSelect *pThis = NULL;
VOID CALLBACK LocalTimerProc(HWND hwnd, UINT message, UINT iTimerID, DWORD dwTime)
{
	if (pThis)
	{
		pThis->ShowInput();
	}
}

CSVGEditSelect::CSVGEditSelect(CSVGDocument *pDocument)
	:CSVGEditBase(pDocument)
	,m_selectMode(SM_NONE)
	,m_bCanMove(false)
	,m_bCanCopy(true)
{
	m_ptDown.x = 0;
	m_ptDown.y = 0;
	m_nHandle = -1;
	m_pTransformAction = NULL;
	m_pSizetoAction = NULL;
	pThis = this;
	m_bOnTimer = false;
	m_pEditText = NULL;
	m_TimerID = 0;
}

CSVGEditSelect::~CSVGEditSelect()
{
	pThis = NULL;
	KillFocusTimer();
}

void CSVGEditSelect::SetmouseType(unsigned short type)
{
	m_mousetype = (mousetype)type;
}

void  CSVGEditSelect::ShowHandleCursor(int nhandle)
{
	LPCTSTR id = IDC_ARROW;
	switch (nhandle)
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
	case ROTATE_HANDLE:
	{
		::SetCursor(::LoadCursor(ghInstance, MAKEINTRESOURCE(IDC_ROTATE)));
		return;
	}
	default:
	{
		if (nhandle >= 0)
		{
			::SetCursor(::LoadCursor(ghInstance, MAKEINTRESOURCE(IDC_ROTATEMOVE)));
			return;
		}
	}
	}
	::SetCursor(::LoadCursor(NULL, id));
}

void CSVGEditSelect::StartTimer()
{
	if (!m_bOnTimer)
	{
		m_bOnTimer = true;
		m_TimerID = ::SetTimer(NULL, 0, 500, LocalTimerProc);
	}
}

void  CSVGEditSelect::SetTextInputMode(float fx, float fy)
{
	std::vector<CSVGElementShape*>vcSelShape = m_pDocument->getSelectShapes();
	if (vcSelShape.size() == 1)
	{
		if (vcSelShape[0]->getNodeType() == SVG_TEXT)
		{
			m_pEditText = (CSVGElementText*)vcSelShape[0];
			m_pEditText->InitInputCursor(fx, fy);
			StartTimer();
		}
		CSVGElementShape *pIniershape = m_pDocument->getInnerShape();
		if (pIniershape && pIniershape->getNodeType() == SVG_TEXT)
		{
			m_pEditText = (CSVGElementText*)pIniershape;
			m_pEditText->InitInputCursor(fx, fy);
			StartTimer();
		}
	}
}

void CSVGEditSelect::SetTextInputMode()
{
	if (!m_bOnTimer)
	{
		std::vector<CSVGElementShape*>vcSelShape = m_pDocument->getSelectShapes();
		if (vcSelShape.size() == 1 && vcSelShape[0]->getNodeType() == SVG_TEXT)
		{
			m_pEditText = (CSVGElementText*)vcSelShape[0];
			m_pEditText->InitInputCursor(0);
			StartTimer();
		}
	}
}

void  CSVGEditSelect::EndInputMode()
{
	KillFocusTimer();
}

void CSVGEditSelect::KillFocusTimer()
{
	if (m_bOnTimer)
	{
		m_pEditText = NULL;
		::KillTimer(NULL, m_TimerID);
		m_pDocument->HideInput();
		m_bOnTimer = false;
	}
}

void CSVGEditSelect::ShowInput()
{
	if (m_pDocument->IsShowInput())
	{
		m_pDocument->HideInput();
	}
	else
	{
		if (m_pEditText)
		{
			CSVGLine  line = m_pEditText->GetInputLine();
			m_pDocument->ShowInput(line.x1,line.y1,line.x2,line.y2);
		}
	}
}

bool CSVGEditSelect::IsInputing()
{
	return m_bOnTimer;
}

void CSVGEditSelect::OnLButtonDown(const POINT &point)
{
	CSVGEditBase::OnLButtonDown(point);
	BeginAction();
	ShowHandleCursor(m_nHandle);
	m_selectMode = SM_NONE;
	KillFocusTimer();
	CSVGElementShape *pSelectG = m_pDocument->GetSelectG();
	if (pSelectG)
	{
		m_nHandle = pSelectG->HitTest(point.x, point.y, m_pDocument->getScreenMatrix());
		if (m_nHandle >= 0)
		{
			ShowHandleCursor(m_nHandle);
			m_pDocument->setCurrentShape(pSelectG);
			if (m_nHandle == ROTATE_HANDLE)
			{
				m_selectMode = SM_ROTATE;
			}
			else
			{
				m_selectMode = SM_SIZE;
			}
			m_pDocument->Draw(NULL);
			return;
		}
	}	
	else
	{
		std::vector<CSVGElementShape*>vcSelShape = m_pDocument->getSelectShapes();
		for (size_t i = 0; i < vcSelShape.size(); ++i)
		{
			CSVGElementShape *pShape = vcSelShape[i];
			if ((m_nHandle = pShape->HitTest(point.x, point.y, m_pDocument->getScreenMatrix())) >= 0)
			{
				if (pShape->getNodeType() == SVG_PATH)
				{
					((CSVGElementPath*)pShape)->setSelectIndex(m_nHandle);
					((CSVGElementPath*)pShape)->resetFocus();
					m_pDocument->ReSetElementToArea(pShape);
				}
				ShowHandleCursor(m_nHandle);
				m_pDocument->setCurrentShape(pShape);
				if (m_nHandle == ROTATE_HANDLE)
				{
					m_selectMode = SM_ROTATE;
				}
				else
				{
					m_selectMode = SM_SIZE;
				}
				m_pDocument->Draw(NULL);
				return;
			}
		}
	}
	std::vector<CSVGElementShape*> vcSelectShape = m_pDocument->getSelectShapes();
	CSVGElementShape *pShape = m_pDocument->getShapeByPoint(point.x, point.y);
	if (pShape)
	{
		CSVGElementShape *pSelShape = pShape;
		CSVGElement *pParent = pShape->getParentNode();
		while (pParent && pParent->getNodeType() != SVG_SVG)
		{//只有在根节点下才能被选中
			if (pParent &&pParent->IsShapeElement())
			{
				pShape = (CSVGElementShape*)pParent;
				pParent = pShape->getParentNode();
			}
			else
			{
				break;
			}
		}
		if (::GetKeyState(VK_SHIFT) >= 0)//Shift没被按着
		{
			if (!m_pDocument->IsSelectShape(pShape))
			{//原先的图形不是被选中的,就要取消所有的选中
				m_pDocument->unSelectAll();
			}
			else
			{//原先的图形被选中,且只有一个的时候,要判断这次是否点中文本元素
				if (vcSelectShape.size() == 1 && pSelShape->getNodeType() == SVG_TEXT)
				{
					CSVGPoint pt(m_downPoint.x, m_downPoint.y);
					const CSVGMatrix * screenmat = m_pDocument->getScreenMatrix();
					CSVGMatrix mat = *screenmat;
					mat.Inverse();
					pt = pt.matrixTransform(mat);
					SetTextInputMode(pt.x, pt.y);
					ShowInput();
				}
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
		m_pDocument->resetSelectGroup();
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
	m_pDocument->setCurrentShape(NULL);
	m_ptDown.x = point.x;
	m_ptDown.y = point.y;
}

void CSVGEditSelect::OnLButtonUp(const POINT &point)
{
	EndAction();
	std::vector<CSVGElementShape*> vcShapes = m_pDocument->getSelectShapes();
	for (size_t i = 0; i < vcShapes.size(); ++i)
	{
		vcShapes[i]->EndAction();
	}	
	m_pDocument->setCurrentShape(NULL);
	if (m_selectMode == SM_RECTSEL)
	{
		m_pDocument->dragSelectShape();
		m_pDocument->clearDragFocus();
		m_pDocument->resetSelectGroup();
		m_pDocument->Draw(NULL);
	}
	m_selectMode = SM_NONE;
	m_bCanMove = false;
	m_bCanCopy = true;
	m_pSizetoAction    = NULL;
	m_pTransformAction = NULL;
	m_pDocument->postSelectChange();
	CSVGEditBase::OnLButtonUp(point);
}

void CSVGEditSelect::OnMouseMove(const POINT &point)
{
	POINT delta;
	delta.x = point.x - m_lastPoint.x;
	delta.y = point.y - m_lastPoint.y;
	switch (m_selectMode)
	{
	case SM_SIZE:
	{
		KillFocusTimer();
		CSVGElementShape * pCurrentShape = m_pDocument->getCurrentShape();
		if (pCurrentShape)
		{
			if (m_pSizetoAction == NULL)
			{
				m_pSizetoAction = new CActionComposite(m_pDocument);
				m_pDocument->GetActionHistory()->AddAction(m_pSizetoAction);
				pCurrentShape->BeginAction(m_pSizetoAction);				
			}
			ShowHandleCursor(m_nHandle);
			pCurrentShape->SizeTo(m_nHandle, point.x, point.y, m_pDocument->getScreenMatrix());
			pCurrentShape->resetPath();
			pCurrentShape->resetFocus();
			pCurrentShape->ClearXML();
			m_pDocument->ResetSelelectShapeArea();
			m_pDocument->ReSetElementToArea(pCurrentShape);
			m_pDocument->Draw(NULL);
			m_pDocument->SetModify();
		}
		break;
	}
	case SM_MOVE:
	{
		//只有偏离8个像素点以上才给移动		
		if (!m_bCanMove)
		{
			if (abs(point.x - m_ptDown.x) >= 8 || abs(point.y - m_ptDown.y) >= 8)
			{
				m_bCanMove = true;
				delta.x = point.x - m_ptDown.x;
				delta.y = point.y - m_ptDown.y;
				
				std::vector<CSVGElementShape*> vcShapes = m_pDocument->getSelectShapes();
				if (m_pTransformAction == NULL)
				{
					m_pTransformAction = new CActionComposite(m_pDocument);
					m_pDocument->GetActionHistory()->AddAction(m_pTransformAction);
					for (size_t i = 0; i < vcShapes.size(); ++i)
					{
						vcShapes[i]->AddActionAttr(m_pTransformAction,lpSVGAttrname[ATTR_TRANSFORM]);
					}
				}
			}
		}
		if (m_bCanMove)
		{
			KillFocusTimer();
			
			if (GetKeyState(VK_CONTROL) < 0 && m_bCanCopy)
			{
				std::string strsel = m_pDocument->CopySelectShape();
				m_pDocument->PasteBuff(strsel.c_str());
				m_bCanCopy = false;
				m_pDocument->SetModify();
				break;
			}
			if (!m_bCanCopy)
			{
				::SetCursor(::LoadCursor(ghInstance, MAKEINTRESOURCE(IDC_COPY)));
			}

			CSVGElementShape *pSelectG = m_pDocument->GetSelectG();
			if (pSelectG)
			{
				pSelectG->Move(delta.x, delta.y, m_pDocument->getScreenMatrix());
				m_pDocument->SetModify();
				m_pDocument->ResetSelelectShapeArea();
				m_pDocument->ReSetElementToArea(pSelectG);
			}
			else
			{
				std::vector<CSVGElementShape*> vcShapes = m_pDocument->getSelectShapes();
				for (size_t i = 0; i < vcShapes.size(); ++i)
				{
					vcShapes[i]->Move(delta.x, delta.y, m_pDocument->getScreenMatrix());
					m_pDocument->SetModify();
					m_pDocument->ReSetElementToArea(vcShapes[i]);
				}
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
		CSVGElementShape *pSelectG = m_pDocument->GetSelectG();
		if (pSelectG)
		{
			int nhandle = pSelectG->HitTest(point.x, point.y, m_pDocument->getScreenMatrix());
			if (nhandle >= 0)
			{
				ShowHandleCursor(nhandle);
			}
		}		

		std::vector<CSVGElementShape*>vcSelShape = m_pDocument->getSelectShapes();
		for (size_t i = 0; i < vcSelShape.size(); ++i)
		{
			CSVGElementShape *pShape = vcSelShape[i];
			int nhandle = pShape->HitTest(point.x, point.y, m_pDocument->getScreenMatrix());
			if (nhandle >= 0)
			{
				ShowHandleCursor(nhandle);
			}
		}

		break;
	}
	case SM_ROTATE:
	{
		CSVGElementShape * pCurrentShape = m_pDocument->getCurrentShape();
		if (pCurrentShape)
		{
			KillFocusTimer();
			ShowHandleCursor(m_nHandle);
			CSVGRect matrect = pCurrentShape->getMatrixBBox();
			CSVGPoint ptrotate(matrect.x + matrect.width / 2, matrect.y + matrect.height / 2);
			POINT ptCenter;
			ptCenter.x = ptrotate.x;
			ptCenter.y = ptrotate.y;
			float angle = g_angle(ptCenter, m_lastPoint, point);
			pCurrentShape->Rotate(angle*180/3.1415926, ptrotate.x, ptrotate.y);
			pCurrentShape->resetFocus();
			pCurrentShape->resetPath();
			m_pDocument->ResetSelelectShapeArea();
			m_pDocument->ReSetElementToArea(pCurrentShape);
			m_pDocument->Draw(NULL);
			m_pDocument->SetModify();
		}
		break;
	}
	case SM_RECTSEL://拉框多选
	{
 		int x1 = m_downPoint.x <= point.x ? m_downPoint.x : point.x;
 		int y1 = m_downPoint.y <= point.y ? m_downPoint.y : point.y;
		int x2 = x1 + abs(point.x - m_downPoint.x);
		int y2 = y1 + abs(point.y - m_downPoint.y);
		CSVGPoint pt1 = m_pDocument->mapScreenPoint(x1, y1);
		CSVGPoint pt2 = m_pDocument->mapScreenPoint(x2, y2);
		m_pDocument->setDragFocus(pt1.x, pt1.y, pt2.x, pt2.y);
		m_pDocument->Draw(NULL);
		break;
	}
	}
	CSVGEditBase::OnMouseMove(point);
}

void CSVGEditSelect::OnLButtonDblClk(const POINT& point)
{
	std::vector<CSVGElementShape*> vcShapes = m_pDocument->getSelectShapes();
	if (vcShapes.size() == 1)
	{
		if (vcShapes[0]->IsGroupElement())
		{
			CSVGElementShape *pShape = m_pDocument->getShapeByPoint(point.x, point.y);
			if (!pShape->IsGroupElement())
			{
				m_pDocument->setInnerShape(pShape);			
				m_pDocument->Draw(NULL);
			}
		}
	}
}

void CSVGEditSelect::OnVKeyDown(int virtKey)
{
	if (IsInputing() && m_pEditText)
	{
		if (virtKey == VK_LEFT)
		{
			if (m_pEditText->isTextBottom())
			{
				m_pEditText->MoveFocusDownPos();
			}
			else
			{
				m_pEditText->MoveFocusLeftPos();
			}
		}
		else if (virtKey == VK_UP)
		{
			if (m_pEditText->isTextBottom())
			{
				m_pEditText->MoveFocusLeftPos();
			}
			else
			{
				m_pEditText->MoveFocusUpPos();
			}
		}
		else if (virtKey == VK_RIGHT)
		{
			if (m_pEditText->isTextBottom())
			{
				m_pEditText->MoveFocusUpPos();
			}
			else
			{
				m_pEditText->MoveFocusRightPos();
			}
		}
		else if (virtKey == VK_DOWN)
		{
			if (m_pEditText->isTextBottom())
			{
				m_pEditText->MoveFocusRightPos();
			}
			else
			{
				m_pEditText->MoveFocusDownPos();
			}
		}
		else if (virtKey == VK_HOME)
		{
			m_pEditText->MoveFocusHomePos();
		}
		else if (virtKey == VK_END)
		{
			m_pEditText->MoveFocusEndPos();
		}
		else if (virtKey == VK_BACK)
		{
			m_pEditText->DeleteLeftChar();
			m_pEditText->resetPath();
			m_pEditText->resetFocus();
			m_pDocument->ReSetElementToArea(m_pEditText);
			m_pDocument->Draw(NULL);
		}
		else if (virtKey == VK_DELETE)
		{
			m_pEditText->DeleteRightChar();
			m_pEditText->resetPath();
			m_pEditText->resetFocus();
			m_pDocument->ReSetElementToArea(m_pEditText);
			m_pDocument->Draw(NULL);
		}
		else if (virtKey == VK_RETURN)
		{
			m_pEditText->NewLine();
			m_pEditText->resetPath();
			m_pEditText->resetFocus();
			m_pDocument->ReSetElementToArea(m_pEditText);
			m_pDocument->Draw(NULL);
		}
		m_pDocument->SetModify();
	}
}

void CSVGEditSelect::InputChar(const char *str)
{
	m_pEditText->InputChar(str);
	m_pEditText->resetPath();
	m_pEditText->resetFocus();
	m_pDocument->ReSetElementToArea(m_pEditText);
	m_pDocument->Draw(NULL);
}