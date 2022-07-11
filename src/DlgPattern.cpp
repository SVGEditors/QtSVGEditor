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
#include "DlgPattern.h"
#include "MenuColor.h"
#include <QPainter>
#include "ElementMaker.h"
#include "SVGGlobal.h"

CDlgPattern::CDlgPattern()
{
	m_ui.setupUi(this);
	m_nIndex = 0;
	m_foreColor = Qt::black;
	m_backColor = Qt::white;

	m_pColorMenuFore = new CMenuColor(0, this);
	connect(m_pColorMenuFore, SIGNAL(colorSelected(QColor)), this, SLOT(OnMenuForeColor(QColor)));

	m_pColorMenuBack = new CMenuColor(0, this);
	connect(m_pColorMenuBack, SIGNAL(colorSelected(QColor)), this, SLOT(OnMenuBackColor(QColor)));


	connect(m_ui.pushButtonForeColor, SIGNAL(clicked()), this, SLOT(OnForeColor()));
	connect(m_ui.pushButtonBackColor, SIGNAL(clicked()), this, SLOT(OnBackColor()));


	connect(m_ui.pushButtonOK, SIGNAL(clicked()), this, SLOT(OnOK()));
	connect(m_ui.pushButtonCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));

	connect(m_ui.pushButton1, SIGNAL(clicked()), this, SLOT(Button1()));
	connect(m_ui.pushButton2, SIGNAL(clicked()), this, SLOT(Button2()));
	connect(m_ui.pushButton3, SIGNAL(clicked()), this, SLOT(Button3()));
	connect(m_ui.pushButton4, SIGNAL(clicked()), this, SLOT(Button4()));
	connect(m_ui.pushButton5, SIGNAL(clicked()), this, SLOT(Button5()));
	connect(m_ui.pushButton6, SIGNAL(clicked()), this, SLOT(Button6()));
	connect(m_ui.pushButton7, SIGNAL(clicked()), this, SLOT(Button7()));
	connect(m_ui.pushButton8, SIGNAL(clicked()), this, SLOT(Button8()));
	connect(m_ui.pushButton9, SIGNAL(clicked()), this, SLOT(Button9()));
	connect(m_ui.pushButton10, SIGNAL(clicked()), this, SLOT(Button10()));
	connect(m_ui.pushButton11, SIGNAL(clicked()), this, SLOT(Button11()));
	connect(m_ui.pushButton12, SIGNAL(clicked()), this, SLOT(Button12()));

	InitButtonIcon();
	ReDrawPreview();
}

CDlgPattern::~CDlgPattern()
{
}

