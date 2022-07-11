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
#include "SVGElementStyleable.h"
#include <QPainterPath>
#include <QPen>
#include <QBrush>
class CActionXMLNode;
class  CSVGElementShape : public CSVGElementStyleable
{
	friend class CSVGElementClipPath;
	friend class CSVGDocument;
public:
	CSVGElementShape(CSVGDocument* ownerDoc, const std::wstring &tagname, unsigned short nodetype);
	virtual ~CSVGElementShape();
	virtual bool          IsShapeElement() { return true; }
	std::wstring          getUseStyle(const std::wstring &attrname);
	CSVGRectF             getBBox();
	CSVGRectF             getMatrixBBox();
	void                  DoClipPath(QPainter *painter, const CSVGMatrix &clipmat);
	virtual void          Draw(QPainter *painter, const CSVGMatrix *screenmat);
	virtual void          resetpatternPath(int nNewWidth, int nNewHeight);
	virtual bool          IsPtInShape(float /*fx*/, int /*fy*/, std::wstring &/*retgroupid*/, std::wstring &/*retshapeid*/) { return false; }
	virtual bool          IsShapeInRect(const CSVGRectF &rectf);
	virtual int           HitTest(int nx, int ny, const CSVGMatrix *screenmat);
	void                  resetBBox();
	virtual  std::set<std::wstring> getAttrNames();
	virtual void          resetPath();
	QPainterPath         &GetPainterPath();
protected:
	virtual void          calBBox();
protected:
	CSVGRectF                 m_bbox;
	std::vector<AreaManager*> m_vcArea;
	QPainterPath              m_Path;
};
