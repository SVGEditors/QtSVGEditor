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
#include <QWidget>
#include <QEvent>
#include <QDateTime>
#include "SVGDocument.h"

struct EditPath
{
	QString strFullPath;
	QString strFileName;
	EditPath()
	{
	}
};

class CMainFrame;

class CSVGView :public QWidget
{
	Q_OBJECT
public:
	CSVGView(CMainFrame *pSVGEditor);
	~CSVGView();
	virtual bool eventFilter(QObject *o, QEvent *e);
	virtual void wheelEvent(QWheelEvent *event);
	virtual void paintEvent(QPaintEvent *event);
	virtual void resizeEvent(QResizeEvent *event);
	virtual void timerEvent(QTimerEvent *event);
	void         dragEnterEvent(QDragEnterEvent *event);
	void         dropEvent(QDropEvent * event);
	void         DisConnectPanProperty();
	void         ConnectPanProperty();
	void         ReloadFile();
	bool         IsModify();
	std::wstring GetSelectWidth();
	std::wstring GetSelectHeight();
public:
	bool         Save(QString &strName);
	bool         SaveAs(QString &strName);
	void         SaveAsPng();
	void         SaveAsPng(int nwidth, int nheight);
	void         SaveAsJpg();
	void         SetDrawTool(DRAWTOOL drawtool);
	void         ZoomRestore();
	void         LayerTop();
	void         LayerBottom();
	void         LayerUp();
	void         LayerDown();
	void         TurnRight();
	void         TurnLeft();
	void         MirrorRight();
	void         MirrorBottom();
	void         Group();
	void         UnGroup();
	void         LeftAlign();
	void         TopAlign();
	void         RightAlign();
	void         BottomAlign();
	void         VerLineAlign();
	void         HorLineAlign();
	void         HorSpaceAlign();
	void         VerSpaceAlign();
	void         SameWidthAlign();
	void         SameHeightAlign();
	void         SameSizeAlign();

	void         SetAttributes(const std::vector<std::wstring>&AttrNames, const std::vector<std::wstring>&AttrValues, bool bChangeDefult);
	void         FillUrl(const std::wstring &strxml);
	void         ShowGrid();
	void         ShowRule();
public:
	void         OnKeyEvent(QKeyEvent *pKeyEvent);
	void         OnMouseMove(QMouseEvent *pMouseEvent);
	void         OnLButtonDown(QMouseEvent *pMouseEvent);
	void         OnMiddleButtonDown(QMouseEvent *pMouseEvent);
	void         OnMiddleButtonUp(QMouseEvent *pMouseEvent);
	void         OnLButtonDBClick(QMouseEvent *pMouseEvent);
	void         OnLButtonUp(QMouseEvent *pMouseEvent);
	void         OnRButtonUp(QMouseEvent *pMouseEvent);
	void         open(const std::wstring &filepath);
	void         InsertImage(const std::wstring &filepath);
private:
	void         ResetCursor(MOUSESHAPE mouseshape);
public slots:
	void OntextChanged(const QString &strName, const QString &strValue);
	void OnCopy();
	void OnPaste();
	void OnDel();
	void OnUndo();
	void OnRedo();
	void OnGroup();
	void OnUnGroup();
	void OnRotate(int nValue, bool bFinish);
	void OnZoomSize(int nWidth, int nHeight, bool bFinish);
	void OnDrawLine();
	void OnDrawRect();
	void OnDrawCircle();
	void OnDrawEllipse();
	void OnDrawPolyline();
	void OnDrawPolygon();
	void OnDrawPath();
	void OnDrawText();
	void OnDrawImage();
	void OnDrawContinue();
	void OnCanvasNoBack();
	void OnCanvasDeepBlue();
	void OnCanvasMidBlue();
	void OnCanvasLightBlue();
	void OnCanvasWhite();
	void OnCanvasDeepBlack();
	void OnCanvasMoreColor();
	void OnReplaceImage();
public:
	bool    CanUndo();
	bool    CanRedo();
	bool    CanCopy();
	bool    CanPaste();
	bool    CanContinueDraw();
	bool    CanRotate();
	bool    CanGroup();
	bool    CanUnGroup();
	bool    HasSelectShape();
	bool    HasSelectMoreOneShape();
	bool    hasSelectMoveTwoShape();
	bool    HasLockShape();
	void    Lock();
	void    UnLock();
public:
	void    ResetProperty();

private:
	std::wstring			 m_strcommand;
private:
	QPoint                   m_mouseruntimept;
	QPoint                   m_lastmousept;
	QPoint                   m_buttondownpt;
	int                      m_nZoomTimerId;
	int                      m_nZoomTimerEllapse;
	int                      m_nMouseWheelDelta;
public:
	CMainFrame            *  m_pMainFrame;
	EditPath                 m_EditPath;
	CSVGDocument             m_SVGDocument;
	unsigned short           m_lstsvgtype;
	int                      m_timerid;
	int                      m_timermenuid;
	QDateTime                m_lstDateTime;
};
