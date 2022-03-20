
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
#include "SVGDocument.h"
#include <QFile>
#include <QTime>
#include <QPolygon>
#include <math.h>

#include "SVGRectF.h"
#include "SVGElement.h"
#include "SVGElementShape.h"
#include "SVGElementStyle.h"
#include "SVGElementRect.h"
#include "SVGElementSVG.h"
#include "SVGElementLinearGradient.h"
#include "SVGElementRadialGradient.h"
#include "SVGElementStop.h"
#include "SVGElementCircle.h"
#include "SVGElementEllipse.h"
#include "SVGElementLine.h"
#include "SVGElementG.h"
#include "SVGElementPath.h"
#include "SVGElementImage.h"
#include "SVGElementPolyline.h"
#include "SVGElementPolygon.h"
#include "SVGElementText.h"
#include "SVGElementTextContent.h"
#include "SVGElementTspan.h"
#include "SVGElementComment.h"
#include "SVGElementScript.h"
#include "SVGElementDataSection.h"
#include "SVGElementDefs.h"
#include "SVGElementClipPath.h"
#include "SVGElementDoctype.h"
#include "SVGElementPattern.h"
#include "SVGElementUse.h"
#include "SVGElementA.h"
#include "SVGElementSymbol.h"
#include "SVGElementSwitch.h"
#include "ElementMaker.h"


bool IsXSmall(CSVGRectF rt1, CSVGRectF rt2)
{
	if (rt1.x < rt2.x)
		return true;
	return false;
}

bool IsShapeXSmall(CSVGElement* pShape1, CSVGElement* pShape2)
{
	if (pShape1->IsShapeElement() && pShape2->IsShapeElement())
	{
		CSVGRectF bbox1 = ((CSVGElementShape*)pShape1)->getBBox();
		bbox1 = bbox1.matrixTransform(pShape1->getUsedMatrix());

		CSVGRectF bbox2 = ((CSVGElementShape*)pShape2)->getBBox();
		bbox2 = bbox2.matrixTransform(pShape2->getUsedMatrix());

		return IsXSmall(bbox1, bbox2);
	}

	return false;
}

bool IsShapeIndexSmall(CSVGElementShape* pShape1, CSVGElementShape* pShape2)
{
	int nIndex1 = pShape1->GetElementIndex();
	int nIndex2 = pShape2->GetElementIndex();
	if (nIndex1 < nIndex2)
		return true;
	return false;
}

#define INCH 0.03937
CSVGDocument::CSVGDocument()
	:CSVGElement(this, L"document", XML_DOCUMENT)
{
	m_bCanBuildIdmap = true;
	m_bNeedResetElementIndex = false;
	m_mapIdElement[L"idnoused"] = NULL;
	m_nWidth = 800;
	m_nHeight = 300;
	m_nSVGWidth = 600;
	m_nSVGHeight = 600;
	m_defaultfontname = L"SIMSUN";
	m_defaultfontsize = L"24pt";
	m_defaultstrokewidth = L"1";
	m_defaultstroke = L"#000000";
	m_defaultfillopacity = L"0";
	m_defaultfill = L"#FFFFFF";
	m_defaultfontcolor = L"#000000";
	m_bLoadingfile = true;
	m_bModify = false;
	m_nIdIndex = 0;
	m_bShowGrid = true;
	m_bShowRule = true;
	m_bShowCanvas = true;
	m_bModifyBackground = true;
	m_bModifyForeground = true;
	m_bIsMMouseDowning = false;
	m_bIsMouseDowning = false;
	m_bDownRemoveSelect = false;
	m_bDragCopy = false;
	m_pInnerSelectShape = NULL;
	m_pActionAttrs = NULL;
	m_nTextFocusPos = -1;
	m_bShowFlash = true;
	m_svgRootElement = NULL;
	m_bHasLockShape = false;
	m_CanvasBackColor = Qt::white;
	m_bHasCanvasBack = false;
	NewDocument();
	m_screenMat = CSVGMatrix(1, 0, 0, 1, 0, 0);
	OnZoomRestore();
}

CSVGDocument::~CSVGDocument()
{
	Clear();
}

void CSVGDocument::Resize(float fwidth, float fheight)
{
	m_nWidth = fwidth;
	m_nHeight = fheight;
	m_bModifyBackground = true;
	m_bModifyForeground = true;
}

void  CSVGDocument::Clear()
{
	std::list<AreaManager*>::iterator iter = m_AreaManagerList.begin();
	std::list<AreaManager*>::iterator iterend = m_AreaManagerList.end();
	for (; iter != iterend; ++iter)
	{
		AreaManager *pAreaManager = *iter;
		delete pAreaManager;
	}
	m_AreaManagerList.clear();
	m_PointAreaManagerMap.clear();
	m_mapIdElement.clear();
	m_setIds.clear();
	deleteAllChild();
	m_svgRootElement = NULL;
	m_ActionHistory.ClearAll();
	m_pActionAttrs = NULL;
	m_pInnerSelectShape = NULL;
	m_bModify = false;
}

void CSVGDocument::NewDocument()
{
	m_svgRootElement = createElement(L"svg");
	QString strViewBox = "0 0 ";
	strViewBox += QString::number(m_nSVGWidth);
	strViewBox += " ";
	strViewBox += QString::number(m_nSVGHeight);
	m_svgRootElement->setAttribute(L"viewBox", strViewBox.toStdWString(), false);
	addChild(m_svgRootElement, true, false);
}

void CSVGDocument::ReSetAllElementIndex()
{
	int nIndex = 0;
	ReSetChildElementIndex(m_svgRootElement, nIndex);
}

void CSVGDocument::ReSetChildElementIndex(CSVGElement *pElement, int &nIndex)
{
	if (!pElement)
		return;
	pElement->m_ElementIndex = ++nIndex;
	std::wstring strid = pElement->getAttribute(L"id");
	if (strid.length() == 0)
	{
		pElement->setAttribute(L"id", GenerateId(pElement->getTagName()), false);
	}
	for (int i = 0; i < (int)pElement->getChildCount(); ++i)
	{
		ReSetChildElementIndex(pElement->getChild(i), nIndex);
	}
}

void CSVGDocument::ResettoAreaIfChange(CSVGElement *pElement)
{
	if (pElement && pElement->IsShapeElement())
	{
		CSVGElementShape *pShape = (CSVGElementShape*)pElement;
		if (pElement->IsAttrChange())
		{
			pShape->setAttrChange(false);
			pShape->resetPath();
			pShape->resetUseMatrix();
			AddElementToArea(pShape);
		}
		for (int i = 0; i < (int)pElement->getChildCount(); ++i)
		{
			ResettoAreaIfChange(pElement->getChild(i));
		}
	}
}

void CSVGDocument::PrepareDraw()
{
	if (m_bNeedResetElementIndex)
	{
		m_bNeedResetElementIndex = false;
		ReSetAllElementIndex();
	}
	ResettoAreaIfChange(m_svgRootElement);
}

void  CSVGDocument::Draw(QPainter *painter)
{
	m_bModifyContent = false;
	CSVGRectF rectScreen(0.0, 0.0, (float)m_nWidth + 100, (float)m_nHeight + 100);
	std::vector<AreaManager*>vcArea;
	ReDrawBackground();
	ReDrawForeground();
	painter->resetMatrix();
	painter->drawImage(0, 0, m_layerbottomImage);
	CSVGMatrix rmat = m_screenMat;
	rmat.Inverse();
	rectScreen = rectScreen.matrixTransform(rmat);
	vcArea = GetAreaManagerByRect(rectScreen);

	painter->resetMatrix();
	for (size_t i = 0; i < vcArea.size(); ++i)
	{
		AreaManager *pManager = (AreaManager*)vcArea[i];
		CSVGRectF matrect = pManager->rect;

		CSVGRectF drawrect = matrect;
		drawrect = drawrect.matrixTransform(m_screenMat);
		if (!pManager->m_pbitmap)
		{
			pManager->bneedRepaint = true;
			pManager->m_pbitmap = new QImage((int)(drawrect.width + 1), (int)(drawrect.height + 1), QImage::Format_ARGB32);
			pManager->m_pbitmap->fill(0);
		}
		if (pManager->bneedRepaint)
		{
			pManager->bneedRepaint = false;
			pManager->m_pbitmap->fill(0);
			QPainter areapainter(pManager->m_pbitmap);
			areapainter.setRenderHint(QPainter::Antialiasing, false);
			areapainter.setRenderHint(QPainter::TextAntialiasing, false);
			areapainter.setRenderHint(QPainter::SmoothPixmapTransform, false);
			CSVGMatrix mat = m_screenMat;
			mat.LTranslate(-drawrect.x, -drawrect.y);
			std::list<CSVGElementShape*>::iterator iter = pManager->areaElement.begin();
			for (; iter != pManager->areaElement.end(); ++iter)
			{
				areapainter.setClipping(false);
				(*iter)->Draw(&areapainter, &mat);
			}
		}
		painter->setRenderHint(QPainter::Antialiasing, false);
		painter->setRenderHint(QPainter::TextAntialiasing, false);
		QImage bgimage = m_layerbottomImage.copy((int)(drawrect.x + 0.5), (int)(drawrect.y + 0.5), (int)(drawrect.width + 1), (int)(drawrect.height + 1));
		QPainter painterbg(&bgimage);
		painterbg.drawImage(0, 0, *pManager->m_pbitmap);
		painter->drawImage(QPoint((int)(drawrect.x + 0.5), (int)(drawrect.y + 0.5)), bgimage);
	}
	painter->drawImage(0, 0, m_layertopImage);
}

void CSVGDocument::DrawAll(QPainter *painter, const CSVGMatrix &mat)
{
	painter->resetMatrix();
	for (int i = 0; i < (int)m_svgRootElement->getChildCount(); ++i)
	{
		CSVGElement* pElement = m_svgRootElement->getChild(i);
		if (pElement->IsShapeElement())
		{
			CSVGElementShape *pShape = (CSVGElementShape*)pElement;
			pShape->Draw(painter, &mat);
		}
	}
}

QString CSVGDocument::GetFilePath()
{
	return QString::fromStdWString(m_workpath);
}

bool CSVGDocument::LoadFromFile(const std::wstring &filePath)
{
	if (filePath.length() == 0)
		return false;

	m_bShowGrid = true;
	m_bShowRule = true;
	m_bShowCanvas = true;
	m_bModifyBackground = true;
	m_bModifyForeground = true;

	m_workpath = filePath;
	std::wstring::size_type pos = m_workpath.find_last_of('\\');
	m_workpath = m_workpath.substr(0, pos);
	Clear();
	QFile file(QString::fromStdWString(filePath));
	if (file.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		QByteArray bytedate = file.readAll();
		QString str = bytedate;
		std::wstring strw = str.toStdWString();
		file.close();
		wchar_t *pWCBuff = new wchar_t[strw.length() + 1];
		std::wmemset(pWCBuff, 0, strw.length() + 1);
		std::wmemcpy(pWCBuff, strw.c_str(), strw.length());
		LoadFromBuff(pWCBuff);
		delete[]pWCBuff;
		return true;
	}
	if (!m_svgRootElement)
	{
		m_svgRootElement = createElement(L"svg");
		QString strViewBox = "0 0 ";
		strViewBox += QString::number(m_nSVGWidth);
		strViewBox += " ";
		strViewBox += QString::number(m_nSVGHeight);
		m_svgRootElement->setAttribute(L"viewBox", strViewBox.toStdWString(), false);
		addChild(m_svgRootElement, true, false);
	}
	return false;
}

void CSVGDocument::SetFilePath(const std::wstring &filePath)
{
	m_workpath = filePath;
	std::wstring::size_type pos = m_workpath.find_last_of(L'\\');
	m_workpath = m_workpath.substr(0, pos);
}

void CSVGDocument::MakeGroup()
{
	if (m_svgRootElement)
	{
		if (m_svgRootElement->getChildCount() > 1)
		{
			QVector<CSVGElement*>vcChild;
			for (int i = m_svgRootElement->getChildCount() - 1; i >= 0; --i)
			{
				CSVGElement* pChild = m_svgRootElement->getChild(i);
				m_svgRootElement->removeChildOnly(pChild);
				vcChild.push_front(pChild);
			}
			CSVGElement *pGroup = createElement(L"g");
			for (int i = 0; i < vcChild.size(); ++i)
			{
				pGroup->addChild(vcChild[i], false, false);
			}
			m_svgRootElement->appendChild(pGroup);
			std::wstring strclass = m_svgRootElement->getAttribute(L"class");
			if (strclass.length() > 0)
			{
				pGroup->setAttribute(L"class", strclass);
			}
		}
	}
}

bool CSVGDocument::LoadFromBuff(const std::wstring &strtext)
{
	wchar_t *pBuff = new wchar_t[strtext.length() + 1];
	std::wmemset(pBuff, 0, strtext.length() + 1);
	std::wmemcpy(pBuff, strtext.c_str(), strtext.length());
	wchar_t *pTempBuff = pBuff;

	Clear();
	ParserBuffer(pBuff, this);

	for (int i = 0; i < (int)getChildCount(); ++i)
	{
		if (getChild(i)->getNodeType() == SVG_SVG)
		{
			m_svgRootElement = getChild(i);
			break;
		}
	}

	if (!m_svgRootElement)
	{
		m_svgRootElement = createElement(L"svg");
		QString strViewBox = "0 0 ";
		strViewBox += QString::number(m_nSVGWidth);
		strViewBox += " ";
		strViewBox += QString::number(m_nSVGHeight);
		m_svgRootElement->setAttribute(L"viewBox", strViewBox.toStdWString(), false);
		addChild(m_svgRootElement, true, false);

		for (int i = 0; i < (int)getChildCount(); ++i)
		{
			CSVGElement *pChild = getChild(i);
			m_svgRootElement->appendChild(pChild, false);
		}
	}
	if (m_svgRootElement)
	{
		std::wstring strwidth = m_svgRootElement->getAttribute(L"width");
		std::wstring strheight = m_svgRootElement->getAttribute(L"height");
		std::wstring strviewbox = m_svgRootElement->getAttribute(L"viewBox");
		QString qViewBox = QString::fromStdWString(strviewbox);
		qViewBox = qViewBox.replace(",", " ");
		qViewBox = qViewBox.replace(";", " ");
		QStringList viewboxlst = qViewBox.split(" ");
		std::wstring strusewidth = strwidth;
		std::wstring struseheight = strheight;
		std::wstring strviewboxwidth = strwidth;
		std::wstring strviewboxheight = strheight;
		if (viewboxlst.size() == 4)
		{
			strviewboxwidth = viewboxlst[2].toStdWString();
			strviewboxheight = viewboxlst[3].toStdWString();
		}
		if (strwidth.length() == 0 || strwidth.find(L"%") != std::wstring::npos)
		{
			strusewidth = strviewboxwidth;
		}
		if (strheight.length() == 0 || strheight.find(L"%") != std::wstring::npos)
		{
			struseheight = strviewboxheight;
		}
		int nWidth = global.atof_3(strusewidth);
		int nHeight = global.atof_3(struseheight);
		if (nWidth > 0)
		{
			m_nSVGWidth = nWidth;
		}
		if (nHeight > 0)
		{
			m_nSVGHeight = nHeight;
		}
	}
	delete[]pTempBuff;
	return true;
}

void CSVGDocument::ResetUrlUsed()
{
	m_usedurlids.clear();
	std::vector<CSVGElement *> vcElement = GetAllElements();
	for (int i = 0; i < (int)vcElement.size(); ++i)
	{
		if (vcElement[i]->IsShapeElement())
		{
			CSVGElementShape* pShape = (CSVGElementShape*)vcElement[i];
			std::wstring strfill = pShape->getUseStyle(L"fill");
			if (strfill.find(L"url(") != std::wstring::npos)
			{
				std::wstring strid = global.UrlStringGetId(strfill);
				m_usedurlids.insert(strid);
			}
			std::wstring strstroke = pShape->getUseStyle(L"stroke");
			if (strstroke.find(L"url(") != std::wstring::npos)
			{
				std::wstring strid = global.UrlStringGetId(strstroke);
				m_usedurlids.insert(strid);
			}
		}
		std::wstring strhref = vcElement[i]->getAttribute(L"xlink:href");
		if (strhref.length() > 0)
		{
			std::wstring strid = global.UrlStringGetId(strhref);
			m_usedurlids.insert(strid);
		}
	}
	for (int i = 0; i < (int)m_vcStyle.size(); ++i)
	{
		std::vector<CSVGElementStyleClass *> vcClass = m_vcStyle[i]->GetClass();
		for (int j = 0; j < (int)vcClass.size(); ++j)
		{
			std::vector<std::wstring> vcIds = vcClass[j]->GetUrlIDs();
			for (int k = 0; k < (int)vcIds.size(); ++k)
			{
				m_usedurlids.insert(vcIds[k]);
			}
		}
	}
}

std::wstring  CSVGDocument::Toxml()
{
	ResetUrlUsed();
	RepairNameSpace();
	std::wstring strxml;
	int nlevel = 0;
	m_bCanBuildIdmap = false;
	if (m_svgRootElement)
	{
		std::wstring strDomText;
		m_svgRootElement->Toxml(strDomText, 0);

		CSVGDocument mydoc;
		mydoc.LoadFromBuff(strDomText);
		mydoc.ResetUrlUsed();
		mydoc.RepairNameSpace();
		CSVGElement *pRoot = mydoc.GetRootSVGElement();
		std::vector<CSVGElement *>vcNodes = pRoot->m_ChildNodes;
		pRoot->m_ChildNodes.clear();
		for (int i = vcNodes.size() - 1; i >= 0; --i)
		{
			CSVGElement *pChild = vcNodes[i];
			if (pChild->IsShapeElement())
			{
				CSVGElementShape *pShape = (CSVGElementShape*)pChild;
				pShape->resetUseMatrix();
				CSVGRectF bbox = pShape->getMatrixBBox();
				pRoot->pushfront(pChild, false, false);
			}
			else
			{
				pRoot->pushfront(pChild, false, false);
			}
		}
		pRoot->Toxml(strxml, nlevel);
	}
	m_bCanBuildIdmap = true;
	return strxml;
}

std::wstring CSVGDocument::ToFileXml()
{
	std::wstring strxml = L"<?xml version=\"1.0\" encoding=\"utf-8\" standalone=\"no\"?>";
	strxml += Toxml();
	return strxml;
}

CSVGElement *CSVGDocument::createElement(const std::wstring &tagName)
{
	if (tagName.length() == 0)
		return NULL;
	CSVGElement * pElement = NULL;
	unsigned short nIndex = global.getIndexByTagName(tagName);
	switch (nIndex)
	{
	case SVG_RECT:
	{
		pElement = new CSVGElementRect(this);
	}
	break;
	case SVG_TEXT:
	{
		pElement = new CSVGElementText(this);
	}
	break;
	case SVG_TSPAN:
	{
		pElement = new CSVGElementTspan(this);
	}
	break;
	case SVG_TEXTCONTENT:
	{
		pElement = new CSVGElementTextContent(this);
	}
	break;
	case SVG_G:
	{
		pElement = new CSVGElementG(this);
	}
	break;
	case SVG_USE:
	{
		pElement = new CSVGElementUse(this);
	}
	break;
	case SVG_SCRIPT:
	{
		pElement = new CSVGElementScript(this);
	}
	break;
	case SVG_IMAGE:
	{
		pElement = new CSVGElementImage(this);
	}
	break;
	case SVG_LINE:
	{
		pElement = new CSVGElementLine(this);
	}
	break;
	case SVG_POLYLINE:
	{
		pElement = new CSVGElementPolyline(this);
	}
	break;
	case SVG_POLYGON:
	{
		pElement = new CSVGElementPolygon(this);
	}
	break;
	case SVG_PATH:
	{
		pElement = new CSVGElementPath(this);
	}
	break;
	case SVG_ELLIPSE:
	{
		pElement = new CSVGElementEllipse(this);
	}
	break;
	case SVG_CIRCLE:
	{
		pElement = new CSVGElementCircle(this);
	}
	break;
	case SVG_DEFS:
	{
		pElement = new CSVGElementDefs(this);
	}
	break;
	case SVG_PATTERN:
	{
		pElement = new CSVGElementPattern(this);
	}
	break;
	case SVG_LINEARGRADIENT:
	{
		pElement = new CSVGElementLinearGradient(this);
	}
	break;
	case SVG_RADIALGRADIENT:
	{
		pElement = new CSVGElementRadialGradient(this);
	}
	break;
	case SVG_STOP:
	{
		pElement = new CSVGElementStop(this);
	}
	break;
	case SVG_STYLE:
	{
		pElement = new CSVGElementStyle(this);
	}
	break;
	case SVG_CLIPPATH:
	{
		pElement = new CSVGElementClipPath(this);
	}
	break;
	case SVG_A:
	{
		pElement = new CSVGElementA(this);
	}
	break;
	case SVG_SWITCH:
	{
		pElement = new CSVGElementSwitch(this);
	}
	break;
	case SVG_SYMBOL:
	{
		pElement = new CSVGElementSymbol(this);
	}
	break;
	case XML_DATA:
	{
		pElement = new CSVGElementDataSection(this);
	}
	break;
	case XML_COMMENT:
	{
		pElement = new CSVGElementComment(this);
	}
	break;
	case XML_DOCTYPE:
	{
		pElement = new CSVGElementDoctype(this);
	}
	break;
	case SVG_SVG:
	{
		pElement = new CSVGElementSVG(this);
	}
	break;
	default:
	{
		pElement = new CSVGElement(this, tagName, 0);
	}
	}
	m_bNeedResetElementIndex = true;
	return pElement;
}

void CSVGDocument::ParserBuffer(wchar_t *pBuffer, CSVGElement *pParent)
{
	if (!pBuffer)
		return;
	m_bLoadingfile = true;
	std::list<CSVGElement*>ParentList;
	ParentList.push_back(pParent);
	CSVGElement* pCurrParent = pParent;
	CSVGElement *pElement = NULL;
	CSVGElement *pPreserveSpaceElement = NULL;
	wchar_t *pTempText = NULL;
	bool preservespace = false;
	int nCount = 0;
	while (*pBuffer)
	{
		nCount++;
		while (!preservespace && *pBuffer && (global.gspace[*pBuffer] == 1))
			pBuffer++;

		if (*pBuffer == L'<' && *(pBuffer + 1) != L'/')
		{
			if (*(pBuffer + 1) == L'?')
			{//<?xml version="1.0" encoding="utf-8" standalone="yes"?>
				pTempText = pBuffer;
				while (*pBuffer && (*pBuffer != L'?' || *(pBuffer + 1) != L'>'))
					pBuffer++;
				if (*pBuffer == L'?' && *(pBuffer + 1) == L'>')
				{
					pBuffer += 2;
					wchar_t c = *pBuffer;
					*pBuffer = 0;
					pElement = createElement(L"comment_node");
					pCurrParent->addChild(pElement, false, false);
					pElement->setNodeValue(pTempText, false);
					*pBuffer = c;
				}
			}
			else if (*(pBuffer + 1) == '!')
			{//<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 1.1//EN" "http://www.w3.org/Graphics/SVG/1.1/DTD/svg11-flat-20030114.dtd">
				pTempText = pBuffer;
				if (*(pBuffer + 2) == L'-' && *(pBuffer + 3) == L'-')
				{//<!--//-->
					while (*pBuffer && (*pBuffer != L'-' || *(pBuffer + 1) != L'-' || *(pBuffer + 2) != L'>'))
						pBuffer++;
					if (*pBuffer == L'-' && *(pBuffer + 1) == L'-' && *(pBuffer + 2) == L'>')
					{
						pBuffer += 3;//-->
						char c = *pBuffer;
						*pBuffer = 0;
						pElement = createElement(L"comment_node");
						pCurrParent->addChild(pElement, false, false);
						pElement->setNodeValue(pTempText, false);
						*pBuffer = c;
					}
				}
				else
				{//<!  // >
					while (*pBuffer && global.gspace[*pBuffer] == 0)
						pBuffer++;
					CSVGElement *pElement = NULL;
					if (*pBuffer && global.gspace[*pBuffer] == 1)
					{
						wchar_t c = *pBuffer;
						*pBuffer = 0;
						const wchar_t * pName = pTempText + 2;
						pElement = createElement(pName);
						pCurrParent->addChild(pElement, false, false);
						*pBuffer = c;
						if (pElement->getNodeType() == XML_DATA)
						{
							while (*pBuffer && std::wcsncmp(pBuffer, L"]]>", 3) != 0)
							{
								pBuffer++;
							}
						}
					}
					while (*pBuffer && *pBuffer != L'>')
						pBuffer++;
					if (*pBuffer == L'>')
					{
						pBuffer += 1;
						char c = *pBuffer;
						*pBuffer = 0;
						if (!pElement)
						{
							pElement = createElement(L"comment_node");
							pCurrParent->addChild(pElement, false, false);
						}
						pElement->setNodeValue(pTempText, false);
						*pBuffer = c;
					}
				}
			}
			else
			{
				pTempText = pBuffer + 1;
				while (*pBuffer && global.gspace[*pBuffer] == 0 && *pBuffer != L'/' && *pBuffer != L'>')
					pBuffer++;
				if (*pBuffer)
				{
					wchar_t c = *pBuffer;
					*pBuffer = 0;
					pElement = createElement(pTempText);
					pCurrParent->addChild(pElement, false, false);
					*pBuffer = c;
				}
				while (*pBuffer)
				{
					while (*pBuffer && global.gspace[*pBuffer] == 1)
						pBuffer++;

					if (std::wcsncmp(pBuffer, L"/>", 2) == 0)
					{
						pBuffer += 2;
						break;
					}
					else if (*pBuffer == L'>')
					{
						pBuffer++;
						if (!pElement)
							break;
						pCurrParent = pElement;
						ParentList.push_back(pElement);
						if (!pPreserveSpaceElement && pElement->getNodeType() == SVG_TEXT)
						{
							std::wstring xmlspace = pElement->getAttribute(L"xml:space");
							if (xmlspace == L"preserve")
							{
								preservespace = true;
								pPreserveSpaceElement = pElement;
							}
						}
						break;
					}
					while (*pBuffer && global.gspace[*pBuffer] == 1)
						pBuffer++;
					pTempText = pBuffer;
					while (*pBuffer && *pBuffer != L'=')
						pBuffer++;
					if (*pBuffer == L'=')
					{
						wchar_t c = *pBuffer;
						*pBuffer = 0;
						std::wstring attrname = pTempText;
						std::wstring attrval;
						*pBuffer = c;
						bool bSingleQuatation = false;
						while (*pBuffer && *pBuffer != '\"' &&*pBuffer != '\'')
							pBuffer++;
						if (*pBuffer == '\"' || *pBuffer == '\'')
						{
							if (*pBuffer == '\'')
							{
								bSingleQuatation = true;
							}
							pBuffer++;
							pTempText = pBuffer;
						}
						if (!bSingleQuatation)
						{
							while (*pBuffer && *pBuffer != '\"')
								pBuffer++;
						}
						else
						{
							while (*pBuffer && *pBuffer != '\'')
								pBuffer++;
						}
						if (*pBuffer == L'\"' || *pBuffer == L'\'')
						{
							wchar_t c = *pBuffer;
							*pBuffer = 0;
							attrval = pTempText;
							*pBuffer = c;
						}
						if (pElement)
						{
							pElement->setAttribute(attrname.c_str(), attrval.c_str(), false);
						}
					}
					pBuffer++;
				}
			}
		}
		else if (*pBuffer == L'<' && *(pBuffer + 1) == L'/')
		{
			pBuffer += 2;
			while (*pBuffer && *pBuffer != L'>')
				pBuffer++;
			if (*pBuffer == L'>')
				pBuffer++;
			if (pCurrParent)
			{
				pCurrParent->parseReady();
			}
			if (ParentList.size() > 0)
			{
				ParentList.pop_back();
				if (ParentList.size() > 0)
				{
					CSVGElement* pLast = ParentList.back();
					if (pLast == pPreserveSpaceElement)
					{
						preservespace = false;
					}
					pCurrParent = pLast;
				}
			}
		}
		else
		{
			if (*pBuffer)
			{
				pTempText = pBuffer;
				while (*pBuffer && *pBuffer != L'<')
				{
					pBuffer++;
				}
				if (*pBuffer == L'<')
				{
					wchar_t c = *pBuffer;
					*pBuffer = 0;
					pCurrParent->setNodeValue(pTempText, false);
					*pBuffer = c;
				}
			}
		}
	}
	m_bLoadingfile = false;
}

