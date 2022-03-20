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
#include "ToolButtonColorMenu.h"
#include <QStyleOptionToolButton>
#include "QPainter"
#include <QColor>

CToolButtonColorMenu::CToolButtonColorMenu(int nIconIndex, QColor defaultclr, QWidget *parent)
	:QToolButton(parent)
{
	m_nIconIndex = nIconIndex;
	if (m_nIconIndex == 2)
	{
		m_pMenu = new CMenuColor(1, parent);
	}
	else
	{
		m_pMenu = new CMenuColor(0, parent);
	}
	m_currentColor = defaultclr;
	QIcon icon = GetUnderLineColorIcon(m_currentColor);
	this->setIcon(icon);
	connect(m_pMenu, SIGNAL(colorSelected(QColor)), this, SLOT(OnColorSelect(QColor)));
	connect(m_pMenu, SIGNAL(colorSelectedRadient(const std::wstring&)), this, SLOT(OncolorSelectedRadient(const std::wstring&)));
	connect(m_pMenu, SIGNAL(colorSelectedPattern(const std::wstring&)), this, SLOT(OncolorSelectedPattern(const std::wstring&)));
	connect(m_pMenu, SIGNAL(colorSelectedPicture(const std::wstring&)), this, SLOT(OncolorSelectedPicture(const std::wstring&)));
}

CToolButtonColorMenu::~CToolButtonColorMenu()
{

}

void CToolButtonColorMenu::mousePressEvent(QMouseEvent *event)
{
	if (Qt::LeftButton == event->button())
	{
		QRect popupButtonRect = rect();
		if (popupButtonRect.isValid() && popupButtonRect.contains(event->pos()))
		{
			QRect rcRight = QRect(24, 0, popupButtonRect.width() - 24, popupButtonRect.height());
			if (rcRight.contains(event->pos()))
			{
				QPoint pt(0, popupButtonRect.height());
				pt = mapToGlobal(pt);
				m_pMenu->exec(pt);
			}
			else
			{
				OnColorSelect(m_currentColor);
			}
		}
	}
}

QIcon CToolButtonColorMenu::GetUnderLineColorIcon(const QColor &clr)
{
	QIcon icon;
	QImage img;
	if (m_nIconIndex == 0)
	{
		img = QImage(":/images/textcolor.png");
		setIconSize(QSize(32, 24));
	}
	else if (m_nIconIndex == 1)
	{
		img = QImage(":/images/linecolor.png");
		setIconSize(QSize(32, 24));
	}
	else if (m_nIconIndex == 2)
	{
		img = QImage(":/images/fillcolor.png");
		setIconSize(QSize(32, 24));
	}
	QPainter painter(&img);
	painter.setPen(Qt::NoPen);
	painter.setBrush(clr);
	painter.drawRect(0, 20, 20, 4);
	QPixmap pix = QPixmap::fromImage(img);
	icon.addPixmap(pix);
	return icon;
}

void CToolButtonColorMenu::OnColorSelect(QColor clr)
{
	m_currentColor = clr;
	QIcon icon = GetUnderLineColorIcon(clr);
	this->setIcon(icon);
	emit ColorSelected(clr);
}

void CToolButtonColorMenu::OncolorSelectedRadient(const std::wstring &str)
{
	emit colorSelectedRadient(str);
}

void CToolButtonColorMenu::OncolorSelectedPattern(const std::wstring &str)
{
	emit colorSelectedPattern(str);
}

void CToolButtonColorMenu::OncolorSelectedPicture(const std::wstring &str)
{
	emit colorSelectedPicture(str);
}
