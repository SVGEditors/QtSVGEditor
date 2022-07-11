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
#include <QListWidget>
#include <QTableWidget>
#include <QTabWidget>
#include <QTextEdit>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QtWidgets/QMainWindow>
#include "SVGView.h"
#include "PanePropertyTableView.h"
#include "TabWidgetEx.h"
#include "QFontComboBox"
#include "ToolButtonColorMenu.h"
#include "ToolButtonComboxMenu.h"
#include <QLabel>

class CMainFrame : public QMainWindow
{
	Q_OBJECT
public:
	CMainFrame(QWidget *parent = Q_NULLPTR);
public:
	void timerEvent(QTimerEvent *event) override;
	void closeEvent(QCloseEvent *e) override;
	void OpenFile(const QString &strPath);
	void ForceCloseFile(const QString &strPath);
	void ReNameFile(const QString &strOldPath, const QString &strNewPath);
	void SetCurrentFontName(QString strFontName);
	void SetCurrentFontSize(QString strFontSize);
	bool maybeSave();
	bool ShowSaveMessage(QWidget *pWidget);
	void ShowMousePosition(QPoint pt);
	virtual void dragEnterEvent(QDragEnterEvent *event);
	virtual void dropEvent(QDropEvent *event);
public slots:

	void fileNew();
	void fileOpen();
	void fileSave();
	void fileSaveAs();
	void fileSaveASPng();
	void fileSaveASJpg();

	void copy();
	void paste();
	void undo();
	void redo();

	void perpertyview();

	void drawline();
	void drawrect();
	void drawcircle();
	void drawellipse();
	void drawpolyline();
	void drawpolygon();
	void drawpath();
	void drawarc();
	void drawtext();
	void drawtextAAA();
	void drawtextv();
	void drawimage();
	void drawcontinue();
	void drawcustom(const QString &strText);

	void arrow();
	void arrowmatrix();
	void zoomrestore();
	void zoomout();
	void zoomin();
	void zoommove();
	void rotate();

	void layertop();
	void layerbottom();
	void layerup();
	void layerdown();

	void mirrorright();
	void mirrorbottom();
	void turnright();
	void turnleft();


	void alignleft();
	void aligntop();
	void alignright();
	void alignverline();
	void alignhorline();
	void alignbottom();
	void alignhorspace();
	void alignverspace();
	void alignsamewidth();
	void alignsameheight();
	void alignsamesize();

	void OntextBold();
	void OntextItalic();
	void OntextUnderLine();
	void OntextColor(QColor clr);


	void OnShowRule();
	void OnShowGrid();


	void OnLock();
	void OnUnlock();

	void tabClose(int index);
	void tabcurrentChanged(int index);
	void OnGroup();
	void OnUnGroup();
	void OnStrokeWidth(int nIndex);
	void OnStrokeDashType(int nIndex);
	void OnStrokeColor(QColor clr);
	void OnFillColor(QColor clr);
	void OnFillUrl(const std::wstring &str);
	void OncustomContextMenuRequested(const QPoint& pt);
	void OpenFolder(QAction * action);
	void OpenByDefault(QAction * action);
	void ReLoadFile(QAction * action);
	void OnCloseTab(QAction * action);
	void OnCloseAllTab(QAction * action);
	void OnCloseRightTab(QAction * action);
	//
	void OnAbout();
	void OnMenuViewToShow();
public:
	void ResetViewFileMark(CSVGView *pSVGView);
private:
	void setupFileActions();
	void setupEditActions();
	void setupViewActions();
	void setupDrawActions();
	void setupZoomActions();
	void setupGroupActions();
	void setupFontActions();
	void setupAlignActions();
	void setupColorActions();
	void setupCanvasActions();
	void setupLockActions();
	void setupStatusBar();
	void setupHelp();
	void OntextFamilyChange(const QString &f);
	void OntextFontSizeChange(const QString &f);
	void createDockWindows();
public:
	void    ResetToolBarEnable();
	QAction *m_pactionSave;
	QAction *m_pActionundo;
	QAction *m_pActionredo;
	QAction *m_pActioncopy;
	QAction *m_pActionpaste;
	QAction *m_pActionDrawLine;
	QAction *m_pActionDrawRect;
	QAction *m_pActionDrawCircle;
	QAction *m_pActionDrawEllipse;
	QAction *m_pActionDrawPolyline;
	QAction *m_pActionDrawPolygon;
	QAction *m_pActionDrawpath;
	QAction *m_pActionDrawtext;
	QAction *m_pActionDrawImage;
	QAction *m_pActionDrawContinue;
	QAction *m_pActionArrow;
	QAction *m_pActionRestore;
	QAction *m_pActionZoomin;
	QAction *m_pActionZoomout;
	QAction *m_pActionZoommove;
	QAction *m_pActionRotate;
	QAction *m_pActionGroup;
	QAction *m_pActionUnGroup;
	QAction *m_pActionMoveTop;
	QAction *m_pActionMoveBottom;
	QAction *m_pActionMoveup;
	QAction *m_pActionMoveDown;
	QAction *m_pActionTurnRight;
	QAction *m_pActionTurnLeft;
	QAction *m_pActionMoveHor;
	QAction *m_pActionMoveVer;
	QAction *m_pActionAlignTop;
	QAction *m_pActionAlignBottom;
	QAction *m_pActionAlignLeft;
	QAction *m_pActionAlignRight;
	QAction *m_pActionAlignVerMid;
	QAction *m_pActionAlignHorMid;
	QAction *m_pActionAlignHorSame;
	QAction *m_pActionAlignVerSame;
	QAction *m_pActionAlignWidthSame;
	QAction *m_pActionAlignHeightSame;
	QAction *m_pActionAlignSizeSame;

	QAction  *m_pActionTextBold;
	QAction  *m_pActionTextItalic;
	QAction  *m_pActionTextUnderline;
	QAction  *m_pActionRule;
	QAction  *m_pActionGrid;

	QAction  *m_pActionLock;
	QAction  *m_pActionUnLock;

	QAction  *m_pActionExportPng;
	QAction  *m_pActionExportJpg;

	CToolButtonColorMenu * m_ptextColorbutton;
	CToolButtonComboxMenu *m_textlinewidthbutton;
	CToolButtonComboxMenu *m_textlinetypebutton;
	CToolButtonColorMenu  *m_lineColorbutton;
	CToolButtonColorMenu  *m_fillColorbutton;
public:
	CPanePropertyTableView *m_RightProperty;
private:
	QDockWidget *m_pPropertyDock;
	QAction *m_ActionPropertyView;

	CTabWidgetEx           *m_pTabWidget;
	CSVGView               *m_pcurrentView;
	QTabWidget             *m_RightGraphicTab;
	QAction                *actionTextColor;
	QFontComboBox          *m_comboFont;
	QComboBox              *m_comboSize;
	QLabel                 *m_statusLabelmousept;

	bool                    m_bLockTabSelChange;
	bool                    m_bShowCanvasRule;
	bool                    m_bShowCanvasGrid;
};