std::wstring CDlgPattern::GetXML()
{
	std::wstring strfill = global.QColorToStr(m_backColor);
	std::wstring strstroke = global.QColorToStr(m_foreColor);
	CXMLMakerNode patternnode(L"pattern");
	patternnode.SetAttribute(L"patternUnits", L"userSpaceOnUse");
	patternnode.SetAttribute(L"patternContentUnits", L"userSpaceOnUse");
	switch (m_nIndex)
	{
	case 0:
	{
		patternnode.SetAttribute(L"height", L"10");
		patternnode.SetAttribute(L"width", L"10");

		CXMLMakerNode rectElement(L"rect");
		rectElement.SetAttribute(L"x", L"0");
		rectElement.SetAttribute(L"y", L"0");
		rectElement.SetAttribute(L"height", L"10");
		rectElement.SetAttribute(L"width", L"10");
		rectElement.SetAttribute(L"fill", strfill);
		rectElement.SetAttribute(L"fill-opacity", L"1");
		patternnode.m_vcNode.push_back(rectElement);

		CXMLMakerNode lineElement(L"line");
		lineElement.SetAttribute(L"x1", L"0");
		lineElement.SetAttribute(L"x2", L"1");
		lineElement.SetAttribute(L"y1", L"0");
		lineElement.SetAttribute(L"y2", L"0");
		lineElement.SetAttribute(L"stroke", strstroke);
		lineElement.SetAttribute(L"stroke-width", L"1");
		lineElement.SetAttribute(L"stroke-opacity", L"1");
		patternnode.m_vcNode.push_back(lineElement);
	}
	break;
	case 1:
	{
		patternnode.SetAttribute(L"height", L"10");
		patternnode.SetAttribute(L"width", L"10");
		CXMLMakerNode rectElement(L"rect");;

		rectElement.SetAttribute(L"x", L"0");
		rectElement.SetAttribute(L"y", L"0");
		rectElement.SetAttribute(L"height", L"10");
		rectElement.SetAttribute(L"width", L"10");
		rectElement.SetAttribute(L"fill", strfill);
		rectElement.SetAttribute(L"fill-opacity", L"1");
		patternnode.m_vcNode.push_back(rectElement);


		CXMLMakerNode lineElement1(L"line");;
		lineElement1.SetAttribute(L"x1", L"0");
		lineElement1.SetAttribute(L"x2", L"1");
		lineElement1.SetAttribute(L"y1", L"0");
		lineElement1.SetAttribute(L"y2", L"0");
		lineElement1.SetAttribute(L"stroke", strstroke);
		lineElement1.SetAttribute(L"stroke-width", L"1");
		lineElement1.SetAttribute(L"stroke-opacity", L"1");
		patternnode.m_vcNode.push_back(lineElement1);

		CXMLMakerNode lineElement2(L"line");;
		lineElement2.SetAttribute(L"x1", L"5");
		lineElement2.SetAttribute(L"x2", L"6");
		lineElement2.SetAttribute(L"y1", L"5");
		lineElement2.SetAttribute(L"y2", L"5");
		lineElement2.SetAttribute(L"stroke", strstroke);
		lineElement2.SetAttribute(L"stroke-width", L"1");
		lineElement2.SetAttribute(L"stroke-opacity", L"1");
		patternnode.m_vcNode.push_back(lineElement2);
	}
	break;
	case 2:
	{
		patternnode.SetAttribute(L"height", L"5");
		patternnode.SetAttribute(L"width", L"5");
		CXMLMakerNode rectElement(L"rect");;
		rectElement.SetAttribute(L"x", L"0");
		rectElement.SetAttribute(L"y", L"0");
		rectElement.SetAttribute(L"height", L"5");
		rectElement.SetAttribute(L"width", L"5");
		rectElement.SetAttribute(L"fill", strfill);
		rectElement.SetAttribute(L"fill-opacity", L"1");
		patternnode.m_vcNode.push_back(rectElement);

		CXMLMakerNode lineElement(L"line");
		lineElement.SetAttribute(L"x1", L"0");
		lineElement.SetAttribute(L"x2", L"1");
		lineElement.SetAttribute(L"y1", L"0");
		lineElement.SetAttribute(L"y2", L"0");
		lineElement.SetAttribute(L"stroke", strstroke);
		lineElement.SetAttribute(L"stroke-width", L"1");
		lineElement.SetAttribute(L"stroke-opacity", L"1");
		patternnode.m_vcNode.push_back(lineElement);
	}
	break;
	case 3:
	{
		patternnode.SetAttribute(L"height", L"6");
		patternnode.SetAttribute(L"width", L"6");
		CXMLMakerNode rectElement(L"rect");;
		rectElement.SetAttribute(L"x", L"0");
		rectElement.SetAttribute(L"y", L"0");
		rectElement.SetAttribute(L"height", L"6");
		rectElement.SetAttribute(L"width", L"6");
		rectElement.SetAttribute(L"fill", strfill);
		rectElement.SetAttribute(L"fill-opacity", L"1");
		patternnode.m_vcNode.push_back(rectElement);

		CXMLMakerNode lineElement1(L"line");
		lineElement1.SetAttribute(L"x1", L"0");
		lineElement1.SetAttribute(L"x2", L"1");
		lineElement1.SetAttribute(L"y1", L"0");
		lineElement1.SetAttribute(L"y2", L"0");
		lineElement1.SetAttribute(L"stroke", strstroke);
		lineElement1.SetAttribute(L"stroke-width", L"1");
		lineElement1.SetAttribute(L"stroke-opacity", L"1");
		patternnode.m_vcNode.push_back(lineElement1);

		CXMLMakerNode lineElement2(L"line");
		lineElement2.SetAttribute(L"x1", L"3");
		lineElement2.SetAttribute(L"x2", L"4");
		lineElement2.SetAttribute(L"y1", L"3");
		lineElement2.SetAttribute(L"y2", L"3");
		lineElement2.SetAttribute(L"stroke", strstroke);
		lineElement2.SetAttribute(L"stroke-width", L"1");
		lineElement2.SetAttribute(L"stroke-opacity", L"1");
		patternnode.m_vcNode.push_back(lineElement2);
	}
	break;
	case 4:
	{
		patternnode.SetAttribute(L"height", L"2");
		patternnode.SetAttribute(L"width", L"2");
		CXMLMakerNode rectElement(L"rect");;
		rectElement.SetAttribute(L"x", L"0");
		rectElement.SetAttribute(L"y", L"0");
		rectElement.SetAttribute(L"height", L"2");
		rectElement.SetAttribute(L"width", L"2");
		rectElement.SetAttribute(L"fill", strfill);
		rectElement.SetAttribute(L"fill-opacity", L"1");
		patternnode.m_vcNode.push_back(rectElement);

		CXMLMakerNode lineElement(L"line");;
		lineElement.SetAttribute(L"x1", L"0");
		lineElement.SetAttribute(L"x2", L"1");
		lineElement.SetAttribute(L"y1", L"0");
		lineElement.SetAttribute(L"y2", L"0");
		lineElement.SetAttribute(L"stroke", strstroke);
		lineElement.SetAttribute(L"stroke-width", L"1");
		lineElement.SetAttribute(L"stroke-opacity", L"1");
		patternnode.m_vcNode.push_back(lineElement);
	}
	break;
	case 5:
	{
		patternnode.SetAttribute(L"height", L"3");
		patternnode.SetAttribute(L"width", L"3");
		CXMLMakerNode rectElement(L"rect");;
		rectElement.SetAttribute(L"x", L"0");
		rectElement.SetAttribute(L"y", L"0");
		rectElement.SetAttribute(L"height", L"3");
		rectElement.SetAttribute(L"width", L"3");
		rectElement.SetAttribute(L"fill", strfill);
		rectElement.SetAttribute(L"fill-opacity", L"1");
		patternnode.m_vcNode.push_back(rectElement);


		CXMLMakerNode lineElement1(L"line");;
		lineElement1.SetAttribute(L"x1", L"0");
		lineElement1.SetAttribute(L"x2", L"1");
		lineElement1.SetAttribute(L"y1", L"0");
		lineElement1.SetAttribute(L"y2", L"0");
		lineElement1.SetAttribute(L"stroke", strstroke);
		lineElement1.SetAttribute(L"stroke-width", L"1");
		lineElement1.SetAttribute(L"stroke-opacity", L"1");
		patternnode.m_vcNode.push_back(lineElement1);

		CXMLMakerNode lineElement2(L"line");;
		lineElement2.SetAttribute(L"x1", L"2");
		lineElement2.SetAttribute(L"x2", L"3");
		lineElement2.SetAttribute(L"y1", L"2");
		lineElement2.SetAttribute(L"y2", L"2");
		lineElement2.SetAttribute(L"stroke", strstroke);
		lineElement2.SetAttribute(L"stroke-width", L"1");
		lineElement2.SetAttribute(L"stroke-opacity", L"1");
		patternnode.m_vcNode.push_back(lineElement2);
	}
	break;
	case 6:
	{
		patternnode.SetAttribute(L"height", L"10");
		patternnode.SetAttribute(L"width", L"10");
		CXMLMakerNode rectElement(L"rect");;
		rectElement.SetAttribute(L"x", L"0");
		rectElement.SetAttribute(L"y", L"0");
		rectElement.SetAttribute(L"height", L"10");
		rectElement.SetAttribute(L"width", L"10");
		rectElement.SetAttribute(L"fill", strfill);
		rectElement.SetAttribute(L"fill-opacity", L"1");
		patternnode.m_vcNode.push_back(rectElement);

		CXMLMakerNode lineElement(L"line");
		lineElement.SetAttribute(L"x1", L"0");
		lineElement.SetAttribute(L"x2", L"10");
		lineElement.SetAttribute(L"y1", L"0");
		lineElement.SetAttribute(L"y2", L"0");
		lineElement.SetAttribute(L"stroke", strstroke);
		lineElement.SetAttribute(L"stroke-width", L"1");
		lineElement.SetAttribute(L"stroke-opacity", L"1");
		patternnode.m_vcNode.push_back(lineElement);
	}
	break;
	case 7:
	{
		patternnode.SetAttribute(L"height", L"10");
		patternnode.SetAttribute(L"width", L"10");
		CXMLMakerNode rectElement(L"rect");;
		rectElement.SetAttribute(L"x", L"0");
		rectElement.SetAttribute(L"y", L"0");
		rectElement.SetAttribute(L"height", L"10");
		rectElement.SetAttribute(L"width", L"10");
		rectElement.SetAttribute(L"fill", strfill);
		rectElement.SetAttribute(L"fill-opacity", L"1");
		patternnode.m_vcNode.push_back(rectElement);

		CXMLMakerNode lineElement(L"line");
		lineElement.SetAttribute(L"x1", L"0");
		lineElement.SetAttribute(L"x2", L"0");
		lineElement.SetAttribute(L"y1", L"0");
		lineElement.SetAttribute(L"y2", L"10");
		lineElement.SetAttribute(L"stroke", strstroke);
		lineElement.SetAttribute(L"stroke-width", L"1");
		lineElement.SetAttribute(L"stroke-opacity", L"1");
		patternnode.m_vcNode.push_back(lineElement);
	}
	break;
	case 8:
	{
		patternnode.SetAttribute(L"height", L"10");
		patternnode.SetAttribute(L"width", L"10");
		CXMLMakerNode rectElement(L"rect");;
		rectElement.SetAttribute(L"x", L"0");
		rectElement.SetAttribute(L"y", L"0");
		rectElement.SetAttribute(L"height", L"10");
		rectElement.SetAttribute(L"width", L"10");
		rectElement.SetAttribute(L"fill", strfill);
		rectElement.SetAttribute(L"fill-opacity", L"1");
		patternnode.m_vcNode.push_back(rectElement);


		CXMLMakerNode lineElement1(L"line");
		lineElement1.SetAttribute(L"x1", L"0");
		lineElement1.SetAttribute(L"x2", L"0");
		lineElement1.SetAttribute(L"y1", L"0");
		lineElement1.SetAttribute(L"y2", L"10");
		lineElement1.SetAttribute(L"stroke", strstroke);
		lineElement1.SetAttribute(L"stroke-width", L"1");
		lineElement1.SetAttribute(L"stroke-opacity", L"1");
		patternnode.m_vcNode.push_back(lineElement1);

		CXMLMakerNode lineElement2(L"line");
		lineElement2.SetAttribute(L"x1", L"0");
		lineElement2.SetAttribute(L"x2", L"10");
		lineElement2.SetAttribute(L"y1", L"0");
		lineElement2.SetAttribute(L"y2", L"0");
		lineElement2.SetAttribute(L"stroke", strstroke);
		lineElement2.SetAttribute(L"stroke-width", L"1");
		lineElement2.SetAttribute(L"stroke-opacity", L"1");
		patternnode.m_vcNode.push_back(lineElement2);
	}
	break;
	case 9:
	{
		patternnode.SetAttribute(L"height", L"10");
		patternnode.SetAttribute(L"width", L"10");
		CXMLMakerNode rectElement(L"rect");;
		rectElement.SetAttribute(L"x", L"0");
		rectElement.SetAttribute(L"y", L"0");
		rectElement.SetAttribute(L"height", L"10");
		rectElement.SetAttribute(L"width", L"10");
		rectElement.SetAttribute(L"fill", strfill);
		rectElement.SetAttribute(L"fill-opacity", L"1");
		patternnode.m_vcNode.push_back(rectElement);

		CXMLMakerNode lineElement(L"line");
		lineElement.SetAttribute(L"x1", L"0");
		lineElement.SetAttribute(L"x2", L"10");
		lineElement.SetAttribute(L"y1", L"0");
		lineElement.SetAttribute(L"y2", L"10");
		lineElement.SetAttribute(L"stroke", strstroke);
		lineElement.SetAttribute(L"stroke-width", L"1");
		lineElement.SetAttribute(L"stroke-opacity", L"1");
		patternnode.m_vcNode.push_back(lineElement);
	}
	break;
	case 10:
	{
		patternnode.SetAttribute(L"height", L"10");
		patternnode.SetAttribute(L"width", L"10");
		CXMLMakerNode rectElement(L"rect");;
		rectElement.SetAttribute(L"x", L"0");
		rectElement.SetAttribute(L"y", L"0");
		rectElement.SetAttribute(L"height", L"10");
		rectElement.SetAttribute(L"width", L"10");
		rectElement.SetAttribute(L"fill", strfill);
		rectElement.SetAttribute(L"fill-opacity", L"1");
		patternnode.m_vcNode.push_back(rectElement);

		CXMLMakerNode lineElement(L"line");
		lineElement.SetAttribute(L"x1", L"0");
		lineElement.SetAttribute(L"x2", L"10");
		lineElement.SetAttribute(L"y1", L"10");
		lineElement.SetAttribute(L"y2", L"0");
		lineElement.SetAttribute(L"stroke", strstroke);
		lineElement.SetAttribute(L"stroke-width", L"1");
		lineElement.SetAttribute(L"stroke-opacity", L"1");
		patternnode.m_vcNode.push_back(lineElement);
	}
	break;
	case 11:
	{
		patternnode.SetAttribute(L"height", L"5");
		patternnode.SetAttribute(L"width", L"5");
		CXMLMakerNode rectElement(L"rect");;
		rectElement.SetAttribute(L"x", L"0");
		rectElement.SetAttribute(L"y", L"0");
		rectElement.SetAttribute(L"height", L"5");
		rectElement.SetAttribute(L"width", L"5");
		rectElement.SetAttribute(L"fill", strfill);
		rectElement.SetAttribute(L"fill-opacity", L"1");
		patternnode.m_vcNode.push_back(rectElement);

		CXMLMakerNode lineElement(L"line");
		lineElement.SetAttribute(L"x1", L"0");
		lineElement.SetAttribute(L"x2", L"5");
		lineElement.SetAttribute(L"y1", L"0");
		lineElement.SetAttribute(L"y2", L"5");
		lineElement.SetAttribute(L"stroke", strstroke);
		lineElement.SetAttribute(L"stroke-width", L"1");
		lineElement.SetAttribute(L"stroke-opacity", L"1");
		patternnode.m_vcNode.push_back(lineElement);
	}
	break;
	case 12:
	{
		patternnode.SetAttribute(L"height", L"5");
		patternnode.SetAttribute(L"width", L"5");
		CXMLMakerNode rectElement(L"rect");;
		rectElement.SetAttribute(L"x", L"0");
		rectElement.SetAttribute(L"y", L"0");
		rectElement.SetAttribute(L"height", L"5");
		rectElement.SetAttribute(L"width", L"5");
		rectElement.SetAttribute(L"fill", strfill);
		rectElement.SetAttribute(L"fill-opacity", L"1");
		patternnode.m_vcNode.push_back(rectElement);

		CXMLMakerNode lineElement(L"line");
		lineElement.SetAttribute(L"x1", L"0");
		lineElement.SetAttribute(L"x2", L"5");
		lineElement.SetAttribute(L"y1", L"5");
		lineElement.SetAttribute(L"y2", L"0");
		lineElement.SetAttribute(L"stroke", strstroke);
		lineElement.SetAttribute(L"stroke-width", L"1");
		lineElement.SetAttribute(L"stroke-opacity", L"1");
		patternnode.m_vcNode.push_back(lineElement);
	}
	break;
	default:
		break;
	}
	std::wstring strxml = patternnode.GetXML();
	return strxml;
}

