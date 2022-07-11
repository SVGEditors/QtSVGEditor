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
#include "SVGDef.h"
#include <string>
class  CSVGMatrix
{
public:
	CSVGMatrix();
	CSVGMatrix(float fa, float fb, float fc, float fd, float fe, float ff);
	CSVGMatrix(const CSVGMatrix& o);
	virtual ~CSVGMatrix();
	const CSVGMatrix& operator=(const CSVGMatrix& o);
public:
	float m_a;
	float m_b;
	float m_c;
	float m_d;
	float m_e;
	float m_f;
public:
	void operator*=(const CSVGMatrix& secondMatrix);
	bool operator == (const CSVGMatrix & res) const;
	void SetEmpty();
	bool IsEmpty();
	void Add(const CSVGMatrix& secondMatrix);
	void LAdd(const CSVGMatrix& secondMatrix);
	void Inverse();
	void Translate(float x, float y);
	void LTranslate(float x, float y);
	void Scale(float scaleFactor);
	void Scale(float scaleFactorX, float scaleFactorY);
	void LScale(float sx, float sy);
	void Rotate(float angle);
	void LRotate(float angle);
	void RotateFromVector(float x, float y);
	void FlipX();
	void FlipY();
	void SkewX(float angle);
	void SkewY(float angle);
	static CSVGMatrix getNullMatrix();
public:
	void setText(const std::wstring &pText);
	std::wstring  getText() const;
private:
	void parseMatrix(wchar_t *pText);
private:
	std::wstring m_strtext;
};
