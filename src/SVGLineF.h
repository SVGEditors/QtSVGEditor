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
#include "SVGMatrix.h"
#include "SVGPointF.h"
class CSVGLine
{
public:
	CSVGLine();
	CSVGLine(const CSVGPoint &pt1, const  CSVGPoint& pt2);
	~CSVGLine();
	const CSVGLine& operator=(const CSVGLine& o);
	CSVGLine(int nx1, int ny1, int nx2, int ny2);
	bool  operator==(const CSVGLine& res) const;
	void SetEmpty();
	bool IsEmpty();
public:
	int x1;
	int y1;
	int x2;
	int y2;
};

//////////////////////////////////////////////////////////////////////////
class CSVGLineF
{
public:
	CSVGLineF();
	CSVGLineF(const CSVGPointF &pt1, const  CSVGPointF& pt2);
	~CSVGLineF();
	const CSVGLineF& operator=(const CSVGLineF& o);
	CSVGLineF(float fx1, float fy1, float fx2, float fy2);
	bool  operator==(const CSVGLineF& res) const;
	void SetEmpty();
	bool IsEmpty();
public:
	CSVGLineF matrixTransform(const CSVGMatrix &_mat);
public:
	float x1;
	float y1;
	float x2;
	float y2;
};
