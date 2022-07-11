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
#include "PanePropertyTableView.h"
#include <QHeaderView>
#include <QPixmap>
#include "MenuColor.h"
#include "SVGGlobal.h"

CTextItem::CTextItem(QTableWidget *parent)
	:QWidget(parent)
{
	m_pLineEdit = NULL;
	m_bLockChange = false;
	m_pTableWidget = parent;
}

CTextItem::CTextItem(QString strText, int nRow, int nCol, QTableWidget *parent)
	: QWidget(parent)
{
	m_pTableWidget = parent;
	m_bLockChange = false;
	m_strText = strText;
	m_strOldText = strText;
	m_nRow = nRow;
	m_nCol = nCol;
	m_pLineEdit = new CLineEditEX(this);
	m_pLineEdit->setText(m_strText);
	m_pLineEdit->setStyleSheet("border:0 solid #CCCCCC;");
	connect(m_pLineEdit, SIGNAL(editingFinished()), this, SLOT(OneditingFinished()));
	connect(m_pLineEdit, SIGNAL(focusIn()), this, SLOT(OnFocusIn()));
}

void CTextItem::ResetValue(QString strValue)
{
	m_strText = strValue;
	m_strOldText = strValue;
	m_pLineEdit->setText(m_strText);
}

void CTextItem::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	if (m_pLineEdit)
	{
		QRectF rc = this->rect();
		m_pLineEdit->setGeometry(rc.left(), rc.top(), rc.width(), rc.height());
	}
}

void CTextItem::SetCurrentText(QString strValue)
{
	m_bLockChange = true;
	m_pLineEdit->setText(strValue);
	m_bLockChange = false;
}

void CTextItem::OneditingFinished()
{
	m_strText = m_pLineEdit->text();
	if (m_strOldText != m_strText)
	{
		m_strOldText = m_strText;
		if (!m_bLockChange)
		{
			emit textChanged(m_nRow, m_strText);
		}
	}
}

void CTextItem::SetItemFocus()
{
	if (m_pLineEdit)
	{
		m_pLineEdit->setFocus();
		m_pLineEdit->selectAll();
	}
}

void CTextItem::OnFocusIn()
{
	if (m_pTableWidget)
	{
		m_pTableWidget->setCurrentCell(m_nRow, m_nCol);
	}
}

//////////////////////////////////////////////////////////////////////////
CColorItem::CColorItem(QString strValue, int nRow, int nCol, QTableWidget *parent)
	: CTextItem(parent)
{
	m_nRow = nRow;
	m_nCol = nCol;
	m_strOldText = strValue;
	m_pColorButton = new QPushButton(this);
	m_pLineEdit = new CLineEditEX(this);
	m_pLineEdit->setStyleSheet("border:1 solid #CCCCCC;");

	SVGCOLORREF svgclr = global.StrToColor(strValue.toStdWString());
	QColor clr(SVGGetRValue(svgclr), SVGGetGValue(svgclr), SVGGetBValue(svgclr), SVGGetAValue(svgclr));
	m_clr = clr;
	connect(m_pColorButton, SIGNAL(clicked()), this, SLOT(OnButtonClick()));
	connect(m_pLineEdit, SIGNAL(editingFinished()), this, SLOT(OneditingFinished()));
	connect(m_pLineEdit, SIGNAL(focusIn()), this, SLOT(OnFocusIn()));
	ColorToButton();
	ColorToEdit();
}

void CColorItem::ResetValue(QString strValue)
{
	m_strOldText = strValue;
	SVGCOLORREF svgclr = global.StrToColor(strValue.toStdWString());
	QColor clr(SVGGetRValue(svgclr), SVGGetGValue(svgclr), SVGGetBValue(svgclr), SVGGetAValue(svgclr));
	m_clr = clr;
	ColorToButton();
}

void CColorItem::ColorToButton()
{
	QSize btnsize = m_pColorButton->size();
	QPixmap pixmap(btnsize);
	pixmap.fill(m_clr);
	m_pColorButton->setIconSize(m_pColorButton->size());
	m_pColorButton->setIcon(QIcon(pixmap));
}

void   CColorItem::ColorToEdit()
{
	SVGCOLORREF clr = SVGRGBA(m_clr.red(), m_clr.green(), m_clr.blue(), m_clr.alpha());
	std::wstring strclr = global.ColorToStr(clr);
	m_pLineEdit->setText(QString::fromStdWString(strclr));
}

