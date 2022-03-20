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
#pragma once
#include "SVGElementShape.h"
#include "SVGPathSeg.h"
struct BezierData
{
	CSVGPointF pt[4];
};
class  CSVGElementPath : public CSVGElementShape
{
public:
	CSVGElementPath(CSVGDocument* ownerDoc);
	~CSVGElementPath();
	virtual bool  IsPtInShape(float fx, int fy, std::wstring &retgroupid, std::wstring &retshapeid);
public:
	virtual void  setAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag = true);
	virtual void  calBBox();
	virtual void  resetPath();
	virtual void  resetpatternPath(int nNewWidth, int nNewHeight);
	void          unClosePath();
	void          ClosePath();
	int           GetPathSegSize();
	std::list<CSVGPathSeg*> &GetPathSeg();
	std::wstring  pathToString();
	virtual void         SetTwoPrecision();
public:
	CSVGPathSegMoveto*       createSVGPathSegMoveto(float x, float y);
	CSVGPathSegLineto*       createSVGPathSegLineto(float x, float y);
	CSVGPathSegCurvetoCubic* createSVGPathSegCurvetoCubic(float x1, float y1, float x2, float y2, float x, float y);
	CSVGPathSegArc         * createSVGPathSegArc(float r1, float r2, float angle, float x, float y);
	void                     createSVGPathSegClose();
	void                     changeCurvetoCubic(CSVGPathSegCurvetoCubic *pCubic, float x1, float y1, float x2, float y2);
	void                     removeSeg(CSVGPathSeg *pPathseg);
	CSVGPathSeg             *getLastSeg(int nIndex);
	CSVGPathSeg             *getBeginSeg(int nIndex);
	std::vector<BezierData>  arctobezier(float startx, float starty, float rX, float rY, float xaxisrotation,
		unsigned short largearcflag, unsigned short sweepflag, float endx, float endy);
private:
	bool  addorsetAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag);
	double calculatevectorangle(double ux, double uy, double vx, double vy);
	CSVGPointF GetPt(CSVGPointF pt0, CSVGPointF pt1, CSVGPointF pt2, CSVGPointF pt3, float t);
	void clear();
	void parseD(const wchar_t* value);
	CSVGRectF GetBezierBox(CSVGPointF &pt_0, CSVGPointF &pt_1, CSVGPointF &pt_2, CSVGPointF& pt_3);
	void getBezierpoints(CSVGPointF &pt_0, CSVGPointF &pt_1, CSVGPointF &pt_2, CSVGPointF& pt_3, std::vector<CSVGPointF>&vcpoint);
	std::list<CSVGPathSeg*> vcpathseg;
	std::vector<CSVGPointF> getPathpoints();
	std::vector<CSVGPointF> m_pathpts;
};