CSVGElementStyleClass *CSVGDocument::getClassByName(const std::wstring &name)
{
	CSVGElementStyleClass * pClass = NULL;
	for (size_t i = 0; i < m_vcStyle.size(); ++i)
	{
		CSVGElementStyle* pStyle = m_vcStyle[i];
		pClass = pStyle->getClassByName(name);
		if (pClass)
		{
			return pClass;
		}
	}
	return NULL;
}

void CSVGDocument::AddStyleElement(CSVGElementStyle *pStyleElement)
{
	if (!pStyleElement)
		return;
	bool bExist = false;
	for (size_t i = 0; i < m_vcStyle.size(); ++i)
	{
		if (m_vcStyle[i] == pStyleElement)
			bExist = true;
	}
	if (!bExist)
	{
		m_vcStyle.push_back(pStyleElement);
	}
}

bool CSVGDocument::RemoveStyleElement(CSVGElementStyle *pStyleElement)
{
	for (size_t i = 0; i < m_vcStyle.size(); ++i)
	{
		if (m_vcStyle[i] == pStyleElement)
		{
			m_vcStyle.erase(m_vcStyle.begin() + i);
			return true;
		}
	}
	return false;
}

void  CSVGDocument::buildIdElement(CSVGElement *pElement, const std::wstring &id)
{
	if (!m_bCanBuildIdmap)
		return;
	int nPos = id.find(L"_");
	if (nPos >= 0)
	{
		std::wstring strright = id.substr(nPos + 1, id.length() - nPos - 1);
		if (strright.length() > 0)
		{
			int nNumber = global.strtoi(strright);
			if (m_nIdIndex <= nNumber)
			{
				m_nIdIndex = nNumber + 1;
			}
		}
	}
	m_mapIdElement[id] = pElement;
	m_setIds.insert(id);
}

CSVGElement *CSVGDocument::getElementById(const std::wstring &id)
{
	if (id.length() > 0)
	{
		std::map<std::wstring, CSVGElement*>::iterator iter = m_mapIdElement.find(id);
		if (iter != m_mapIdElement.end())
			return iter->second;
	}
	return NULL;
}

void CSVGDocument::removeFromId(CSVGElement *pElement)
{
	std::wstring wid = pElement->getAttribute(L"id");
	std::map<std::wstring, CSVGElement*>::iterator iter = m_mapIdElement.find(wid);
	if (iter != m_mapIdElement.end())
	{
		m_mapIdElement.erase(iter);
	}
}

void CSVGDocument::setLocalFolder(const std::wstring &folder)
{
	m_localfolder = folder;
}

std::wstring CSVGDocument::getLocalFolder()
{
	return m_localfolder;
}

void CSVGDocument::getShapeShape(CSVGElementShape *pParent, float fx, float fy, CSVGElementShape *&pretShape)
{
	std::wstring strgroupid;
	std::wstring strshapid;
	if (pParent && !pParent->IsLock() && pParent->IsPtInShape(fx, fy, strgroupid, strshapid))
	{
		pretShape = pParent;
		return;
	}
	for (int i = pParent->m_ChildNodes.size() - 1; i >= 0; --i)
	{
		if (pParent->m_ChildNodes[i]->IsShapeElement())
		{
			CSVGElementShape* pShape = (CSVGElementShape*)pParent->m_ChildNodes[i];
			getShapeShape(pShape, fx, fy, pretShape);
			if (pretShape)
				break;
		}
	}
}

CSVGElementShape *CSVGDocument::getShapeByPoint(int nx, int ny)
{
	CSVGElementShape * pRetShape = NULL;
	CSVGPointF pt2((float)nx, (float)ny);
	AreaManager* pAreaManager = GetAreaManagerFromPoint((int)pt2.x, (int)pt2.y);
	if (pAreaManager)
	{
		std::list<CSVGElementShape*>::iterator itor = pAreaManager->areaElement.end();
		CSVGMatrix mat;
		for (; itor != pAreaManager->areaElement.begin();)
		{
			--itor;
			CSVGElementShape *pElement = *itor;
			if (pElement)
			{
				getShapeShape(pElement, pt2.x, pt2.y, pRetShape);
				if (pRetShape)
				{
					break;
				}
			}
		}
	}
	return  pRetShape;
}

CSVGPointF CSVGDocument::ScreenToLogicalPoint(int x, int y)
{
	CSVGMatrix screenmat;
	CSVGElement *pPageNode = getElementById(L"page");
	if (pPageNode)
	{
		std::wstring strmatrix = pPageNode->getAttribute(L"screenmatrix");
		screenmat.setText(strmatrix);
		screenmat.Inverse();
	}
	CSVGPointF pt(x, y);
	pt = pt.matrixTransform(screenmat);
	return pt;
}

CSVGPointF CSVGDocument::LogicalToScreenPoint(float fx, float fy)
{
	CSVGMatrix screenmat;
	CSVGElement *pPageNode = getElementById(L"page");
	if (pPageNode)
	{
		std::wstring strmatrix = pPageNode->getAttribute(L"screenmatrix");
		screenmat.setText(strmatrix);
	}
	CSVGPointF pt(fx, fy);
	pt = pt.matrixTransform(screenmat);
	return pt;
}

std::wstring CSVGDocument::GetLayerUp(CSVGElement *pElement)
{
	std::wstring strid = L"";
	if (pElement && pElement->IsShapeElement())
	{
		CSVGElementShape *pShape = (CSVGElementShape*)pElement;
		CSVGElement      *pParent = pShape->getParentNode();
		if (!pParent)
		{
			return strid;
		}
		for (int i = pParent->m_ChildNodes.size() - 1; i >= 0; --i)
		{
			if (pParent->m_ChildNodes[i] != pElement)
			{
				strid = pParent->m_ChildNodes[i]->getAttributeId();
				break;
			}
			else if (i == pParent->m_ChildNodes.size() - 1 && pParent->m_ChildNodes[i] == pElement)
			{
				return L"";
			}
		}
		CSVGRectF matbbox = pShape->getMatrixBBox();
		bool bLocationChild = false;
		for (size_t i = 0; i < pParent->m_ChildNodes.size(); ++i)
		{
			if (pParent->m_ChildNodes[i] == pShape && bLocationChild == false)
			{
				bLocationChild = true;
				continue;
			}
			if (bLocationChild)
			{
				if (pParent->m_ChildNodes[i]->IsShapeElement())
				{
					CSVGElementShape *pNextChild = (CSVGElementShape*)pParent->m_ChildNodes[i];
					CSVGRectF nextmatbbox = pNextChild->getMatrixBBox();
					if (pNextChild->IsShapeInRect(matbbox)
						|| pShape->IsShapeInRect(nextmatbbox))
					{
						if (i < pParent->m_ChildNodes.size() - 1)
						{
							pNextChild = (CSVGElementShape*)pParent->m_ChildNodes[i + 1];
							strid = pNextChild->getAttributeId();
						}
						else
						{
							strid = L"";
						}
						break;
					}
				}
			}
		}
	}
	return strid;
}

std::wstring CSVGDocument::GetLayerDown(CSVGElement *pElement)
{
	std::wstring strid = L"";
	if (pElement && pElement->IsShapeElement())
	{
		CSVGElementShape *pShape = (CSVGElementShape*)pElement;
		CSVGElement      *pParent = pShape->getParentNode();
		if (!pParent)
		{
			return strid;
		}

		for (int i = 0; i < (int)pParent->m_ChildNodes.size(); ++i)
		{
			if (pParent->m_ChildNodes[i] != pElement)
			{
				strid = pParent->m_ChildNodes[i]->getAttributeId();
				break;
			}
			else if (i == 0 && pParent->m_ChildNodes[i] == pElement)
			{
				return L"";
			}
		}

		CSVGRectF matbbox = pShape->getMatrixBBox();
		bool bLocationChild = false;
		for (int i = pParent->m_ChildNodes.size() - 1; i >= 0; --i)
		{
			if (pParent->m_ChildNodes[i] == pShape && bLocationChild == false)
			{
				bLocationChild = true;
				continue;
			}
			if (bLocationChild)
			{
				if (pParent->m_ChildNodes[i]->IsShapeElement())
				{
					CSVGElementShape *pNextChild = (CSVGElementShape*)pParent->m_ChildNodes[i];
					CSVGRectF nextmatbbox = pNextChild->getMatrixBBox();
					if (pNextChild->IsShapeInRect(matbbox)
						|| pShape->IsShapeInRect(nextmatbbox))
					{
						strid = pNextChild->getAttributeId();
						break;
					}
				}
			}
		}
	}
	return strid;
}

std::wstring CSVGDocument::GetLayerTop(CSVGElement *pElement)
{
	std::wstring strid = L"";
	if (pElement && pElement->IsShapeElement())
	{
		CSVGElementShape *pShape = (CSVGElementShape*)pElement;
		CSVGElement      *pParent = pShape->getParentNode();
		if (!pParent)
		{
			return strid;
		}
	}
	return strid;
}

std::wstring CSVGDocument::GetLayerBottom(CSVGElement *pElement)
{
	std::wstring strid = L"";
	if (pElement && pElement->IsShapeElement())
	{
		CSVGElementShape *pShape = (CSVGElementShape*)pElement;
		CSVGElement      *pParent = pShape->getParentNode();
		if (!pParent)
		{
			return strid;
		}
		for (int i = 0; i < (int)pParent->m_ChildNodes.size(); ++i)
		{
			if (pParent->m_ChildNodes[i] != pElement)
			{
				strid = pParent->m_ChildNodes[i]->getAttributeId();
				break;
			}
		}
	}
	return strid;
}

std::wstring CSVGDocument::GetTurnRight(CSVGElement *pElement)
{
	std::wstring strmat;
	if (pElement->IsShapeElement())
	{
		CSVGElementShape *pShape = (CSVGElementShape*)pElement;
		CSVGRectF bbox = pShape->getMatrixBBox();
		float centerx = bbox.x + bbox.width / 2;
		float centery = bbox.y + bbox.height / 2;

		CSVGMatrix UsedMat = pShape->getUsedMatrix();
		CSVGMatrix reUsedMat = UsedMat;
		reUsedMat.Inverse();

		CSVGMatrix mat = pShape->getMatrix();
		mat.Add(reUsedMat);
		mat.Translate(centerx, centery);
		mat.Rotate(90);
		mat.Translate(-centerx, -centery);
		mat.Add(UsedMat);

		strmat = mat.getText();
	}
	return strmat;
}

std::wstring CSVGDocument::GetTurnLeft(CSVGElement *pElement)
{
	std::wstring strmat;
	if (pElement->IsShapeElement())
	{
		CSVGElementShape *pShape = (CSVGElementShape*)pElement;
		CSVGRectF bbox = pShape->getMatrixBBox();
		float centerx = bbox.x + bbox.width / 2;
		float centery = bbox.y + bbox.height / 2;
		CSVGMatrix UsedMat = pShape->getUsedMatrix();
		CSVGMatrix reUsedMat = UsedMat;
		reUsedMat.Inverse();

		CSVGMatrix mat = pShape->getMatrix();
		mat.Add(reUsedMat);
		mat.Translate(centerx, centery);
		mat.Rotate(-90);
		mat.Translate(-centerx, -centery);
		mat.Add(UsedMat);

		strmat = mat.getText();
	}
	return strmat;
}

std::wstring CSVGDocument::GetMirrorRight(CSVGElement *pElement)
{
	std::wstring strmat;
	if (pElement->IsShapeElement())
	{
		CSVGElementShape *pShape = (CSVGElementShape*)pElement;
		CSVGRectF bbox = pShape->getMatrixBBox();
		float centerx = bbox.x + bbox.width / 2;
		float centery = bbox.y + bbox.height / 2;
		CSVGMatrix UsedMat = pShape->getUsedMatrix();
		CSVGMatrix reUsedMat = UsedMat;
		reUsedMat.Inverse();

		CSVGMatrix mat = pShape->getMatrix();
		mat.Add(reUsedMat);
		mat.Translate(centerx, centery);
		mat.Scale(-1, 1);
		mat.Translate(-centerx, -centery);
		mat.Add(UsedMat);

		strmat = mat.getText();
	}
	return strmat;
}

std::wstring CSVGDocument::GetMirrorBottom(CSVGElement *pElement)
{
	std::wstring strmat;
	if (pElement->IsShapeElement())
	{
		CSVGElementShape *pShape = (CSVGElementShape*)pElement;
		CSVGRectF bbox = pShape->getMatrixBBox();
		float centerx = bbox.x + bbox.width / 2;
		float centery = bbox.y + bbox.height / 2;
		CSVGMatrix UsedMat = pShape->getUsedMatrix();
		CSVGMatrix reUsedMat = UsedMat;
		reUsedMat.Inverse();

		CSVGMatrix mat = pShape->getMatrix();
		mat.Add(reUsedMat);
		mat.Translate(centerx, centery);
		mat.Scale(1, -1);
		mat.Translate(-centerx, -centery);
		mat.Add(UsedMat);

		strmat = mat.getText();
	}
	return strmat;
}

bool SortElementByX(CSVGElement *pElement1, CSVGElement *pElement2)
{
	if (pElement1 && pElement2)
	{
		if (pElement1->IsShapeElement() && pElement2->IsShapeElement())
		{
			CSVGElementShape *pShape1 = (CSVGElementShape*)pElement1;
			CSVGElementShape *pShape2 = (CSVGElementShape*)pElement2;
			CSVGRectF matbox1 = pShape1->getMatrixBBox();
			CSVGRectF matbox2 = pShape2->getMatrixBBox();
			if (matbox1.x < matbox2.x)
			{
				return true;
			}
		}
	}
	return false;
}
bool SortElementByY(CSVGElement *pElement1, CSVGElement *pElement2)
{
	if (pElement1 && pElement2)
	{
		if (pElement1->IsShapeElement() && pElement2->IsShapeElement())
		{
			CSVGElementShape *pShape1 = (CSVGElementShape*)pElement1;
			CSVGElementShape *pShape2 = (CSVGElementShape*)pElement2;
			CSVGRectF matbox1 = pShape1->getMatrixBBox();
			CSVGRectF matbox2 = pShape2->getMatrixBBox();
			if (matbox1.y < matbox2.y)
			{
				return true;
			}
		}
	}
	return false;
}

bool SortElementByIndex(CSVGElement *pElement1, CSVGElement *pElement2)
{
	if (pElement1 && pElement2)
	{
		if (pElement1->GetElementIndex() < pElement2->GetElementIndex())
			return true;
	}
	return false;
}

bool IsYSmall(CSVGRectF rt1, CSVGRectF rt2)
{
	if (rt1.y < rt2.y)
		return true;
	return false;
}
bool IsShapeYSmall(CSVGElement* pShape1, CSVGElement* pShape2)
{
	if (pShape1->IsShapeElement() && pShape2->IsShapeElement())
	{
		CSVGRectF bbox1 = ((CSVGElementShape*)pShape1)->getBBox();
		bbox1 = bbox1.matrixTransform(pShape1->getUsedMatrix());
		CSVGRectF bbox2 = ((CSVGElementShape*)pShape2)->getBBox();
		bbox2 = bbox2.matrixTransform(pShape2->getUsedMatrix());
		return IsYSmall(bbox1, bbox2);
	}
	return false;
}

std::wstring CSVGDocument::GetDefaultFontName()
{
	return m_defaultfontname;
}

std::wstring  CSVGDocument::GetDefaultFontSize()
{
	return m_defaultfontsize;
}


std::wstring CSVGDocument::GetDefaultStroke()
{
	return m_defaultstroke;
}

std::wstring CSVGDocument::GetDefaultDashLine()
{
	return m_defaultdashline;
}

void IsInRoot(CSVGElement *pParent, CSVGElement *pElement, bool &bExist)
{
	if (!pParent || !pElement)
		return;
	for (size_t i = 0; i < pParent->getChildCount(); ++i)
	{
		if (pParent->getChild(i) == pElement)
		{
			bExist = true;
			break;
		}
		else
		{
			IsInRoot(pParent->getChild(i), pElement, bExist);
		}
	}
}

void CSVGDocument::ParseGetRefText(CSVGElement *pRootElement, CSVGElement *pShapeElement, std::wstring& strText)
{
	ResetUrlUsed();
	std::wstring strfill = pShapeElement->getAttribute(L"fill");
	std::wstring strid = global.UrlStringGetId(strfill);
	if (strid.length() > 0)
	{
		CSVGElement *pElement = getElementById(strid.c_str());
		if (pElement)
		{
			pElement->Toxml(strText, 0);
		}
	}
	std::wstring strclass = pShapeElement->getAttribute(L"class");
	if (strclass.length() > 0)
	{
		CSVGElementStyleClass *pClass = this->getClassByName(strclass);
		if (pClass)
		{
			std::wstring strValue = pClass->getText();
			strText += L"<style type=\"text/css\">\n";
			strText += L"." + strclass + L"{" + strValue + L"}\n";
			strText += L"</style>\n";
			std::vector<std::wstring>vcUrls = pClass->GetUrlIDs();
			for (int k = 0; k < (int)vcUrls.size(); ++k)
			{
				if (vcUrls[k].length() > 0)
				{
					std::wstring strxmlid = L"id=\"" + vcUrls[k];
					if (strText.find(strxmlid) == std::wstring::npos)
					{
						CSVGElement *pElement = getElementById(vcUrls[k].c_str());
						if (pElement)
						{
							pElement->Toxml(strText, 0);
						}
					}
				}
			}
		}
	}
	for (unsigned int i = 0; i < (int)pShapeElement->getChildCount(); ++i)
	{
		CSVGElement *pNode = pShapeElement->getChild(i);;
		if (pNode->IsShapeElement())
		{
			CSVGElementShape *pElement = (CSVGElementShape*)pNode;
			ParseGetRefText(pRootElement, pElement, strText);
		}
	}
}

void CSVGDocument::ReMoveElementFromArea(CSVGElementShape *pElement)
{
	if (!pElement)
		return;
	for (size_t i = 0; i < pElement->m_vcArea.size(); ++i)
	{
		AreaManager *pAreaManager = pElement->m_vcArea[i];
		if (pAreaManager)
		{
			std::list<CSVGElementShape*>::iterator iter = pAreaManager->areaElement.begin();
			for (; iter != pAreaManager->areaElement.end(); ++iter)
			{
				if (*iter == pElement)
				{
					iter = pAreaManager->areaElement.erase(iter);
					pAreaManager->bneedRepaint = true;
					break;
				}
			}
		}
	}
	pElement->m_vcArea.clear();
	if (pElement->getNodeType() == SVG_G || pElement->getNodeType() == SVG_SVG)
	{
		for (int i = 0; i < (int)pElement->getChildCount(); ++i)
		{
			CSVGElement *pChild = pElement->getChild(i);
			if (pChild->IsShapeElement())
			{
				ReMoveElementFromArea((CSVGElementShape*)pChild);
			}
		}
	}
}

void CSVGDocument::AddElementToArea(CSVGElementShape *pElement)
{
	if (pElement->m_nodeType == SVG_G
		|| pElement->m_nodeType == SVG_SVG
		|| pElement->m_nodeType == SVG_SYMBOL
		|| pElement->m_nodeType == SVG_SWITCH
		|| pElement->m_nodeType == SVG_A)
	{
		for (size_t i = 0; i < pElement->getChildCount(); ++i)
		{
			CSVGElement* pChild = pElement->getChild(i);
			if (pChild->IsShapeElement())
			{
				if (pChild->IsShow())
				{
					AddElementToArea((CSVGElementShape*)pChild);
				}
			}
		}
		return;
	}
	ReMoveElementFromArea(pElement);
	CSVGRectF box = pElement->getMatrixBBox();
	std::vector<AreaManager *> vcArea = GetAreaManagerByRect(box);
	pElement->m_vcArea = vcArea;
	for (int i = 0; i < (int)vcArea.size(); ++i)
	{
		AddElementToArea(vcArea[i], pElement);
	}
}

void CSVGDocument::AddElementToArea(AreaManager* pAreaManager, CSVGElementShape *pElement)
{
	if (pAreaManager && pElement->IsShow())
	{
		pAreaManager->bneedRepaint = true;
		std::list<CSVGElementShape*> &area = pAreaManager->areaElement;
		std::list<CSVGElementShape*>::iterator iter = area.begin();
		std::list<CSVGElementShape*>::iterator _Last = area.end();
		int nElementIndex = pElement->GetElementIndex();
		int notherElementIndex = 0;
		bool bInsert = false;
		for (; iter != _Last;)
		{
			notherElementIndex = (*iter)->GetElementIndex();
			if (!bInsert && (notherElementIndex > nElementIndex))
			{
				iter = area.insert(iter, pElement);
				bInsert = true;
			}
			else
			{
				++iter;
			}
		}
		if (!bInsert)
		{
			area.push_back(pElement);
		}
	}
}

CSVGLine CSVGDocument::FormatFloatToAreaInt(const CSVGLineF &fline)
{
	CSVGLine nline;
	if (fline.x1 < 0)
	{
		nline.x1 = ((int)(fline.x1 / AREA_GRID) - 1) * AREA_GRID;
	}
	else
	{
		nline.x1 = (int)(fline.x1 / AREA_GRID) *AREA_GRID;
	}

	if (fline.y1 < 0)
	{
		nline.y1 = ((int)(fline.y1 / AREA_GRID) - 1) * AREA_GRID;
	}
	else
	{
		nline.y1 = (int)(fline.y1 / AREA_GRID) *AREA_GRID;
	}

	if (fline.x2 < 0)
	{
		nline.x2 = (int)(fline.x2 / AREA_GRID) *AREA_GRID;
	}
	else
	{
		nline.x2 = ((int)(fline.x2 / AREA_GRID) + 1) * AREA_GRID;
	}
	if (fline.y2 < 0)
	{
		nline.y2 = (int)(fline.y2 / AREA_GRID) *AREA_GRID;
	}
	else
	{
		nline.y2 = ((int)(fline.y2 / AREA_GRID) + 1) * AREA_GRID;
	}
	return nline;
}

long long CSVGDocument::MakeWord(int x, int y)
{
	long long data1 = ((long long)x << sizeof(long) * 8);
	long long data2 = 0x00000000FFFFFFFF & (long long)y;
	long long data = data1 | data2;
	return data;
}

std::vector<AreaManager*> CSVGDocument::GetAreaManagerByRect(const CSVGRectF &rect)
{
	std::vector<AreaManager*>vcAreaManager;
	CSVGLine nLine = FormatFloatToAreaInt(rect.ToLineF());
	for (int i = nLine.x1; i < nLine.x2; i += AREA_GRID)
	{
		for (int j = nLine.y1; j < nLine.y2; j += AREA_GRID)
		{
			long long nAreaId = MakeWord(i, j);
			std::map<long long, AreaManager *>::iterator iter = m_PointAreaManagerMap.find(nAreaId);
			if (iter != m_PointAreaManagerMap.end())
			{
				vcAreaManager.push_back(iter->second);
			}
			else
			{
				AreaManager *pNewArea = new AreaManager;
				pNewArea->rect = CSVGRectF(i, j, AREA_GRID, AREA_GRID);
				pNewArea->nAreaId = nAreaId;
				m_AreaManagerList.push_back(pNewArea);
				m_PointAreaManagerMap[nAreaId] = pNewArea;
				vcAreaManager.push_back(pNewArea);
			}
		}
	}
	return vcAreaManager;
}

AreaManager* CSVGDocument::GetAreaManagerFromPoint(int x, int y)
{
	int nx1 = 0;
	int ny1 = 0;
	if (x < 0)
	{
		nx1 = ((int)(x*1.0 / AREA_GRID) - 1) * AREA_GRID;
	}
	else
	{
		nx1 = (int)(x*1.0 / AREA_GRID) *AREA_GRID;
	}
	if (y < 0)
	{
		ny1 = ((int)(y*1.0 / AREA_GRID) - 1) * AREA_GRID;
	}
	else
	{
		ny1 = (int)(y*1.0 / AREA_GRID) *AREA_GRID;
	}
	long long nAreaId = MakeWord(nx1, ny1);
	std::map<long long, AreaManager *> ::iterator iter = m_PointAreaManagerMap.find(nAreaId);
	if (iter != m_PointAreaManagerMap.end())
	{
		return iter->second;
	}
	return NULL;
}

bool CSVGDocument::IsModify()
{
	return m_bModify;
}

void CSVGDocument::SetModify(bool bModify)
{
	m_bModify = bModify;
}

void CSVGDocument::DrawHorGridLine(QPainter &painter)
{
	CSVGPointF ptArr(0, 0);
	CSVGPointF ptArr100(100, 100);
	ptArr = ptArr.matrixTransform(m_screenMat);
	ptArr100 = ptArr100.matrixTransform(m_screenMat);

	float fscale = (ptArr100.x - ptArr.x)*1.0 / 100;
	int  nDistance = 100;
	nDistance = ((1.0 / fscale * 100) / 10) * 10;
	if (nDistance > 50 && nDistance < 150)
	{
		nDistance = 100;
	}
	if (nDistance >= 150 && nDistance < 200)
	{
		nDistance = 200;
	}
	float fDistance = nDistance*1.0*fscale;

	QPolygon BoldPoints;
	QPolygon ThinPoints;

	for (int i = ptArr.x; i < m_nWidth; i += fDistance)
	{
		for (int j = 0; j < 10; ++j)
		{
			int nxPt = i + j*(fDistance / 10);
			if (nxPt < 20)
				continue;
			if (j == 0)
			{
				BoldPoints.push_back(QPoint(nxPt, 0));
				BoldPoints.push_back(QPoint(nxPt, m_nHeight));
				BoldPoints.push_back(QPoint(nxPt, 0));
			}
			else
			{
				ThinPoints.push_back(QPoint(nxPt, 0));
				ThinPoints.push_back(QPoint(nxPt, m_nHeight));
				ThinPoints.push_back(QPoint(nxPt, 0));
			}
		}
	}

	for (int i = (ptArr.x - fDistance); i >= -fDistance; i -= fDistance)
	{
		for (int j = 0; j < 10; ++j)
		{
			int nxPt = i + j*(fDistance / 10);
			if (nxPt < 20)
				continue;
			if (j == 0)
			{
				BoldPoints.push_back(QPoint(nxPt, 0));
				BoldPoints.push_back(QPoint(nxPt, m_nHeight));
				BoldPoints.push_back(QPoint(nxPt, 0));
			}
			else
			{
				ThinPoints.push_back(QPoint(nxPt, 0));
				ThinPoints.push_back(QPoint(nxPt, m_nHeight));
				ThinPoints.push_back(QPoint(nxPt, 0));
			}
		}
	}
	QPen boldpen(QBrush(QColor(0, 0, 0, 50)), 1);
	painter.setPen(boldpen);
	painter.drawPolyline(BoldPoints);
	QPen thinpen(QBrush(QColor(0, 0, 0, 25)), 1);
	painter.setPen(thinpen);
	painter.drawPolyline(ThinPoints);
}

