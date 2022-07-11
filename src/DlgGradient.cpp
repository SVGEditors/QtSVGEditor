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
#include "DlgGradient.h"
#include <QPainter>
#include <QMouseEvent>
#include "ElementMaker.h"
#include "SVGGlobal.h"

CDlgGradient::CDlgGradient()
{
	m_ui.setupUi(this);
	m_ptCenter = QPoint(100, 100);
	m_direct = GRADIENT_LEFTTORIGHT;

	COLORPOS clrpos1;
	clrpos1.clr = Qt::white;
	clrpos1.fpos = 0;
	m_vcColor.push_back(clrpos1);
	COLORPOS clrpos2;
	clrpos2.clr = Qt::black;
	clrpos2.fpos = 1;
	m_vcColor.push_back(clrpos2);

	QImage image(":/images/gradienttype.png");
	QImage image1 = image.copy(0, 0, 32, 32);
	QImage image2 = image.copy(32, 0, 32, 32);
	QImage image3 = image.copy(64, 0, 32, 32);
	QImage image4 = image.copy(96, 0, 32, 32);
	QImage image5 = image.copy(128, 0, 32, 32);
	m_ui.pushButtonLeftToRight->setIconSize(QSize(32, 32));
	m_ui.pushButtonLeftToRight->setIcon(QIcon(QPixmap::fromImage(image1)));
	m_ui.pushButtonTopToBottom->setIconSize(QSize(32, 32));
	m_ui.pushButtonTopToBottom->setIcon(QIcon(QPixmap::fromImage(image2)));
	m_ui.pushButtonLeftBottomToRightTop->setIconSize(QSize(32, 32));
	m_ui.pushButtonLeftBottomToRightTop->setIcon(QIcon(QPixmap::fromImage(image3)));
	m_ui.pushButtonLeftTopToRightBottom->setIconSize(QSize(32, 32));
	m_ui.pushButtonLeftTopToRightBottom->setIcon(QIcon(QPixmap::fromImage(image4)));
	m_ui.pushButtonRadient->setIconSize(QSize(32, 32));
	m_ui.pushButtonRadient->setIcon(QIcon(QPixmap::fromImage(image5)));

	m_ui.pushButtonColorPick->setIconSize(QSize(64, 64));

	m_pColorMenu = new CMenuColor(0, this);

	connect(m_ui.pushButtonColorPick, SIGNAL(clicked()), this, SLOT(OnColorMenu()));
	connect(m_ui.pushButtonHeadToTail, SIGNAL(clicked()), this, SLOT(OnHeadToTail()));
	connect(m_pColorMenu, SIGNAL(colorSelected(QColor)), this, SLOT(OnColor(QColor)));
	connect(m_ui.horizontalSlider, SIGNAL(valueChanged(int)), this, SLOT(OnSlidervalueChanged(int)));

	connect(m_ui.pushButtonLeftToRight, SIGNAL(clicked()), this, SLOT(OnButton1()));
	connect(m_ui.pushButtonTopToBottom, SIGNAL(clicked()), this, SLOT(OnButton2()));
	connect(m_ui.pushButtonLeftBottomToRightTop, SIGNAL(clicked()), this, SLOT(OnButton3()));
	connect(m_ui.pushButtonLeftTopToRightBottom, SIGNAL(clicked()), this, SLOT(OnButton4()));
	connect(m_ui.pushButtonRadient, SIGNAL(clicked()), this, SLOT(OnButton5()));
	connect(m_ui.comboBoxHis, SIGNAL(currentIndexChanged(int)), this, SLOT(OncurrentHistoryIndexChanged(int)));

	connect(m_ui.pushButtonOK, SIGNAL(clicked()), this, SLOT(OnOk()));
	connect(m_ui.pushButtonCancel, SIGNAL(clicked()), this, SLOT(OnCancel()));


	m_ui.LabelMultiColor->installEventFilter(this);
	m_ui.LabelPeView->setMouseTracking(true);
	m_ui.LabelPeView->installEventFilter(this);

	OnColor(Qt::blue);
	InitHistory();
	ReDrawMultiColorBar();
	ReDrawPreView();
}


CDlgGradient::~CDlgGradient()
{
}

