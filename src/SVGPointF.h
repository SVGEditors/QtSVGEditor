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
#include "SVGMatrix.h"

class CSVGPoint
{
public:
	CSVGPoint();
	CSVGPoint(int nx, int ny);
public:
	int x;
	int y;
};

//////////////////////////////////////////////////////////////////////////
class CSVGPointF
{
public:
	friend class CSVGRectF;
public:
	CSVGPointF();
	CSVGPointF(float _x, float _y);
	CSVGPointF(const CSVGPointF& o);
	~CSVGPointF();
	const CSVGPointF& operator=(const CSVGPointF& o);
public:
	CSVGPointF matrixTransform(const CSVGMatrix &_mat);
	float length()const;
	CSVGPointF normalize();
	CSVGPointF add(const CSVGPointF &v);
	CSVGPointF multiply(float f);
	float dot(const CSVGPointF &v);
	float angle(const CSVGPointF &v);
public:
	bool operator!(void);
	bool operator==(const CSVGPointF& o);
public:
	float x;
	float y;
};