void CDlgPattern::OnMenuForeColor(QColor clr)
{
	QPixmap pix(32, 32);
	pix.fill(clr);
	m_ui.pushButtonForeColor->setIconSize(QSize(32, 32));
	m_ui.pushButtonForeColor->setIcon(QIcon(pix));
	m_foreColor = clr;
	ReDrawPreview();
}

void CDlgPattern::OnMenuBackColor(QColor clr)
{
	QPixmap pix(32, 32);
	pix.fill(clr);
	m_ui.pushButtonBackColor->setIconSize(QSize(32, 32));
	m_ui.pushButtonBackColor->setIcon(QIcon(pix));
	m_backColor = clr;
	ReDrawPreview();
}

void CDlgPattern::Button1()
{
	m_nIndex = 0;
	ReDrawPreview();
}
void CDlgPattern::Button2()
{
	m_nIndex = 1;
	ReDrawPreview();
}
void CDlgPattern::Button3()
{
	m_nIndex = 2;
	ReDrawPreview();
}
void CDlgPattern::Button4()
{
	m_nIndex = 3;
	ReDrawPreview();
}
void CDlgPattern::Button5()
{
	m_nIndex = 4;
	ReDrawPreview();
}
void CDlgPattern::Button6()
{
	m_nIndex = 5;
	ReDrawPreview();
}
void CDlgPattern::Button7()
{
	m_nIndex = 6;
	ReDrawPreview();
}
void CDlgPattern::Button8()
{
	m_nIndex = 7;
	ReDrawPreview();
}
void CDlgPattern::Button9()
{
	m_nIndex = 8;
	ReDrawPreview();
}
void CDlgPattern::Button10()
{
	m_nIndex = 9;
	ReDrawPreview();
}
void CDlgPattern::Button11()
{
	m_nIndex = 10;
	ReDrawPreview();
}
void CDlgPattern::Button12()
{
	m_nIndex = 11;
	ReDrawPreview();
}

