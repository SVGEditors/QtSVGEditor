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
#include "SVGElementRadialGradient.h"
#include "SVGDocument.h"

CSVGElementRadialGradient::CSVGElementRadialGradient(CSVGDocument* ownerDoc)
	:CSVGElementGradient(ownerDoc, L"radialGradient", SVG_RADIALGRADIENT)
{
	m_bShow = false;
}

CSVGElementRadialGradient::~CSVGElementRadialGradient()
{

}

void CSVGElementRadialGradient::setAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
{
	if (addorsetAttribute(attrname, value, bSetModfiyFlag))
	{
		CSVGElement::setAttribute(attrname, value, bSetModfiyFlag);
	}
	else
	{
		CSVGElementGradient::setAttribute(attrname, value, bSetModfiyFlag);
	}
}

bool CSVGElementRadialGradient::addorsetAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
{
	if (attrname == L"cx")
	{
		m_cx = value;
	}
	else if (attrname == L"cy")
	{
		m_cy = value;
	}
	else if (attrname == L"r")
	{
		m_r = value;
	}
	else if (attrname == L"fx")
	{
		m_fx = value;
	}
	else if (attrname == L"fy")
	{
		m_fy = value;
	}
	else
	{
		return false;
	}
	if (bSetModfiyFlag)
	{
		pathHasChange();
	}
	return true;
}

void CSVGElementRadialGradient::reBuildBrush()
{
	CSVGElementGradient::BuildFromStopElement();
	if (m_cx.length() == 0)
		m_cx = L"0.5";
	if (m_cy.length() == 0)
		m_cy = L"0.5";
	if (m_r.length() == 0)
		m_r = L"0.5";
	if (m_fx.length() == 0)
		m_fx = m_cx;
	if (m_fy.length() == 0)
		m_fy = m_cy;
}

QBrush CSVGElementRadialGradient::getBrush(const CSVGRectF &bbox, float fopacity)
{
	Q_UNUSED(fopacity);
	if (IsAttrChange())
	{
		reBuildBrush();
		CSVGElement::setAttrChange(false);
	}

	QRadialGradient radial;
	if (nlen > 0)
	{
		for (int i = 0; i < nlen; ++i)
		{
			float foffset = offset[i];
			int avalue = SVGGetAValue(color[i]);
			radial.setColorAt(foffset, QColor(SVGGetRValue(color[i]), SVGGetGValue(color[i]), SVGGetBValue(color[i]), avalue));
		}
	}

	float fcx = 0;
	float fcy = 0;
	float fr = 0;
	float ffx = 0;
	float ffy = 0;
	CSVGRectF  gradientBox;
	if (gradientUnits == L"userSpaceOnUse")
	{
		fcx = global.atof_3(m_cx.c_str());
		fcy = global.atof_3(m_cy.c_str());
		fr = global.atof_3(m_r.c_str());
		ffx = global.atof_3(m_fx.c_str());
		ffy = global.atof_3(m_fy.c_str());

		CSVGMatrix gradientmat = GetGradientMatrix();
		CSVGPointF ptCenter(fcx, fcy);
		ptCenter = ptCenter.matrixTransform(gradientmat);
		fcx = ptCenter.x;
		fcy = ptCenter.y;

		CSVGPointF ptF(ffx, ffy);
		ptF = ptF.matrixTransform(gradientmat);
		ffx = ptF.x;
		ffy = ptF.y;

		float fleft = fcx - fr;
		float ftop = fcy - fr;
		float fright = fcx + fr;
		float fbottom = fcy + fr;
		if (fleft > bbox.x)
		{
			fleft = bbox.x;
		}
		if (ftop > bbox.y)
		{
			ftop = bbox.y;
		}
		if (fright < bbox.x + bbox.width)
		{
			fright = bbox.x + bbox.width;
		}
		if (fbottom < bbox.y + bbox.height)
		{
			fbottom = bbox.y + bbox.height;
		}
		fcx = fcx - fleft;
		fcy = fcy - ftop;
		ffx = ffx - fleft;
		ffy = ffy - ftop;

		radial.setCenter(QPointF(fcx, fcy));
		radial.setCenterRadius(fr);
		radial.setFocalPoint(QPointF(ffx, ffy));

		int nNewWidth = fright - fleft;
		int nNewHeight = fbottom - ftop;
		QImage bigimage(nNewWidth, nNewHeight, QImage::Format_ARGB32);
		QPainter painter(&bigimage);
		painter.fillRect(QRect(0, 0, nNewWidth, nNewHeight), QBrush(radial));

		QImage  retimage = bigimage.copy(bbox.x - fleft, bbox.y - ftop, bbox.width, bbox.height);
		QBrush brush(Qt::TexturePattern);
		brush.setTextureImage(retimage);
		QTransform mat;
		mat.translate(bbox.x, bbox.y);
		brush.setTransform(mat);
		return brush;
	}
	else
	{
		CSVGMatrix gradientmat = GetGradientMatrix();
		CSVGPointF ptCenter(fcx, fcy);
		ptCenter = ptCenter.matrixTransform(gradientmat);
		fcx = ptCenter.x;
		fcy = ptCenter.y;

		CSVGPointF ptF(ffx, ffy);
		ptF = ptF.matrixTransform(gradientmat);
		ffx = ptF.x;
		ffy = ptF.y;

		float fmaxlen = SVGmax(bbox.width, bbox.height);
		fcx = global.atof_3(m_cx.c_str()) * fmaxlen;
		fcy = global.atof_3(m_cy.c_str()) * fmaxlen;
		fr = global.atof_3(m_r.c_str()) * fmaxlen;
		ffx = global.atof_3(m_fx.c_str()) * fmaxlen;
		ffy = global.atof_3(m_fy.c_str()) * fmaxlen;

		radial.setCenter(QPointF(fcx, fcy));
		radial.setCenterRadius(fr);
		radial.setFocalPoint(QPointF(ffx, ffy));

		QImage bigimage(fmaxlen, fmaxlen, QImage::Format_ARGB32);
		QPainter painter(&bigimage);
		painter.fillRect(QRect(0, 0, fmaxlen, fmaxlen), QBrush(radial));
		QImage retimage = bigimage.scaled(bbox.width, bbox.height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
		QBrush brush(Qt::TexturePattern);
		brush.setTextureImage(retimage);
		QTransform mat;
		mat.translate(bbox.x, bbox.y);
		brush.setTransform(mat);
		return brush;
	}
	return QBrush();
}

SVGCOLORREF CSVGElementRadialGradient::getOutColor()
{
	if (nlen > 0)
	{
		return color[nlen - 1];
	}
	return 0;
}