void CDlgGradient::OnOk()
{
	QDialog::accept();
}

void CDlgGradient::OnCancel()
{
	QDialog::close();
}

void CDlgGradient::OncurrentHistoryIndexChanged(int index)
{
	if (index >= 0 && index < m_templateClr.size())
	{
		m_vcColor = m_templateClr[index];
		ReDrawMultiColorBar();
		ReDrawPreView();
	}
}

void CDlgGradient::OnColorMenu()
{
	QRect popupButtonRect = m_ui.pushButtonColorPick->rect();
	QPoint pt(0, popupButtonRect.height());
	pt = m_ui.pushButtonColorPick->mapToGlobal(pt);
	m_pColorMenu->exec(pt);
}

void CDlgGradient::OnColor(QColor clr)
{
	SetColor(clr);
	int nAlpha = clr.alpha();
	m_ui.horizontalSlider->setValue((int)(nAlpha*1.0 / 255) * 100);
}

void CDlgGradient::SetColor(QColor clr)
{
	QPixmap pix(64, 64);
	pix.fill(clr);
	m_ui.pushButtonColorPick->setIcon(QIcon(pix));
	m_ui.pushButtonColorPick->setProperty("color", QVariant(clr));

	for (int i = 0; i < m_vcColor.size(); ++i)
	{
		if (m_vcColor[i].bSelect)
		{
			m_vcColor[i].clr = clr;
			ReDrawMultiColorBar();
			ReDrawPreView();
			break;
		}
	}
}

bool CDlgGradient::eventFilter(QObject *o, QEvent *e)
{
	if (o == m_ui.LabelMultiColor)
	{
		if (e->type() == QEvent::MouseButtonPress)
		{
			QMouseEvent *pmouseevt = (QMouseEvent*)e;
			QPoint pt = pmouseevt->globalPos();
			pt = m_ui.LabelMultiColor->mapFromGlobal(pt);
			if (pmouseevt->button() == Qt::LeftButton)
			{
				PressPosColor(pt.x(), pt.y(), false);
			}
			else if (pmouseevt->button() == Qt::RightButton)
			{
				DeletePosColor(pt.x());
			}
			ReDrawMultiColorBar();
			ReDrawPreView();
		}
		if (e->type() == QEvent::MouseButtonDblClick)
		{
			QMouseEvent *pmouseevt = (QMouseEvent*)e;
			QPoint pt = pmouseevt->globalPos();
			pt = m_ui.LabelMultiColor->mapFromGlobal(pt);
			if (pmouseevt->button() == Qt::LeftButton)
			{
				PressPosColor(pt.x(), pt.y(), true);
			}
		}
		else if (e->type() == QEvent::MouseMove)
		{
			QMouseEvent *pmouseevt = (QMouseEvent*)e;
			if (pmouseevt->buttons()&Qt::LeftButton)
			{
				QPoint pt = pmouseevt->globalPos();
				pt = m_ui.LabelMultiColor->mapFromGlobal(pt);
				MoveColor(pt.x());
			}
			ReDrawMultiColorBar();
			ReDrawPreView();
		}
	}
	else if (o == m_ui.LabelPeView)
	{
		if (e->type() == QEvent::MouseButtonPress)
		{
			QMouseEvent *pmouseevt = (QMouseEvent*)e;
			QPoint pt = pmouseevt->globalPos();
			pt = m_ui.LabelPeView->mapFromGlobal(pt);
			if (std::abs(pt.x() - m_ptCenter.x()) < 8 && std::abs(pt.y() - m_ptCenter.y()) < 8)
			{
				m_bSelectCenter = true;
			}
		}
		else if (e->type() == QEvent::MouseMove)
		{
			QMouseEvent *pmouseevt = (QMouseEvent*)e;
			QPoint pt = pmouseevt->globalPos();
			pt = m_ui.LabelPeView->mapFromGlobal(pt);
			if (std::abs(pt.x() - m_ptCenter.x()) < 8 && std::abs(pt.y() - m_ptCenter.y()) < 8)
			{
				setCursor(Qt::ClosedHandCursor);
			}
			else
			{
				setCursor(Qt::ArrowCursor);
			}
			if (pmouseevt->buttons()&Qt::LeftButton)
			{
				if (m_bSelectCenter)
				{
					m_ptCenter = pt;
				}
			}
			ReDrawMultiColorBar();
			ReDrawPreView();
		}
		else if (e->type() == QEvent::MouseButtonRelease)
		{
			m_bSelectCenter = false;
			setCursor(Qt::ArrowCursor);
		}
	}
	return QDialog::eventFilter(o, e);
}