void CDlgPattern::InitButtonIcon()
{
	QImage image(":/images/pattern.png");
	QImage image1 = image.copy(0, 0, 32, 32);
	m_ui.pushButton1->setIconSize(QSize(32, 32));
	m_ui.pushButton1->setIcon(QIcon(QPixmap::fromImage(image1)));

	QImage image2 = image.copy(0, 1 * 32, 32, 32);
	m_ui.pushButton2->setIconSize(QSize(32, 32));
	m_ui.pushButton2->setIcon(QIcon(QPixmap::fromImage(image2)));

	QImage image3 = image.copy(0, 2 * 32, 32, 32);
	m_ui.pushButton3->setIconSize(QSize(32, 32));
	m_ui.pushButton3->setIcon(QIcon(QPixmap::fromImage(image3)));

	QImage image4 = image.copy(0, 3 * 32, 32, 32);
	m_ui.pushButton4->setIconSize(QSize(32, 32));
	m_ui.pushButton4->setIcon(QIcon(QPixmap::fromImage(image4)));

	QImage image5 = image.copy(0, 4 * 32, 32, 32);
	m_ui.pushButton5->setIconSize(QSize(32, 32));
	m_ui.pushButton5->setIcon(QIcon(QPixmap::fromImage(image5)));

	QImage image6 = image.copy(0, 5 * 32, 32, 32);
	m_ui.pushButton6->setIconSize(QSize(32, 32));
	m_ui.pushButton6->setIcon(QIcon(QPixmap::fromImage(image6)));

	QImage image7 = image.copy(0, 6 * 32, 32, 32);
	m_ui.pushButton7->setIconSize(QSize(32, 32));
	m_ui.pushButton7->setIcon(QIcon(QPixmap::fromImage(image7)));

	QImage image8 = image.copy(0, 7 * 32, 32, 32);
	m_ui.pushButton8->setIconSize(QSize(32, 32));
	m_ui.pushButton8->setIcon(QIcon(QPixmap::fromImage(image8)));

	QImage image9 = image.copy(0, 8 * 32, 32, 32);
	m_ui.pushButton9->setIconSize(QSize(32, 32));
	m_ui.pushButton9->setIcon(QIcon(QPixmap::fromImage(image9)));

	QImage image10 = image.copy(0, 9 * 32, 32, 32);
	m_ui.pushButton10->setIconSize(QSize(32, 32));
	m_ui.pushButton10->setIcon(QIcon(QPixmap::fromImage(image10)));

	QImage image11 = image.copy(0, 10 * 32, 32, 32);
	m_ui.pushButton11->setIconSize(QSize(32, 32));
	m_ui.pushButton11->setIcon(QIcon(QPixmap::fromImage(image11)));

	QImage image12 = image.copy(0, 11 * 32, 32, 32);
	m_ui.pushButton12->setIconSize(QSize(32, 32));
	m_ui.pushButton12->setIcon(QIcon(QPixmap::fromImage(image12)));

	QPixmap pix(32, 32);
	pix.fill(Qt::black);
	m_ui.pushButtonForeColor->setIconSize(QSize(32, 32));
	m_ui.pushButtonForeColor->setIcon(QIcon(pix));

	pix.fill(Qt::white);
	m_ui.pushButtonBackColor->setIconSize(QSize(32, 32));
	m_ui.pushButtonBackColor->setIcon(QIcon(pix));
}

