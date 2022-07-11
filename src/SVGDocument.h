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

#include <QPainter>
#include <set>
#include "SVGElement.h"
#include "SVGGlobal.h"
#include "SVGPointF.h"
#include "ElementMaker.h"
#include "FocusManager.h"
#include "ActionHistory.h"

enum MOUSESHAPE
{
	MOUSE_ARROW,
	MOUSE_DRAWPEN,
	MOUSE_ZOOMIN,
	MOUSE_ZOOMOUT,
	MOUSE_SIZETO,
	MOUSE_OPENHAND,
	MOUSE_SELECT,
	MOUSE_CLOSEHAND,
	MOUSE_DRAWCOPY,
	MOUSE_WE,       //  "<->"
	MOUSE_NS,       // "^|V"
	MOUSE_NE,       // "//"
	MOUSE_NW        // "\\"
};

enum DRAWTOOL
{
	DRAWTOOL_SELECT,
	DRAWTOOL_LINE,
	DRAWTOOL_RECT,
	DRAWTOOL_CIRCLE,
	DRAWTOOL_ELLIPSE,
	DRAWTOOL_POLYLINE,
	DRAWTOOL_POLYLINEING,//continue draw
	DRAWTOOL_POLYGON,
	DRAWTOOL_POLYGONING,
	DRAWTOOL_PATH,
	DRAWTOOL_PATHING,
	DRAWTOOL_TEXT,
	DRAWTOOL_TEXTAAA,
	DRAWTOOL_IMAGE,
	DRAWTOOL_ZOOMIN,
	DRAWTOOL_ZOOMOUT,
	DRAWTOOL_MOVE,
	DRAWTOOL_CUSTOM
};

class CSVGElementStyleClass;
class CSVGElementStyle;
class CSVGElementShape;
class CTYLayoutElement;

class CSVGDocument : public CSVGElement
{
	friend class CSVGElementStyle;
public:
	CSVGDocument();
	virtual ~CSVGDocument();
	//////////////////////////////////////////////////////////////////////////
	bool         LoadFromFile(const std::wstring &filePath);
	bool         LoadFromBuff(const std::wstring &strtext);

	void         NewDocument();
	void         SetFilePath(const std::wstring &filePath);
	void         MakeGroup();

	std::wstring ToFileXml();
	std::wstring Toxml();
	bool         IsModify();
	void         SetModify(bool bModify = true);
	QString      GetFilePath();
	//strtype:"jpg"、"png"
	static QImage SVGFileAsImage(const std::wstring &strxml, const std::wstring &strtype, int nScaleWidth, int nScaleHeight);
	static QImage SVGFileAsImage(const std::wstring &strxml, const std::wstring &strtype);
	static QImage SVGFileAsImageBound(const std::wstring &strxml);
	//////////////////////////////////////////////////////////////////////////
	void         Resize(float fwidth, float fheight);
	void         Draw(QPainter *painter);
	void         DrawAll(QPainter *painter, const CSVGMatrix &mat);
	void         PrepareDraw();
	float        GetCanvasWidth();
	float        GetCanvasHeight();
	void         SetCanvasWidth(float fWidth);
	void         SetCanvasHeight(float fHeight);
	bool         IsShowGrid();
	bool         IsShowRule();
	void         ShowGrid(bool bShow);
	void         ShowRule(bool bShow);
	//////////////////////////////////////////////////////////////////////////
	void         ReDrawBackground();
	void         ReDrawForeground();
	void         SetDrawTool(DRAWTOOL drawtool);
	DRAWTOOL     GetDrawTool();
	bool         IsNeedRepaint();
	MOUSESHAPE   OnMouseLButtonDown(QPoint point, bool bCtrl = false, bool balt = false);
	MOUSESHAPE   OnMouseLButtonUp(QPoint point, bool bCtrl = false, bool balt = false);
	MOUSESHAPE   OnMouseRButtonDown(QPoint point);
	MOUSESHAPE   OnMouseRButtonUp(QPoint point);
	MOUSESHAPE   OnMouseMButtonDown(QPoint point);
	MOUSESHAPE   OnMouseMButtonUp(QPoint point);
	MOUSESHAPE   OnMouseLDBClick(QPoint point);
	MOUSESHAPE   OnMouseWheel(QPoint point, int delta);
	MOUSESHAPE   OnMouseMove(QPoint point, bool bCtrl = false, bool balt = false);
	void         OnZoomRestore();

