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
#include "SVGMatrix.h"
#include <math.h>
#include "SVGGlobal.h"

//////////////////////////////////////////////////////////////////////////

CSVGMatrix staticmat;
CSVGMatrix::CSVGMatrix()
	: m_a(1), m_b(0), m_c(0), m_d(1), m_e(0), m_f(0)
{

}
CSVGMatrix::CSVGMatrix(float fa, float fb, float fc, float fd, float fe, float ff)
	: m_a(fa), m_b(fb), m_c(fc), m_d(fd), m_e(fe), m_f(ff)
{

}
CSVGMatrix::CSVGMatrix(const CSVGMatrix &o)
	: m_a(o.m_a), m_b(o.m_b), m_c(o.m_c), m_d(o.m_d), m_e(o.m_e), m_f(o.m_f)
{
}
CSVGMatrix::~CSVGMatrix()
{

}
void CSVGMatrix::operator*=(const CSVGMatrix& secondMatrix)
{
	float a = m_a * secondMatrix.m_a + m_c * secondMatrix.m_b;
	float b = m_b * secondMatrix.m_a + m_d * secondMatrix.m_b;
	float c = m_a * secondMatrix.m_c + m_c * secondMatrix.m_d;
	float d = m_b * secondMatrix.m_c + m_d * secondMatrix.m_d;
	float e = m_a * secondMatrix.m_e + m_c * secondMatrix.m_f + m_e;
	float f = m_b * secondMatrix.m_e + m_d * secondMatrix.m_f + m_f;

	m_a = a; m_b = b; m_c = c; m_d = d; m_e = e; m_f = f;
}

bool CSVGMatrix::operator == (const CSVGMatrix & res) const
{
	if (
		(DBEQUAL(m_a, res.m_a))
		&& (DBEQUAL(m_b, res.m_b))
		&& (DBEQUAL(m_c, res.m_c))
		&& (DBEQUAL(m_d, res.m_d))
		&& (DBEQUAL(m_e, res.m_e))
		&& (DBEQUAL(m_f, res.m_f))
		)
	{
		return true;
	}
	return false;
}
void CSVGMatrix::SetEmpty()
{
	m_a = 1;
	m_b = 0;
	m_c = 0;
	m_d = 1;
	m_e = 0;
	m_f = 0;
	m_strtext = L"";
}
bool CSVGMatrix::IsEmpty()
{
	if (
		(DBEQUAL(m_a, 1))
		&& (DBEQUAL(m_b, 0))
		&& (DBEQUAL(m_c, 0))
		&& (DBEQUAL(m_d, 1))
		&& (DBEQUAL(m_e, 0))
		&& (DBEQUAL(m_f, 0))
		)
	{
		return true;
	}
	return false;
}
void CSVGMatrix::Add(const CSVGMatrix& secondMatrix)
{
	float fa = m_a * secondMatrix.m_a + m_c * secondMatrix.m_b;
	float fb = m_b * secondMatrix.m_a + m_d * secondMatrix.m_b;
	float fc = m_a * secondMatrix.m_c + m_c * secondMatrix.m_d;
	float fd = m_b * secondMatrix.m_c + m_d * secondMatrix.m_d;
	float fe = m_a * secondMatrix.m_e + m_c * secondMatrix.m_f + m_e;
	float ff = m_b * secondMatrix.m_e + m_d * secondMatrix.m_f + m_f;
	m_a = fa;
	m_b = fb;
	m_c = fc;
	m_d = fd;
	m_e = fe;
	m_f = ff;
	m_strtext = L"";
}

void CSVGMatrix::LAdd(const CSVGMatrix& secondMatrix)
{
	float fa = secondMatrix.m_a*m_a + secondMatrix.m_c*m_b;
	float fb = secondMatrix.m_b*m_a + secondMatrix.m_d*m_b;
	float fc = secondMatrix.m_a*m_c + secondMatrix.m_c*m_d;
	float fd = secondMatrix.m_b*m_c + secondMatrix.m_d*m_d;
	float fe = secondMatrix.m_a*m_e + secondMatrix.m_c*m_f + secondMatrix.m_e;
	float ff = secondMatrix.m_b*m_e + secondMatrix.m_d*m_f + secondMatrix.m_f;
	m_a = fa;
	m_b = fb;
	m_c = fc;
	m_d = fd;
	m_e = fe;
	m_f = ff;
	m_strtext = L"";
}

void CSVGMatrix::Inverse()
{
	float temp = (float)1.0 / (m_a*m_d - m_b * m_c);
	float fa = ((float)(m_d*temp));
	float fb = ((float)(-m_b * temp));
	float fc = ((float)(-m_c * temp));
	float fd = ((float)(m_a*temp));
	float fe = ((float)(-m_e * fa - m_f * fc));
	float ff = ((float)(-m_e * fb - m_f * fd));

	m_a = fa;
	m_b = fb;
	m_c = fc;
	m_d = fd;
	m_e = fe;
	m_f = ff;
	m_strtext = L"";
}

void CSVGMatrix::Translate(float x, float y)
{
	CSVGMatrix matrix(1, 0, 0, 1, x, y);
	Add(matrix);
}

void CSVGMatrix::LTranslate(float x, float y)
{
	CSVGMatrix matrix(1, 0, 0, 1, x, y);
	matrix.Add(*this);
	*this = matrix;
}

void CSVGMatrix::Scale(float scaleFactor)
{
	Add(CSVGMatrix(scaleFactor, 0, 0, scaleFactor, 0, 0));
}

