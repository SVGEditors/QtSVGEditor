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
#include "SVGPointF.h"
#include "SVGLineF.h"
#include "SVGMatrix.h"

class  CSVGRect
{
public:
	CSVGRect();
	CSVGRect(int  nx, int  ny, int  nwidth, int  nheight);
	CSVGLine ToLine();
public:
	int x;
	int y;
	int width;
	int height;
};


//////////////////////////////////////////////////////////////////////////
class  CSVGRectF
{
public:
	CSVGRectF();
	~CSVGRectF();
	const CSVGRectF& operator=(const CSVGRectF& o);
	CSVGRectF(float fx, float fy, float fwidth, float fheight);
	bool  operator==(const CSVGRectF& res) const;
public:
	void Empty();
	bool IsEmpty() const;
	bool PtInRect(const CSVGPointF &pt)const;
	bool Intersectrect(const CSVGRectF &rect)const;
	bool IsIncludeRect(const CSVGRectF &rect) const;
	CSVGRectF matrixTransform(const CSVGMatrix &_mat);
	void AddRect(const CSVGRectF& rect);
	CSVGRectF Inflate(float _x, float _y);
	bool RectInThis(const CSVGRectF &rect);
	CSVGLineF ToLineF() const;
public:
	float x;
	float y;
	float width;
	float height;
};