void CSVGDocument::DrawVerGridLine(QPainter &painter)
{
	CSVGPointF ptArr(0, 0);
	CSVGPointF ptArr100(100, 100);
	ptArr = ptArr.matrixTransform(m_screenMat);
	ptArr100 = ptArr100.matrixTransform(m_screenMat);

	float fscale = (ptArr100.x - ptArr.x)*1.0 / 100;

	int  nDistance = 100;

	nDistance = ((1.0 / fscale * 100) / 10) * 10;
	if (nDistance > 50 && nDistance < 150)
	{
		nDistance = 100;
	}
	if (nDistance >= 150 && nDistance < 200)
	{
		nDistance = 200;
	}
	float fDistance = nDistance*1.0*fscale;

	QPolygon BoldPoints;
	QPolygon ThinPoints;

	for (int i = ptArr.y; i < m_nHeight; i += fDistance)
	{
		for (int j = 0; j < 10; ++j)
		{
			int nyPt = i + j*(fDistance / 10);
			if (nyPt < 20)
				continue;
			if (j == 0)
			{
				BoldPoints.push_back(QPoint(0, nyPt));
				BoldPoints.push_back(QPoint(m_nWidth, nyPt));
				BoldPoints.push_back(QPoint(0, nyPt));
			}
			else
			{
				ThinPoints.push_back(QPoint(0, nyPt));
				ThinPoints.push_back(QPoint(m_nWidth, nyPt));
				ThinPoints.push_back(QPoint(0, nyPt));
			}
		}
	}

	for (int i = (ptArr.y - fDistance); i >= -fDistance; i -= fDistance)
	{
		for (int j = 0; j < 10; ++j)
		{
			int nyPt = i + j*(fDistance / 10);
			if (nyPt < 20)
				continue;
			if (j == 0)
			{
				BoldPoints.push_back(QPoint(0, nyPt));
				BoldPoints.push_back(QPoint(m_nWidth, nyPt));
				BoldPoints.push_back(QPoint(0, nyPt));
			}
			else
			{
				ThinPoints.push_back(QPoint(0, nyPt));
				ThinPoints.push_back(QPoint(m_nWidth, nyPt));
				ThinPoints.push_back(QPoint(0, nyPt));
			}
		}
	}
	QPen boldpen(QBrush(QColor(0, 0, 0, 50)), 1);
	painter.setPen(boldpen);
	painter.drawPolyline(BoldPoints);
	QPen thinpen(QBrush(QColor(0, 0, 0, 25)), 1);
	painter.setPen(thinpen);
	painter.drawPolyline(ThinPoints);
}

void CSVGDocument::DrawCanvasRect(QPainter &painter)
{
	QMatrix  mat(m_screenMat.m_a, m_screenMat.m_b, m_screenMat.m_c, m_screenMat.m_d, m_screenMat.m_e, m_screenMat.m_f);
	QRect rc(0, 0, m_nSVGWidth, m_nSVGHeight);
	painter.setMatrix(mat);
	painter.setPen(QColor(100, 100, 100, 255));
	painter.setBrush(m_CanvasBackColor);
	painter.drawRect(rc);
	painter.resetMatrix();
}

void CSVGDocument::DrawHorizonRuler(QPainter &painter)
{
	CSVGPointF ptArr(0, 0);
	CSVGPointF ptArr100(100, 100);
	ptArr = ptArr.matrixTransform(m_screenMat);
	ptArr100 = ptArr100.matrixTransform(m_screenMat);

	float fscale = (ptArr100.x - ptArr.x)*1.0 / 100;
	int  nDistance = 100;
	nDistance = ((1.0 / fscale * 100) / 10) * 10;
	if (nDistance > 50 && nDistance < 150)
	{
		nDistance = 100;
	}
	if (nDistance >= 150 && nDistance < 200)
	{
		nDistance = 200;
	}
	float fDistance = nDistance*1.0*fscale;

	painter.setPen(Qt::black);
	painter.setBrush(Qt::white);
	QRect rc(0, 0, m_nWidth, 20);
	painter.drawRect(rc);

	QPolygon Rightpoints;
	int nIndex = 0;
	for (int i = ptArr.x; i < m_nWidth; i += fDistance)
	{
		for (int j = 0; j < 10; ++j)
		{
			int nxPt = i + j*(fDistance / 10);
			if (nxPt < 20)
				continue;
			int nyPt = 0;
			if (j == 0)
			{
				nyPt = 0;
			}
			else if (j % 5 == 0)
			{
				nyPt = 10;
			}
			else
			{
				nyPt = 15;
			}
			Rightpoints.push_back(QPoint(nxPt, 20));
			Rightpoints.push_back(QPoint(nxPt, nyPt));
			Rightpoints.push_back(QPoint(nxPt, 20));
		}
		if (i >= 0)
		{
			painter.setBrush(Qt::black);
			painter.drawText(QPoint(i, 14), QString::number(nIndex*nDistance));
		}
		nIndex++;
	}
	//////////////////////////////////////////////////////////////////////////
	QPolygon Leftpoints;
	nIndex = 1;
	for (int i = (ptArr.x - fDistance); i >= -fDistance; i -= fDistance)
	{
		for (int j = 0; j < 10; ++j)
		{
			int nxPt = i + j*(fDistance / 10);
			if (nxPt < 20)
				continue;
			int nyPt = 0;
			if (j == 0)
			{
				nyPt = 0;
			}
			else if (j % 5 == 0)
			{
				nyPt = 10;
			}
			else
			{
				nyPt = 15;
			}
			Leftpoints.push_front(QPoint(nxPt, 20));
			Leftpoints.push_front(QPoint(nxPt, nyPt));
			Leftpoints.push_front(QPoint(nxPt, 20));
		}
		if (i >= 0)
		{
			painter.setBrush(Qt::black);
			painter.drawText(QPoint(i, 14), QString::number(-nIndex*nDistance));
		}
		nIndex++;
	}
	painter.setPen(Qt::black);
	painter.setBrush(Qt::white);
	painter.drawPolyline(Rightpoints);
	painter.drawPolyline(Leftpoints);
}


void CSVGDocument::DrawVerticalRuler(QPainter &painter)
{
	CSVGPointF ptArr(0, 0);
	CSVGPointF ptArr100(100, 100);
	ptArr = ptArr.matrixTransform(m_screenMat);
	ptArr100 = ptArr100.matrixTransform(m_screenMat);

	float fscale = (ptArr100.x - ptArr.x)*1.0 / 100;

	int  nDistance = 100;

	nDistance = ((1.0 / fscale * 100) / 10) * 10;
	if (nDistance > 50 && nDistance < 150)
	{
		nDistance = 100;
	}
	if (nDistance >= 150 && nDistance < 200)
	{
		nDistance = 200;
	}
	float fDistance = nDistance*1.0*fscale;

	painter.setPen(Qt::black);
	painter.setBrush(Qt::white);
	QRect rc(0, 0, 20, m_nHeight);
	painter.drawRect(rc);

	QPolygon Bottompoints;
	int nIndex = 0;
	for (int i = ptArr.y; i < m_nHeight; i += fDistance)
	{
		for (int j = 0; j < 10; ++j)
		{
			int nyPt = i + j*(fDistance / 10);
			if (nyPt < 20)
				continue;
			int nxPt = 0;
			if (j == 0)
			{
				nxPt = 0;
			}
			else if (j % 5 == 0)
			{
				nxPt = 10;
			}
			else
			{
				nxPt = 15;
			}
			Bottompoints.push_back(QPoint(20, nyPt));
			Bottompoints.push_back(QPoint(nxPt, nyPt));
			Bottompoints.push_back(QPoint(20, nyPt));
		}
		if (i >= 0)
		{
			painter.setBrush(Qt::black);
			QMatrix mat;
			mat.translate(2, (i + 2));
			mat.rotate(90);
			mat.translate(2, -(i + 2));
			painter.setMatrix(mat);
			painter.drawText(QPoint(2, i + 2), QString::number(nIndex*nDistance));
			painter.resetMatrix();
		}
		nIndex++;
	}


	QPolygon Toppoints;
	nIndex = 1;
	for (int i = (ptArr.y - fDistance); i >= -fDistance; i -= fDistance)
	{
		for (int j = 0; j < 10; ++j)
		{
			int nyPt = i + j*(fDistance / 10);
			if (nyPt < 20)
				continue;
			int nxPt = 0;
			if (j == 0)
			{
				nxPt = 0;
			}
			else if (j % 5 == 0)
			{
				nxPt = 10;
			}
			else
			{
				nxPt = 15;
			}

			Toppoints.push_front(QPoint(20, nyPt));
			Toppoints.push_front(QPoint(nxPt, nyPt));
			Toppoints.push_front(QPoint(20, nyPt));
		}
		if (i >= 0)
		{
			painter.setBrush(Qt::black);
			QMatrix mat;
			mat.translate(2, (i + 2));
			mat.rotate(90);
			mat.translate(2, -(i + 2));
			painter.setMatrix(mat);
			painter.drawText(QPoint(2, i + 2), QString::number(-nIndex*nDistance));
			painter.resetMatrix();
		}
		nIndex++;
	}
	QPen pen(Qt::black, 1, Qt::SolidLine);
	painter.setPen(pen);
	painter.setBrush(Qt::white);
	painter.drawPolyline(Toppoints);
	painter.drawPolyline(Bottompoints);
}

void CSVGDocument::DrawRuleCross(QPainter &painter)
{
	painter.setBrush(QColor(255, 255, 255, 255));
	painter.setPen(Qt::black);
	painter.drawRect(QRect(0, 0, 20, 20));
	painter.setPen(Qt::DashLine);
	painter.drawLine(QPoint(0, 10), QPoint(20, 10));
	painter.drawLine(QPoint(10, 0), QPoint(10, 20));
}

QPoint CSVGDocument::GetLogicPoint(const QPoint &point)
{
	CSVGMatrix mat = m_screenMat;
	mat.Inverse();
	CSVGPointF pt(point.x(), point.y());
	pt = pt.matrixTransform(mat);
	return QPoint(pt.x, pt.y);
}

void CSVGDocument::SetHasCanvasBack(bool bHas)
{
	m_bModifyBackground = true;
	m_bHasCanvasBack = bHas;
	if (bHas == false)
	{
		m_CanvasBackColor = Qt::white;
	}
}

void CSVGDocument::SetCanvasBackColor(QColor clr)
{
	m_CanvasBackColor = clr;
	m_bModifyBackground = true;
}

std::vector<CSVGElement*> CSVGDocument::GetDragingRectElement(const CSVGRectF &rect)
{
	std::vector<CSVGElement*> vcRetElement;
	for (int i = m_svgRootElement->getChildCount() - 1; i >= 0; --i)
	{
		CSVGElement *pChild = m_svgRootElement->getChild(i);
		if (pChild->IsShapeElement())
		{
			CSVGElementShape *pShape = (CSVGElementShape*)pChild;
			if (pShape->IsShapeInRect(rect))
			{
				vcRetElement.push_back(pShape);
			}
		}
	}
	return vcRetElement;
}

std::set<std::wstring> CSVGDocument::GetIdsSet()
{
	return m_setIds;
}

std::wstring CSVGDocument::GenerateId(const std::wstring& tagName)
{
	std::wstring strid;
	while (true)
	{
		strid = tagName + L"_" + global.itostring(m_nIdIndex);
		std::set<std::wstring> ::iterator iter = m_setIds.find(strid);
		if (iter == m_setIds.end())
		{
			m_setIds.insert(strid);
			break;
		}
		m_nIdIndex++;
	}
	return strid;
}

void CSVGDocument::ChangeElementId(std::map<std::wstring, std::wstring>&oldtonewmap, CSVGElement*pElement)
{
	std::wstring strid = GenerateId(pElement->getTagName());
	std::wstring stroldid = pElement->getAttribute(L"id");
	oldtonewmap[stroldid] = strid;
	pElement->setAttribute(L"id", strid);
	for (int i = 0; i < (int)pElement->getChildCount(); ++i)
	{
		ChangeElementId(oldtonewmap, pElement->getChild(i));
	}
}

void CSVGDocument::ChangeElementUrlId(std::map<std::wstring, std::wstring>&oldtonewmap, CSVGElement*pElement)
{
	std::wstring strfill = pElement->getAttribute(L"fill");
	std::wstring strfillid = global.UrlStringGetId(strfill);
	if (strfillid.length() > 0)
	{
		std::map<std::wstring, std::wstring>::iterator iter = oldtonewmap.find(strfillid);
		if (iter != oldtonewmap.end())
		{
			std::wstring strnewid = iter->second;
			std::wstring strnewfill = L"url(#" + strnewid + L")";
			pElement->setAttribute(L"fill", strnewfill);
		}
	}

	std::wstring strclippath = pElement->getAttribute(L"clip-path");
	std::wstring strclippathid = global.UrlStringGetId(strclippath);
	if (strclippathid.length() > 0)
	{
		std::map<std::wstring, std::wstring>::iterator iter = oldtonewmap.find(strclippathid);
		if (iter != oldtonewmap.end())
		{
			std::wstring strnewid = iter->second;
			std::wstring strnewclippath = L"url(#" + strnewid + L")";
			pElement->setAttribute(L"clip-path", strnewclippath);
		}
	}

	for (int i = 0; i < (int)pElement->getChildCount(); ++i)
	{
		ChangeElementUrlId(oldtonewmap, pElement->getChild(i));
	}
}

void CSVGDocument::ChangeAllId(std::set<std::wstring>&vcsetId)
{
	if (!m_svgRootElement)
		return;
	m_setIds = vcsetId;
	m_mapIdElement.clear();
	std::map<std::wstring, std::wstring>oldtonewmap;
	for (int i = 0; i < (int)m_svgRootElement->getChildCount(); ++i)
	{
		ChangeElementId(oldtonewmap, m_svgRootElement->getChild(i));
	}
	for (int i = 0; i < (int)m_svgRootElement->getChildCount(); ++i)
	{
		ChangeElementUrlId(oldtonewmap, m_svgRootElement->getChild(i));
	}

	std::map<std::wstring, std::wstring>oldtonewclassname;
	for (int i = 0; i < (int)m_vcStyle.size(); ++i)
	{
		std::vector<CSVGElementStyleClass *> vcClass = m_vcStyle[i]->GetClass();
		for (int j = 0; j < (int)vcClass.size(); ++j)
		{
			vcClass[j]->ChangeClassName(oldtonewclassname);
			std::vector<std::wstring> vcIds = vcClass[j]->GetUrlIDs();
			for (int k = 0; k < (int)vcIds.size(); ++k)
			{
				std::map<std::wstring, std::wstring>::iterator iter = oldtonewmap.find(vcIds[k]);
				if (iter != oldtonewmap.end())
				{
					vcClass[j]->ChangeUrlID(iter->first, iter->second);
				}
			}
		}
	}
	std::vector<CSVGElement *>vcElement;
	if (m_svgRootElement)
	{
		m_svgRootElement->GetAllElementFlat(vcElement);
		for (int i = 0; i < (int)vcElement.size(); ++i)
		{
			std::wstring strclass = vcElement[i]->getAttribute(L"class");
			if (strclass.length() > 0)
			{
				std::map<std::wstring, std::wstring>::iterator iter = oldtonewclassname.find(strclass);
				if (iter != oldtonewclassname.end())
				{
					vcElement[i]->setAttribute(L"class", iter->second);
				}
			}
		}
	}
	resetElement();
}

float CSVGDocument::GetCanvasWidth()
{
	float fwidth = 0;
	if (m_svgRootElement)
	{
		std::wstring strwidth = m_svgRootElement->getAttribute(L"width");
		fwidth = global.atof_3(strwidth);
	}
	return fwidth;
}

float CSVGDocument::GetCanvasHeight()
{
	float fheight = 0;
	if (m_svgRootElement)
	{
		std::wstring strheight = m_svgRootElement->getAttribute(L"tyheight");
		fheight = global.atof_3(strheight);
	}
	return fheight;
}

void CSVGDocument::SetCanvasWidth(float fWidth)
{
	if (m_svgRootElement)
	{
		std::wstring strwidth = global.ftostring(fWidth);
		m_svgRootElement->setAttribute(L"width", strwidth);
	}
}

void CSVGDocument::SetCanvasHeight(float fHeight)
{
	if (m_svgRootElement)
	{
		std::wstring strheight = global.ftostring(fHeight);
		m_svgRootElement->setAttribute(L"height", strheight);
	}
}

void  CSVGDocument::RepairNameSpace()
{
	if (m_svgRootElement)
	{
		m_svgRootElement->setAttribute(L"xmlns", L"http://www.w3.org/2000/svg");
		m_svgRootElement->setAttribute(L"xmlns:xlink", L"http://www.w3.org/1999/xlink");
		m_svgRootElement->setAttribute(L"version", L"1.1");
		std::set<std::wstring>vcname;
		std::set<std::wstring>vcprefix;
		m_svgRootElement->AddToLabelName(vcname);
		std::set<std::wstring>::iterator iter = vcname.begin();
		for (; iter != vcname.end(); ++iter)
		{
			int npos = (*iter).find(':');
			if (npos > 0)
			{
				std::wstring strprefix = (*iter).substr(0, npos);
				vcprefix.insert(strprefix);
			}
		}
		std::set<std::wstring> vcAttrName = m_svgRootElement->getAttrNames();
		std::map<std::wstring, std::wstring>mapnamespace;
		mapnamespace[L"xmlns:dc"] = L"http://purl.org/dc/elements/1.1/";
		mapnamespace[L"xmlns:cc"] = L"http://creativecommons.org/ns#";
		mapnamespace[L"xmlns:rdf"] = L"http://www.w3.org/1999/02/22-rdf-syntax-ns#";
		mapnamespace[L"xmlns:svg"] = L"http://www.w3.org/2000/svg";
		mapnamespace[L"xmlns:xlink"] = L"http://www.w3.org/1999/xlink";
		mapnamespace[L"xmlns:sodipodi"] = L"http://sodipodi.sourceforge.net/DTD/sodipodi-0.dtd";
		mapnamespace[L"xmlns:inkscape"] = L"http://www.inkscape.org/namespaces/inkscape";
		mapnamespace[L"xmlns:xmlns"] = L"";
		mapnamespace[L"xmlns:xml"] = L"";


		iter = vcprefix.begin();
		for (; iter != vcprefix.end(); ++iter)
		{
			std::wstring strxmlnsName = L"xmlns:" + *iter;
			bool bExist = false;
			if (vcAttrName.find(strxmlnsName) != vcAttrName.end())
			{
				bExist = true;
			}
			if (!bExist)
			{
				std::wstring strspaceValue = L"http://www.svgsvg.com";
				if (mapnamespace.find(strxmlnsName) != mapnamespace.end())
				{
					strspaceValue = mapnamespace[strxmlnsName];
				}
				if (strspaceValue.length() > 0)
				{
					m_svgRootElement->setAttribute(strxmlnsName, strspaceValue);
				}
			}
		}
	}
}

void CSVGDocument::ReDrawBackground()
{
	if (m_bModifyBackground)
	{
		m_bModifyBackground = false;
		m_layerbottomImage = QImage(m_nWidth, m_nHeight, QImage::Format_RGB32);
		QPainter painter(&m_layerbottomImage);
		painter.fillRect(0, 0, m_nWidth, m_nHeight, QColor(200, 200, 200, 255));
		if (m_bShowGrid)
		{
			DrawHorGridLine(painter);
			DrawVerGridLine(painter);
		}
		if (m_bShowCanvas)
		{
			DrawCanvasRect(painter);
		}
	}
}

void CSVGDocument::ReDrawForeground()
{
	if (m_bModifyForeground)
	{
		m_bModifyForeground = false;
		m_layertopImage = QImage(m_nWidth, m_nHeight, QImage::Format_ARGB32);
		m_layertopImage.fill(0);
		QPainter painter(&m_layertopImage);
		QMatrix mat(m_screenMat.m_a, m_screenMat.m_b, m_screenMat.m_c, m_screenMat.m_d, m_screenMat.m_e, m_screenMat.m_f);

		if (m_bShowRule)
		{
			DrawHorizonRuler(painter);
			DrawVerticalRuler(painter);
			DrawRuleCross(painter);
		}
		painter.setMatrix(mat);
		QVector<CFocusForElement *>vcFocusElement = m_FocusManager.GetFocusElement();
		for (int i = 0; i < vcFocusElement.size(); ++i)
		{
			QVector<FocusLineInfo>vcLines = vcFocusElement[i]->GetFocusLines();
			for (int j = 0; j < vcLines.size(); ++j)
			{
				QLineF line(vcLines[j].m_FocusLine.x1, vcLines[j].m_FocusLine.y1, vcLines[j].m_FocusLine.x2, vcLines[j].m_FocusLine.y2);
				QPen pen(vcLines[j].m_clr, vcLines[j].m_nLineWidth, vcLines[j].m_ps);
				painter.setPen(pen);
				painter.drawLine(line);
			}

			QVector<FocusPointInfo>vcKeyPoints = vcFocusElement[i]->GetFocusPoints();
			for (int j = 0; j < vcKeyPoints.size(); ++j)
			{
				QRect rc(vcKeyPoints[j].point.x - 5, vcKeyPoints[j].point.y - 5, 10, 10);
				painter.setPen(Qt::black);
				painter.setBrush(vcKeyPoints[j].clr);
				painter.drawRect(rc);
			}
		}
		FlashLineInfo  *pFlashFocus = m_FocusManager.GetFlashFocus();
		if (pFlashFocus && m_bShowFlash)
		{
			QLineF line1(pFlashFocus->m_FocusLine1.x1, pFlashFocus->m_FocusLine1.y1, pFlashFocus->m_FocusLine1.x2, pFlashFocus->m_FocusLine1.y2);
			QPen pen1(pFlashFocus->m_clr1, pFlashFocus->m_nLineWidth1, Qt::SolidLine);
			painter.setPen(pen1);
			painter.drawLine(line1);

			QLineF line2(pFlashFocus->m_FocusLine2.x1, pFlashFocus->m_FocusLine2.y1, pFlashFocus->m_FocusLine2.x2, pFlashFocus->m_FocusLine2.y2);
			QPen pen2(pFlashFocus->m_clr2, pFlashFocus->m_nLineWidth2, Qt::SolidLine);
			painter.setPen(pen2);
			painter.drawLine(line2);
		}

		painter.resetMatrix();
		if (std::fabs(m_dragSelectFocus.width) >= 1 && std::fabs(m_dragSelectFocus.height) >= 1)
		{
			QPen pen(Qt::blue, 1, Qt::DotLine);
			painter.setPen(pen);
			painter.setBrush(QColor(100, 100, 200, 50));
			painter.drawRect(m_dragSelectFocus.x, m_dragSelectFocus.y, m_dragSelectFocus.width, m_dragSelectFocus.height);
		}
	}
}

