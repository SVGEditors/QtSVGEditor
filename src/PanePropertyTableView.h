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
#pragma once
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QColor>
#include <QComboBox>
#include "LineEditEX.h"

class CTextItem : public QWidget
{
	Q_OBJECT
public:
	CTextItem(QTableWidget *parent);
	CTextItem(QString strText, int nRow, int nCol, QTableWidget *parent);
	virtual void ResetValue(QString strValue);
	virtual void resizeEvent(QResizeEvent *event);
	void SetCurrentText(QString strValue);
	virtual void SetItemFocus();
Q_SIGNALS:
	void textChanged(int nrow, const QString &str);
public slots:
	void OneditingFinished();
	void OnFocusIn();
protected:
	CLineEditEX  *m_pLineEdit;
	QString      m_strText;
	QString      m_strOldText;
	int          m_nRow;
	int          m_nCol;
	bool         m_bLockChange;
	QTableWidget *m_pTableWidget;
};

class CColorItem : public CTextItem
{
	Q_OBJECT
public:
	CColorItem(QString strValue, int nRow, int nCol, QTableWidget *parent);
	virtual void ResetValue(QString strValue);
	virtual void resizeEvent(QResizeEvent *event);
public slots:
	void OnButtonClick();
	void OncolorSelected(QColor clr);
	void OneditingFinished();
public:
	void SetColor(QColor clr);
	QColor GetColor();
	QString GetText();
private:
	void   ColorToButton();
	void   ColorToEdit();
private:
	QPushButton *m_pColorButton;
	QColor       m_clr;
};

//////////////////////////////////////////////////////////////////////////
class CChooseItem : public CTextItem
{
	Q_OBJECT
public:
	CChooseItem(QString strValue, int nRow, int nCol, QTableWidget *parent);
	virtual void ResetValue(QString strValue);
	virtual void resizeEvent(QResizeEvent *event);
	void SetText(QString strValue);
	QString GetText();
public slots:
	void OnButtonClick();
Q_SIGNALS:
	void ButtonClick(int nrow);
private:
	QPushButton *m_pButton;
};

//////////////////////////////////////////////////////////////////////////
class CSliderItem : public CTextItem
{
	Q_OBJECT
public:
	CSliderItem(QString strValue, int nMinValue, int nMaxValue, int nRow, int nCol, QTableWidget *parent);
	virtual void ResetValue(QString strValue);
	virtual void resizeEvent(QResizeEvent *event);
public slots:
	void OnSlidervalueChanged(int value);
	void OneditingFinished();
	void OnsliderReleased();
private:
	void   TextToSlider(QString strText);
private:
	QSlider     *m_pSlider;
	int          m_nMinValue;
	int          m_nMaxValue;
};

class CComboxItem : public CTextItem
{
	Q_OBJECT
public:
	CComboxItem(QString strValue, int nRow, int nCol, const QStringList &droplist, bool bEditable, QTableWidget *parent);
	virtual void ResetValue(QString strValue);
	virtual void resizeEvent(QResizeEvent *event);
	virtual void SetItemFocus();
	void SetCurrentText(QString strValue);
	QString GetText();
Q_SIGNALS:
	void textChanged(int nrow, const QString &str);
public slots:
	void OnEndFocus(const QString &str);
private:
	QComboBox *m_pCombox;
};

//////////////////////////////////////////////////////////////////////////
class CPanePropertyTableView : public QTableWidget
{
	Q_OBJECT
public:
	CPanePropertyTableView(QWidget *parent = Q_NULLPTR);
	~CPanePropertyTableView();
	QSize sizeHint() const;
	void ClearProperty();
	void AddHeadName(QString strHeadName);
	bool UpdateProperty(QString strName, QString strValue);
	void AddProperty(QString strName, QString strValue);
	void LockNotify(bool bNotify);
public slots:
	void OnitemSelectionChanged();
Q_SIGNALS:
	void textChanged(const QString &strName, const QString &strValue);
public slots:
	void OntextChanged(int nrow, const QString &str);
private:
	bool m_bNotify;
};