	void         SetShowGrid(bool bShow);
	void         SetShowRule(bool bShow);
	void         SetShowCanas(bool bShow);
	void         SelectAll();
	std::wstring Copy();
	std::wstring GetShapeFull(CSVGElement *pElement);
	std::wstring CopyToXml(std::vector<CSVGElement*>&vcElement);
	void CopyElementTo(CSVGElement *pSrc, CSVGElement *pToElement);
	CActionComposite * Paste(const std::wstring &str);
	void         Paste(const std::wstring &str, QPoint centerpoint);
	void MoveSelect(int nx, int ny);
	bool         CanUndo();
	bool         CanRedo();
	void         Undo();
	void         Redo();
	bool         Delete();
	void         LayerTop();
	void         LayerBottom();
	void         LayerUP();
	void         LayerDown();
	void         TurnRight();
	void         TurnLeft();
	void         MirrorRight();
	void         MirrorBottom();
	void         Group();
	void         UnGroup();
	bool         CanUnGroup();

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
	void         InputText(const std::wstring &strText);
	bool         InputBackSpace();
	bool         KeyLeft();
	bool         KeyRight();
	bool         KeyUp();
	bool         KeyDown();
	void         InsertImage(const std::wstring &strimagedata);

	void         Rotate(int nValue, bool bFinish);
	void         ZoomSizeFixValue(int nWidth, int nHeight, bool bFinish);
	void         SetAttributes(const std::vector<std::wstring>&AttrNames, const std::vector<std::wstring>&AttrValues, bool bChangeDefult = false);
	void         SetShowFalsh(bool bShow);
	bool         GetShowFlash();
	bool         HasTextFlash();
	void         SetNeedRepaintAll();
	void         SetContentHasChange();

	std::wstring GetSelectWidth();
	std::wstring GetSelectHeight();
	//////////////////////////////////////////////////////////////////////////
	std::vector<CSVGElement*> GetDragingRectElement(const CSVGRectF &rect);
	CSVGElement *createElement(const std::wstring &tagName);
	CSVGElement *getElementById(const std::wstring &id);

	void         removeFromId(CSVGElement *pElement);
	CSVGElementShape *getShapeByPoint(int nx, int ny);
	CSVGElement      *GetRootSVGElement();
	CSVGElement      *GetSelectChild();
	//////////////////////////////////////////////////////////////////////////
	void         buildIdElement(CSVGElement *pElement, const std::wstring &id);
	void         setLocalFolder(const std::wstring &folder);
	std::wstring getLocalFolder();
	CSVGElementStyleClass *getClassByName(const std::wstring &name);
	void AddStyleElement(CSVGElementStyle *pStyleElement);
	bool RemoveStyleElement(CSVGElementStyle *pStyleElement);
	CSVGPointF    ScreenToLogicalPoint(int x, int y);
	CSVGPointF    LogicalToScreenPoint(float fx, float fy);
	//////////////////////////////////////////////////////////////////////////
	std::wstring GetLayerTop(CSVGElement *pElement);
	std::wstring GetLayerBottom(CSVGElement *pElement);
	std::wstring GetLayerUp(CSVGElement *pElement);
	std::wstring GetLayerDown(CSVGElement *pElement);
	//////////////////////////////////////////////////////////////////////////
	std::wstring GetMirrorRight(CSVGElement *pElement);
	std::wstring GetMirrorBottom(CSVGElement *pElement);
	std::wstring GetTurnRight(CSVGElement *pElement);
	std::wstring GetTurnLeft(CSVGElement *pElement);

	std::wstring  GetDefaultFontName();
	std::wstring  GetDefaultFontSize();

	std::wstring GetDefaultStroke();
	std::wstring GetDefaultDashLine();