void CDlgGradient::OnSlidervalueChanged(int value)
{
	QVariant data = m_ui.pushButtonColorPick->property("color");
	QColor color = qvariant_cast<QColor>(data);
	int ntransprant = (value *1.0 / 100) * 255;
	color.setAlpha(ntransprant);
	SetColor(color);
}

void CDlgGradient::InitHistory()
{
	QVector<COLORPOS>clrpos;
	clrpos.push_back(COLORPOS(Qt::black, 0));
	clrpos.push_back(COLORPOS(Qt::white, 0.5));
	clrpos.push_back(COLORPOS(Qt::black, 1));
	m_templateClr.push_back(clrpos);

	clrpos.clear();
	clrpos.push_back(COLORPOS(Qt::white, 0));
	clrpos.push_back(COLORPOS(Qt::black, 0.5));
	clrpos.push_back(COLORPOS(Qt::white, 1));
	m_templateClr.push_back(clrpos);

	clrpos.clear();
	clrpos.push_back(COLORPOS(Qt::yellow, 0));
	clrpos.push_back(COLORPOS(Qt::blue, 0.5));
	clrpos.push_back(COLORPOS(Qt::red, 1));
	m_templateClr.push_back(clrpos);

	clrpos.clear();
	clrpos.push_back(COLORPOS(Qt::green, 0));
	clrpos.push_back(COLORPOS(Qt::gray, 1));
	m_templateClr.push_back(clrpos);

	clrpos.clear();
	clrpos.push_back(COLORPOS(Qt::darkBlue, 0));
	clrpos.push_back(COLORPOS(Qt::yellow, 0.25));
	clrpos.push_back(COLORPOS(Qt::cyan, 0.75));
	clrpos.push_back(COLORPOS(Qt::green, 1));
	m_templateClr.push_back(clrpos);

	m_ui.comboBoxHis->setIconSize(QSize(200, 50));
	for (int i = 0; i < m_templateClr.size(); ++i)
	{
		QPixmap pix(200, 50);
		pix.fill(Qt::blue);
		QVector<COLORPOS> &clrArr = m_templateClr[i];
		QIcon icon = GetGradientImage(clrArr, 200, 50, GRADIENT_LEFTTORIGHT);
		m_ui.comboBoxHis->addItem(icon, "");
	}
}

QIcon CDlgGradient::GetGradientImage(QVector<COLORPOS>&clrArr, int nWidth, int nHeight, GRADIENTDIRECT Direct)
{
	QIcon icon;
	QBrush brush;
	if (Direct != GRADIENT_CENTER)
	{
		qreal xStart = 0;
		qreal yStart = 0;
		qreal xFinalStop = 0;
		qreal yFinalStop = 0;
		switch (Direct)
		{
		case GRADIENT_LEFTTORIGHT:
			xStart = 0;
			yStart = 0;
			xFinalStop = nWidth;
			yFinalStop = 0;
			break;
		case GRADIENT_TOPTOBOTTOM:
			xStart = 0;
			yStart = 0;
			xFinalStop = 0;
			yFinalStop = nHeight;
			break;
		case GRADIENT_LEFTTOTORIGHTBOTTOM:
			xStart = 0;
			yStart = 0;
			xFinalStop = nWidth;
			yFinalStop = nHeight;
			break;
		case GRADIENT_LEFTBOTTOMTORIGHTTOP:
			xStart = 0;
			yStart = nHeight;
			xFinalStop = nWidth;
			yFinalStop = 0;
			break;
		default:
			break;
		}
		QLinearGradient linegradient(xStart, yStart, xFinalStop, yFinalStop);
		for (int i = 0; i < clrArr.size(); ++i)
		{
			linegradient.setColorAt(clrArr[i].fpos, clrArr[i].clr);
		}
		brush = QBrush(linegradient);
	}
	else
	{
		QRadialGradient radiengradient(100, 100, nWidth*1.0 / 2, m_ptCenter.x(), m_ptCenter.y());
		for (int i = 0; i < clrArr.size(); ++i)
		{
			radiengradient.setColorAt(clrArr[i].fpos, clrArr[i].clr);
		}
		brush = QBrush(radiengradient);
	}
	QImage image = QImage(nWidth, nHeight, QImage::Format_RGB32);
	QPainter painter(&image);
	painter.setBrush(brush);
	painter.setPen(Qt::NoPen);
	painter.drawRect(0, 0, nWidth, nHeight);
	QPixmap pix = QPixmap::fromImage(image);
	icon = QIcon(pix);
	return icon;
}