void CSVGMatrix::LScale(float sx, float sy)
{
	CSVGMatrix res(sx, 0, 0, sy, 0, 0);
	res.Add(*this);
	*this = res;
}

void CSVGMatrix::Scale(float scaleFactorX, float scaleFactorY)
{
	Add(CSVGMatrix(scaleFactorX, 0, 0, scaleFactorY, 0, 0));
}

void CSVGMatrix::Rotate(float angle)
{
	angle = (float)(angle*SVGPI / 180);
	Add(CSVGMatrix(cos(angle), sin(angle), -sin(angle), cos(angle), 0, 0));
}

void CSVGMatrix::LRotate(float angle)
{
	angle = (float)(angle*SVGPI / 180);
	CSVGMatrix res(cos(angle), sin(angle), -sin(angle), cos(angle), 0, 0);
	res.Add(*this);
	*this = res;
}

void CSVGMatrix::RotateFromVector(float x, float y)
{
	Q_UNUSED(x);
	Q_UNUSED(y);
	Add(CSVGMatrix(1, 0, 0, 1, 0, 0));
}

void CSVGMatrix::FlipX()
{
	Add(CSVGMatrix(-1, 0, 0, 1, 0, 0));
}

void CSVGMatrix::FlipY()
{
	Add(CSVGMatrix(1, 0, 0, -1, 0, 0));
}

void CSVGMatrix::SkewX(float angle)
{
	Add(CSVGMatrix(1, 0, tan((float)(angle*SVGPI / 180)), 1, 0, 0));
}

void CSVGMatrix::SkewY(float angle)
{
	Add(CSVGMatrix(1, tan((float)(angle*SVGPI / 180)), 0, 1, 0, 0));
}
const CSVGMatrix& CSVGMatrix::operator=(const CSVGMatrix& o)
{
	if (this == &o)
		return *this;
	m_a = o.m_a;
	m_b = o.m_b;
	m_c = o.m_c;
	m_d = o.m_d;
	m_e = o.m_e;
	m_f = o.m_f;
	m_strtext = L"";
	return *this;
}

void CSVGMatrix::setText(const std::wstring & Text)
{
	SetEmpty();
	m_strtext = Text;
	int nLen = (int)m_strtext.length();
	wchar_t *pBeforeBuff = new wchar_t[nLen + 1];
	std::wmemset(pBeforeBuff, 0, nLen + 1);
	wchar_t *pBuff = pBeforeBuff;
	std::wmemcpy(pBuff, Text.c_str(), nLen);
	pBuff[nLen] = 0;
	while (*pBuff && global.gspace[*pBuff] == 1)
		pBuff++;
	wchar_t *pTempBuff = pBuff;
	while (*pBuff)
	{
		if (*pBuff == L')')
		{
			wchar_t c = *pBuff;
			*pBuff = 0;
			parseMatrix(pTempBuff);
			*pBuff = c;
			pBuff++;
			while (*pBuff && global.gspace[*pBuff] == 1)
			{
				pBuff++;
			}
			pTempBuff = pBuff;
		}
		else
		{
			pBuff++;
		}
	}
	delete[]pBeforeBuff;
}

void CSVGMatrix::parseMatrix(wchar_t *pText)
{
	if (pText == 0)
		return;
	wchar_t *pnext = NULL;
	wchar_t* token = std::wcstok(pText, L"(,) ", &pnext);
	wchar_t name[128];
	std::wmemset(name, 0, 128);
	int nCount = 0;
	float fdata[6];
	memset(fdata, 0, 6 * sizeof(float));
	while (token)
	{
		if (nCount == 0)
		{
			int nLen = (int)std::wcslen(token);
			if (nLen < 128)
			{
				std::wcsncpy(name, token, nLen);
				name[nLen] = 0;
			}
		}
		else if (nCount <= 6)
		{
			fdata[nCount - 1] = global.atof_3(token);
		}
		else
		{
			break;
		}
		nCount++;
		token = std::wcstok(NULL, L"(,) ", &pnext);
	}
	if (std::wcscmp(name, L"matrix") == 0)
	{
		CSVGMatrix second(fdata[0], fdata[1], fdata[2], fdata[3], fdata[4], fdata[5]);
		Add(second);
	}
	else if (std::wcscmp(name, L"translate") == 0)
	{
		Translate(fdata[0], fdata[1]);
	}
	else if (std::wcscmp(name, L"scale") == 0)
	{
		float fydata = fdata[1];
		if (-0.0001 < fydata && fydata < 0.0001)
		{
			fydata = fdata[0];
		}
		Scale(fdata[0], fydata);
	}
	else if (std::wcscmp(name, L"rotate") == 0)
	{
		Rotate(fdata[0]);
	}
	else if (std::wcscmp(name, L"skewX") == 0)
	{
		SkewX(fdata[0]);
	}
	else if (std::wcscmp(name, L"skewY") == 0)
	{
		SkewY(fdata[0]);
	}
}

std::wstring CSVGMatrix::getText() const
{
	std::wstring strtext;
	strtext += L"matrix(";
	strtext += global.ftostring(m_a);
	strtext += L",";
	strtext += global.ftostring(m_b);
	strtext += L",";
	strtext += global.ftostring(m_c);
	strtext += L",";
	strtext += global.ftostring(m_d);
	strtext += L",";
	strtext += global.ftostring(m_e);
	strtext += L",";
	strtext += global.ftostring(m_f);
	strtext += L")";
	return strtext.c_str();
}

CSVGMatrix CSVGMatrix::getNullMatrix()
{
	staticmat.SetEmpty();
	return staticmat;
}
