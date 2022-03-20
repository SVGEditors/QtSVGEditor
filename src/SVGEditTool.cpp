
#include "SVGDocument.h"
#include "SVGElementShape.h"
#include "SVGEditTool.h"
#include "SVGEditSelect.h"
#include "SVGEditLine.h"
#include "SVGEditRect.h"
#include "SVGEditEllipse.h"
#include "SVGEditPolyline.h"
#include "SVGEditPath.h"
#include "SVGEditImage.h"
#include "SVGEditText.h"
#include "SVGEditZoom.h"
#include "SVGEditMove.h"


CSVGEditTool::CSVGEditTool(CSVGDocument *pDoc, int nType)
{
	m_pDoc = pDoc;
	m_pTool = NULL;
	setToolType(nType);
	m_nType = nType;
}

CSVGEditTool::~CSVGEditTool()
{
	Clear();
}

void CSVGEditTool::Clear()
{
	if (m_pTool)
	{
		delete m_pTool;
		m_pTool = NULL;
	}
}

void CSVGEditTool::setToolType(int ntype)
{
	Clear();
	m_nType = ntype;
	switch (ntype)
	{
	case TOOLTYPE_SELECT:
	{
		m_pTool = new CSVGEditSelect(m_pDoc);
		break;
	}
	case TOOLTYPE_LINE:
	{
		m_pTool = new CSVGEditLine(m_pDoc);
		break;
	}
	case TOOLTYPE_RECT:
	{
		m_pTool = new CSVGEditRect(m_pDoc);
		break;
	}
	case TOOLTYPE_CIRCLE:
	case TOOLTYPE_ELLIPSE:
	{
		m_pTool = new CSVGEditEllipse(m_pDoc, ntype);
		break;
	}
	case TOOLTYPE_POLYLINE:
	case TOOLTYPE_POLYGON:
	{
		m_pTool = new CSVGEditPolyline(m_pDoc, ntype);
		break;
	}
	case TOOLTYPE_CONTINUE_POLYLINE:
	case TOOLTYPE_CONTINUE_POLYGON:
	{
		std::vector<CSVGElementShape*>vcShape = m_pDoc->getSelectShapes();
		if(vcShape.size() == 1 && (vcShape[0]->getNodeType() == SVG_POLYLINE
			|| vcShape[0]->getNodeType() == SVG_POLYGON))
		{
			m_pTool = new CSVGEditPolyline(m_pDoc, ntype, vcShape[0]);
		}
		break;
	}
	case TOOLTYPE_IMAGE:
	{
		m_pTool = new CSVGEditImage(m_pDoc);
		break;
	}
	case TOOLTYPE_PATHARC:
	case TOOLTYPE_PATHFAN:
	case TOOLTYPE_PATH:
	{
		m_pTool = new CSVGEditPath(m_pDoc, ntype);
		break;
	}
	case TOOLTYPE_CONTINUE_ARC:
	case TOOLTYPE_CONTINUE_PATH:
	{
		std::vector<CSVGElementShape*>vcShape = m_pDoc->getSelectShapes();
		if (vcShape.size() == 1 && vcShape[0]->getNodeType() == SVG_PATH)
		{
			m_pTool = new CSVGEditPath(m_pDoc, ntype, vcShape[0]);
		}
		break;
	}
	case TOOLTYPE_TEXT:
	case TOOLTYPE_TEXTV:
	{
		m_pTool = new CSVGEditText(m_pDoc, ntype);
		break;
	}
	case TOOLTYPE_ZOOMIN:
	case TOOLTYPE_ZOOMOUT:
	{
		m_pTool = new CSVGEditZoom(m_pDoc, ntype);
		break;
	}
	case TOOLTYPE_MOVE:
	{
		m_pTool = new CSVGEditMove(m_pDoc);
		break;
	}
	default:
	{
		m_pTool = new CSVGEditSelect(m_pDoc);
		m_nType = TOOLTYPE_SELECT;
		break;
	}
	}
}

bool CSVGEditTool::IsInputing()
{
	if (m_nType == TOOLTYPE_SELECT)
	{
		CSVGEditSelect *pSelect = (CSVGEditSelect*)m_pTool;
		return pSelect->IsInputing();
	}
	return false;
}

void CSVGEditTool::OnVKeyDown(int virtKey)
{
	if (m_nType == TOOLTYPE_SELECT)
	{
		CSVGEditSelect *pSelect = (CSVGEditSelect*)m_pTool;
		if (pSelect->IsInputing())
		{
			pSelect->OnVKeyDown(virtKey);
		}
	}
}


void  CSVGEditTool::InputChar(const char *str)
{
	if (m_nType == TOOLTYPE_SELECT)
	{
		CSVGEditSelect *pSelect = (CSVGEditSelect*)m_pTool;
		if (pSelect->IsInputing())
		{
			pSelect->InputChar(str);
		}
	}
}

void  CSVGEditTool::SetTextInputMode()
{
	if (m_nType == TOOLTYPE_SELECT)
	{
		CSVGEditSelect *pSelect = (CSVGEditSelect*)m_pTool;
		pSelect->SetTextInputMode();
	}
}

void CSVGEditTool::EndInputMode()
{
	if (m_nType == TOOLTYPE_SELECT)
	{
		CSVGEditSelect *pSelect = (CSVGEditSelect*)m_pTool;
		pSelect->EndInputMode();
	}
}

int   CSVGEditTool::getToolType()
{
	return m_nType;
}

bool CSVGEditTool::IsFinish()
{
	if (m_pTool)
	{
		return m_pTool->IsFinish();
	}
	return true;
}

void CSVGEditTool::OnLButtonDown(const POINT &point)
{
	if (m_pTool)
	{
		m_pTool->OnLButtonDown(point);
	}
	m_pDoc->ZoomRestore(point);
}

void CSVGEditTool::OnLButtonUp(const POINT &point)
{
	if (m_pTool)
	{
		m_pTool->OnLButtonUp(point);
	}
}

void CSVGEditTool::OnMButtonDown(const POINT &point)
{
	if (m_pTool)
	{
		m_pTool->OnMButtonDown(point);
	}
}

void CSVGEditTool::OnMouseMove(const POINT &point)
{
	if (m_pTool)
	{
		m_pTool->OnMouseMove(point);
	}
}

void CSVGEditTool::OnRButtonUp(const POINT &point)
{
	if (m_pTool)
	{
		m_pTool->OnRButtonUp(point);
	}
}

void CSVGEditTool::OnLButtonDblClk(const POINT& point)
{
	if (m_pTool)
	{
		m_pTool->OnLButtonDblClk(point);
	}
}

void CSVGEditTool::OnEscPushDown()
{
	if (m_pTool)
	{
		m_pTool->OnEscPushDown();
	}
}