bool  CDlgGradient::IsSelectColor(int nXPos)
{
	int nImageWidth = m_ui.LabelMultiColor->width() - 40;
	if (nXPos < 20)
	{
		nXPos = 20;
	}
	if (nXPos > nImageWidth + 20)
	{
		nXPos = nImageWidth + 20;
	}
	int nPos = nXPos - 20;
	float fpos = nPos * 1.0 / nImageWidth;
	float fDelta = 8.0 / nImageWidth;
	//判断当前是否存在;
	for (int i = 0; i < m_vcColor.size(); ++i)
	{
		if (((m_vcColor[i].fpos - fDelta) < fpos)
			&& (fpos < m_vcColor[i].fpos + fDelta))
		{//说明是存在的;
			return true;
		}
	}
	return false;
}

void  CDlgGradient::DeletePosColor(int nXPos)
{
	int nImageWidth = m_ui.LabelMultiColor->width() - 40;
	if (nXPos < 20)
	{
		nXPos = 20;
	}
	if (nXPos > nImageWidth + 20)
	{
		nXPos = nImageWidth + 20;
	}
	int nPos = nXPos - 20;
	float fpos = nPos * 1.0 / nImageWidth;
	float fDelta = 8.0 / nImageWidth;
	//判断当前是否存在;
	for (int i = 0; i < m_vcColor.size(); ++i)
	{
		if (i == 0 || i == m_vcColor.size() - 1)
		{//头尾不允许删除
			continue;
		}
		if (((m_vcColor[i].fpos - fDelta) < fpos)
			&& (fpos < m_vcColor[i].fpos + fDelta))
		{//说明是存在的;
			m_vcColor.erase(m_vcColor.begin() + i);
			return;
		}
	}
}

bool IsXSmall(COLORPOS rt1, COLORPOS rt2)
{
	if (rt1.fpos < rt2.fpos)
		return true;
	return false;
}

void  CDlgGradient::MoveColor(int nXPos)
{
	int nImageWidth = m_ui.LabelMultiColor->width() - 40;
	if (nXPos < 20)
	{
		nXPos = 20;
	}
	if (nXPos > nImageWidth + 20)
	{
		nXPos = nImageWidth + 20;
	}
	int nPos = nXPos - 20;
	float fpos = nPos * 1.0 / nImageWidth;

	COLORPOS DefaultClr;
	bool bChange = false;
	for (int i = 0; i < m_vcColor.size(); ++i)
	{
		if (m_vcColor[i].bSelect)
		{
			m_vcColor[i].fpos = fpos;
			DefaultClr = m_vcColor[i];
			bChange = true;
			break;
		}
	}
	//移动完之后需要排序
	std::sort(m_vcColor.begin(), m_vcColor.end(), IsXSmall);

	if (bChange)
	{
		//检查一下头尾是否被移走,如果不在,就增加一下头尾;
		if (m_vcColor.size() > 0)
		{
			if (m_vcColor[0].fpos >= 0.01)
			{
				COLORPOS headclr = DefaultClr;
				headclr.fpos = 0;
				headclr.bSelect = false;
				m_vcColor.insert(0, headclr);
			}
			if (m_vcColor[m_vcColor.size() - 1].fpos <= 0.99)
			{
				COLORPOS tailclr = DefaultClr;
				tailclr.fpos = 1;
				tailclr.bSelect = false;
				m_vcColor.push_back(tailclr);
			}
		}
	}
}