void CSVGDocument::SetDrawTool(DRAWTOOL drawtool)
{
	if (drawtool == m_DrawTool)
		return;
	if (m_DrawTool == DRAWTOOL_POLYLINEING
		|| m_DrawTool == DRAWTOOL_POLYGONING)
	{
		CSVGElement *pElement = GetFirstSelectElement();
		if (pElement && pElement->getNodeType() == SVG_POLYLINE
			|| pElement->getNodeType() == SVG_POLYGON)
		{
			CSVGElementPolyline *pPolyLine = (CSVGElementPolyline*)pElement;
			std::list<CSVGPointF> listpt = pPolyLine->getPoints();
			std::list<CSVGPointF>::reverse_iterator  iter = listpt.rbegin();
			CSVGPointF pt1;
			CSVGPointF pt2;
			for (int i = 0; iter != listpt.rend(); ++iter, ++i)
			{
				if (i == 0)
				{
					pt1 = (*iter);
				}
				else if (i == 1)
				{
					pt2 = (*iter);
				}
				else
				{
					break;
				}
			}
			if (std::fabs(pt1.x - pt2.x) <= 2 && std::fabs(pt1.y - pt2.y) <= 2)
			{
				listpt.pop_back();
			}
			pPolyLine->setAttribute(L"points", pPolyLine->PointsToValue(listpt));
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (m_DrawTool == DRAWTOOL_LINE
		|| m_DrawTool == DRAWTOOL_RECT
		|| m_DrawTool == DRAWTOOL_CIRCLE
		|| m_DrawTool == DRAWTOOL_ELLIPSE
		|| m_DrawTool == DRAWTOOL_POLYLINEING
		|| m_DrawTool == DRAWTOOL_POLYGONING
		|| m_DrawTool == DRAWTOOL_PATH
		|| m_DrawTool == DRAWTOOL_IMAGE)
	{
		CSVGElement *pElement = GetFirstSelectElement();
		if (pElement)
		{
			if (pElement->getNodeType() == SVG_LINE)
			{
				float fx1 = pElement->getFloatAttribute(L"x1");
				float fy1 = pElement->getFloatAttribute(L"y1");
				float fx2 = pElement->getFloatAttribute(L"x2");
				float fy2 = pElement->getFloatAttribute(L"y2");
				if (std::fabs(fx1 - fx2) <= 2 && std::fabs(fy1 - fy2) <= 2)
				{
					ClearAllSelect();
					m_ActionHistory.popback();
					m_svgRootElement->deleteChild(pElement);
				}
			}
			else if (pElement->getNodeType() == SVG_RECT)
			{
				float fwidth = pElement->getFloatAttribute(L"width");
				float fheight = pElement->getFloatAttribute(L"height");
				if (std::fabs(fwidth) <= 1 && std::fabs(fheight) <= 1)
				{
					ClearAllSelect();
					m_ActionHistory.popback();
					m_svgRootElement->deleteChild(pElement);
				}
			}
			else if (pElement->getNodeType() == SVG_CIRCLE)
			{
				float fr = pElement->getFloatAttribute(L"r");
				if (std::fabs(fr) <= 1)
				{
					ClearAllSelect();
					m_ActionHistory.popback();
					m_svgRootElement->deleteChild(pElement);
				}
			}
			else if (pElement->getNodeType() == SVG_ELLIPSE)
			{
				float frx = pElement->getFloatAttribute(L"rx");
				float fry = pElement->getFloatAttribute(L"rx");
				if (std::fabs(frx) <= 1 || std::fabs(fry) <= 1)
				{
					ClearAllSelect();
					m_ActionHistory.popback();
					m_svgRootElement->deleteChild(pElement);
				}
			}
			else if (pElement->getNodeType() == SVG_POLYLINE)
			{
				CSVGElementPolyline *pPolyLine = (CSVGElementPolyline*)pElement;
				std::list<CSVGPointF> listpt = pPolyLine->getPoints();
				if (listpt.size() <= 1)
				{
					ClearAllSelect();
					m_ActionHistory.popback();
					m_svgRootElement->deleteChild(pElement);
				}
			}
			else if (pElement->getNodeType() == SVG_POLYGON)
			{
				CSVGElementPolygon *pPolyGon = (CSVGElementPolygon*)pElement;
				std::list<CSVGPointF> listpt = pPolyGon->getPoints();
				if (listpt.size() <= 2)
				{
					ClearAllSelect();
					m_ActionHistory.popback();
					m_svgRootElement->deleteChild(pElement);
				}
			}
			else if (pElement->getNodeType() == SVG_PATH)
			{
				CSVGElementPath *pPath = (CSVGElementPath*)pElement;
				if (pPath->GetPathSegSize() == 1)
				{
					ClearAllSelect();
					m_ActionHistory.popback();
					m_svgRootElement->deleteChild(pElement);
				}
			}
			else if (pElement->getNodeType() == SVG_TEXT)
			{
				std::wstring strText = pElement->getNodeValue();
				if (strText.length() == 0)
				{
					ClearAllSelect();
					m_ActionHistory.popback();
					m_svgRootElement->deleteChild(pElement);
				}
			}
		}
	}
	m_DrawTool = drawtool;
}

DRAWTOOL CSVGDocument::GetDrawTool()
{
	return m_DrawTool;
}

bool CSVGDocument::IsNeedRepaint()
{
	if (m_bModifyBackground || m_bModifyForeground || m_bModifyContent)
	{
		return true;
	}
	return false;
}

void CSVGDocument::OnZoomRestore()
{
	m_DrawTool = DRAWTOOL_SELECT;
	m_screenMat = CSVGMatrix(1, 0, 0, 1, 100, 100);
	m_bModifyBackground = true;
	m_bModifyForeground = true;
	SetNeedRepaintAll();
}

void CSVGDocument::ClearAllSelect()
{
	m_vcSelectShapes.clear();
	m_pInnerSelectShape = NULL;
	m_FocusManager.ClearAll();
	m_lstFocusPointInfo = FocusPointInfo();
	EndElementAttrAction();
	m_bModifyForeground = true;
	m_nTextFocusPos = -1;
}

void CSVGDocument::RemoveInnerSelect()
{
	if (m_pInnerSelectShape)
	{
		m_FocusManager.RemoveElement(m_pInnerSelectShape);
		m_pInnerSelectShape = NULL;
		m_bModifyForeground = true;
	}
}

void CSVGDocument::ResetAllFocus()
{
	m_FocusManager.ResetAllFocus();
	m_bModifyForeground = true;
}

void CSVGDocument::AddToSelect(CSVGElement *pElement, QColor clr)
{
	if (!pElement)
		return;
	if (pElement->IsLock())
		return;

	CSVGElement *pShape = pElement;
	if (pElement->getParentNode() != m_svgRootElement)
	{
		if (m_pInnerSelectShape)
		{
			m_FocusManager.RemoveElement(m_pInnerSelectShape);
		}
		m_pInnerSelectShape = pElement;
		m_FocusManager.AddElement(pElement, Qt::red);
		CSVGElement *pParent = pShape->getParentNode();
		while (pParent && pParent->getNodeType() != SVG_SVG)
		{
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
		m_FocusManager.AddElement(pShape, clr);
	}
	else
	{
		m_FocusManager.AddElement(pShape, clr);
	}
	m_bModifyForeground = true;
	for (int i = 0; i < m_vcSelectShapes.size(); ++i)
	{
		if (m_vcSelectShapes[i] == pShape)
		{
			return;
		}
	}
	m_vcSelectShapes.push_back(pShape);
}

void CSVGDocument::RemoveFromSelect(CSVGElement *pElement)
{
	for (int i = 0; i < m_vcSelectShapes.size(); ++i)
	{
		if (m_vcSelectShapes[i] == pElement)
		{
			m_FocusManager.RemoveElement(pElement);
			m_vcSelectShapes.remove(i);
			break;
		}
	}
	m_bModifyForeground = true;
}

QVector<CSVGElement*> CSVGDocument::GetSelectElements()
{
	return m_vcSelectShapes;
}

CSVGElement *CSVGDocument::GetFirstSelectElement()
{
	if (m_vcSelectShapes.size() > 0)
	{
		return m_vcSelectShapes[0];
	}
	return NULL;
}

CSVGElement *CSVGDocument::GetAloneSelectElement(bool bChildIfExist)
{
	if (m_vcSelectShapes.size() == 1)
	{
		if (bChildIfExist && m_pInnerSelectShape)
		{
			return m_pInnerSelectShape;
		}
		return m_vcSelectShapes[0];
	}
	return NULL;
}

MOUSESHAPE CSVGDocument::OnMouseLButtonDown(QPoint point, bool bCtrl, bool balt)
{
	MOUSESHAPE mouseshape = MOUSE_ARROW;
	if (m_DrawTool == DRAWTOOL_LINE
		|| m_DrawTool == DRAWTOOL_RECT
		|| m_DrawTool == DRAWTOOL_CIRCLE
		|| m_DrawTool == DRAWTOOL_ELLIPSE
		|| m_DrawTool == DRAWTOOL_POLYLINE
		|| m_DrawTool == DRAWTOOL_POLYLINEING
		|| m_DrawTool == DRAWTOOL_POLYGON
		|| m_DrawTool == DRAWTOOL_POLYGONING
		|| m_DrawTool == DRAWTOOL_PATH)
	{
		mouseshape = MOUSE_DRAWPEN;
	}
	else if (m_DrawTool == DRAWTOOL_ZOOMIN)
	{
		mouseshape = MOUSE_ZOOMIN;
	}
	else if (m_DrawTool == DRAWTOOL_ZOOMOUT)
	{
		mouseshape = MOUSE_ZOOMOUT;
	}
	else if (m_DrawTool == DRAWTOOL_MOVE)
	{
		mouseshape = MOUSE_CLOSEHAND;
	}
	CSVGPointF pt(point.x(), point.y());
	CSVGMatrix rmat = m_screenMat;
	rmat.Inverse();
	pt = pt.matrixTransform(rmat);
	m_mouseDownPt = point;
	m_lstChangeDownPt = point;
	m_bIsMouseDowning = true;
	m_lstFocusPointInfo = FocusPointInfo();

	m_bCanMove = false;
	switch (m_DrawTool)
	{
	case DRAWTOOL_SELECT:
	{
		m_bDownRemoveSelect = false;
		m_bModifyForeground = true;
		bool bHasSelected = false;
		mouseshape = MOUSE_ARROW;
		if (m_vcSelectShapes.size() == 1)
		{
			CSVGElement *pElement = m_vcSelectShapes[0];
			m_lstFocusPointInfo = m_FocusManager.GetElementFocusPointInfo(pElement, pt);
			if (m_lstFocusPointInfo.nIndex == 10000 || m_lstFocusPointInfo.nIndex == 10004)
			{
				mouseshape = MOUSE_NW;
			}
			else if (m_lstFocusPointInfo.nIndex == 10002 || m_lstFocusPointInfo.nIndex == 10006)
			{
				mouseshape = MOUSE_NE;
			}
			else if (m_lstFocusPointInfo.nIndex == 10001 || m_lstFocusPointInfo.nIndex == 10005)
			{
				mouseshape = MOUSE_NS;
			}
			else if (m_lstFocusPointInfo.nIndex == 10003 || m_lstFocusPointInfo.nIndex == 10007)
			{
				mouseshape = MOUSE_WE;
			}
			else if (m_lstFocusPointInfo.nIndex >= 0 || m_lstFocusPointInfo.nCtrlIndex >= 0)
			{
				mouseshape = MOUSE_SIZETO;
				if (pElement->getNodeType() == SVG_PATH)
				{
					m_FocusManager.ResetFocus(pElement, m_lstFocusPointInfo.nIndex);
				}
			}
			else if (m_lstFocusPointInfo.nIndex < 0)
			{
				CSVGElement *pSingleElement = GetAloneSelectElement(true);
				if (!bCtrl && pSingleElement &&pSingleElement->getNodeType() == SVG_TEXT)
				{
					CSVGElementText *pTextElement = (CSVGElementText*)pSingleElement;
					m_nTextFocusPos = pTextElement->GetTextPos(pt.x, pt.y);
					m_FocusManager.ResetFocus(pTextElement, m_nTextFocusPos);
					if (m_nTextFocusPos >= 0)
					{
						bHasSelected = true;
					}
				}
			}
			if (m_lstFocusPointInfo.nIndex >= 0)
				bHasSelected = true;
		}
		if (!bHasSelected)
		{
			CSVGElementShape *pShape = getShapeByPoint(pt.x, pt.y);
			if (pShape)
			{
				CSVGElement *pParent = pShape->getParentNode();
				while (pParent && pParent != m_svgRootElement)
				{
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

				bool bAdd = true;
				if (bCtrl)
				{
					for (int i = 0; i < m_vcSelectShapes.size(); ++i)
					{
						if (m_vcSelectShapes[i] == pShape)
						{
							bAdd = false;
							mouseshape = MOUSE_SELECT;
							m_bDownRemoveSelect = true;
							break;
						}
					}
					if (bAdd)
					{
						AddToSelect(pShape, Qt::green);
						mouseshape = MOUSE_SELECT;
					}
				}
				else
				{
					for (int i = 0; i < m_vcSelectShapes.size(); ++i)
					{
						if (m_vcSelectShapes[i] == pShape)
						{
							bAdd = false;
							mouseshape = MOUSE_SELECT;
							break;
						}
					}
					if (bAdd)
					{
						ClearAllSelect();
						AddToSelect(pShape, Qt::green);
						mouseshape = MOUSE_SELECT;
					}
				}
			}
			else
			{
				ClearAllSelect();
				m_dragSelectFocus = CSVGRectF(point.x(), point.y(), 0, 0);
			}
		}
		break;
	}
	case DRAWTOOL_LINE:
	{
		m_bModifyContent = true;
		CSVGElement *pElement = createElement(L"line");
		pElement->setAttribute(L"stroke", m_defaultstroke);
		pElement->setAttribute(L"stroke-width", m_defaultstrokewidth);
		pElement->setAttribute(L"stroke-opacity", L"1");
		pElement->setAttribute(L"stroke-dasharray", m_defaultdashline);
		pElement->setFloatAttribute(L"x1", pt.x);
		pElement->setFloatAttribute(L"y1", pt.y);
		pElement->setFloatAttribute(L"x2", pt.x);
		pElement->setFloatAttribute(L"y2", pt.y);
		m_svgRootElement->addChild(pElement, true);
		CActionAdd *pAction = new CActionAdd(this, m_svgRootElement, pElement);
		m_ActionHistory.AddAction(pAction);

		ClearAllSelect();
		AddToSelect(pElement, Qt::green);
		break;
	}
	case DRAWTOOL_RECT:
	{
		m_bModifyContent = true;
		CSVGElement *pElement = createElement(L"rect");
		pElement->setAttribute(L"stroke", m_defaultstroke);
		pElement->setAttribute(L"stroke-width", m_defaultstrokewidth);
		pElement->setAttribute(L"stroke-dasharray", m_defaultdashline);
		pElement->setAttribute(L"stroke-opacity", L"1");
		pElement->setAttribute(L"fill", m_defaultfill);
		pElement->setAttribute(L"fill-opacity", m_defaultfillopacity);
		pElement->setFloatAttribute(L"x", pt.x);
		pElement->setFloatAttribute(L"y", pt.y);
		pElement->setFloatAttribute(L"width", 0);
		pElement->setFloatAttribute(L"height", 0);
		m_svgRootElement->addChild(pElement, true);
		ClearAllSelect();
		AddToSelect(pElement, Qt::green);
		CActionAdd *pAction = new CActionAdd(this, m_svgRootElement, pElement);
		m_ActionHistory.AddAction(pAction);
		break;
	}
	case DRAWTOOL_CIRCLE:
	{
		m_bModifyContent = true;
		CSVGElement *pElement = createElement(L"circle");
		pElement->setAttribute(L"stroke", m_defaultstroke);
		pElement->setAttribute(L"stroke-width", m_defaultstrokewidth);
		pElement->setAttribute(L"stroke-dasharray", m_defaultdashline);
		pElement->setAttribute(L"stroke-opacity", L"1");
		pElement->setAttribute(L"fill", m_defaultfill);
		pElement->setAttribute(L"fill-opacity", m_defaultfillopacity);
		pElement->setFloatAttribute(L"cx", pt.x);
		pElement->setFloatAttribute(L"cy", pt.y);
		pElement->setFloatAttribute(L"r", 0);
		m_svgRootElement->addChild(pElement, true);
		ClearAllSelect();
		AddToSelect(pElement, Qt::green);
		CActionAdd *pAction = new CActionAdd(this, m_svgRootElement, pElement);
		m_ActionHistory.AddAction(pAction);
		break;
	}
	case DRAWTOOL_ELLIPSE:
	{
		m_bModifyContent = true;
		CSVGElement *pElement = createElement(L"ellipse");
		pElement->setAttribute(L"stroke", m_defaultstroke);
		pElement->setAttribute(L"stroke-width", m_defaultstrokewidth);
		pElement->setAttribute(L"stroke-dasharray", m_defaultdashline);
		pElement->setAttribute(L"stroke-opacity", L"1");
		pElement->setAttribute(L"fill", m_defaultfill);
		pElement->setAttribute(L"fill-opacity", m_defaultfillopacity);
		pElement->setFloatAttribute(L"cx", pt.x);
		pElement->setFloatAttribute(L"cy", pt.y);
		pElement->setFloatAttribute(L"rx", 0);
		pElement->setFloatAttribute(L"ry", 0);
		m_svgRootElement->addChild(pElement, true);
		ClearAllSelect();
		AddToSelect(pElement, Qt::green);
		CActionAdd *pAction = new CActionAdd(this, m_svgRootElement, pElement);
		m_ActionHistory.AddAction(pAction);
		break;
	}
	case DRAWTOOL_POLYLINE:
	{
		m_bModifyContent = true;
		CSVGElement *pElement = createElement(L"polyline");
		pElement->setAttribute(L"stroke", m_defaultstroke);
		pElement->setAttribute(L"stroke-width", m_defaultstrokewidth);
		pElement->setAttribute(L"stroke-dasharray", m_defaultdashline);
		pElement->setAttribute(L"stroke-opacity", L"1");
		pElement->setAttribute(L"fill", m_defaultfill);
		pElement->setAttribute(L"fill-opacity", m_defaultfillopacity);
		std::wstring strpts;
		strpts += global.ftostring(pt.x);
		strpts += L" ";
		strpts += global.ftostring(pt.y);
		strpts += L" ";
		strpts += global.ftostring(pt.x);
		strpts += L" ";
		strpts += global.ftostring(pt.y);

		pElement->setAttribute(L"points", strpts);
		m_svgRootElement->addChild(pElement, true);
		ClearAllSelect();
		AddToSelect(pElement, Qt::green);
		m_DrawTool = DRAWTOOL_POLYLINEING;
		CActionAdd *pAction = new CActionAdd(this, m_svgRootElement, pElement);
		m_ActionHistory.AddAction(pAction);
		break;
	}
	case DRAWTOOL_POLYLINEING:
	{
		CSVGElement *pElement = GetFirstSelectElement();
		if (pElement && pElement->getNodeType() == SVG_POLYLINE)
		{
			CSVGElementPolyline *pPolyLine = (CSVGElementPolyline*)pElement;
			std::list<CSVGPointF> listpt = pPolyLine->getPoints();
			CSVGPointF lastpt(pt.x, pt.y);
			CSVGMatrix usemat = pPolyLine->getUsedMatrix();
			usemat.Inverse();
			lastpt = lastpt.matrixTransform(usemat);
			listpt.push_back(lastpt);
			if (balt)
			{
				listpt = FormatToRegularPolygon(listpt);
			}
			pPolyLine->setAttribute(L"points", pPolyLine->PointsToValue(listpt));
			ClearAllSelect();
			AddToSelect(pElement, Qt::green);
		}
		break;
	}
	case DRAWTOOL_POLYGON:
	{
		m_bModifyContent = true;
		CSVGElement *pElement = createElement(L"polygon");
		pElement->setAttribute(L"stroke", m_defaultstroke);
		pElement->setAttribute(L"stroke-width", m_defaultstrokewidth);
		pElement->setAttribute(L"stroke-dasharray", m_defaultdashline);
		pElement->setAttribute(L"stroke-opacity", L"1");
		pElement->setAttribute(L"fill", m_defaultfill);
		pElement->setAttribute(L"fill-opacity", m_defaultfillopacity);
		std::wstring strpts;
		strpts += global.ftostring(pt.x);
		strpts += L" ";
		strpts += global.ftostring(pt.y);
		strpts += L" ";
		strpts += global.ftostring(pt.x);
		strpts += L" ";
		strpts += global.ftostring(pt.y);
		pElement->setAttribute(L"points", strpts);
		m_svgRootElement->addChild(pElement, true);
		ClearAllSelect();
		AddToSelect(pElement, Qt::green);
		CActionAdd *pAction = new CActionAdd(this, m_svgRootElement, pElement);
		m_ActionHistory.AddAction(pAction);
		m_DrawTool = DRAWTOOL_POLYGONING;
		break;
	}
	case DRAWTOOL_POLYGONING:
	{
		CSVGElement *pElement = GetFirstSelectElement();
		if (pElement && pElement->getNodeType() == SVG_POLYGON)
		{
			CSVGElementPolygon *pPolygon = (CSVGElementPolygon*)pElement;
			std::list<CSVGPointF> listpt = pPolygon->getPoints();
			CSVGPointF lastpt(pt.x, pt.y);
			CSVGMatrix usemat = pPolygon->getUsedMatrix();
			usemat.Inverse();
			lastpt = lastpt.matrixTransform(usemat);
			listpt.push_back(lastpt);
			if (balt)
			{
				listpt = FormatToRegularPolygon(listpt);
			}
			pPolygon->setAttribute(L"points", pPolygon->PointsToValue(listpt));
			ClearAllSelect();
			AddToSelect(pElement, Qt::green);
		}
		break;
	}
	case DRAWTOOL_PATH:
	{
		m_bModifyContent = true;
		CSVGElement *pElement = createElement(L"path");
		pElement->setAttribute(L"stroke", m_defaultstroke);
		pElement->setAttribute(L"stroke-width", m_defaultstrokewidth);
		pElement->setAttribute(L"stroke-dasharray", m_defaultdashline);
		pElement->setAttribute(L"stroke-opacity", L"1");
		pElement->setAttribute(L"fill", m_defaultfill);
		pElement->setAttribute(L"fill-opacity", m_defaultfillopacity);

		CSVGElementPath *pPath = (CSVGElementPath*)pElement;
		pPath->createSVGPathSegMoveto((float)pt.x, (float)pt.y);
		pPath->createSVGPathSegLineto((float)pt.x, (float)pt.y);
		std::wstring strdata = pPath->pathToString();
		pPath->setAttribute(L"d", strdata);

		m_svgRootElement->addChild(pElement, true);
		ClearAllSelect();
		AddToSelect(pElement, Qt::green);
		CActionAdd *pAction = new CActionAdd(this, m_svgRootElement, pElement);
		m_ActionHistory.AddAction(pAction);
		m_DrawTool = DRAWTOOL_PATHING;
		break;
	}
	case DRAWTOOL_PATHING:
	{
		m_bModifyContent = true;
		CSVGElement *pElement = GetFirstSelectElement();
		if (pElement && pElement->getNodeType() == SVG_PATH)
		{
			CSVGElementPath *pPath = (CSVGElementPath*)pElement;

			CSVGPointF lastpt(pt.x, pt.y);
			CSVGMatrix usemat = pPath->getUsedMatrix();
			usemat.Inverse();
			lastpt = lastpt.matrixTransform(usemat);

			pPath->removeSeg(pPath->getLastSeg(0));
			CSVGPathSeg *pLastSeg = pPath->getLastSeg(0);
			if (pLastSeg && pLastSeg->getType() == PATH_C)
			{
				CSVGPathSegCurvetoCubic *pPreCubic = (CSVGPathSegCurvetoCubic*)pLastSeg;
				if (std::fabs(pPreCubic->x1 - pPreCubic->x2) <= 0.001
					&& std::fabs(pPreCubic->y1 - pPreCubic->y2) <= 0.001)
				{
					pPath->createSVGPathSegLineto((float)lastpt.x, (float)lastpt.y);
				}
				else
				{
					float fx1 = 2 * pPreCubic->x - pPreCubic->x2;
					float fy1 = 2 * pPreCubic->y - pPreCubic->y2;
					pPath->createSVGPathSegCurvetoCubic(fx1, fy1, lastpt.x, lastpt.y, (float)lastpt.x, (float)lastpt.y);
				}
			}
			else
			{
				pPath->createSVGPathSegLineto((float)lastpt.x, (float)lastpt.y);
			}

			pLastSeg = pPath->getLastSeg(0);
			if (pLastSeg && pLastSeg->getType() == PATH_C)
			{
				CSVGPathSegCurvetoCubic *pPreCubic = (CSVGPathSegCurvetoCubic*)pLastSeg;
				if (std::fabs(pPreCubic->x1 - pPreCubic->x2) <= 0.001
					&& std::fabs(pPreCubic->y1 - pPreCubic->y2) <= 0.001)
				{
					pPath->createSVGPathSegLineto((float)lastpt.x, (float)lastpt.y);
				}
				else
				{
					float fx1 = 2 * pPreCubic->x - pPreCubic->x2;
					float fy1 = 2 * pPreCubic->y - pPreCubic->y2;
					pPath->createSVGPathSegCurvetoCubic(fx1, fy1, lastpt.x, lastpt.y, (float)lastpt.x, (float)lastpt.y);
				}
			}
			else
			{
				pPath->createSVGPathSegLineto((float)lastpt.x, (float)lastpt.y);
			}
			std::wstring strdata = pPath->pathToString();
			pPath->setAttribute(L"d", strdata);
			ClearAllSelect();
			AddToSelect(pElement, Qt::green);
			SetContentHasChange();
		}
		break;
	}
	case DRAWTOOL_TEXT:
	case DRAWTOOL_TEXTAAA:
	{
		m_bModifyContent = true;
		CSVGElement *pElement = createElement(L"text");
		pElement->setAttribute(L"stroke-opacity", L"0");
		pElement->setAttribute(L"fill", m_defaultfontcolor);
		pElement->setAttribute(L"fill-opacity", L"1");
		pElement->setAttribute(L"font-size", m_defaultfontsize);
		pElement->setAttribute(L"font-family", m_defaultfontname);
		pElement->setFloatAttribute(L"x", pt.x);
		pElement->setFloatAttribute(L"y", pt.y);
		pElement->setNodeValue(L"");
		m_svgRootElement->addChild(pElement, true);
		if (m_DrawTool == DRAWTOOL_TEXTAAA)
		{
			pElement->setNodeValue(L"###");
		}
		ClearAllSelect();
		AddToSelect(pElement, Qt::green);
		m_nTextFocusPos = 0;
		m_bShowFlash = true;
		m_FocusManager.ResetFocus(pElement, m_nTextFocusPos);
		CActionAdd *pAction = new CActionAdd(this, m_svgRootElement, pElement);
		m_ActionHistory.AddAction(pAction);
		break;
	}
	}
	return mouseshape;
}

MOUSESHAPE CSVGDocument::OnMouseLButtonUp(QPoint point, bool bCtrl, bool balt)
{
	Q_UNUSED(balt);
	m_bDragCopy = false;
	EndElementAttrAction();
	if (std::fabs(m_dragSelectFocus.width) >= 1 || std::fabs(m_dragSelectFocus.height) >= 1)
	{
		m_dragSelectFocus = CSVGRectF(0, 0, 0, 0);
		m_bModifyForeground = true;
	}
	CSVGPointF pt(point.x(), point.y());
	CSVGMatrix rmat = m_screenMat;
	rmat.Inverse();
	pt = pt.matrixTransform(rmat);

	m_lstFocusPointInfo = FocusPointInfo();
	MOUSESHAPE mouseshape = MOUSE_ARROW;
	if (m_DrawTool == DRAWTOOL_LINE
		|| m_DrawTool == DRAWTOOL_RECT
		|| m_DrawTool == DRAWTOOL_CIRCLE
		|| m_DrawTool == DRAWTOOL_ELLIPSE
		|| m_DrawTool == DRAWTOOL_POLYLINE
		|| m_DrawTool == DRAWTOOL_POLYLINEING
		|| m_DrawTool == DRAWTOOL_POLYGON
		|| m_DrawTool == DRAWTOOL_POLYGONING
		|| m_DrawTool == DRAWTOOL_PATH)
	{
		mouseshape = MOUSE_DRAWPEN;
	}
	else if (m_DrawTool == DRAWTOOL_SELECT)
	{
		if (m_lstFocusPointInfo.nIndex < 0 && point == m_mouseDownPt)
		{
			CSVGElementShape *pShape = getShapeByPoint(pt.x, pt.y);
			if (pShape)
			{
				if (bCtrl)
				{
					for (int i = 0; i < m_vcSelectShapes.size(); ++i)
					{
						if (m_vcSelectShapes[i] == pShape && m_bDownRemoveSelect)
						{
							RemoveFromSelect(pShape);
							break;
						}
					}
				}
			}
		}
	}
	else if (m_DrawTool == DRAWTOOL_ZOOMIN)
	{
		mouseshape = MOUSE_ZOOMIN;
	}
	else if (m_DrawTool == DRAWTOOL_ZOOMOUT)
	{
		mouseshape = MOUSE_ZOOMOUT;
	}
	else if (m_DrawTool == DRAWTOOL_MOVE)
	{
		mouseshape = MOUSE_OPENHAND;
	}

	m_bIsMouseDowning = false;
	if (m_DrawTool == DRAWTOOL_ZOOMIN)
	{
		m_screenMat.LTranslate(-point.x(), -point.y());
		m_screenMat.LScale(1.2f, 1.2f);
		m_screenMat.LTranslate(point.x(), point.y());
		m_bModifyBackground = true;
		m_bModifyForeground = true;
		SetNeedRepaintAll();
	}
	else if (m_DrawTool == DRAWTOOL_ZOOMOUT)
	{
		m_screenMat.LTranslate(-point.x(), -point.y());
		m_screenMat.LScale(0.8f, 0.8f);
		m_screenMat.LTranslate(point.x(), point.y());
		m_bModifyBackground = true;
		m_bModifyForeground = true;
		SetNeedRepaintAll();
	}
	return mouseshape;
}

MOUSESHAPE CSVGDocument::OnMouseRButtonDown(QPoint point)
{
	Q_UNUSED(point);
	MOUSESHAPE mouseshape = MOUSE_ARROW;

	return mouseshape;
}

MOUSESHAPE CSVGDocument::OnMouseRButtonUp(QPoint point)
{
	Q_UNUSED(point);
	MOUSESHAPE mouseshape = MOUSE_ARROW;
	m_bIsMouseDowning = false;
	return mouseshape;
}

MOUSESHAPE CSVGDocument::OnMouseMButtonDown(QPoint point)
{
	MOUSESHAPE mouseshape = MOUSE_ARROW;
	m_bIsMMouseDowning = true;
	m_lstMiddleDownPt = point;
	return mouseshape;
}

MOUSESHAPE CSVGDocument::OnMouseMButtonUp(QPoint point)
{
	Q_UNUSED(point);
	MOUSESHAPE mouseshape = MOUSE_ARROW;
	m_bIsMMouseDowning = false;
	return mouseshape;
}

MOUSESHAPE CSVGDocument::OnMouseLDBClick(QPoint point)
{
	if (m_vcSelectShapes.size() == 1)
	{
		CSVGElement *pElement = m_vcSelectShapes[0];
		std::wstring tywidgettype = pElement->getAttribute(L"tywidgettype");
		if (m_pInnerSelectShape)
		{
			m_FocusManager.RemoveElement(m_pInnerSelectShape);
			m_pInnerSelectShape = NULL;
		}
		if (tywidgettype.length() == 0)
		{
			CSVGPointF pt(point.x(), point.y());
			CSVGMatrix rmat = m_screenMat;
			rmat.Inverse();
			pt = pt.matrixTransform(rmat);
			CSVGElementShape *pShape = getShapeByPoint(pt.x, pt.y);
			if (pShape && pShape != pElement)
			{
				m_pInnerSelectShape = pShape;
				m_FocusManager.AddElement(pShape, Qt::red);
				m_bModifyForeground = true;
			}
		}
	}
	return MOUSE_ARROW;
}

MOUSESHAPE CSVGDocument::OnMouseWheel(QPoint point, int delta)
{
	MOUSESHAPE mouseshape = MOUSE_ARROW;
	if (delta < 0)
	{
		m_screenMat.LTranslate(-point.x(), -point.y());
		m_screenMat.LScale(0.8f, 0.8f);
		m_screenMat.LTranslate(point.x(), point.y());
		m_bModifyBackground = true;
		m_bModifyForeground = true;
		SetNeedRepaintAll();
	}
	else if (delta > 0)
	{
		m_screenMat.LTranslate(-point.x(), -point.y());
		m_screenMat.LScale(1.2f, 1.2f);
		m_screenMat.LTranslate(point.x(), point.y());
		m_bModifyBackground = true;
		m_bModifyForeground = true;
		SetNeedRepaintAll();
	}
	return mouseshape;
}

MOUSESHAPE CSVGDocument::OnMouseMove(QPoint point, bool bCtrl, bool balt)
{
	CSVGPointF pt(point.x(), point.y());
	CSVGMatrix rmat = m_screenMat;
	rmat.Inverse();
	pt = pt.matrixTransform(rmat);

	CSVGPointF lstptlogic(m_lstChangeDownPt.x(), m_lstChangeDownPt.y());
	lstptlogic = lstptlogic.matrixTransform(rmat);


	CSVGPointF downptlogic(m_mouseDownPt.x(), m_mouseDownPt.y());
	downptlogic = downptlogic.matrixTransform(rmat);

	MOUSESHAPE mouseshape = MOUSE_ARROW;
	if (m_DrawTool == DRAWTOOL_LINE
		|| m_DrawTool == DRAWTOOL_RECT
		|| m_DrawTool == DRAWTOOL_CIRCLE
		|| m_DrawTool == DRAWTOOL_ELLIPSE
		|| m_DrawTool == DRAWTOOL_POLYLINE
		|| m_DrawTool == DRAWTOOL_POLYLINEING
		|| m_DrawTool == DRAWTOOL_POLYGON
		|| m_DrawTool == DRAWTOOL_POLYGONING
		|| m_DrawTool == DRAWTOOL_PATH)
	{
		mouseshape = MOUSE_DRAWPEN;
	}
	else if (m_DrawTool == DRAWTOOL_ZOOMIN)
	{
		mouseshape = MOUSE_ZOOMIN;
	}
	else if (m_DrawTool == DRAWTOOL_ZOOMOUT)
	{
		mouseshape = MOUSE_ZOOMOUT;
	}
	else if (m_DrawTool == DRAWTOOL_MOVE)
	{
		if (m_bIsMouseDowning)
		{
			mouseshape = MOUSE_CLOSEHAND;
		}
		else
		{
			mouseshape = MOUSE_OPENHAND;
		}
	}
	if (m_bIsMouseDowning && m_DrawTool == DRAWTOOL_MOVE)
	{
		m_screenMat.LTranslate(point.x() - m_lstChangeDownPt.x(), point.y() - m_lstChangeDownPt.y());
		m_bModifyBackground = true;
		m_bModifyForeground = true;
	}
	else if (m_bIsMMouseDowning)
	{
		m_screenMat.LTranslate(point.x() - m_lstChangeDownPt.x(), point.y() - m_lstChangeDownPt.y());
		m_bModifyBackground = true;
		m_bModifyForeground = true;
	}
	else if (m_bIsMouseDowning && m_DrawTool == DRAWTOOL_SELECT)
	{
		RemoveInnerSelect();
		if (m_lstFocusPointInfo.nIndex >= 0)
		{
			if (m_lstFocusPointInfo.nIndex == 10000 || m_lstFocusPointInfo.nIndex == 10004)
			{
				mouseshape = MOUSE_NW;
			}
			else if (m_lstFocusPointInfo.nIndex == 10002 || m_lstFocusPointInfo.nIndex == 10006)
			{
				mouseshape = MOUSE_NE;
			}
			else if (m_lstFocusPointInfo.nIndex == 10001 || m_lstFocusPointInfo.nIndex == 10005)
			{
				mouseshape = MOUSE_NS;
			}
			else if (m_lstFocusPointInfo.nIndex == 10003 || m_lstFocusPointInfo.nIndex == 10007)
			{
				mouseshape = MOUSE_WE;
			}
			else if (m_lstFocusPointInfo.nIndex >= 0 || m_lstFocusPointInfo.nCtrlIndex >= 0)
			{
				mouseshape = MOUSE_SIZETO;
			}
			BeginElementSizeTo();
			ElementSizeTo(pt, bCtrl);
		}
		else
		{
			QVector<CSVGElement *>vcElement = GetSelectElements();
			if (vcElement.size() > 0 && std::fabs(m_dragSelectFocus.width) < 1)
			{
				if (!m_bCanMove)
				{
					if (std::fabs(point.x() - m_mouseDownPt.x()) > 5 || std::fabs(point.y() - m_mouseDownPt.y()) > 5)
					{
						m_bCanMove = true;
					}
				}
				if (m_bCanMove)
				{
					CActionComposite * pComposite = NULL;
					if (bCtrl && !m_bDragCopy)
					{
						std::sort(m_vcSelectShapes.begin(), m_vcSelectShapes.end(), SortElementByIndex);
						vcElement = GetSelectElements();
						m_bDragCopy = true;
						std::wstring strCopyText;
						for (int i = 0; i < vcElement.size(); ++i)
						{
							vcElement[i]->Toxml(strCopyText, 0);
						}
						strCopyText = L"<svg>" + strCopyText + L"</svg>";
						m_nTextFocusPos = -1;
						ClearAllSelect();
						pComposite = Paste(strCopyText);
					}
					vcElement = GetSelectElements();
					BeginElementTransformAction(pComposite);

					for (int i = 0; i < vcElement.size(); ++i)
					{
						CSVGMatrix mat;
						mat.Translate(pt.x - lstptlogic.x, pt.y - lstptlogic.y);
						CSVGMatrix newmat = vcElement[i]->DoMatrixTranslate(mat);
						vcElement[i]->setAttribute(L"transform", newmat.getText());
						vcElement[i]->resetUseMatrix();
						AddToSelect(vcElement[i], Qt::green);
						m_bModifyForeground = true;
					}
					if (m_bDragCopy)
					{
						mouseshape = MOUSE_DRAWCOPY;
					}
				}
				if (mouseshape != MOUSE_DRAWCOPY)
				{
					mouseshape = MOUSE_SELECT;
				}
			}
			else
			{
				m_dragSelectFocus.width = (point.x() - m_mouseDownPt.x());
				m_dragSelectFocus.height = (point.y() - m_mouseDownPt.y());
				m_bModifyForeground = true;
				CSVGRectF rc = m_dragSelectFocus;
				rc = rc.matrixTransform(rmat);
				ClearAllSelect();
				std::vector<CSVGElement *> vcElement = GetDragingRectElement(rc);
				for (int i = 0; i < (int)vcElement.size(); ++i)
				{
					AddToSelect(vcElement[i], Qt::green);
				}
			}
		}
	}
	else if (m_bIsMouseDowning && m_DrawTool == DRAWTOOL_LINE)
	{
		m_bModifyContent = true;
		CSVGElement *pElement = GetFirstSelectElement();
		if (pElement && pElement->getNodeType() == SVG_LINE)
		{
			if (bCtrl)
			{
				float fx1 = pElement->getFloatAttribute(L"x1");
				float fy1 = pElement->getFloatAttribute(L"y1");
				CSVGPointF pt1(fx1, fy1);
				CSVGPointF retpt = GetFormatPoint(pt1, pt);
				pElement->setFloatAttribute(L"x2", retpt.x);
				pElement->setFloatAttribute(L"y2", retpt.y);
			}
			else
			{
				pElement->setFloatAttribute(L"x2", pt.x);
				pElement->setFloatAttribute(L"y2", pt.y);
			}
		}
		AddToSelect(pElement, Qt::green);
	}
	else if (m_bIsMouseDowning && m_DrawTool == DRAWTOOL_RECT)
	{
		m_bModifyContent = true;
		CSVGElement *pElement = GetFirstSelectElement();
		if (pElement && pElement->getNodeType() == SVG_RECT)
		{
			float fwidth = pt.x - downptlogic.x;
			float fheight = pt.y - downptlogic.y;
			if (bCtrl)
			{
				fheight = fwidth;
			}
			pElement->setFloatAttribute(L"width", fwidth);
			pElement->setFloatAttribute(L"height", fheight);
		}
		AddToSelect(pElement, Qt::green);
	}
	else if (m_bIsMouseDowning && m_DrawTool == DRAWTOOL_CIRCLE)
	{
		m_bModifyContent = true;
		CSVGElement *pElement = GetFirstSelectElement();
		if (pElement && pElement->getNodeType() == SVG_CIRCLE)
		{
			float fwidth = (pt.x - downptlogic.x) / 2;
			float fheight = (pt.y - downptlogic.y) / 2;
			float fr = fwidth > fheight ? fwidth : fheight;
			pElement->setFloatAttribute(L"cx", downptlogic.x + fr);
			pElement->setFloatAttribute(L"cy", downptlogic.y + fr);
			pElement->setFloatAttribute(L"r", fr);
		}
		AddToSelect(pElement, Qt::green);
	}
	else if (m_bIsMouseDowning && m_DrawTool == DRAWTOOL_ELLIPSE)
	{
		m_bModifyContent = true;
		CSVGElement *pElement = GetFirstSelectElement();
		if (pElement && pElement->getNodeType() == SVG_ELLIPSE)
		{
			float frx = (pt.x - downptlogic.x) / 2;
			float fry = (pt.y - downptlogic.y) / 2;
			pElement->setFloatAttribute(L"cx", downptlogic.x + frx);
			pElement->setFloatAttribute(L"cy", downptlogic.y + fry);
			pElement->setFloatAttribute(L"rx", frx);
			pElement->setFloatAttribute(L"ry", fry);
		}
		AddToSelect(pElement, Qt::green);
	}
	else if (m_DrawTool == DRAWTOOL_POLYLINEING)
	{
		m_bModifyContent = true;
		CSVGElement *pElement = GetFirstSelectElement();
		if (pElement && pElement->getNodeType() == SVG_POLYLINE)
		{
			CSVGElementPolyline *pPolyLine = (CSVGElementPolyline*)pElement;
			std::list<CSVGPointF> listpt = pPolyLine->getPoints();
			if (listpt.size() > 0)
				listpt.pop_back();
			CSVGPointF lastpt(pt.x, pt.y);
			CSVGMatrix usemat = pPolyLine->getUsedMatrix();
			usemat.Inverse();
			lastpt = lastpt.matrixTransform(usemat);
			if ((bCtrl || balt) && listpt.size() >= 1)
			{
				CSVGPointF pt1 = listpt.back();
				lastpt = GetFormatPoint(pt1, lastpt);
			}
			listpt.push_back(lastpt);
			if (!balt || listpt.size() <= 2)
			{
				pPolyLine->setAttribute(L"points", pPolyLine->PointsToValue(listpt));
			}
			else if (balt)
			{
				listpt = FormatToRegularPolygon(listpt);
				pPolyLine->setAttribute(L"points", pPolyLine->PointsToValue(listpt));
			}
		}
		AddToSelect(pElement, Qt::green);
	}
	else if (m_DrawTool == DRAWTOOL_POLYGONING)
	{
		m_bModifyContent = true;
		CSVGElement *pElement = GetFirstSelectElement();
		if (pElement && pElement->getNodeType() == SVG_POLYGON)
		{
			CSVGElementPolygon *pPolyGon = (CSVGElementPolygon*)pElement;
			std::list<CSVGPointF> listpt = pPolyGon->getPoints();
			if (listpt.size() > 0)
				listpt.pop_back();
			CSVGPointF lastpt(pt.x, pt.y);
			CSVGMatrix usemat = pPolyGon->getUsedMatrix();
			usemat.Inverse();
			lastpt = lastpt.matrixTransform(usemat);
			if ((bCtrl || balt) && listpt.size() >= 1)
			{
				CSVGPointF pt1 = listpt.back();
				lastpt = GetFormatPoint(pt1, lastpt);
			}
			listpt.push_back(lastpt);
			if (!balt || listpt.size() <= 2)
			{
				pPolyGon->setAttribute(L"points", pPolyGon->PointsToValue(listpt));
			}
			else if (balt)
			{
				listpt = FormatToRegularPolygon(listpt);
				pPolyGon->setAttribute(L"points", pPolyGon->PointsToValue(listpt));
			}
		}
		AddToSelect(pElement, Qt::green);
	}
	else if (m_DrawTool == DRAWTOOL_PATHING)
	{
		m_bModifyContent = true;
		m_bModifyForeground = true;
		CSVGElement *pElement = GetFirstSelectElement();
		if (pElement && pElement->getNodeType() == SVG_PATH)
		{
			CSVGElementPath *pPath = (CSVGElementPath*)pElement;
			CSVGPointF lastpt(pt.x, pt.y);
			CSVGMatrix usemat = pPath->getUsedMatrix();
			usemat.Inverse();
			lastpt = lastpt.matrixTransform(usemat);

			if (m_bIsMouseDowning)
			{
				CSVGPathSeg * pPreSeg = pPath->getLastSeg(2);
				CSVGPathSeg * pLastSeg1 = pPath->getLastSeg(1);
				CSVGPathSeg * pLastSeg0 = pPath->getLastSeg(0);
				if (pLastSeg1->getType() == PATH_L)
				{
					float fx = pLastSeg1->x;
					float fy = pLastSeg1->y;
					pPath->removeSeg(pPath->getLastSeg(0));
					pPath->removeSeg(pPath->getLastSeg(0));
					pLastSeg1 = pPath->createSVGPathSegCurvetoCubic(lastpt.x, lastpt.y, lastpt.x, lastpt.y, fx, fy);
					pLastSeg0 = pPath->createSVGPathSegCurvetoCubic(lastpt.x, lastpt.y, lastpt.x, lastpt.y, fx, fy);
				}
				if (pLastSeg1->getType() == PATH_C)
				{
					CSVGPathSegCurvetoCubic *pCubic0 = (CSVGPathSegCurvetoCubic*)pLastSeg0;
					CSVGPathSegCurvetoCubic *pCubic1 = (CSVGPathSegCurvetoCubic*)pLastSeg1;
					float fx1 = 0;
					float fy1 = 0;
					if (pPreSeg)
					{
						if (pPreSeg->getType() == PATH_C)
						{
							CSVGPathSegCurvetoCubic *pPreCublic = (CSVGPathSegCurvetoCubic *)pPreSeg;
							fx1 = 2 * pPreCublic->x - pPreCublic->x2;
							fy1 = 2 * pPreCublic->y - pPreCublic->y2;
						}
						else
						{
							fx1 = pPreSeg->x;
							fy1 = pPreSeg->y;
						}
					}
					pCubic1->x1 = fx1;
					pCubic1->y1 = fy1;
					float fx = pCubic1->x;
					float fy = pCubic1->y;
					pCubic1->x2 = 2 * fx - lastpt.x;
					pCubic1->y2 = 2 * fy - lastpt.y;

					pCubic0->x1 = pCubic1->x;
					pCubic0->y1 = pCubic1->y;
					pCubic0->x = pCubic1->x;
					pCubic0->y = pCubic1->y;
					pCubic0->x2 = pCubic0->x;
					pCubic0->y2 = pCubic0->y;

					std::wstring strd = pPath->pathToString();
					pPath->setAttribute(L"d", strd);

				}
				m_FocusManager.ResetFocus(pPath, pPath->GetPathSegSize() - 2);
			}
			else
			{
				CSVGPathSeg * pLastSeg1 = pPath->getLastSeg(1);
				CSVGPathSeg * pLastSeg = pPath->getLastSeg(0);
				if (pLastSeg)
				{
					if (pLastSeg->getType() == PATH_C)
					{
						CSVGPathSegCurvetoCubic *pLstCublic = (CSVGPathSegCurvetoCubic *)pLastSeg;
						if (pLastSeg)
						{
							if (pLastSeg1->getType() == PATH_C)
							{
								CSVGPathSegCurvetoCubic *pCubic = (CSVGPathSegCurvetoCubic*)pLastSeg1;
								pLstCublic->x1 = 2 * pCubic->x - pCubic->x2;
								pLstCublic->y1 = 2 * pCubic->y - pCubic->y2;
								pLstCublic->x = lastpt.x;
								pLstCublic->y = lastpt.y;
								pLstCublic->x2 = lastpt.x;
								pLstCublic->y2 = lastpt.y;
							}
						}
					}
					else
					{
						pLastSeg->x = lastpt.x;
						pLastSeg->y = lastpt.y;
					}
					std::wstring strd = pPath->pathToString();
					pPath->setAttribute(L"d", strd);
					m_FocusManager.ResetFocus(pPath);
				}
			}
		}
	}
	else if (m_DrawTool == DRAWTOOL_SELECT)
	{
		CSVGElementShape *pShape = getShapeByPoint(pt.x, pt.y);
		if (pShape)
		{
			if (pShape->getNodeType() != SVG_TEXT)
			{
				mouseshape = MOUSE_SELECT;
			}
		}
		if (m_vcSelectShapes.size() == 1)
		{
			CSVGElement *pElement = m_vcSelectShapes[0];
			FocusPointInfo pointinfo = m_FocusManager.GetElementFocusPointInfo(pElement, pt);
			if (pointinfo.nIndex == 10000 || pointinfo.nIndex == 10004)
			{
				mouseshape = MOUSE_NW;
			}
			else if (pointinfo.nIndex == 10002 || pointinfo.nIndex == 10006)
			{
				mouseshape = MOUSE_NE;
			}
			else if (pointinfo.nIndex == 10001 || pointinfo.nIndex == 10005)
			{
				mouseshape = MOUSE_NS;
			}
			else if (pointinfo.nIndex == 10003 || pointinfo.nIndex == 10007)
			{
				mouseshape = MOUSE_WE;
			}
			else if (pointinfo.nIndex >= 0 || pointinfo.nCtrlIndex >= 0)
			{
				mouseshape = MOUSE_SIZETO;
			}
		}
	}
	m_lstChangeDownPt = point;
	m_lstMiddleDownPt = point;

	return mouseshape;
}

void CSVGDocument::SetNeedRepaintAll()
{
	std::list<AreaManager *>::iterator iter = m_AreaManagerList.begin();
	for (; iter != m_AreaManagerList.end(); ++iter)
	{
		delete (*iter)->m_pbitmap;
		(*iter)->m_pbitmap = NULL;
	}
}

void CSVGDocument::SetContentHasChange()
{
	m_bModifyContent = true;
}

void CSVGDocument::BeginElementSizeTo()
{
	if (m_pActionAttrs)
		return;
	if (!m_lstFocusPointInfo.pElement)
		return;
	m_pActionAttrs = new CActionComposite(this);
	m_ActionHistory.AddAction(m_pActionAttrs);
	CSVGElement *pElement = m_lstFocusPointInfo.pElement;
	if (pElement->getNodeType() == SVG_LINE)
	{
		CActionAttr *pAttr1 = new CActionAttr(this, pElement, L"x1");
		m_pActionAttrs->PushBack(pAttr1);
		CActionAttr *pAttr2 = new CActionAttr(this, pElement, L"y1");
		m_pActionAttrs->PushBack(pAttr2);
		CActionAttr *pAttr3 = new CActionAttr(this, pElement, L"x2");
		m_pActionAttrs->PushBack(pAttr3);
		CActionAttr *pAttr4 = new CActionAttr(this, pElement, L"y2");
		m_pActionAttrs->PushBack(pAttr4);

	}
	else if (pElement->getNodeType() == SVG_RECT)
	{
		CActionAttr *pAttr1 = new CActionAttr(this, pElement, L"x");
		m_pActionAttrs->PushBack(pAttr1);
		CActionAttr *pAttr2 = new CActionAttr(this, pElement, L"y");
		m_pActionAttrs->PushBack(pAttr2);
		CActionAttr *pAttr3 = new CActionAttr(this, pElement, L"width");
		m_pActionAttrs->PushBack(pAttr3);
		CActionAttr *pAttr4 = new CActionAttr(this, pElement, L"height");
		m_pActionAttrs->PushBack(pAttr4);
	}
	else if (pElement->getNodeType() == SVG_IMAGE)
	{
		CActionAttr *pAttr1 = new CActionAttr(this, pElement, L"transform");
		m_pActionAttrs->PushBack(pAttr1);
	}
	else if (pElement->getNodeType() == SVG_CIRCLE)
	{
		CActionAttr *pAttr1 = new CActionAttr(this, pElement, L"cx");
		m_pActionAttrs->PushBack(pAttr1);
		CActionAttr *pAttr2 = new CActionAttr(this, pElement, L"cy");
		m_pActionAttrs->PushBack(pAttr2);
		CActionAttr *pAttr3 = new CActionAttr(this, pElement, L"r");
		m_pActionAttrs->PushBack(pAttr3);
	}
	else if (pElement->getNodeType() == SVG_ELLIPSE)
	{
		CActionAttr *pAttr1 = new CActionAttr(this, pElement, L"cx");
		m_pActionAttrs->PushBack(pAttr1);
		CActionAttr *pAttr2 = new CActionAttr(this, pElement, L"cy");
		m_pActionAttrs->PushBack(pAttr2);
		CActionAttr *pAttr3 = new CActionAttr(this, pElement, L"rx");
		m_pActionAttrs->PushBack(pAttr3);
		CActionAttr *pAttr4 = new CActionAttr(this, pElement, L"ry");
		m_pActionAttrs->PushBack(pAttr4);
	}
	else if (pElement->getNodeType() == SVG_POLYLINE
		|| pElement->getNodeType() == SVG_POLYGON)
	{
		CActionAttr *pAttr1 = new CActionAttr(this, pElement, L"points");
		m_pActionAttrs->PushBack(pAttr1);
	}
	else if (pElement->getNodeType() == SVG_PATH)
	{
		CActionAttr *pAttr1 = new CActionAttr(this, pElement, L"d");
		m_pActionAttrs->PushBack(pAttr1);
	}
	else if (pElement->getNodeType() == SVG_TEXT)
	{
		CActionAttr *pAttr1 = new CActionAttr(this, pElement, L"transform");
		m_pActionAttrs->PushBack(pAttr1);
	}
	else if (pElement->getNodeType() == SVG_G)
	{
		CSVGElementG *pGElement = (CSVGElementG*)pElement;
		std::wstring strwidgettype = pGElement->getAttribute(L"tywidgettype");
		if (strwidgettype.length() > 0)
		{
			CActionAttr *pAttr1 = new CActionAttr(this, pElement, L"tyx");
			m_pActionAttrs->PushBack(pAttr1);
			CActionAttr *pAttr2 = new CActionAttr(this, pElement, L"tyy");
			m_pActionAttrs->PushBack(pAttr2);
			CActionAttr *pAttr3 = new CActionAttr(this, pElement, L"tywidth");
			m_pActionAttrs->PushBack(pAttr3);
			CActionAttr *pAttr4 = new CActionAttr(this, pElement, L"tyheight");
			m_pActionAttrs->PushBack(pAttr4);
		}
		else
		{
			CActionAttr *pAttr1 = new CActionAttr(this, pElement, L"transform");
			m_pActionAttrs->PushBack(pAttr1);
		}
	}
}

void CSVGDocument::BeginElementTransformAction(CActionComposite *pComposite)
{
	if (m_pActionAttrs)
		return;
	m_pActionAttrs = new CActionComposite(this);
	if (pComposite)
	{
		pComposite->PushBack(m_pActionAttrs);
	}
	else
	{
		m_ActionHistory.AddAction(m_pActionAttrs);
	}
	for (int i = 0; i < m_vcSelectShapes.size(); ++i)
	{
		CActionAttr *pActionAttr = new CActionAttr(this, m_vcSelectShapes[i], L"transform");
		m_pActionAttrs->PushBack(pActionAttr);
	}
}

void CSVGDocument::EndElementAttrAction()
{
	if (m_pActionAttrs)
	{
		std::vector<CActionBase *>&Actions = m_pActionAttrs->GetActions();
		for (int i = 0; i < (int)Actions.size(); ++i)
		{
			if (Actions[i]->m_ActionType == ACTION_ATTR)
			{
				CActionAttr *pActionAttr = (CActionAttr*)Actions[i];
				pActionAttr->EndValue();
			}
		}
		m_pActionAttrs = NULL;
	}
}

void CSVGDocument::ElementSizeTo(const CSVGPointF &point, bool bCtrl)
{
	if (!m_lstFocusPointInfo.pElement)
		return;

	CSVGElement *pElement = m_lstFocusPointInfo.pElement;
	int nIndex = m_lstFocusPointInfo.nIndex;
	int nCtrlIndex = m_lstFocusPointInfo.nCtrlIndex;
	CSVGMatrix mat = m_lstFocusPointInfo.pElement->getUsedMatrix();
	CSVGPointF pt = point;
	mat.Inverse();
	pt = pt.matrixTransform(mat);
	if (pElement->getNodeType() == SVG_LINE)
	{
		if (nIndex == 0)
		{
			if (bCtrl)
			{
				float fx2 = pElement->getFloatAttribute(L"x2");
				float fy2 = pElement->getFloatAttribute(L"y2");
				CSVGPointF pt2(fx2, fy2);
				pt = GetFormatPoint(pt2, pt);
			}
			pElement->setFloatAttribute(L"x1", pt.x);
			pElement->setFloatAttribute(L"y1", pt.y);
		}
		else if (nIndex == 1)
		{
			if (bCtrl)
			{
				float fx1 = pElement->getFloatAttribute(L"x1");
				float fy1 = pElement->getFloatAttribute(L"y1");
				CSVGPointF pt1(fx1, fy1);
				pt = GetFormatPoint(pt1, pt);
			}
			pElement->setFloatAttribute(L"x2", pt.x);
			pElement->setFloatAttribute(L"y2", pt.y);
		}
	}
	else if (pElement->getNodeType() == SVG_RECT)
	{
		float fx = pElement->getFloatAttribute(L"x");
		float fy = pElement->getFloatAttribute(L"y");
		float fwidth = pElement->getFloatAttribute(L"width");
		float fheight = pElement->getFloatAttribute(L"height");
		CSVGPointF pt1(fx, fy);
		CSVGPointF pt3(fx + fwidth, fy + fheight);
		if (nIndex == 10000)
		{
			pt1 = pt;
		}
		else if (nIndex == 10001)
		{
			pt1.y = pt.y;
		}
		else if (nIndex == 10002)
		{
			pt1.y = pt.y;
			pt3.x = pt.x;
		}
		else if (nIndex == 10003)
		{
			pt3.x = pt.x;
		}
		else if (nIndex == 10004)
		{
			pt3 = pt;
		}
		else if (nIndex == 10005)
		{
			pt3.y = pt.y;
		}
		else if (nIndex == 10006)
		{
			pt1.x = pt.x;
			pt3.y = pt.y;
		}
		else if (nIndex == 10007)
		{
			pt1.x = pt.x;
		}
		pElement->setFloatAttribute(L"x", pt1.x);
		pElement->setFloatAttribute(L"y", pt1.y);
		pElement->setFloatAttribute(L"width", pt3.x - pt1.x);
		pElement->setFloatAttribute(L"height", pt3.y - pt1.y);
	}
	else if (pElement->getNodeType() == SVG_CIRCLE)
	{
		float fcx = pElement->getFloatAttribute(L"cx");
		float fcy = pElement->getFloatAttribute(L"cy");
		float fr = pElement->getFloatAttribute(L"r");
		float fx = fcx - fr;
		float fy = fcy - fr;
		float fwidth = 2 * fr;
		float fheight = 2 * fr;
		CSVGPointF ptLeftTop = CSVGPointF(fx, fy);
		CSVGPointF ptRightBottom = CSVGPointF(fx + fwidth, fy + fheight);
		switch (nIndex - 10000)
		{
		case 0:
		{
			float fNewWidth = ptRightBottom.x - pt.x;
			float fNewHeight = ptRightBottom.y - pt.y;
			float fNewR = std::max(fNewHeight, fNewWidth);
			ptLeftTop.x = ptRightBottom.x - fNewR;
			ptLeftTop.y = ptRightBottom.y - fNewR;
			break;
		}
		case 1:
		{
			//0   1   2
			//7       3
			//6   5   4   //5
			float fNewR = ptRightBottom.y - pt.y;
			float fx5 = fcx;
			float fy5 = fcy + fr;

			ptLeftTop.x = fx5 - fNewR / 2;
			ptLeftTop.y = fx5 - fNewR;
			ptRightBottom.x = fx5 + fNewR / 2;
			ptRightBottom.y = fy5;
			break;
		}
		case 2:
		{
			//0   1   2
			//7       3
			//6   5   4  //6
			float fNewWidth = pt.x - ptLeftTop.x;
			float fNewHeight = ptRightBottom.y - pt.y;
			float fNewR = std::max(fNewHeight, fNewWidth);
			float fx6 = fcx - fr;
			float fy6 = fcy + fr;
			ptLeftTop.x = fx6;
			ptLeftTop.y = fy6 - fNewR;
			ptRightBottom.x = fx6 + fNewR;
			ptRightBottom.y = fy6;
			break;
		}
		case 3:
		{
			//0   1   2
			//7       3
			//6   5   4  //7
			float fNewR = pt.x - ptLeftTop.x;
			float fx7 = fcx - fr;
			float fy7 = fcy;

			ptLeftTop.x = fx7;
			ptLeftTop.y = fy7 - fNewR / 2;
			ptRightBottom.x = fx7 + fNewR;
			ptRightBottom.y = fy7 + fNewR / 2;

			break;
		}
		case 4:
		{
			//0   1   2
			//7       3
			//6   5   4  //0
			float fNewWidth = pt.x - ptLeftTop.x;
			float fNewHeight = pt.y - ptLeftTop.y;
			float fNewR = std::max(fNewHeight, fNewWidth);

			float fx0 = fcx - fr;
			float fy0 = fcy - fr;
			ptLeftTop.x = fx0;
			ptLeftTop.y = fy0;
			ptRightBottom.x = fx0 + fNewR;
			ptRightBottom.y = fy0 + fNewR;
			break;
		}
		case 5:
		{
			//0   1   2
			//7       3
			//6   5   4  //1
			float fNewR = pt.y - ptLeftTop.y;
			float fx1 = fcx;
			float fy1 = fcy - fr;
			ptLeftTop.x = fx1 - fNewR / 2;
			ptLeftTop.y = fy1;
			ptRightBottom.x = fx1 + fNewR / 2;
			ptRightBottom.y = fy1 + fNewR;

			ptRightBottom.y = pt.y;
			break;
		}
		case 6:
		{
			//0   1   2
			//7       3
			//6   5   4  //2
			float fx2 = fcx + fr;
			float fy2 = fcy - fr;

			float fNewWidth = fx2 - pt.x;
			float fNewHeight = pt.y - fy2;
			float fNewR = std::max(fNewHeight, fNewWidth);
			ptLeftTop.x = fx2 - fNewR;
			ptLeftTop.y = fy2;
			ptRightBottom.x = fx2;
			ptRightBottom.y = fy2 + fNewR;
			break;
		}
		case 7:
		{
			//0   1   2
			//7       3
			//6   5   4  //3
			float fx3 = fcx + fr;
			float fy3 = fcy;

			float fNewR = fx3 - pt.x;
			ptLeftTop.x = fx3 - fNewR;
			ptLeftTop.y = fy3 - fNewR / 2;
			ptRightBottom.x = fx3;
			ptRightBottom.y = fy3 + fNewR / 2;
			break;
		}
		default:
			break;
		}
		fcx = (ptRightBottom.x + ptLeftTop.x)*1.0 / 2;
		fcy = (ptRightBottom.y + ptLeftTop.y)*1.0 / 2;
		fr = (ptRightBottom.x - ptLeftTop.x)*1.0 / 2;
		pElement->setFloatAttribute(L"cx", fcx);
		pElement->setFloatAttribute(L"cy", fcy);
		pElement->setFloatAttribute(L"r", fr);
	}
	else if (pElement->getNodeType() == SVG_ELLIPSE)
	{
		float fcx = pElement->getFloatAttribute(L"cx");
		float fcy = pElement->getFloatAttribute(L"cy");
		float frx = pElement->getFloatAttribute(L"rx");
		float fry = pElement->getFloatAttribute(L"ry");
		float fx = fcx - frx;
		float fy = fcy - fry;
		float fwidth = 2 * frx;
		float fheight = 2 * fry;
		CSVGPointF ptLeftTop = CSVGPointF(fx, fy);
		CSVGPointF ptRightBottom = CSVGPointF(fx + fwidth, fy + fheight);
		//0     1     2
		//
		//7           3
		//
		//6     5     4
		switch (nIndex - 10000)
		{
		case 0:
		{
			ptLeftTop = pt;
			break;
		}
		case 1:
		{
			ptLeftTop.y = pt.y;
			break;
		}
		case 2:
		{
			ptLeftTop.y = pt.y;
			ptRightBottom.x = pt.x;
			break;
		}
		case 3:
		{
			ptRightBottom.x = pt.x;
			break;
		}
		case 4:
		{
			ptRightBottom = pt;
			break;
		}
		case 5:
		{
			ptRightBottom.y = pt.y;
			break;
		}
		case 6:
		{
			ptLeftTop.x = pt.x;
			ptRightBottom.y = pt.y;
			break;
		}
		case 7:
		{
			ptLeftTop.x = pt.x;
			break;
		}
		default:
			break;
		}

		fcx = (ptRightBottom.x + ptLeftTop.x)*1.0 / 2;
		fcy = (ptRightBottom.y + ptLeftTop.y)*1.0 / 2;
		frx = (ptRightBottom.x - ptLeftTop.x)*1.0 / 2;
		fry = (ptRightBottom.y - ptLeftTop.y)*1.0 / 2;
		pElement->setFloatAttribute(L"cx", fcx);
		pElement->setFloatAttribute(L"cy", fcy);
		pElement->setFloatAttribute(L"rx", frx);
		pElement->setFloatAttribute(L"ry", fry);
	}
	else if (pElement->getNodeType() == SVG_POLYLINE
		|| pElement->getNodeType() == SVG_POLYGON)
	{
		CSVGElementPolyline *pPolyline = (CSVGElementPolyline*)pElement;
		std::list<CSVGPointF> ptlist = pPolyline->getPoints();
		std::list<CSVGPointF>::iterator iter = ptlist.begin();
		for (int i = 0; iter != ptlist.end(); ++iter, ++i)
		{
			if (bCtrl && i == (nIndex - 1) && nIndex < (int)ptlist.size())
			{
				float fx1 = iter->x;
				float fy1 = iter->y;
				CSVGPointF pt1(fx1, fy1);
				++iter;
				CSVGPointF retpt = GetFormatPoint(pt1, pt);
				iter->x = retpt.x;
				iter->y = retpt.y;
				break;
			}
			else if (i == nIndex)
			{
				iter->x = pt.x;
				iter->y = pt.y;
				break;
			}
		}
		pPolyline->setAttribute(L"points", pPolyline->PointsToValue(ptlist));
	}
	else if (pElement->getNodeType() == SVG_PATH)
	{
		CSVGElementPath *pPath = (CSVGElementPath*)pElement;
		CSVGPathSeg *pSeg = pPath->getBeginSeg(nIndex);
		if (pSeg)
		{
			if (nCtrlIndex == 0)
			{
				if (pSeg->getType() == PATH_C)
				{
					CSVGPathSegCurvetoCubic *pCubic = (CSVGPathSegCurvetoCubic*)pSeg;
					pCubic->x2 = pt.x;
					pCubic->y2 = pt.y;
					CSVGPathSeg *pNextSeg = pPath->getBeginSeg(nIndex + 1);
					if (pNextSeg && pNextSeg->getType() == PATH_C)
					{
						CSVGPathSegCurvetoCubic *pNextCubic = (CSVGPathSegCurvetoCubic*)pNextSeg;
						pNextCubic->x1 = 2 * pCubic->x - pCubic->x2;
						pNextCubic->y1 = 2 * pCubic->y - pCubic->y2;
					}
				}
			}
			else if (nCtrlIndex == 1)
			{
				if (pSeg->getType() == PATH_C)
				{
					CSVGPathSegCurvetoCubic *pCubic = (CSVGPathSegCurvetoCubic*)pSeg;
					CSVGPathSeg *pNextSeg = pPath->getBeginSeg(nIndex + 1);
					if (pNextSeg && pNextSeg->getType() == PATH_C)
					{
						CSVGPathSegCurvetoCubic *pNextCubic = (CSVGPathSegCurvetoCubic*)pNextSeg;
						pNextCubic->x1 = pt.x;
						pNextCubic->y1 = pt.y;
					}
					pCubic->x2 = 2 * pCubic->x - pt.x;
					pCubic->y2 = 2 * pCubic->y - pt.y;
				}
			}
			else
			{
				float foffsetx = pt.x - pSeg->x;
				float foffsety = pt.y - pSeg->y;
				pSeg->x = pt.x;
				pSeg->y = pt.y;
				if (pSeg->getType() == PATH_C)
				{
					CSVGPathSegCurvetoCubic *pCubic = (CSVGPathSegCurvetoCubic*)pSeg;
					pCubic->x2 += foffsetx;
					pCubic->y2 += foffsety;
					CSVGPathSeg *pNextSeg = pPath->getBeginSeg(nIndex + 1);
					if (pNextSeg && pNextSeg->getType() == PATH_C)
					{
						CSVGPathSegCurvetoCubic *pNextCubic = (CSVGPathSegCurvetoCubic*)pNextSeg;
						pNextCubic->x1 += foffsetx;
						pNextCubic->y1 += foffsety;
					}
				}
			}
		}
		std::wstring strd = pPath->pathToString();
		pPath->setAttribute(L"d", strd);
	}
	else if (pElement->getNodeType() == SVG_IMAGE)
	{
		pt = point;
		CSVGElementShape *pShape = (CSVGElementShape*)pElement;
		CSVGRectF oldbbox = pShape->getMatrixBBox();
		float fx = oldbbox.x;
		float fy = oldbbox.y;
		float fwidth = oldbbox.width;
		float fheight = oldbbox.height;
		if (fwidth < 5)
			fwidth = 5;
		if (fheight < 5)
			fheight = 5;

		CSVGPointF pt1(fx, fy);
		CSVGPointF pt3(fx + fwidth, fy + fheight);

		if (nIndex == 10000)
		{
			pt1 = pt;
		}
		else if (nIndex == 10001)
		{
			pt1.y = pt.y;
		}
		else if (nIndex == 10002)
		{
			pt1.y = pt.y;
			pt3.x = pt.x;
		}
		else if (nIndex == 10003)
		{
			pt3.x = pt.x;
		}
		else if (nIndex == 10004)
		{
			pt3 = pt;
		}
		else if (nIndex == 10005)
		{
			pt3.y = pt.y;
		}
		else if (nIndex == 10006)
		{
			pt1.x = pt.x;
			pt3.y = pt.y;
		}
		else if (nIndex == 10007)
		{
			pt1.x = pt.x;
		}

		float fnewwidth = pt3.x - pt1.x;
		float fnewheight = pt3.y - pt1.y;
		if (fnewwidth > 5 && fnewheight > 5)
		{
			float fscalex = fnewwidth / fwidth;
			float fscaley = fnewheight / fheight;
			CSVGMatrix mat = pElement->getMatrix();
			mat.LTranslate(-fx, -fy);
			mat.LScale(fscalex, fscaley);
			mat.LTranslate(pt1.x, pt1.y);
			pShape->setAttribute(L"transform", mat.getText());
			pShape->resetUseMatrix();
			m_FocusManager.ResetAllFocus();
		}
	}
	else if (pElement->getNodeType() == SVG_G)
	{
		CSVGElementG *pShape = (CSVGElementG*)pElement;
		std::wstring tywidgettype = pShape->getAttribute(L"tywidgettype");
		if (tywidgettype.length() > 0)
		{
			float fx = pElement->getFloatAttribute(L"tyx");
			float fy = pElement->getFloatAttribute(L"tyy");
			float fwidth = pElement->getFloatAttribute(L"tywidth");
			float fheight = pElement->getFloatAttribute(L"tyheight");
			CSVGPointF pt1(fx, fy);
			CSVGPointF pt3(fx + fwidth, fy + fheight);
			if (nIndex == 10000)
			{
				pt1 = pt;
			}
			else if (nIndex == 10001)
			{
				pt1.y = pt.y;
			}
			else if (nIndex == 10002)
			{
				pt1.y = pt.y;
				pt3.x = pt.x;
			}
			else if (nIndex == 10003)
			{
				pt3.x = pt.x;
			}
			else if (nIndex == 10004)
			{
				pt3 = pt;
			}
			else if (nIndex == 10005)
			{
				pt3.y = pt.y;
			}
			else if (nIndex == 10006)
			{
				pt1.x = pt.x;
				pt3.y = pt.y;
			}
			else if (nIndex == 10007)
			{
				pt1.x = pt.x;
			}
			pShape->setFloatAttribute(L"tyx", pt1.x);
			pShape->setFloatAttribute(L"tyy", pt1.y);
			pShape->setFloatAttribute(L"tywidth", pt3.x - pt1.x);
			pShape->setFloatAttribute(L"tyheight", pt3.y - pt1.y);
			m_FocusManager.ResetAllFocus();
		}
		else
		{
			pt = point;
			CSVGRectF oldbbox = pShape->getMatrixBBox();
			float fx = oldbbox.x;
			float fy = oldbbox.y;
			float fwidth = oldbbox.width;
			float fheight = oldbbox.height;
			if (fwidth < 5)
				fwidth = 5;
			if (fheight < 5)
				fheight = 5;

			CSVGPointF pt1(fx, fy);
			CSVGPointF pt3(fx + fwidth, fy + fheight);

			if (nIndex == 10000)
			{
				pt1 = pt;
			}
			else if (nIndex == 10001)
			{
				pt1.y = pt.y;
			}
			else if (nIndex == 10002)
			{
				pt1.y = pt.y;
				pt3.x = pt.x;
			}
			else if (nIndex == 10003)
			{
				pt3.x = pt.x;
			}
			else if (nIndex == 10004)
			{
				pt3 = pt;
			}
			else if (nIndex == 10005)
			{
				pt3.y = pt.y;
			}
			else if (nIndex == 10006)
			{
				pt1.x = pt.x;
				pt3.y = pt.y;
			}
			else if (nIndex == 10007)
			{
				pt1.x = pt.x;
			}

			float fnewwidth = pt3.x - pt1.x;
			float fnewheight = pt3.y - pt1.y;
			if (fnewwidth > 5 && fnewheight > 5)
			{
				float fscalex = fnewwidth / fwidth;
				float fscaley = fnewheight / fheight;
				CSVGMatrix mat = pElement->getMatrix();
				mat.LTranslate(-fx, -fy);
				mat.LScale(fscalex, fscaley);
				mat.LTranslate(pt1.x, pt1.y);
				pShape->setAttribute(L"transform", mat.getText());
				pShape->resetUseMatrix();
				m_FocusManager.ResetAllFocus();
			}
		}
	}
	m_FocusManager.ResetFocus(pElement, nIndex);
	m_bModifyForeground = true;
}

void CSVGDocument::SelectAll()
{
	ClearAllSelect();
	for (int i = 0; i < (int)m_svgRootElement->getChildCount(); ++i)
	{
		CSVGElement *pChild = m_svgRootElement->getChild(i);
		if (pChild->IsShapeElement())
		{
			AddToSelect(pChild, Qt::green);
		}
	}
	m_bModifyForeground = true;
}

std::wstring CSVGDocument::Copy()
{
	std::wstring strrefText;
	std::sort(m_vcSelectShapes.begin(), m_vcSelectShapes.end(), SortElementByIndex);
	for (int i = 0; i < m_vcSelectShapes.size(); ++i)
	{
		CSVGElement *pShape = m_vcSelectShapes[i];
		ParseGetRefText(pShape, pShape, strrefText);
		pShape->Toxml(strrefText, 0);
		strrefText += L"\n";
	}
	return strrefText;
}

std::wstring CSVGDocument::GetShapeFull(CSVGElement *pElement)
{
	std::wstring strrefText;
	ParseGetRefText(pElement, pElement, strrefText);
	pElement->Toxml(strrefText, 0);
	strrefText += L"\n";
	return strrefText;
}

void CSVGDocument::CopyElementTo(CSVGElement *pSrc, CSVGElement *pToElement)
{
	if (!pSrc || !pToElement)
		return;
	std::wstring strfill = pSrc->getAttribute(L"fill");
	std::wstring strid = global.UrlStringGetId(strfill);
	CSVGElement* pNewElement = NULL;
	if (strid.length() > 0)
	{
		CSVGElement *pElement = getElementById(strid.c_str());
		if (pElement)
		{
			pNewElement = pElement->cloneNode(false);
			pNewElement->setAttribute(L"id", pSrc->getAttributeId());
			pToElement->addChild(pNewElement, false, false);
		}
	}
	std::wstring strclass = pSrc->getAttribute(L"class");
	if (strclass.length() > 0)
	{
		CSVGElementStyleClass *pClass = this->getClassByName(strclass);
		if (pClass)
		{
			CSVGElementStyleClass *pNewClass = pClass->Clone();
			CSVGElementStyle * pStyle = pToElement->GetOrNewStyle();
			if (pStyle)
			{
				if (!pStyle->addClass(pNewClass))
				{
					delete pNewClass;
				}
			}
		}
	}
	for (unsigned int i = 0; i < (int)pSrc->getChildCount(); ++i)
	{
		CSVGElement *pNode = pSrc->getChild(i);;
		if (pNode->IsShapeElement())
		{
			CSVGElementShape *pElement = (CSVGElementShape*)pNode;
			CopyElementTo(pElement, pNewElement);
		}
	}
}

std::wstring CSVGDocument::CopyToXml(std::vector<CSVGElement*>&vcElement)
{
	ResetUrlUsed();
	std::wstring xml;
	CSVGDocument doc;
	doc.NewDocument();
	CSVGElement * pRoot = doc.GetRootSVGElement();
	for (int i = 0; i < (int)vcElement.size(); ++i)
	{
		CopyElementTo(vcElement[i], pRoot);
	}
	return xml;
}

void CSVGDocument::MoveSelect(int nx, int ny)
{
	if (m_vcSelectShapes.size() > 0)
	{
		BeginElementTransformAction();
		for (int i = 0; i < m_vcSelectShapes.size(); ++i)
		{
			CSVGMatrix mat = m_vcSelectShapes[i]->getMatrix();
			mat.LTranslate(nx, ny);
			m_vcSelectShapes[i]->setAttribute(L"transform", mat.getText());
			m_vcSelectShapes[i]->resetUseMatrix();
			m_FocusManager.AddElement(m_vcSelectShapes[i], Qt::green);
		}
		EndElementAttrAction();
		m_bModifyForeground = true;
	}
}

void CSVGDocument::Paste(const std::wstring &str, QPoint centerpoint)
{
	CSVGPointF pt(centerpoint.x(), centerpoint.y());
	CSVGMatrix screenmat = m_screenMat;
	screenmat.Inverse();
	pt = pt.matrixTransform(screenmat);
	CActionComposite * pComposite = Paste(str);
	float  fx1 = 9999999999999.0f;
	float  fy1 = 9999999999999.0f;
	float  fx2 = -999999999999.0f;
	float  fy2 = -999999999999.0f;
	for (int i = 0; i < m_vcSelectShapes.size(); ++i)
	{
		if (m_vcSelectShapes[i]->IsShapeElement())
		{
			CSVGElementShape *pShape = (CSVGElementShape*)m_vcSelectShapes[i];
			CSVGRectF bbox = pShape->getMatrixBBox();
			if (bbox.x < fx1)
			{
				fx1 = bbox.x;
			}
			if (bbox.y < fy1)
			{
				fy1 = bbox.y;
			}
			if (bbox.x + bbox.width > fx2)
			{
				fx2 = bbox.x + bbox.width;
			}
			if (bbox.y + bbox.height > fy2)
			{
				fy2 = bbox.y + bbox.height;
			}
		}
	}
	float foffsetx = pt.x - (fx2 + fx1) / 2;
	float foffsety = pt.y - (fy2 + fy1) / 2;
	BeginElementTransformAction(pComposite);
	for (int i = 0; i < m_vcSelectShapes.size(); ++i)
	{
		CSVGMatrix  mat = m_vcSelectShapes[i]->getMatrix();
		mat.LTranslate(foffsetx, foffsety);
		m_vcSelectShapes[i]->setAttribute(L"transform", mat.getText());
		m_vcSelectShapes[i]->resetUseMatrix();
		m_FocusManager.AddElement(m_vcSelectShapes[i], Qt::green);
	}
	EndElementAttrAction();
	m_bModifyForeground = true;
	SetContentHasChange();
}

CActionComposite * CSVGDocument::Paste(const std::wstring &str)
{
	if (m_nTextFocusPos >= 0)
	{
		CSVGElement *pElement = GetAloneSelectElement(true);
		if (pElement && pElement->getNodeType() == SVG_TEXT)
		{
			CActionComposite *pComposite = new CActionComposite(this);
			m_ActionHistory.AddAction(pComposite);
			CActionValue *pActionVal = new CActionValue(this, pElement);
			pComposite->PushBack(pActionVal);
			int nLength = str.length();
			CSVGElementText *pTextElement = (CSVGElementText*)pElement;
			std::wstring strnodeval = pTextElement->getNodeValue();
			QString strVal = QString::fromStdWString(strnodeval);
			strVal = strVal.insert(m_nTextFocusPos, QString::fromStdWString(str));
			pTextElement->setNodeValue(strVal.toStdWString());
			pActionVal->EndValue();
			pTextElement->resetPath();
			m_nTextFocusPos += nLength;
			m_FocusManager.ResetFocus(pTextElement, m_nTextFocusPos);
			SetContentHasChange();
			m_bModifyForeground = true;
			return pComposite;
		}
	}
	std::wstring strText = str;
	if (strText.find(L"<svg") == std::wstring::npos)
	{
		strText = L"<svg>" + strText + L"</svg>";
	}

	CSVGDocument doc;
	if (doc.LoadFromBuff(strText))
	{
		ClearAllSelect();
		CActionComposite *pComposite = new CActionComposite(this);
		m_ActionHistory.AddAction(pComposite);
		doc.m_nIdIndex = m_nIdIndex;
		std::set<std::wstring> ids = doc.GetIdsSet();
		doc.ChangeAllId(ids);
		QVector<CSVGElement*>vcChild;
		for (int i = 0; i < (int)doc.m_svgRootElement->getChildCount(); ++i)
		{
			CSVGElement* pElement = doc.m_svgRootElement->getChild(i);
			vcChild.push_back(pElement);
		}
		for (int i = 0; i < vcChild.size(); ++i)
		{
			doc.m_svgRootElement->removeChild(vcChild[i]);
			m_svgRootElement->appendChild(vcChild[i]);
			vcChild[i]->resetUseMatrix();
			CActionAdd *pAddAction = new CActionAdd(this, m_svgRootElement, vcChild[i]);
			pComposite->PushBack(pAddAction);
			AddToSelect(vcChild[i], Qt::green);
		}
		ReSetAllElementIndex();
		SetContentHasChange();
		m_bModifyForeground = true;
		return pComposite;
	}
	return NULL;
}

bool CSVGDocument::CanUndo()
{
	return m_ActionHistory.CanUndo();
}

bool CSVGDocument::CanRedo()
{
	return m_ActionHistory.CanRedo();
}

void CSVGDocument::Undo()
{
	if (m_DrawTool == DRAWTOOL_PATHING)
	{
		CSVGElement *pElement = GetFirstSelectElement();
		if (pElement->getNodeType() == SVG_PATH)
		{
			CSVGElementPath *pPath = (CSVGElementPath*)pElement;
			if (pPath->GetPathSegSize() > 1)
			{
				CSVGPathSeg * pLastSeg = pPath->getLastSeg(0);
				if (pLastSeg)
				{
					pPath->removeSeg(pLastSeg);
				}
				std::wstring strd = pPath->pathToString();
				pPath->setAttribute(L"d", strd);
				SetContentHasChange();
				m_bModifyForeground = true;
				m_FocusManager.ResetFocus(pPath);
			}
		}
	}
	else if (m_DrawTool == DRAWTOOL_POLYLINEING
		|| m_DrawTool == DRAWTOOL_POLYGONING)
	{
		CSVGElement *pElement = GetFirstSelectElement();
		if (pElement->getNodeType() == SVG_POLYLINE
			|| pElement->getNodeType() == SVG_POLYGON)
		{
			CSVGElementPolyline *pPolyline = (CSVGElementPolyline*)pElement;
			std::list<CSVGPointF> lspt = pPolyline->getPoints();
			if (lspt.size() >= 1)
			{
				lspt.pop_back();
			}
			std::wstring strpt = pPolyline->PointsToValue(lspt);
			pPolyline->setAttribute(L"points", strpt);
			SetContentHasChange();
			m_bModifyForeground = true;
			m_FocusManager.ResetFocus(pPolyline);
		}
	}
	else
	{
		m_ActionHistory.Undo();
	}
}

void CSVGDocument::Redo()
{
	m_ActionHistory.Redo();
}

void CSVGDocument::SetAttributes(const std::vector<std::wstring>&AttrNames, const std::vector<std::wstring>&AttrValues, bool bChangeDefult)
{
	if (AttrNames.size() != AttrValues.size())
		return;

	if (m_vcSelectShapes.size() == 0)
	{
		for (int i = 0; i < (int)AttrNames.size(); ++i)
		{
			if (AttrNames[i] == L"width")
			{
				m_svgRootElement->setAttribute(L"width", AttrValues[i]);

				std::wstring strwidth = AttrValues[i];
				std::wstring strheight = m_svgRootElement->getAttribute(L"height");
				std::wstring strviewbox = L"0 0 ";
				strviewbox += strwidth;
				strviewbox += L" ";
				strviewbox += strheight;
				m_svgRootElement->setAttribute(L"viewBox", strviewbox);
				m_nSVGWidth = global.atof_3(strwidth);
				m_nSVGHeight = global.atof_3(strheight);
				m_bModifyBackground = true;
			}
			else if (AttrNames[i] == L"height")
			{
				m_svgRootElement->setAttribute(L"height", AttrValues[i]);

				std::wstring strwidth = m_svgRootElement->getAttribute(L"width");
				std::wstring strheight = AttrValues[i];
				std::wstring strviewbox = L"0 0 ";
				strviewbox += strwidth;
				strviewbox += L" ";
				strviewbox += strheight;
				m_svgRootElement->setAttribute(L"viewBox", strviewbox);
				m_nSVGWidth = global.atof_3(strwidth);
				m_nSVGHeight = global.atof_3(strheight);
				m_bModifyBackground = true;
			}
		}
	}

	SetContentHasChange();
	CActionComposite *pActionCompose = NULL;
	if (m_vcSelectShapes.size() > 0)
	{
		pActionCompose = new CActionComposite(this);
		m_ActionHistory.AddAction(pActionCompose);
	}
	if (m_pInnerSelectShape)
	{
		for (int j = 0; j < (int)AttrNames.size(); ++j)
		{
			CActionAttr *pAttr = new CActionAttr(this, m_pInnerSelectShape, AttrNames[j]);
			pActionCompose->PushBack(pAttr);
			std::wstring strVal = AttrValues[j];
			if (m_pInnerSelectShape->getNodeType() == SVG_TEXT)
			{
				if (AttrNames[j] == global.getAttrNameByIndex(ATTR_FONT_WEIGHT))
				{
					std::wstring stroldval = m_pInnerSelectShape->getAttribute(AttrNames[j]);
					if (stroldval == L"bold")
					{
						strVal = L"";
					}
				}
				else if (AttrNames[j] == global.getAttrNameByIndex(ATTR_FONT_STYLE))
				{//italic
					std::wstring stroldval = m_pInnerSelectShape->getAttribute(AttrNames[j]);
					if (stroldval == L"italic")
					{
						strVal = L"";
					}
				}
				else if (AttrNames[j] == global.getAttrNameByIndex(ATTR_TEXT_DECORATION))
				{
					std::wstring stroldval = m_pInnerSelectShape->getAttribute(AttrNames[j]);
					if (stroldval == L"underline")
					{
						strVal = L"";
					}
				}
			}
			if (AttrNames[j] == L"textcontent" && m_pInnerSelectShape->getNodeType() == SVG_TEXT)
			{
				m_pInnerSelectShape->setNodeValue(AttrValues[j]);
			}
			else
			{
				m_pInnerSelectShape->setAttribute(AttrNames[j], strVal);
			}
			m_pInnerSelectShape->resetUseMatrix();
			pAttr->EndValue();
		}
	}
	else
	{
		for (int i = 0; i < m_vcSelectShapes.size(); ++i)
		{
			for (int j = 0; j < (int)AttrNames.size(); ++j)
			{
				CActionAttr *pAttr = new CActionAttr(this, m_vcSelectShapes[i], AttrNames[j]);
				pActionCompose->PushBack(pAttr);
				std::wstring strVal = AttrValues[j];
				if (m_vcSelectShapes[i]->getNodeType() == SVG_TEXT)
				{
					if (AttrNames[j] == global.getAttrNameByIndex(ATTR_FONT_WEIGHT))
					{
						std::wstring stroldval = m_vcSelectShapes[i]->getAttribute(AttrNames[j]);
						if (stroldval == L"bold")
						{
							strVal = L"";
						}
					}
					else if (AttrNames[j] == global.getAttrNameByIndex(ATTR_FONT_STYLE))
					{
						std::wstring stroldval = m_vcSelectShapes[i]->getAttribute(AttrNames[j]);
						if (stroldval == L"italic")
						{
							strVal = L"";
						}
					}
					else if (AttrNames[j] == global.getAttrNameByIndex(ATTR_TEXT_DECORATION))
					{
						std::wstring stroldval = m_vcSelectShapes[i]->getAttribute(AttrNames[j]);
						if (stroldval == L"underline")
						{
							strVal = L"";
						}
					}
				}
				if (AttrNames[j] == L"textcontent" && m_vcSelectShapes[i]->getNodeType() == SVG_TEXT)
				{
					m_vcSelectShapes[i]->setNodeValue(AttrValues[j]);
				}
				else
				{
					m_vcSelectShapes[i]->setAttribute(AttrNames[j], strVal);
				}
				m_vcSelectShapes[i]->resetUseMatrix();
				pAttr->EndValue();
			}
		}
	}
	for (int i = 0; i < (int)AttrNames.size(); ++i)
	{
		if (bChangeDefult)
		{
			if (AttrNames[i] == L"stroke")
			{
				m_defaultstroke = AttrValues[i];
			}
			else if (AttrNames[i] == L"stroke-width")
			{
				m_defaultstrokewidth = AttrValues[i];
			}
			else if (AttrNames[i] == L"fill")
			{
				CSVGElement *pElement = GetAloneSelectElement(true);
				if (pElement && pElement->getNodeType() == SVG_TEXT)
				{
					m_defaultfontcolor = AttrValues[i];
				}
				else
				{
					m_defaultfill = AttrValues[i];
				}
			}
			else if (AttrNames[i] == L"fill-opacity")
			{
				m_defaultfillopacity = AttrValues[i];
			}
			else if (AttrNames[i] == L"font-family")
			{
				m_defaultfontname = AttrValues[i];
			}
			else if (AttrNames[i] == L"font-size")
			{
				m_defaultfontsize = AttrValues[i];
			}
			else if (AttrNames[i] == L"stroke-dasharray")
			{
				m_defaultdashline = AttrValues[i];
			}
		}
	}
	m_FocusManager.ResetAllFocus();
	m_bModifyForeground = true;
}

bool CSVGDocument::Delete()
{
	if (m_nTextFocusPos >= 0)
	{
		CSVGElement *pElement = GetAloneSelectElement(true);
		if (pElement && pElement->getNodeType() == SVG_TEXT)
		{
			CSVGElementText *pTextElement = (CSVGElementText*)pElement;
			std::wstring strnodeval = pTextElement->getNodeValue();
			if ((int)strnodeval.length() < m_nTextFocusPos)
			{
				return false;
			}
			CActionValue *pActionVal = new CActionValue(this, pElement);
			m_ActionHistory.AddAction(pActionVal);
			QString strVal = QString::fromStdWString(strnodeval);
			strVal = strVal.remove(m_nTextFocusPos, 1);
			pTextElement->setNodeValue(strVal.toStdWString());
			pActionVal->EndValue();
			pTextElement->resetPath();
			pTextElement->resetUseMatrix();
			m_FocusManager.ResetAllFocus();
			m_FocusManager.ResetFocus(pElement, m_nTextFocusPos);
			SetContentHasChange();
			m_bModifyForeground = true;
		}
		return true;
	}

	if (m_vcSelectShapes.size())
	{
		if (m_pInnerSelectShape)
		{
			CActionComposite *pComposite = new CActionComposite(this);
			m_ActionHistory.AddAction(pComposite);
			CActionDel *pActionDel = new CActionDel(this, m_pInnerSelectShape);
			pComposite->PushBack(pActionDel);
			m_pInnerSelectShape->remove();
			RemoveInnerSelect();
		}
		else
		{
			std::sort(m_vcSelectShapes.begin(), m_vcSelectShapes.end(), SortElementByIndex);
			CActionComposite *pComposite = new CActionComposite(this);
			m_ActionHistory.AddAction(pComposite);
			for (int i = 0; i < m_vcSelectShapes.size(); ++i)
			{
				CActionDel *pActionDel = new CActionDel(this, m_vcSelectShapes[i]);
				pComposite->PushBack(pActionDel);
				m_vcSelectShapes[i]->remove();
			}
			ClearAllSelect();
		}
		SetContentHasChange();
	}
	return true;
}

void CSVGDocument::LayerTop()
{
	if (m_vcSelectShapes.size() == 1)
	{
		CSVGElement *pElement = GetFirstSelectElement();
		CActionLayer *pLayer = new CActionLayer(this, pElement);
		m_ActionHistory.AddAction(pLayer);
		m_svgRootElement->appendChild(pElement);
		pLayer->EndValue();
		ReSetAllElementIndex();
		SetContentHasChange();
	}
}

void CSVGDocument::LayerBottom()
{
	if (m_vcSelectShapes.size() == 1)
	{
		CSVGElement *pElement = GetFirstSelectElement();
		CActionLayer *pLayer = new CActionLayer(this, pElement);
		m_ActionHistory.AddAction(pLayer);
		m_svgRootElement->insertBefore(pElement, NULL);
		pLayer->EndValue();
		ReSetAllElementIndex();
		SetContentHasChange();
	}
}

void CSVGDocument::LayerUP()
{
	if (m_vcSelectShapes.size() == 1)
	{
		CSVGElement *pElement = GetFirstSelectElement();
		std::wstring strId = GetLayerUp(pElement);

		CActionLayer *pLayer = new CActionLayer(this, pElement);
		m_ActionHistory.AddAction(pLayer);
		CSVGElement *pInserBefore = getElementById(strId);
		if (!pInserBefore)
		{
			m_svgRootElement->insertAfter(pElement, NULL);
		}
		else
		{
			m_svgRootElement->insertBefore(pElement, pInserBefore);
		}
		pLayer->EndValue();
		ReSetAllElementIndex();
		SetContentHasChange();
	}
}

void CSVGDocument::LayerDown()
{
	if (m_vcSelectShapes.size() == 1)
	{
		CSVGElement *pElement = GetFirstSelectElement();
		std::wstring strId = GetLayerDown(pElement);

		CActionLayer *pLayer = new CActionLayer(this, pElement);
		m_ActionHistory.AddAction(pLayer);
		CSVGElement *pInserBefore = getElementById(strId);
		if (!pInserBefore)
		{
			m_svgRootElement->insertBefore(pElement, NULL);
		}
		else
		{
			m_svgRootElement->insertBefore(pElement, pInserBefore);
		}
		pLayer->EndValue();
		ReSetAllElementIndex();
		SetContentHasChange();
	}
}

void CSVGDocument::TurnRight()
{
	if (m_vcSelectShapes.size() > 0)
	{
		CActionComposite *pComposite = new CActionComposite(this);
		m_ActionHistory.AddAction(pComposite);
		for (int i = 0; i < m_vcSelectShapes.size(); ++i)
		{
			CActionAttr *pAttrAction = new CActionAttr(this, m_vcSelectShapes[i], L"transform");
			pComposite->PushBack(pAttrAction);
			std::wstring strnewmat = GetTurnRight(m_vcSelectShapes[i]);
			m_vcSelectShapes[i]->setAttribute(L"transform", strnewmat);
			m_vcSelectShapes[i]->resetUseMatrix();
			pAttrAction->EndValue();
			m_FocusManager.AddElement(m_vcSelectShapes[i], Qt::green);
		}
		SetContentHasChange();
		m_bModifyForeground = true;
	}
}

void CSVGDocument::TurnLeft()
{
	if (m_vcSelectShapes.size() > 0)
	{
		CActionComposite *pComposite = new CActionComposite(this);
		m_ActionHistory.AddAction(pComposite);
		for (int i = 0; i < m_vcSelectShapes.size(); ++i)
		{
			CActionAttr *pAttrAction = new CActionAttr(this, m_vcSelectShapes[i], L"transform");
			pComposite->PushBack(pAttrAction);
			std::wstring strnewmat = GetTurnLeft(m_vcSelectShapes[i]);
			m_vcSelectShapes[i]->setAttribute(L"transform", strnewmat);
			m_vcSelectShapes[i]->resetUseMatrix();
			pAttrAction->EndValue();
			m_FocusManager.AddElement(m_vcSelectShapes[i], Qt::green);
		}
		SetContentHasChange();
		m_bModifyForeground = true;
	}
}

void CSVGDocument::MirrorRight()
{
	if (m_vcSelectShapes.size() > 0)
	{
		CActionComposite *pComposite = new CActionComposite(this);
		m_ActionHistory.AddAction(pComposite);
		for (int i = 0; i < m_vcSelectShapes.size(); ++i)
		{
			CActionAttr *pAttrAction = new CActionAttr(this, m_vcSelectShapes[i], L"transform");
			pComposite->PushBack(pAttrAction);
			std::wstring strnewmat = GetMirrorRight(m_vcSelectShapes[i]);
			m_vcSelectShapes[i]->setAttribute(L"transform", strnewmat);
			m_vcSelectShapes[i]->resetUseMatrix();
			pAttrAction->EndValue();
			m_FocusManager.AddElement(m_vcSelectShapes[i], Qt::green);
		}
		SetContentHasChange();
		m_bModifyForeground = true;
	}
}

void CSVGDocument::MirrorBottom()
{
	if (m_vcSelectShapes.size() > 0)
	{
		CActionComposite *pComposite = new CActionComposite(this);
		m_ActionHistory.AddAction(pComposite);
		for (int i = 0; i < m_vcSelectShapes.size(); ++i)
		{
			CActionAttr *pAttrAction = new CActionAttr(this, m_vcSelectShapes[i], L"transform");
			pComposite->PushBack(pAttrAction);
			std::wstring strnewmat = GetMirrorBottom(m_vcSelectShapes[i]);
			m_vcSelectShapes[i]->setAttribute(L"transform", strnewmat);
			m_vcSelectShapes[i]->resetUseMatrix();
			pAttrAction->EndValue();
			m_FocusManager.AddElement(m_vcSelectShapes[i], Qt::green);
		}
		SetContentHasChange();
		m_bModifyForeground = true;
	}
}

void CSVGDocument::Group()
{
	if (m_vcSelectShapes.size() >= 2)
	{
		std::sort(m_vcSelectShapes.begin(), m_vcSelectShapes.end(), SortElementByIndex);
		CSVGElementShape *pGElement = (CSVGElementShape*)createElement(L"g");
		m_svgRootElement->addChild(pGElement, false);
		for (int i = 0; i < m_vcSelectShapes.size(); ++i)
		{
			CSVGElement* pShape = m_vcSelectShapes[i];
			m_svgRootElement->removeChildOnly(pShape);
			pGElement->addChild(pShape, false);
		}
		pGElement->resetUseMatrix();
		pGElement->resetPath();
		ClearAllSelect();
		AddToSelect(pGElement, Qt::green);
		CActionGroup *pAction = new CActionGroup(this, pGElement);
		m_ActionHistory.AddAction(pAction);
		SetContentHasChange();
		m_bModifyForeground = true;
	}
}

bool CSVGDocument::CanUnGroup()
{
	bool bCanUnGroup = false;
	for (int i = 0; i < m_vcSelectShapes.size(); ++i)
	{
		if (m_vcSelectShapes[i]->getNodeType() == SVG_G)
		{
			bCanUnGroup = true;
			break;
		}
	}
	return bCanUnGroup;
}

CSVGElement *CSVGDocument::GetRootSVGElement()
{
	return m_svgRootElement;
}

CSVGElement *CSVGDocument::GetSelectChild()
{
	return m_pInnerSelectShape;
}

void  CSVGDocument::TrySetCSSFromElement(CActionComposite *pCompos, CSVGElement *pSrc, CSVGElement *pDes)
{
	if (!pCompos || !pSrc || !pDes)
		return;
	std::vector<std::wstring>vcAttrName;
	vcAttrName.push_back(L"fill");
	vcAttrName.push_back(L"fill-opacity");
	vcAttrName.push_back(L"stroke");
	vcAttrName.push_back(L"stroke-opacity");
	vcAttrName.push_back(L"opacity");
	for (size_t i = 0; i < vcAttrName.size(); ++i)
	{
		std::wstring strSrcVal = pSrc->getAttribute(vcAttrName[i]);
		if (strSrcVal.length() > 0)
		{
			std::wstring strDesVal = pDes->getAttribute(vcAttrName[i]);
			if (strDesVal.length() == 0)
			{
				CActionAttr *pActionAttr = new CActionAttr(this, pDes, vcAttrName[i]);
				pDes->setAttribute(vcAttrName[i], strSrcVal);
				pActionAttr->EndValue();
				pCompos->PushBack(pActionAttr);
			}
		}
	}
}

void CSVGDocument::UnGroup()
{
	if (CanUnGroup())
	{
		CActionComposite *pActionComposite = new CActionComposite(this);
		m_ActionHistory.AddAction(pActionComposite);
		QVector<CSVGElement *> vcSelectShape = m_vcSelectShapes;
		ClearAllSelect();
		for (int i = vcSelectShape.size() - 1; i >= 0; --i)
		{
			if (vcSelectShape[i]->getNodeType() == SVG_G)
			{
				CSVGElementG *pShapeGroup = (CSVGElementG*)vcSelectShape[i];
				pShapeGroup->StyleOrClasstoAttr();
				CActionUnGroup *pAction = new CActionUnGroup(this, pShapeGroup);
				pActionComposite->PushBack(pAction);
				CSVGElement*pParent = pShapeGroup->getParentNode();
				for (int j = pShapeGroup->getChildCount() - 1; j >= 0; --j)
				{
					CSVGElement* pChildElement = pShapeGroup->getChild(j);
					pShapeGroup->removeChildOnly(pChildElement);
					pParent->insertAfter(pChildElement, pShapeGroup, false);
					if (pChildElement->IsShapeElement())
					{
						TrySetCSSFromElement(pActionComposite, pShapeGroup, (CSVGElementShape*)pChildElement);
						CSVGElementShape *pChildShape = (CSVGElementShape*)pChildElement;
						CSVGMatrix gMat = pShapeGroup->getMatrix();
						CSVGMatrix ownerMat = pChildShape->getMatrix();
						CSVGMatrix newMat = gMat;
						newMat.Add(ownerMat);
						pChildShape->setAttribute(L"transform", newMat.getText());
						pChildShape->resetUseMatrix();
						AddToSelect(pChildShape, Qt::green);
					}
				}
				pParent->removeChildOnly(pShapeGroup);
			}
		}
	}
}

void CSVGDocument::Rotate(int nValue, bool bFinish)
{
	if (m_vcSelectShapes.size() > 0)
	{
		if (!bFinish)
		{
			BeginElementTransformAction();
		}
		for (int i = 0; i < m_vcSelectShapes.size(); ++i)
		{
			if (m_vcSelectShapes[i]->IsShapeElement())
			{
				CSVGElementShape *pShape = (CSVGElementShape*)m_vcSelectShapes[i];
				CSVGRectF matbbox = pShape->getMatrixBBox();
				CSVGPointF pt(matbbox.x + matbbox.width / 2, matbbox.y + matbbox.height / 2);
				CSVGMatrix mat = pShape->getUsedMatrix();
				mat.LTranslate(-pt.x, -pt.y);
				mat.LRotate(nValue);
				mat.LTranslate(pt.x, pt.y);
				pShape->setAttribute(L"transform", mat.getText());
				pShape->resetUseMatrix();
				m_FocusManager.AddElement(pShape, Qt::green);
				m_bModifyForeground = true;
			}
		}
		SetContentHasChange();
		m_bModifyForeground = true;
		if (bFinish)
		{
			EndElementAttrAction();
		}
	}
}

void CSVGDocument::ZoomSizeFixValue(int nWidth, int nHeight, bool bFinish)
{
	if (m_vcSelectShapes.size() > 0)
	{
		if (!bFinish)
		{
			BeginElementTransformAction();
		}
		for (int i = 0; i < m_vcSelectShapes.size(); ++i)
		{
			if (m_vcSelectShapes[i]->IsShapeElement())
			{
				CSVGElementShape *pShape = (CSVGElementShape*)m_vcSelectShapes[i];
				CSVGRectF matbbox = pShape->getMatrixBBox();
				CSVGPointF pt(matbbox.x + matbbox.width / 2, matbbox.y + matbbox.height / 2);
				CSVGMatrix mat = pShape->getUsedMatrix();
				mat.LTranslate(-pt.x, -pt.y);
				mat.LScale(nWidth*1.0 / matbbox.width, nHeight*1.0 / matbbox.height);
				mat.LTranslate(pt.x, pt.y);
				pShape->setAttribute(L"transform", mat.getText());
				pShape->resetUseMatrix();
				m_FocusManager.AddElement(pShape, Qt::green);
				m_bModifyForeground = true;
			}
		}
		SetContentHasChange();
		m_bModifyForeground = true;
		if (bFinish)
		{
			EndElementAttrAction();
		}
	}
}

std::wstring CSVGDocument::GetSelectWidth()
{
	std::wstring strwidth;
	if (m_vcSelectShapes.size() > 0)
	{
		if (m_vcSelectShapes[0]->IsShapeElement())
		{
			CSVGElementShape *pShape = (CSVGElementShape*)m_vcSelectShapes[0];
			CSVGRectF matbbox = pShape->getMatrixBBox();
			strwidth = global.ftostring(matbbox.width);
		}
	}
	return strwidth;
}

std::wstring CSVGDocument::GetSelectHeight()
{
	std::wstring strheight;
	if (m_vcSelectShapes.size() > 0)
	{
		if (m_vcSelectShapes[0]->IsShapeElement())
		{
			CSVGElementShape *pShape = (CSVGElementShape*)m_vcSelectShapes[0];
			CSVGRectF matbbox = pShape->getMatrixBBox();
			strheight = global.ftostring(matbbox.height);
		}
	}
	return strheight;
}


void CSVGDocument::LeftAlign()
{
	if (m_vcSelectShapes.size() >= 2)
	{
		std::wstring strmat;
		float fleftx = 99999999990.f;
		for (int i = 0; i < m_vcSelectShapes.size(); ++i)
		{
			if (m_vcSelectShapes[i]->IsShapeElement())
			{
				CSVGElementShape *pShape = (CSVGElementShape*)m_vcSelectShapes[i];
				CSVGRectF matbox = pShape->getMatrixBBox();
				if (matbox.x < fleftx)
				{
					fleftx = matbox.x;
				}
			}
		}
		BeginElementTransformAction();
		for (int i = 0; i < m_vcSelectShapes.size(); ++i)
		{
			if (m_vcSelectShapes[i]->IsShapeElement())
			{
				CSVGElementShape *pShape = (CSVGElementShape*)m_vcSelectShapes[i];
				CSVGRectF matbox = pShape->getMatrixBBox();
				float fxtranslate = fleftx - matbox.x;
				CSVGMatrix mat = pShape->getMatrix();
				mat.LTranslate(fxtranslate, 0);
				pShape->setAttribute(L"transform", mat.getText());
				pShape->resetUseMatrix();
				m_FocusManager.AddElement(pShape, Qt::green);
			}
		}
		EndElementAttrAction();
		m_bModifyForeground = true;
		SetContentHasChange();
	}
}

void CSVGDocument::TopAlign()
{
	if (m_vcSelectShapes.size() >= 2)
	{
		std::wstring strmat;
		float flefty = 9999999999.0f;
		for (int i = 0; i < m_vcSelectShapes.size(); ++i)
		{
			if (m_vcSelectShapes[i]->IsShapeElement())
			{
				CSVGElementShape *pShape = (CSVGElementShape*)m_vcSelectShapes[i];
				CSVGRectF matbox = pShape->getMatrixBBox();
				if (matbox.y < flefty)
				{
					flefty = matbox.y;
				}
			}
		}
		BeginElementTransformAction();
		for (int i = 0; i < m_vcSelectShapes.size(); ++i)
		{
			if (m_vcSelectShapes[i]->IsShapeElement())
			{
				CSVGElementShape *pShape = (CSVGElementShape*)m_vcSelectShapes[i];
				CSVGRectF matbox = pShape->getMatrixBBox();
				float fytranslate = flefty - matbox.y;
				CSVGMatrix mat = pShape->getMatrix();
				mat.LTranslate(0, fytranslate);
				pShape->setAttribute(L"transform", mat.getText());
				pShape->resetUseMatrix();
				m_FocusManager.AddElement(pShape, Qt::green);
			}
		}
		EndElementAttrAction();
		m_bModifyForeground = true;
		SetContentHasChange();
	}
}

void CSVGDocument::RightAlign()
{
	if (m_vcSelectShapes.size() >= 2)
	{
		std::wstring strmat;
		float frightx = -9999999999.0f;
		for (int i = 0; i < m_vcSelectShapes.size(); ++i)
		{
			if (m_vcSelectShapes[i]->IsShapeElement())
			{
				CSVGElementShape *pShape = (CSVGElementShape*)m_vcSelectShapes[i];
				CSVGRectF matbox = pShape->getMatrixBBox();
				if ((matbox.x + matbox.width) > frightx)
				{
					frightx = matbox.x + matbox.width;
				}
			}
		}
		BeginElementTransformAction();
		for (int i = 0; i < m_vcSelectShapes.size(); ++i)
		{
			if (m_vcSelectShapes[i]->IsShapeElement())
			{
				CSVGElementShape *pShape = (CSVGElementShape*)m_vcSelectShapes[i];
				CSVGRectF matbox = pShape->getMatrixBBox();
				float fxtranslate = frightx - (matbox.x + matbox.width);
				CSVGMatrix mat = pShape->getMatrix();
				mat.LTranslate(fxtranslate, 0);
				pShape->setAttribute(L"transform", mat.getText());
				pShape->resetUseMatrix();
				m_FocusManager.AddElement(pShape, Qt::green);
			}
		}
		EndElementAttrAction();
		m_bModifyForeground = true;
		SetContentHasChange();
	}
}

void CSVGDocument::BottomAlign()
{
	if (m_vcSelectShapes.size() >= 2)
	{
		std::wstring strmat;
		float fbottomy = -9999999999.0f;
		for (int i = 0; i < m_vcSelectShapes.size(); ++i)
		{
			if (m_vcSelectShapes[i]->IsShapeElement())
			{
				CSVGElementShape *pShape = (CSVGElementShape*)m_vcSelectShapes[i];
				CSVGRectF matbox = pShape->getMatrixBBox();
				if ((matbox.y + matbox.height) > fbottomy)
				{
					fbottomy = matbox.y + matbox.height;
				}
			}
		}
		BeginElementTransformAction();
		for (int i = 0; i < m_vcSelectShapes.size(); ++i)
		{
			if (m_vcSelectShapes[i]->IsShapeElement())
			{
				CSVGElementShape *pShape = (CSVGElementShape*)m_vcSelectShapes[i];
				CSVGRectF matbox = pShape->getMatrixBBox();
				float fytranslate = fbottomy - (matbox.y + matbox.height);
				CSVGMatrix mat = pShape->getMatrix();
				mat.LTranslate(0, fytranslate);
				pShape->setAttribute(L"transform", mat.getText());
				pShape->resetUseMatrix();
				m_FocusManager.AddElement(pShape, Qt::green);
			}
		}
		EndElementAttrAction();
		m_bModifyForeground = true;
		SetContentHasChange();
	}
}

void CSVGDocument::VerLineAlign()
{
	if (m_vcSelectShapes.size() >= 2)
	{
		float fxmiddle = 0;
		CSVGRectF firstbbox;
		if (m_vcSelectShapes[0]->IsShapeElement())
		{
			CSVGElementShape *pShape = (CSVGElementShape*)m_vcSelectShapes[0];
			firstbbox = pShape->getMatrixBBox();
			fxmiddle = firstbbox.x + firstbbox.width / 2;
		}
		BeginElementTransformAction();
		for (int i = 0; i < m_vcSelectShapes.size(); ++i)
		{
			if (m_vcSelectShapes[i]->IsShapeElement())
			{
				CSVGElementShape *pShape = (CSVGElementShape*)m_vcSelectShapes[i];
				CSVGRectF matbox = pShape->getMatrixBBox();
				float fxtranslate = fxmiddle - (matbox.x + matbox.width / 2);
				CSVGMatrix mat = pShape->getMatrix();
				mat.LTranslate(fxtranslate, 0);
				pShape->setAttribute(L"transform", mat.getText());
				pShape->resetUseMatrix();
				m_FocusManager.AddElement(pShape, Qt::green);
			}
		}
		EndElementAttrAction();
		m_bModifyForeground = true;
		SetContentHasChange();
	}
}

void CSVGDocument::HorLineAlign()
{
	if (m_vcSelectShapes.size() >= 2)
	{
		float fymiddle = 0;
		CSVGRectF firstbbox;
		if (m_vcSelectShapes[0]->IsShapeElement())
		{
			CSVGElementShape *pShape = (CSVGElementShape*)m_vcSelectShapes[0];
			firstbbox = pShape->getMatrixBBox();
			fymiddle = firstbbox.y + firstbbox.height / 2;
		}
		BeginElementTransformAction();
		for (int i = 0; i < m_vcSelectShapes.size(); ++i)
		{
			if (m_vcSelectShapes[i]->IsShapeElement())
			{
				CSVGElementShape *pShape = (CSVGElementShape*)m_vcSelectShapes[i];
				CSVGRectF matbox = pShape->getMatrixBBox();
				float fytranslate = fymiddle - (matbox.y + matbox.height / 2);
				CSVGMatrix mat = pShape->getMatrix();
				mat.LTranslate(0, fytranslate);
				pShape->setAttribute(L"transform", mat.getText());
				pShape->resetUseMatrix();
				m_FocusManager.AddElement(pShape, Qt::green);
			}
		}
		EndElementAttrAction();
		m_bModifyForeground = true;
		SetContentHasChange();
	}
}

void CSVGDocument::HorSpaceAlign()
{
	if (m_vcSelectShapes.size() >= 3)
	{
		BeginElementTransformAction();
		std::vector<CSVGRectF>rectvc;
		for (int i = 0; i < m_vcSelectShapes.size(); ++i)
		{
			if (m_vcSelectShapes[i]->IsShapeElement())
			{
				CSVGElementShape *pShape = (CSVGElementShape*)m_vcSelectShapes[i];
				CSVGRectF matbox = pShape->getMatrixBBox();
				rectvc.push_back(matbox);
			}
		}
		if (rectvc.size() != m_vcSelectShapes.size())
			return;
		float fSpace = 0;
		float lastpos = 0;
		std::sort(rectvc.begin(), rectvc.end(), IsXSmall);
		std::sort(m_vcSelectShapes.begin(), m_vcSelectShapes.end(), IsShapeXSmall);
		for (size_t i = 0; i < rectvc.size(); ++i)
		{
			CSVGRectF rect = rectvc[i];
			if (i == 0)
			{
				lastpos = rect.x + rect.width;
			}
			else
			{
				fSpace += (rect.x - lastpos);
				lastpos = rect.x + rect.width;
			}
		}
		float fdivspace = fSpace / (m_vcSelectShapes.size() - 1);
		float flastpos = rectvc[0].x + rectvc[0].width + fdivspace;
		for (int i = 1; i < m_vcSelectShapes.size(); ++i)
		{
			float fx = flastpos - rectvc[i].x;
			CSVGMatrix mat = m_vcSelectShapes[i]->getMatrix();
			mat.LTranslate(fx, 0);
			m_vcSelectShapes[i]->setAttribute(L"transform", mat.getText());
			m_vcSelectShapes[i]->resetUseMatrix();
			m_FocusManager.AddElement(m_vcSelectShapes[i], Qt::green);
			flastpos = flastpos + rectvc[i].width + fdivspace;
		}
		EndElementAttrAction();
		m_bModifyForeground = true;
		SetContentHasChange();
	}
}

void CSVGDocument::VerSpaceAlign()
{
	if (m_vcSelectShapes.size() >= 3)
	{
		std::vector<CSVGRectF>rectvc;
		for (int i = 0; i < m_vcSelectShapes.size(); ++i)
		{
			if (m_vcSelectShapes[i]->IsShapeElement())
			{
				CSVGElementShape *pShape = (CSVGElementShape*)m_vcSelectShapes[i];
				CSVGRectF matbox = pShape->getMatrixBBox();
				rectvc.push_back(matbox);
			}
		}
		float fSpace = 0;
		float lastpos = 0;
		std::sort(rectvc.begin(), rectvc.end(), IsYSmall);
		std::sort(m_vcSelectShapes.begin(), m_vcSelectShapes.end(), IsShapeYSmall);
		if (rectvc.size() != m_vcSelectShapes.size())
			return;
		for (size_t i = 0; i < rectvc.size(); ++i)
		{
			CSVGRectF rect = rectvc[i];
			if (i == 0)
			{
				lastpos = rect.y + rect.height;
			}
			else
			{
				fSpace += (rect.y - lastpos);
				lastpos = rect.y + rect.height;
			}
		}
		float fdivspace = fSpace / (m_vcSelectShapes.size() - 1);
		float flastpos = rectvc[0].y + rectvc[0].height + fdivspace;
		for (int i = 1; i < m_vcSelectShapes.size(); ++i)
		{
			float fy = flastpos - rectvc[i].y;
			CSVGMatrix mat = m_vcSelectShapes[i]->getMatrix();
			mat.LTranslate(0, fy);
			m_vcSelectShapes[i]->setAttribute(L"transform", mat.getText());
			m_vcSelectShapes[i]->resetUseMatrix();
			m_FocusManager.AddElement(m_vcSelectShapes[i], Qt::green);
			flastpos = flastpos + rectvc[i].height + fdivspace;
		}
		EndElementAttrAction();
		m_bModifyForeground = true;
		SetContentHasChange();
	}
}

void CSVGDocument::SameWidthAlign()
{
	if (m_vcSelectShapes.size() >= 2)
	{
		CSVGRectF firstbbox;
		if (m_vcSelectShapes[0]->IsShapeElement())
		{
			CSVGElementShape *pShape = (CSVGElementShape*)m_vcSelectShapes[0];
			firstbbox = pShape->getMatrixBBox();
		}
		if (firstbbox.width <= 0)
			return;
		BeginElementTransformAction();
		for (int i = 1; i < m_vcSelectShapes.size(); ++i)
		{
			if (m_vcSelectShapes[i]->IsShapeElement())
			{
				CSVGElementShape *pShape = (CSVGElementShape*)m_vcSelectShapes[i];
				CSVGRectF bbox = pShape->getMatrixBBox();
				if (bbox.width > 0)
				{
					float fscalex = firstbbox.width / bbox.width;
					CSVGMatrix mat = pShape->getMatrix();
					mat.LTranslate(-bbox.x, -bbox.y);
					mat.LScale(fscalex, 1);
					mat.LTranslate(bbox.x, bbox.y);
					pShape->setAttribute(L"transform", mat.getText());
					m_vcSelectShapes[i]->resetUseMatrix();
					m_FocusManager.AddElement(m_vcSelectShapes[i], Qt::green);
				}
			}
		}
		EndElementAttrAction();
		m_bModifyForeground = true;
		SetContentHasChange();
	}
}

void CSVGDocument::SameHeightAlign()
{
	if (m_vcSelectShapes.size() >= 2)
	{
		CSVGRectF firstbbox;
		if (m_vcSelectShapes[0]->IsShapeElement())
		{
			CSVGElementShape *pShape = (CSVGElementShape*)m_vcSelectShapes[0];
			firstbbox = pShape->getMatrixBBox();
		}
		if (firstbbox.height <= 0)
			return;
		BeginElementTransformAction();
		for (int i = 1; i < m_vcSelectShapes.size(); ++i)
		{
			if (m_vcSelectShapes[i]->IsShapeElement())
			{
				CSVGElementShape *pShape = (CSVGElementShape*)m_vcSelectShapes[i];
				CSVGRectF bbox = pShape->getMatrixBBox();
				if (bbox.height > 0)
				{
					float fscaley = firstbbox.height / bbox.height;
					CSVGMatrix mat = pShape->getMatrix();
					mat.LTranslate(-bbox.x, -bbox.y);
					mat.LScale(1, fscaley);
					mat.LTranslate(bbox.x, bbox.y);
					pShape->setAttribute(L"transform", mat.getText());
					m_vcSelectShapes[i]->resetUseMatrix();
					m_FocusManager.AddElement(m_vcSelectShapes[i], Qt::green);
				}
			}
		}
		EndElementAttrAction();
		m_bModifyForeground = true;
		SetContentHasChange();
	}
}

void CSVGDocument::SameSizeAlign()
{
	if (m_vcSelectShapes.size() >= 2)
	{
		CSVGRectF firstbbox;
		if (m_vcSelectShapes[0]->IsShapeElement())
		{
			CSVGElementShape *pShape = (CSVGElementShape*)m_vcSelectShapes[0];
			firstbbox = pShape->getMatrixBBox();
		}
		if (firstbbox.width <= 0 || firstbbox.height <= 0)
			return;
		BeginElementTransformAction();
		for (int i = 1; i < m_vcSelectShapes.size(); ++i)
		{
			if (m_vcSelectShapes[i]->IsShapeElement())
			{
				CSVGElementShape *pShape = (CSVGElementShape*)m_vcSelectShapes[i];
				CSVGRectF bbox = pShape->getMatrixBBox();
				if (bbox.width > 0 && bbox.height > 0)
				{
					float fscalex = firstbbox.width / bbox.width;
					float fscaley = firstbbox.height / bbox.height;
					CSVGMatrix mat = pShape->getMatrix();
					mat.LTranslate(-bbox.x, -bbox.y);
					mat.LScale(fscalex, fscaley);
					mat.LTranslate(bbox.x, bbox.y);
					pShape->setAttribute(L"transform", mat.getText());
					m_vcSelectShapes[i]->resetUseMatrix();
					m_FocusManager.AddElement(m_vcSelectShapes[i], Qt::green);
				}
			}
		}
		EndElementAttrAction();
		m_bModifyForeground = true;
		SetContentHasChange();
	}
}

bool CSVGDocument::InputBackSpace()
{
	CSVGElement *pElement = GetAloneSelectElement(true);
	if (pElement && pElement->getNodeType() == SVG_TEXT)
	{
		CSVGElementText *pTextElement = (CSVGElementText*)pElement;
		std::wstring strnodeval = pTextElement->getNodeValue();
		if (m_nTextFocusPos == 0)
		{
			return false;
		}
		CActionValue *pActionVal = new CActionValue(this, pElement);
		m_ActionHistory.AddAction(pActionVal);
		QString strVal = QString::fromStdWString(strnodeval);
		m_nTextFocusPos -= 1;
		strVal = strVal.remove(m_nTextFocusPos, 1);
		pTextElement->setNodeValue(strVal.toStdWString());
		pActionVal->EndValue();
		pTextElement->resetPath();
		pTextElement->resetUseMatrix();
		m_FocusManager.ResetAllFocus();
		m_FocusManager.ResetFocus(pElement, m_nTextFocusPos);
		SetContentHasChange();
		m_bModifyForeground = true;
		m_bShowFlash = true;
		return true;
	}
	return false;
}

bool CSVGDocument::KeyLeft()
{
	if (m_nTextFocusPos >= 0)
	{
		if (m_nTextFocusPos > 0)
		{
			m_nTextFocusPos -= 1;
			CSVGElement * pElement = GetAloneSelectElement(true);
			if (pElement && pElement->getNodeType() == SVG_TEXT)
			{
				m_FocusManager.ResetFocus(pElement, m_nTextFocusPos);
				m_bModifyForeground = true;
				m_bShowFlash = true;
			}
		}
		return true;
	}
	MoveSelect(-1, 0);
	return false;
}

bool CSVGDocument::KeyRight()
{
	if (m_nTextFocusPos >= 0)
	{
		CSVGElement * pElement = GetAloneSelectElement(true);
		if (pElement && pElement->getNodeType() == SVG_TEXT)
		{
			std::wstring strval = pElement->getNodeValue();
			if ((int)strval.length() > m_nTextFocusPos)
			{
				m_nTextFocusPos += 1;
			}
			m_FocusManager.ResetFocus(pElement, m_nTextFocusPos);
			m_bModifyForeground = true;
			m_bShowFlash = true;
		}
		return true;
	}
	MoveSelect(1, 0);
	return false;
}

bool CSVGDocument::KeyUp()
{
	MoveSelect(0, -1);
	return false;
}

bool CSVGDocument::KeyDown()
{
	MoveSelect(0, 1);
	return false;
}

void CSVGDocument::InsertImage(const std::wstring &strimagedata)
{
	CSVGElementImage *pImageElement = (CSVGElementImage*)createElement(L"image");
	m_svgRootElement->addChild(pImageElement, true);
	pImageElement->setAttribute(L"x", L"0");
	pImageElement->setAttribute(L"y", L"0");
	CActionAdd *pActionAdd = new CActionAdd(this, m_svgRootElement, pImageElement);
	m_ActionHistory.AddAction(pActionAdd);
	int nwidth = 0;
	int nheight = 0;
	if (strimagedata.length() > 0)
	{
		QImage image = CSVGElementImage::LoadFromBase64(strimagedata);
		nwidth = image.width();
		nheight = image.height();
	}
	pImageElement->setAttribute(L"width", global.itostring(nwidth));
	pImageElement->setAttribute(L"height", global.itostring(nheight));
	pImageElement->setAttribute(L"xlink:href", L"data:image/png;base64," + strimagedata);

	pImageElement->resetPath();
	ClearAllSelect();
	AddToSelect(pImageElement, Qt::green);
	m_bModifyForeground = true;
}

void CSVGDocument::InputText(const std::wstring &strText)
{
	CSVGElement *pElement = GetAloneSelectElement(true);
	if (pElement && pElement->getNodeType() == SVG_TEXT)
	{
		CActionValue *pActionVal = new CActionValue(this, pElement);
		m_ActionHistory.AddAction(pActionVal);
		int nLength = strText.length();
		CSVGElementText *pTextElement = (CSVGElementText*)pElement;
		std::wstring strnodeval = pTextElement->getNodeValue();
		QString strVal = QString::fromStdWString(strnodeval);
		strVal = strVal.insert(m_nTextFocusPos, QString::fromStdWString(strText));
		pTextElement->setNodeValue(strVal.toStdWString());
		pActionVal->EndValue();
		pTextElement->resetPath();
		pTextElement->resetUseMatrix();
		m_nTextFocusPos += nLength;
		m_FocusManager.ResetAllFocus();
		m_FocusManager.ResetFocus(pTextElement, m_nTextFocusPos);
		SetContentHasChange();
		m_bModifyForeground = true;
		m_bShowFlash = true;
	}
}

void CSVGDocument::SetShowGrid(bool bShow)
{
	m_bShowGrid = bShow;
	m_bModifyBackground = true;
}

void CSVGDocument::SetShowRule(bool bShow)
{
	m_bShowRule = bShow;
	m_bModifyForeground = true;
}

void CSVGDocument::SetShowCanas(bool bShow)
{
	m_bShowCanvas = bShow;
	m_bModifyBackground = true;
}

QImage CSVGDocument::SVGFileAsImageBound(const std::wstring &strxml)
{
	QImage image = QImage(32, 32, QImage::Format_ARGB32);
	image.fill(0);
	CSVGDocument document;
	document.LoadFromBuff(strxml);
	if (document.m_svgRootElement)
	{
		int nwidth = 32;
		int nheight = 32;
		CSVGElementShape *pShape = (CSVGElementShape*)document.m_svgRootElement;
		CSVGRectF bbox = pShape->getBBox();
		if (bbox.width > 0 && bbox.height > 0)
		{
			nwidth = bbox.width;
			nheight = bbox.height;
		}

		image = QImage(nwidth, nheight, QImage::Format_ARGB32);
		image.fill(0);
		QPainter painter(&image);
		document.PrepareDraw();
		document.DrawAll(&painter, CSVGMatrix());
	}
	return image;
}

QImage CSVGDocument::SVGFileAsImage(const std::wstring &strxml, const std::wstring &strtype)
{
	QImage image = QImage(32, 32, QImage::Format_ARGB32);
	image.fill(0);
	if (strtype == L"jpg")
	{
		image = QImage(32, 32, QImage::Format_RGB32);
		image.fill(Qt::white);
	}
	CSVGDocument document;
	document.LoadFromBuff(strxml);
	if (document.m_svgRootElement)
	{
		int nwidth = 0;
		int nheight = 0;
		std::wstring strwidth = document.m_svgRootElement->getAttribute(L"width");
		std::wstring strheight = document.m_svgRootElement->getAttribute(L"height");
		QString qwidth = QString::fromStdWString(strwidth);
		QString qheight = QString::fromStdWString(strheight);
		if (qwidth.indexOf("%") > 0)
		{
			nwidth = 0;
		}
		else
		{
			nwidth = qwidth.toInt();
		}
		if (qheight.indexOf("%") > 0)
		{
			nheight = 0;
		}
		else
		{
			nheight = qheight.toInt();
		}
		if (nwidth <= 1 || nheight <= 1)
		{
			std::wstring viewBox = document.m_svgRootElement->getAttribute(L"viewBox");
			QString strViewBox = QString::fromStdWString(viewBox);
			strViewBox = strViewBox.replace(" ", ",");
			strViewBox = strViewBox.replace(";", ",");
			QStringList lstArr = strViewBox.split(",");
			if (lstArr.size() == 4)
			{
				nwidth = lstArr[2].toInt();
				nheight = lstArr[3].toInt();
			}
		}
		image = QImage(nwidth, nheight, QImage::Format_ARGB32);
		image.fill(0);
		if (strtype == L"jpg")
		{
			image = QImage(nwidth, nheight, QImage::Format_RGB32);
			image.fill(Qt::white);
		}
		QPainter painter(&image);
		document.PrepareDraw();
		document.DrawAll(&painter, CSVGMatrix());
	}
	return image;
}

QImage CSVGDocument::SVGFileAsImage(const std::wstring &strxml, const std::wstring &strtype, int nScaleWidth, int nScaleHeight)
{
	Q_UNUSED(strtype);
	QImage image = QImage(32, 32, QImage::Format_ARGB32);
	image.fill(0);
	CSVGDocument document;
	document.LoadFromBuff(strxml);
	if (document.m_svgRootElement)
	{
		CSVGElementG *pGroup = (CSVGElementG*)document.createElement(L"g");
		QVector<CSVGElement*>vcChild;
		for (int i = 0; i < (int)document.m_svgRootElement->getChildCount(); ++i)
		{
			vcChild.push_back(document.m_svgRootElement->getChild(i));
		}
		for (int i = 0; i < vcChild.size(); ++i)
		{
			vcChild[i]->remove();
			pGroup->appendChild(vcChild[i]);
		}
		document.m_svgRootElement->appendChild(pGroup);
		pGroup->resetUseMatrix();

		CSVGRectF matbox = pGroup->getMatrixBBox();

		int nwidth = matbox.width;
		int nheight = matbox.height;

		if (nScaleWidth == -1 && nScaleHeight == -1)
		{
			nScaleWidth = nwidth;
			nScaleHeight = nheight;
		}

		float fscalex = nScaleWidth*1.0 / nwidth;
		float fscaley = nScaleHeight*1.0 / nheight;
		float fscale = fscalex;
		float foffsetx = 0;
		float foffsety = 0;
		if (std::fabs(fscalex - fscaley) < 0.001)
		{
		}
		else if (fscalex > fscaley)
		{
			fscale = fscaley;
			foffsety = 0;
			foffsetx = (nheight - nwidth)*1.0 / 2;
		}
		else if (fscalex < fscaley)
		{
			fscale = fscalex;
			foffsetx = 0;
			foffsety = (nwidth - nheight)*1.0 / 2;
		}

		CSVGMatrix mat;
		mat.Scale(fscale, fscale);
		mat.Translate(-matbox.x + foffsetx, -matbox.y + foffsety);
		pGroup->setAttribute(L"transform", mat.getText());

		image = QImage(nScaleWidth, nScaleHeight, QImage::Format_ARGB32);
		image.fill(0);
		QPainter painter(&image);
		document.PrepareDraw();
		document.DrawAll(&painter, CSVGMatrix());
	}
	return image;
}

CSVGPointF CSVGDocument::GetFormatPoint(CSVGPointF pt1, CSVGPointF pt)
{
	CSVGPointF retpt = pt;

	float bdiv = pt.y - pt1.y;
	float div = pt.x - pt1.x;

	if (abs(div) < 0.000000001)
		div = (float)0.000000001;
	float k = bdiv / div;
	float newy2 = pt.y;
	if (k >= 0)
	{
		if (k < 0.84)
		{
			retpt.y = pt1.y;
		}
		else if (k >= 0.84 && k <= 1.2)
		{
			newy2 = (pt.x - pt1.x) + pt1.y;
			retpt.y = newy2;
		}
		else if (k > 1.2)
		{
			retpt.x = pt1.x;
		}
	}
	if (k < 0)
	{
		if (k < -1.2)
		{
			retpt.x = pt1.x;
		}
		else if (k >= -1.2 && k <= -0.84)
		{
			newy2 = -(pt.x - pt1.x) + pt1.y;
			retpt.y = newy2;
		}
		else if (k > -0.84)
		{
			retpt.y = pt1.y;
		}
	}
	return retpt;
}


std::list<CSVGPointF> GetRegularPoints(CSVGPointF ptcenter, CSVGPointF firstpt, int nSize)
{
	std::list<CSVGPointF>listpt;
	listpt.push_back(firstpt);

	float a = 2 * 3.14159265 / nSize;
	float	x0 = ptcenter.x;
	float   y0 = ptcenter.y;
	float	x1 = firstpt.x;
	float   y1 = firstpt.y;

	float	r = std::sqrt((y1 - y0) * (y1 - y0) + (x1 - x0) * (x1 - x0));
	float   b = std::atan((y1 - y0) / (x1 - x0));

	for (int i = 1; i < nSize; ++i)
	{
		float y = -r * std::sin((a * i + b)) + y0;
		float x = -r * std::cos((a * i + b)) + x0;
		CSVGPointF pt(x, y);
		listpt.push_back(pt);
	}

	return listpt;
}

std::list<CSVGPointF> CSVGDocument::FormatToRegularPolygon(const std::list<CSVGPointF> &listpt)
{
	std::list<CSVGPointF> retpts = listpt;
	if (retpts.size() <= 2)
		return retpts;
	std::list<CSVGPointF>::iterator iter = retpts.begin();
	CSVGPointF pt1 = *iter;
	CSVGPointF pt2 = *(++iter);
	CSVGPointF pt3 = *(++iter);

	float fdistance = std::sqrt((pt1.x - pt2.x)*(pt1.x - pt2.x) + (pt1.y - pt2.y)*(pt1.y - pt2.y)) / 2;
	float fAngle = (retpts.size() - 2) * 90 / retpts.size();
	float fL = fdistance * std::tan(fAngle*3.1415926 / 180);

	CSVGPointF ptA = pt1;
	CSVGPointF ptB = pt2;
	CSVGPointF ptC = CSVGPointF((ptA.x + ptB.x) / 2, (ptA.y + ptB.y) / 2);

	float fx1 = ptC.x + (fL *(ptA.y - ptC.y) / std::sqrt(std::pow(ptA.x - ptC.x, 2) + std::pow(ptA.y - ptC.y, 2)));
	float fy1 = ptC.y - (fL *(ptA.x - ptC.x) / std::sqrt(std::pow(ptA.x - ptC.x, 2) + std::pow(ptA.y - ptC.y, 2)));

	float fx2 = ptC.x - (fL *(ptA.y - ptC.y) / std::sqrt(std::pow(ptA.x - ptC.x, 2) + std::pow(ptA.y - ptC.y, 2)));
	float fy2 = ptC.y + (fL *(ptA.x - ptC.x) / std::sqrt(std::pow(ptA.x - ptC.x, 2) + std::pow(ptA.y - ptC.y, 2)));

	float fdistance1 = std::sqrt((fx1 - pt3.x)*(fx1 - pt3.x) + (fy1 - pt2.y)*(fy1 - pt3.y));
	float fdistance2 = std::sqrt((fx2 - pt3.x)*(fx2 - pt3.x) + (fy2 - pt2.y)*(fy2 - pt3.y));
	CSVGPointF ptCenter(fx1, fy1);
	if (fdistance1 > fdistance2)
	{
		ptCenter = CSVGPointF(fx2, fy2);
	}
	retpts = GetRegularPoints(ptCenter, pt1, retpts.size());

	return retpts;
}

void CSVGDocument::FillUrl(const std::wstring &strxml)
{
	CActionComposite *pComposite = new CActionComposite(this);
	m_ActionHistory.AddAction(pComposite);
	std::wstring strfillid;
	CSVGDocument doc;
	if (doc.LoadFromBuff(strxml))
	{
		QVector<CSVGElement*>vcChild;
		for (int i = 0; i < (int)doc.m_svgRootElement->getChildCount(); ++i)
		{
			CSVGElement* pElement = doc.m_svgRootElement->getChild(i);
			vcChild.push_back(pElement);
		}
		for (int i = 0; i < vcChild.size(); ++i)
		{
			doc.m_svgRootElement->removeChild(vcChild[i]);
			m_svgRootElement->appendChild(vcChild[i]);
			vcChild[i]->resetUseMatrix();
			CActionAdd *pAddAction = new CActionAdd(this, m_svgRootElement, vcChild[i]);
			pComposite->PushBack(pAddAction);
		}
		if (vcChild.size() == 1)
		{
			strfillid = global.MakeId();
			vcChild[0]->setAttribute(L"id", strfillid);
		}
		ReSetAllElementIndex();

		if (strfillid.length() > 0)
		{
			if (m_pInnerSelectShape)
			{
				std::wstring hreflink = L"url(#" + strfillid + L")";
				CActionAttr *pActionFillAttr = new CActionAttr(this, m_pInnerSelectShape, L"fill");
				CActionAttr *pActionFillOpacityAttr = new CActionAttr(this, m_pInnerSelectShape, L"fill-opaity");
				pComposite->PushBack(pActionFillAttr);
				pComposite->PushBack(pActionFillOpacityAttr);
				m_pInnerSelectShape->setAttribute(L"fill", hreflink);
				m_pInnerSelectShape->setAttribute(L"fill-opacity", L"1");
				pActionFillAttr->EndValue();
				pActionFillOpacityAttr->EndValue();
			}
			else
			{
				for (int i = 0; i < m_vcSelectShapes.size(); ++i)
				{
					CActionAttr *pActionFillAttr = new CActionAttr(this, m_vcSelectShapes[i], L"fill");
					CActionAttr *pActionFillOpacityAttr = new CActionAttr(this, m_vcSelectShapes[i], L"fill-opaity");
					pComposite->PushBack(pActionFillAttr);
					pComposite->PushBack(pActionFillOpacityAttr);
					std::wstring hreflink = L"url(#" + strfillid + L")";
					m_vcSelectShapes[i]->setAttribute(L"fill", hreflink);
					m_vcSelectShapes[i]->setAttribute(L"fill-opacity", L"1");
					pActionFillAttr->EndValue();
					pActionFillOpacityAttr->EndValue();
				}
			}
		}
		SetContentHasChange();
		m_bModifyForeground = true;
	}
}

void CSVGDocument::SetShowFalsh(bool bShow)
{
	if (m_bShowFlash != bShow)
	{
		m_bModifyForeground = true;
	}
	m_bShowFlash = bShow;
}

bool CSVGDocument::GetShowFlash()
{
	return m_bShowFlash;
}

bool CSVGDocument::HasTextFlash()
{
	if (m_nTextFocusPos >= 0)
		return true;
	return false;
}

bool CSVGDocument::IsShowGrid()
{
	return m_bShowGrid;
}

bool CSVGDocument::IsShowRule()
{
	return m_bShowRule;
}

void CSVGDocument::ShowGrid(bool bShow)
{
	m_bShowGrid = bShow;
	m_bModifyBackground = true;
}

void CSVGDocument::ShowRule(bool bShow)
{
	m_bShowRule = bShow;
	m_bModifyForeground = true;
}

void CSVGDocument::GetChildElement(CSVGElement *pElement, std::vector<CSVGElement*> &vcRet)
{
	if (pElement)
	{
		vcRet.push_back(pElement);
		int nCount = pElement->getChildCount();
		for (int i = 0; i < nCount; ++i)
		{
			CSVGElement* pChild = pElement->getChild(i);
			GetChildElement(pChild, vcRet);
		}
	}
}

std::vector<CSVGElement*> CSVGDocument::GetAllElements()
{
	std::vector<CSVGElement*> vcRet;
	if (m_svgRootElement)
	{
		GetChildElement(m_svgRootElement, vcRet);
	}
	return vcRet;
}

bool CSVGDocument::IsUrlIdUsedByShape(std::wstring strid)
{
	std::set<std::wstring>::iterator iter = m_usedurlids.find(strid);
	if (iter != m_usedurlids.end())
	{
		return true;
	}
	return false;
}

void CSVGDocument::Lock()
{
	if (m_pInnerSelectShape)
	{
		m_pInnerSelectShape->SetLock(true);
		m_FocusManager.RemoveElement(m_pInnerSelectShape);
		m_pInnerSelectShape = NULL;
	}
	else
	{
		for (int i = 0; i < m_vcSelectShapes.size(); ++i)
		{
			m_vcSelectShapes[i]->SetLock(true);
			m_FocusManager.RemoveElement(m_vcSelectShapes[i]);
		}
		m_vcSelectShapes.clear();
	}
	m_bHasLockShape = true;
	m_bModifyForeground = true;
}

void CSVGDocument::UnLock()
{
	std::vector<CSVGElement *>vcElement = GetAllElements();
	for (int i = 0; i < (int)vcElement.size(); ++i)
	{
		vcElement[i]->SetLock(false);
	}
	m_bHasLockShape = false;
}

bool CSVGDocument::HasLockShape()
{
	return m_bHasLockShape;
}