void CColorItem::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	QRectF rc = this->rect();
	int nColorBtnWidth = rc.height() - 4;
	int nLeftWidth = rc.width() - rc.height();
	m_pLineEdit->setGeometry(rc.left() + 2, rc.top() + 2, nLeftWidth - 4, nColorBtnWidth);
	m_pColorButton->setGeometry(nLeftWidth + 2, rc.top() + 2, nColorBtnWidth, nColorBtnWidth);
	ColorToButton();
	ColorToEdit();
}

void CColorItem::SetColor(QColor clr)
{
	m_clr = clr;
	ColorToButton();
	ColorToEdit();
}

QColor CColorItem::GetColor()
{
	return m_clr;
}

QString CColorItem::GetText()
{
	QString str = m_pLineEdit->text();
	return str;
}

void CColorItem::OncolorSelected(QColor clr)
{
	m_clr = clr;
	ColorToButton();
	ColorToEdit();
	SVGCOLORREF svgclr = SVGRGBA(m_clr.red(), m_clr.green(), m_clr.blue(), m_clr.alpha());
	std::wstring strclr = global.ColorToStr(svgclr);
	m_strOldText = QString::fromStdWString(strclr);
	emit textChanged(m_nRow, QString::fromStdWString(strclr));
}

void CColorItem::OneditingFinished()
{
	QString str = m_pLineEdit->text();
	if (m_strOldText != str)
	{
		SVGCOLORREF clr = global.StrToColor(str.toStdWString());
		OncolorSelected(global.SVGColorToQ(clr));
	}
}

void CColorItem::OnButtonClick()
{
	CMenuColor menuclr;
	connect(&menuclr, SIGNAL(colorSelected(QColor)), this, SLOT(OncolorSelected(QColor)));

	QPoint pt(0, m_pColorButton->height());
	pt = m_pColorButton->mapToGlobal(pt);
	menuclr.exec(pt);
}

//////////////////////////////////////////////////////////////////////////
CChooseItem::CChooseItem(QString strValue, int nRow, int nCol, QTableWidget *parent)
	: CTextItem(parent)
{
	m_nRow = nRow;
	m_nCol = nCol;
	m_pButton = new QPushButton(this);
	m_pLineEdit = new CLineEditEX(this);
	m_pLineEdit->setText(strValue);
	m_pLineEdit->setStyleSheet("border:1 solid #CCCCCC;");
	connect(m_pLineEdit, SIGNAL(focusIn()), this, SLOT(OnFocusIn()));
	connect(m_pButton, SIGNAL(clicked()), this, SLOT(OnButtonClick()));
	m_pButton->setText("...");
}

void CChooseItem::ResetValue(QString strValue)
{
	m_pLineEdit->setText(strValue);
}

void CChooseItem::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	QRectF rc = this->rect();
	int nColorBtnWidth = rc.height() - 4;
	int nLeftWidth = rc.width() - rc.height();
	m_pLineEdit->setGeometry(rc.left() + 2, rc.top() + 2, nLeftWidth - 4, nColorBtnWidth);
	m_pButton->setGeometry(nLeftWidth + 2, rc.top() + 2, nColorBtnWidth, nColorBtnWidth);
}

void CChooseItem::SetText(QString strValue)
{
	m_pLineEdit->setText(strValue);
}

QString CChooseItem::GetText()
{
	return m_pLineEdit->text();
}

void CChooseItem::OnButtonClick()
{
	emit ButtonClick(m_nRow);
}

//////////////////////////////////////////////////////////////////////////
CSliderItem::CSliderItem(QString strValue, int nMinValue, int nMaxValue, int nRow, int nCol, QTableWidget *parent)
	:CTextItem(parent)
{
	m_nRow = nRow;
	m_nCol = nCol;
	m_strOldText = strValue;
	m_nMinValue = nMinValue;
	m_nMaxValue = nMaxValue;
	m_pLineEdit = new CLineEditEX(this);
	m_pLineEdit->setStyleSheet("border:1 solid #CCCCCC;");
	float fNowValue = (strValue.toFloat() - m_nMinValue) / m_nMaxValue;
	m_pLineEdit->setText(QString::number(fNowValue, 'f', 2));
	connect(m_pLineEdit, SIGNAL(focusIn()), this, SLOT(OnFocusIn()));

	m_pSlider = new QSlider(this);
	m_pSlider->setOrientation(Qt::Horizontal);
	m_pSlider->setMaximum(100);
	connect(m_pLineEdit, SIGNAL(editingFinished()), this, SLOT(OneditingFinished()));
	connect(m_pSlider, SIGNAL(valueChanged(int)), this, SLOT(OnSlidervalueChanged(int)));
	connect(m_pSlider, SIGNAL(sliderReleased()), this, SLOT(OnsliderReleased()));
	TextToSlider(strValue);
}