void  CDlgGradient::PressPosColor(int nXPos, int nYPos, bool bShowMenu)
{
	int nImageWidth = m_ui.LabelMultiColor->width() - 40;
	if (nXPos < 20)
	{
		nXPos = 20;
	}
	if (nXPos > nImageWidth + 20)
	{
		nXPos = nImageWidth + 20;
	}

	int nPos = nXPos - 20;
	float fpos = nPos * 1.0 / nImageWidth;
	float fDelta = 8.0 / nImageWidth;

	for (int i = 0; i < m_vcColor.size(); ++i)
	{
		m_vcColor[i].bSelect = false;
	}
	//判断当前是否存在;
	bool bExist = false;
	for (int i = 0; i < m_vcColor.size(); ++i)
	{
		if (((m_vcColor[i].fpos - fDelta) < fpos)
			&& (fpos < m_vcColor[i].fpos + fDelta))
		{//说明是存在的;
			m_vcColor[i].bSelect = true;

			QPixmap pix(64, 64);
			pix.fill(m_vcColor[i].clr);
			m_ui.pushButtonColorPick->setIcon(QIcon(pix));
			m_ui.pushButtonColorPick->setProperty("color", QVariant(m_vcColor[i].clr));

			int nAlpha = m_vcColor[i].clr.alpha();
			m_ui.horizontalSlider->setValue((int)(nAlpha*1.0 / 255) * 100);
			bExist = true;
			break;
		}
	}
	//不存在,则插入一个点;
	if (!bExist)
	{
		for (int i = 0; i < m_vcColor.size() - 1; ++i)
		{
			if (m_vcColor[i].fpos < fpos &&fpos < m_vcColor[i + 1].fpos)
			{
				QVariant data = m_ui.pushButtonColorPick->property("color");
				QColor color = qvariant_cast<QColor>(data);
				COLORPOS clrpos;
				clrpos.clr = color;
				clrpos.fpos = fpos;
				clrpos.bSelect = true;
				m_vcColor.insert(i + 1, 1, clrpos);
				break;
			}
		}
	}
	//弹出颜色选项
	if (bShowMenu)
	{
		QPoint pt(nXPos, nYPos);
		pt = m_ui.LabelMultiColor->mapToGlobal(pt);
		m_pColorMenu->exec(pt);
	}
}

void  CDlgGradient::ReDrawMultiColorBar()
{
	int nImageWidth = m_ui.LabelMultiColor->width() - 40;
	QIcon icon = GetGradientImage(m_vcColor, nImageWidth, 40, GRADIENT_LEFTTORIGHT);
	QPixmap pixmap = icon.pixmap(nImageWidth, 40);

	QImage allImage(m_ui.LabelMultiColor->width(), 70, QImage::Format_ARGB32);
	allImage.fill(0);
	QPainter painter(&allImage);
	painter.drawPixmap(20, 0, pixmap);

	painter.setRenderHint(QPainter::Antialiasing);
	painter.setPen(Qt::black);

	for (int i = 0; i < m_vcColor.size(); ++i)
	{//绘制三角形
		COLORPOS &clrpos = m_vcColor[i];
		QPolygon polygon;
		int nXPos = nImageWidth * clrpos.fpos + 20;
		polygon.push_back(QPoint(nXPos, 40));
		polygon.push_back(QPoint(nXPos - 8, 58));
		polygon.push_back(QPoint(nXPos + 8, 58));
		if (clrpos.bSelect)
		{
			painter.setBrush(Qt::black);
		}
		else
		{
			painter.setBrush(Qt::white);
		}

		painter.drawPolygon(polygon);
	}

	m_ui.LabelMultiColor->setPixmap(QPixmap::fromImage(allImage));
}

void  CDlgGradient::OnHeadToTail()
{
	for (int i = 0; i < m_vcColor.size(); ++i)
	{
		m_vcColor[i].fpos = 1 - m_vcColor[i].fpos;
	}
	std::sort(m_vcColor.begin(), m_vcColor.end(), IsXSmall);
	ReDrawMultiColorBar();
	ReDrawPreView();
}

