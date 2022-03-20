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
#include "SVGElementTextContent.h"
#include "SVGElementText.h"
#include "SVGDocument.h"


CSVGElementTextContent::CSVGElementTextContent(CSVGDocument* ownerDoc)
	:CSVGElementShape(ownerDoc, L"textcontent", SVG_TEXTCONTENT)
{
	m_btopbottom = false;
	m_nfontPixelSize = 12 * global.m_fpt2pixel;
	m_bShow = false;
	m_fontName = L"SimSun";
	m_bUnderLine = false;
	m_bBold = false;
	m_bItalic = false;
}

CSVGElementTextContent::~CSVGElementTextContent()
{

}

bool  CSVGElementTextContent::IsShapeInRect(const CSVGRectF &rectf)
{
	Q_UNUSED(rectf);
	return false;
}

void CSVGElementTextContent::ResetTextFont(const std::wstring & Text, float nx, float ny)
{
	Q_UNUSED(Text);
	Q_UNUSED(nx);
	Q_UNUSED(ny);
	std::wstring fontsize = GetUseStyle()[ATTR_FONT_SIZE].c_str();
	if (fontsize.length() > 0)
	{
		QString strFontSize = QString::fromStdWString(fontsize);
		if (strFontSize.indexOf("pt", 0, Qt::CaseInsensitive) > 0)
		{
			strFontSize = strFontSize.replace("pt", "", Qt::CaseInsensitive);
			m_nfontPixelSize = global.m_fpt2pixel *(int)(global.atof_3(strFontSize.toStdWString().c_str()));
		}
		else
		{
			strFontSize = strFontSize.replace("px", "", Qt::CaseInsensitive);
			m_nfontPixelSize = (int)(global.atof_3(strFontSize.toStdWString().c_str()));
		}
	}
	m_fontName = GetUseStyle()[ATTR_FONT_FAMILY].c_str();
	if (m_fontName.length() == 0)
	{
		m_fontName = m_pownDocument->GetDefaultFontName();
	}
	m_bItalic = false;
	m_bUnderLine = false;
	m_bBold = false;
	if (GetUseStyle()[ATTR_FONT_STYLE] == L"italic")
	{
		m_bItalic = true;
	}
	if (GetUseStyle()[ATTR_TEXT_DECORATION] == L"underline")
	{
		m_bUnderLine = true;
	}
	if (GetUseStyle()[ATTR_FONT_WEIGHT] == L"bold")
	{
		m_bBold = true;
	}

	m_btopbottom = false;
	if (GetUseStyle()[ATTR_WRITING_MODE] == L"tb")
	{
		m_btopbottom = true;
	}
}

CSVGRectF CSVGElementTextContent::GetTextBox(const std::wstring &val, float fx, float fy)
{
	CSVGRectF box(0, 0, 0, 0);
	QFont font;
	font.setFamily(QString::fromStdWString(m_fontName));
	font.setPixelSize(m_nfontPixelSize);
	font.setBold(m_bBold);
	font.setItalic(m_bItalic);
	font.setUnderline(m_bUnderLine);
	QPainterPath path;
	path.addText(fx, fy, font, QString::fromStdWString(val));
	QRectF rc = path.boundingRect();
	box = CSVGRectF(rc.left(), rc.top(), rc.width(), rc.height());

	std::wstring strAnchor = GetUseStyle()[ATTR_TEXT_ANCHOR];
	float foffsetx = 0;
	CSVGRectF allbox = box;
	if (strAnchor.length() > 0 && val != m_nodeValue)
	{
		allbox = GetTextBox(m_nodeValue, fx, fy);
	}
	if (strAnchor == L"end")
	{
		foffsetx = -allbox.width;
	}
	else if (strAnchor == L"middle")
	{
		foffsetx = -allbox.width / 2;
	}
	box.x += foffsetx;
	return box;
}

void CSVGElementTextContent::resetTextPath(float nx, float ny)
{
	m_Path = QPainterPath();
	ResetStyleUsed();
	ResetTextFont(m_nodeValue, nx, ny);
	m_bbox = GetTextBox(m_nodeValue, nx, ny);

	std::wstring strAnchor = GetUseStyle()[ATTR_TEXT_ANCHOR];
	float foffsetx = 0;
	if (strAnchor == L"end")
	{
		foffsetx = -m_bbox.width;
	}
	else if (strAnchor == L"middle")
	{
		foffsetx = -m_bbox.width / 2;
	}

	QFont font;
	font.setFamily(QString::fromStdWString(m_fontName));
	font.setPixelSize(m_nfontPixelSize);
	font.setBold(m_bBold);
	font.setItalic(m_bItalic);
	font.setUnderline(m_bUnderLine);
	m_Path = QPainterPath();
	m_Path.addText(QPointF(foffsetx + nx, ny), font, QString::fromStdWString(m_nodeValue));
}

int CSVGElementTextContent::GetTextLength()
{
	int ntextlen = m_nodeValue.length();
	return ntextlen;
}

void  CSVGElementTextContent::setNodeValue(const std::wstring &value, bool bSetModfiyFlag)
{
	CSVGElementShape::setNodeValue(value, bSetModfiyFlag);
	FormatSpaceString();
	m_strXml.clear();
	if (bSetModfiyFlag)
	{
		pathHasChange();
	}
}

void CSVGElementTextContent::FormatSpaceString()
{
	if (m_nodeValue.length() > 0)
	{
		wchar_t *pText = NULL;
		int nlen = m_nodeValue.length();
		pText = new wchar_t[nlen + 1];
		std::wmemset(pText, 0, nlen + 1);
		int nIndex = 0;
		for (int i = 0; i < nlen; ++i)
		{
			if (m_nodeValue[i] == L'\t' || m_nodeValue[i] == L'\n')
			{
				pText[nIndex++] = L' ';
			}
			else if (m_nodeValue[i] != L'\r')
			{
				pText[nIndex++] = m_nodeValue[i];
			}
		}
		m_nodeValue = pText;
		delete pText;
	}
}

bool CSVGElementTextContent::Toxml(std::wstring &strxml, int nLevel)
{
	Q_UNUSED(nLevel);
	strxml += m_nodeValue;
	return true;
}