void CSliderItem::ResetValue(QString strValue)
{
	m_strOldText = strValue;
	TextToSlider(strValue);
	float fNowValue = strValue.toFloat();
	m_pLineEdit->setText(QString::number(fNowValue, 'f', 2));
}

void CSliderItem::TextToSlider(QString strText)
{
	float fNowValue = (strText.toFloat() - m_nMinValue) / m_nMaxValue;
	m_pSlider->setValue((int)(fNowValue * 100));
}

void CSliderItem::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	QRectF rc = this->rect();
	int nLeftWidth = rc.width() / 2;
	m_pLineEdit->setGeometry(rc.left() + 2, rc.top() + 2, nLeftWidth - 2, rc.height() - 4);
	m_pSlider->setGeometry(rc.left() + nLeftWidth, rc.top() + 2, nLeftWidth, rc.height() - 4);
}

void CSliderItem::OnSlidervalueChanged(int value)
{
	float fNowValue = m_nMaxValue * value*1.0 / 100 + m_nMinValue;
	QString str = QString::number(fNowValue, 'f', 2);
	m_pLineEdit->setText(str);
}

void CSliderItem::OneditingFinished()
{
	QString str = m_pLineEdit->text();
	if (m_strOldText != str)
	{
		float fNowValue = str.toFloat();
		if (fNowValue >= m_nMaxValue)
		{
			fNowValue = m_nMaxValue;
		}
		if (fNowValue <= m_nMinValue)
		{
			fNowValue = m_nMinValue;
		}
		QString strNow = QString::number(fNowValue);;
		m_strOldText = strNow;
		emit textChanged(m_nRow, strNow);
	}
}

void CSliderItem::OnsliderReleased()
{
	OneditingFinished();
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
CComboxItem::CComboxItem(QString strValue, int nRow, int nCol, const QStringList &droplist, bool bEditable, QTableWidget *parent)
	:CTextItem(parent)
{
	m_strOldText = strValue;
	m_nRow = nRow;
	m_nCol = nCol;
	m_pCombox = new QComboBox(this);
	m_pCombox->addItems(droplist);
	m_pCombox->setEditable(bEditable);
	m_pCombox->setCurrentText(strValue);
	connect(m_pCombox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(OnEndFocus(const QString&)));
}

void CComboxItem::ResetValue(QString strValue)
{
	m_pCombox->setCurrentText(strValue);
}

void CComboxItem::SetCurrentText(QString strValue)
{
	m_pCombox->setCurrentText(strValue);
}

QString CComboxItem::GetText()
{
	return m_pCombox->currentText();
}

void CComboxItem::OnEndFocus(const QString &str)
{
	if (m_strOldText != str)
	{
		m_strOldText = str;
		emit textChanged(m_nRow, str);
	}
}

void CComboxItem::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	if (m_pCombox)
	{
		QRectF rc = this->rect();
		m_pCombox->setGeometry(rc.left(), rc.top(), rc.width(), rc.height());
	}
}

void CComboxItem::SetItemFocus()
{
	if (m_pCombox)
	{
		m_pCombox->setFocus();
	}
}
//////////////////////////////////////////////////////////////////////////
CPanePropertyTableView::CPanePropertyTableView(QWidget *parent)
	:QTableWidget(parent)
{
	m_bNotify = true;
	verticalHeader()->hide();
	horizontalHeader()->hide();
	setColumnCount(2);

	horizontalHeader()->setStretchLastSection(true);

	connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(OnitemSelectionChanged()));
}

QSize CPanePropertyTableView::sizeHint() const
{
	return QSize(400, 100);
}

CPanePropertyTableView::~CPanePropertyTableView()
{
}