void  CDlgGradient::ReDrawPreView()
{
	QRect previewrc = m_ui.LabelPeView->rect();
	QIcon icon = GetGradientImage(m_vcColor, previewrc.width(), previewrc.height(), m_direct);
	QPixmap pixmap = icon.pixmap(previewrc.width(), previewrc.height());
	QImage image(previewrc.width(), previewrc.height(), QImage::Format_ARGB32);
	QPainter painter(&image);
	painter.drawPixmap(0, 0, pixmap);
	if (m_direct == GRADIENT_CENTER)
	{
		painter.setPen(Qt::blue);
		painter.setRenderHint(QPainter::Antialiasing);
		painter.drawEllipse(m_ptCenter, 8, 8);
	}
	m_ui.LabelPeView->setPixmap(QPixmap::fromImage(image));
}

void CDlgGradient::OnButton1()
{
	m_direct = GRADIENT_LEFTTORIGHT;
	ReDrawPreView();
}

void CDlgGradient::OnButton2()
{
	m_direct = GRADIENT_TOPTOBOTTOM;
	ReDrawPreView();
}

void CDlgGradient::OnButton3()
{
	m_direct = GRADIENT_LEFTTOTORIGHTBOTTOM;
	ReDrawPreView();
}

void CDlgGradient::OnButton4()
{
	m_direct = GRADIENT_LEFTBOTTOMTORIGHTTOP;
	ReDrawPreView();
}

void CDlgGradient::OnButton5()
{
	m_direct = GRADIENT_CENTER;
	ReDrawPreView();
}

std::wstring CDlgGradient::GetXML()
{
	std::wstring strXML;
	CXMLMakerNode rootnode;
	if (m_direct == GRADIENT_CENTER)
	{
		rootnode.SetTagName(L"radialGradient");
		rootnode.SetAttribute(L"cx", L"0.5");
		rootnode.SetAttribute(L"cy", L"0.5");
		rootnode.SetAttribute(L"r", L"0.5");
		float fx = m_ptCenter.x()*1.0 / 200;
		float fy = m_ptCenter.y()*1.0 / 200;
		rootnode.SetAttribute(L"fx", global.ftostring(fx));
		rootnode.SetAttribute(L"fy", global.ftostring(fy));
	}
	else
	{
		rootnode.SetTagName(L"linearGradient");
		if (m_direct == GRADIENT_LEFTTORIGHT)
		{
			rootnode.SetAttribute(L"x1", L"0");
			rootnode.SetAttribute(L"y1", L"0");
			rootnode.SetAttribute(L"x2", L"1");
			rootnode.SetAttribute(L"y2", L"0");
		}
		else if (m_direct == GRADIENT_TOPTOBOTTOM)
		{
			rootnode.SetAttribute(L"x1", L"0");
			rootnode.SetAttribute(L"y1", L"0");
			rootnode.SetAttribute(L"x2", L"0");
			rootnode.SetAttribute(L"y2", L"1");
		}
		else if (m_direct == GRADIENT_LEFTTOTORIGHTBOTTOM)
		{
			rootnode.SetAttribute(L"x1", L"0");
			rootnode.SetAttribute(L"y1", L"0");
			rootnode.SetAttribute(L"x2", L"1");
			rootnode.SetAttribute(L"y2", L"1");
		}
		else if (m_direct == GRADIENT_LEFTBOTTOMTORIGHTTOP)
		{
			rootnode.SetAttribute(L"x1", L"0");
			rootnode.SetAttribute(L"y1", L"1");
			rootnode.SetAttribute(L"x2", L"1");
			rootnode.SetAttribute(L"y2", L"0");
		}
	}
	for (int i = 0; i < m_vcColor.size(); ++i)
	{
		COLORPOS &colorpos = m_vcColor[i];
		CXMLMakerNode stopnode(L"stop");
		std::wstring strclr = global.QColorToStr(colorpos.clr);
		stopnode.SetAttribute(L"stop-color", strclr);
		float stopopacity = colorpos.clr.alpha()*1.0 / 255;
		stopnode.SetAttribute(L"stop-opacity", global.ftostring(stopopacity));
		stopnode.SetAttribute(L"offset", global.ftostring(colorpos.fpos));
		rootnode.m_vcNode.push_back(stopnode);
	}
	strXML = rootnode.GetXML();
	return strXML;
}