	void ReSetAllElementIndex();
	void ReSetChildElementIndex(CSVGElement *pElement, int &nIndex);
	void FillUrl(const std::wstring &strxml);
	//////////////////////////////////////////////////////////////////////////
	std::set<std::wstring>GetIdsSet();
	std::wstring GenerateId(const std::wstring &tagName = L"");
	void ChangeAllId(std::set<std::wstring>&vcsetId);
	void ChangeElementId(std::map<std::wstring, std::wstring>&oldtonewmap, CSVGElement*pElement);
	void ChangeElementUrlId(std::map<std::wstring, std::wstring>&oldtonewmap, CSVGElement*pElement);
	std::vector<CSVGElement*>GetAllElements();
	bool IsUrlIdUsedByShape(std::wstring strid);
	//////////////////////////////////////////////////////////////////////////
public:
	//////////////////////////////////////////////////////////////////////////
	void AddToSelect(CSVGElement *pElement, QColor clr);
	void ClearAllSelect();
	void RemoveInnerSelect();
	void ResetAllFocus();
	void RemoveFromSelect(CSVGElement *pElement);
	QVector<CSVGElement*>GetSelectElements();
	CSVGElement *GetFirstSelectElement();
	CSVGElement *GetAloneSelectElement(bool bChildIfExist = false);
public:
	QPoint  GetLogicPoint(const QPoint &point);
	void    SetHasCanvasBack(bool bHas);
	void    SetCanvasBackColor(QColor clr);
	void    Lock();
	void    UnLock();
	bool    HasLockShape();
private:
	void DrawHorizonRuler(QPainter &painter);
	void DrawVerticalRuler(QPainter &painter);
	void DrawRuleCross(QPainter &painter);
	void DrawHorGridLine(QPainter &painter);
	void DrawVerGridLine(QPainter &painter);
	void DrawCanvasRect(QPainter &painter);
	void ElementSizeTo(const CSVGPointF &point, bool bCtrl);
	void BeginElementSizeTo();
	void BeginElementTransformAction(CActionComposite *pComposite = NULL);
	void EndElementAttrAction();
	void TrySetCSSFromElement(CActionComposite *pCompos, CSVGElement *pSrc, CSVGElement *pDes);
	void RepairNameSpace();
	void ResetUrlUsed();
public:
	void  AddElementToArea(CSVGElementShape *pElement);
	void  ReMoveElementFromArea(CSVGElementShape *pElement);
private:
	void ResettoAreaIfChange(CSVGElement *pElement);
	std::vector<AreaManager*> GetAreaManagerByRect(const CSVGRectF &rect);
	void AddElementToArea(AreaManager* pAreaManager, CSVGElementShape *pElement);
	CSVGLine   FormatFloatToAreaInt(const CSVGLineF &fline);
	long long  MakeWord(int x, int y);
	AreaManager * GetAreaManagerFromPoint(int x, int y);
	void GetChildElement(CSVGElement *pElement, std::vector<CSVGElement*> &vcRet);
private:
	void  ParserBuffer(wchar_t *pBuffer, CSVGElement *pParent);
	void  Clear();
	void  getShapeShape(CSVGElementShape *pParent, float fx, float fy, CSVGElementShape *&pretShape);
	void  ParseGetRefText(CSVGElement *pRootElement, CSVGElement *pShapeElement, std::wstring& strText);
	CSVGPointF GetFormatPoint(CSVGPointF pt1, CSVGPointF pt);
	std::list<CSVGPointF> FormatToRegularPolygon(const std::list<CSVGPointF> &listpt);
private:
	bool                                  m_bLoadingfile;
	bool                                  m_bCanBuildIdmap;
	std::vector<CSVGElementStyle*>        m_vcStyle;
	//////////////////////////////////////////////////////////////////////////
	std::map<long long, AreaManager*>     m_PointAreaManagerMap;
	std::list<AreaManager*>               m_AreaManagerList;
	//////////////////////////////////////////////////////////////////////////
	std::map<std::wstring, CSVGElement*>  m_mapIdElement;
	std::set<std::wstring>                m_setIds;
	std::wstring                          m_localfolder;
	std::wstring                          m_defaultfontname;
	std::wstring                          m_defaultfontsize;

	std::wstring                          m_defaultstroke;
	std::wstring                          m_defaultfill;
	std::wstring                          m_defaultfillopacity;
	std::wstring                          m_defaultfontcolor;
	std::wstring                          m_defaultstrokewidth;
	std::wstring                          m_defaultdashline;

	std::wstring                          m_workpath;
	bool                                  m_bModify;
	//////////////////////////////////////////////////////////////////////////
	int                                   m_nWidth;
	int                                   m_nHeight;
	int                                   m_nSVGWidth;
	int                                   m_nSVGHeight;
	int                                   m_nIdIndex;
public:
	QImage                                m_layerbottomImage;
	QImage                                m_layertopImage;
	bool                                  m_bNeedResetElementIndex;
	CSVGElement                         * m_svgRootElement;
	QVector<CSVGElement*>                 m_vcSelectShapes;
	CSVGElement                         * m_pInnerSelectShape;
private:
	CSVGMatrix                            m_screenMat;
	bool                                  m_bShowGrid;
	bool                                  m_bShowRule;
	bool                                  m_bShowCanvas;
	QColor                                m_CanvasBackColor;
	bool                                  m_bHasCanvasBack;
	DRAWTOOL                              m_DrawTool;
	bool                                  m_bModifyBackground;
	bool                                  m_bModifyForeground;
	bool                                  m_bModifyContent;
	QPoint                                m_mouseDownPt;
	QPoint                                m_lstChangeDownPt;
	QPoint                                m_lstMiddleDownPt;
	bool                                  m_bIsMouseDowning;
	bool                                  m_bIsMMouseDowning;
	CFocusManager                         m_FocusManager;
	FocusPointInfo                        m_lstFocusPointInfo;
	bool                                  m_bCanMove;
	CActionHistory                        m_ActionHistory;
	CActionComposite                    * m_pActionAttrs;
	CSVGRectF                             m_dragSelectFocus;
	bool                                  m_bDownRemoveSelect;
	bool                                  m_bDragCopy;
	int                                   m_nTextFocusPos;
	bool                                  m_bShowFlash;
	std::set<std::wstring>                m_usedurlids;
	bool                                  m_bHasLockShape;
};