void CPanePropertyTableView::ClearProperty()
{
	for (int i = rowCount() - 1; i >= 0; --i)
	{
		removeRow(i);
	}
}

void CPanePropertyTableView::AddHeadName(QString strHeadName)
{
	if (rowCount() == 0)
	{
		setRowCount(1);
		setRowHeight(0, 25);
		setSpan(0, 0, 1, 2);
		QTableWidgetItem *pItem = new QTableWidgetItem(strHeadName);
		pItem->setFlags(pItem->flags() & ~Qt::ItemIsEditable);
		pItem->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
		pItem->setBackground(QColor(240, 240, 240));

		setItem(0, 0, pItem);
	}
}

bool CPanePropertyTableView::UpdateProperty(QString strName, QString strValue)
{
	for (int i = 0; i < rowCount(); ++i)
	{
		QTableWidgetItem *pItem = this->item(i, 0);
		if (pItem)
		{
			QString strLabel = pItem->text();
			if (strLabel == strName)
			{
				QWidget *pWidget = cellWidget(i, 1);
				if (pWidget)
				{
					CTextItem *pTextItem = (CTextItem*)pWidget;
					pTextItem->ResetValue(strValue);
					return true;
				}
			}
		}
	}
	return false;
}

void CPanePropertyTableView::AddProperty(QString strName, QString strValue)
{
	if (UpdateProperty(strName, strValue))
	{
		return;
	}
	int nRowCount = rowCount();
	setRowCount(nRowCount + 1);
	setRowHeight(nRowCount, 30);
	QTableWidgetItem *pItem = new QTableWidgetItem(strName);
	pItem->setFlags(pItem->flags() & ~Qt::ItemIsEditable);
	pItem->setBackground(QColor(240, 240, 240));
	setItem(nRowCount, 0, pItem);
	CTextItem *pWidget = NULL;
	if (strName == "fill-opacity" || strName == "stroke-opacity")
	{
		pWidget = new CSliderItem(strValue, 0, 1, nRowCount, 1, this);
		setCellWidget(nRowCount, 1, pWidget);
	}
	else if (strName == "stroke" || strName == "fill")
	{
		COLOR_TYPE clrtype = global.GetColorType(strValue.toStdWString());
		if (clrtype == COLOR_RGB || clrtype == COLOR_NULL)
		{
			pWidget = new CColorItem(strValue, nRowCount, 1, this);
			setCellWidget(nRowCount, 1, pWidget);
		}
		else
		{
			pWidget = new CTextItem(strValue, nRowCount, 1, this);
			setCellWidget(nRowCount, 1, pWidget);
		}
	}
	else if (strName == "stroke-width")
	{
		pWidget = new CSliderItem(strValue, 0, 20, nRowCount, 1, this);
		setCellWidget(nRowCount, 1, pWidget);
	}
	else
	{
		pWidget = new CTextItem(strValue, nRowCount, 1, this);
		setCellWidget(nRowCount, 1, pWidget);
	}
	connect(pWidget, SIGNAL(textChanged(int, const QString&)), this, SLOT(OntextChanged(int, const QString&)));
}

void CPanePropertyTableView::OntextChanged(int nrow, const QString &str)
{
	QString strName;
	if (nrow >= 0 && nrow < rowCount())
	{
		QTableWidgetItem  *pItem = item(nrow, 0);
		if (pItem)
		{
			strName = pItem->text();
		}
	}
	if (m_bNotify)
	{
		emit textChanged(strName, str);
	}
}

void CPanePropertyTableView::OnitemSelectionChanged()
{
	int nRow = this->currentRow();
	int nCol = this->currentColumn();
	if (nCol == 0)
	{
		nCol = 1;
		this->setCurrentCell(nRow, nCol);
		return;
	}
	QTableWidgetItem  *pItem = this->item(nRow, nCol);
	if (pItem)
	{
		this->setFocus();
		this->setItemSelected(pItem, true);
	}
	QWidget * pWidget = this->cellWidget(nRow, nCol);
	if (pWidget)
	{
		if (pWidget->inherits("CTextItem"))
		{
			CTextItem *pItem = (CTextItem*)pWidget;
			pItem->SetItemFocus();
		}
	}
}

void CPanePropertyTableView::LockNotify(bool bNotify)
{
	m_bNotify = bNotify;
}