void CDlgPattern::OnForeColor()
{
	QPoint pt(0, m_ui.pushButtonForeColor->height());
	pt = m_ui.pushButtonForeColor->mapToGlobal(pt);
	m_pColorMenuFore->exec(pt);
}

void CDlgPattern::OnBackColor()
{
	QPoint pt(0, m_ui.pushButtonBackColor->height());
	pt = m_ui.pushButtonBackColor->mapToGlobal(pt);
	m_pColorMenuBack->exec(pt);
}

void CDlgPattern::OnOK()
{
	QDialog::accept();
}

void CDlgPattern::OnCancel()
{
	QDialog::close();
}

void CDlgPattern::ReDrawPreview()
{
	QImage image(":/images/pattern.png");
	image = image.convertToFormat(QImage::Format_RGBA8888_Premultiplied, Qt::NoFormatConversion);
	QImage image1 = image.copy(0, m_nIndex * 32, 32, 32);
	for (int i = 0; i < 32; ++i)
	{
		for (int j = 0; j < 32; ++j)
		{
			QColor  clr = image1.pixel(QPoint(i, j));
			if (clr == QColor(0, 0, 0))
			{
				image1.setPixel(i, j, m_foreColor.rgb());
			}
			else if (clr == QColor(255, 255, 255))
			{
				image1.setPixel(i, j, m_backColor.rgb());
			}
		}
	}
	QImage largeImage(200, 200, QImage::Format_ARGB32);
	QPainter painter(&largeImage);
	painter.drawImage(QRect(0, 0, 200, 200), image1, QRect(0, 0, 32, 32));
	m_ui.pushButtonPreview->setIconSize(QSize(200, 200));
	m_ui.pushButtonPreview->setIcon(QIcon(QPixmap::fromImage(largeImage)));
}
