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
#include "SVGElementLinearGradient.h"
#include "SVGElementStop.h"
#include "SVGDocument.h"
#include <QLinearGradient>

CSVGElementLinearGradient::CSVGElementLinearGradient(CSVGDocument* ownerDoc)
	:CSVGElementGradient(ownerDoc, L"linearGradient", SVG_LINEARGRADIENT)
{
	m_x1 = 0;
	m_y1 = 0;
	m_x2 = 0;
	m_y2 = 0;
	m_bShow = false;
}

CSVGElementLinearGradient::~CSVGElementLinearGradient()
{

}

void CSVGElementLinearGradient::setAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
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

bool CSVGElementLinearGradient::addorsetAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
{
	unsigned short nIndex = global.getIndexByAttrName(attrname);
	switch (nIndex)
	{
	case ATTR_X1:
		m_x1 = global.atof_3(value);
		break;
	case ATTR_Y1:
		m_y1 = global.atof_3(value);
		break;
	case ATTR_X2:
		m_x2 = global.atof_3(value);
		break;
	case ATTR_Y2:
		m_y2 = global.atof_3(value);
		break;
	default:
		return false;
	}
	if (bSetModfiyFlag)
	{
		pathHasChange();
	}
	return true;
}

void CSVGElementLinearGradient::reBuildBrush()
{
	CSVGElementGradient::BuildFromStopElement();
}

QBrush CSVGElementLinearGradient::getBrush(const CSVGRectF &bbox, float fopacity)
{
	if (IsAttrChange())
	{
		reBuildBrush();
		CSVGElement::setAttrChange(false);
	}
	QLinearGradient gradient;
	if (nlen > 0)
	{
		for (int i = 0; i < nlen; ++i)
		{
			float foffset = offset[i];
			gradient.setColorAt(foffset, QColor(SVGGetRValue(color[i]), SVGGetGValue(color[i]), SVGGetBValue(color[i]), SVGGetAValue(color[i])*fopacity));
		}
	}

	float fx1 = m_x1;
	float fy1 = m_y1;
	float fx2 = m_x2;
	float fy2 = m_y2;
	if (gradientUnits == L"userSpaceOnUse")
	{
		CSVGMatrix gradientmat = GetGradientMatrix();
		CSVGPointF pt1(fx1, fy1);
		pt1 = pt1.matrixTransform(gradientmat);
		fx1 = pt1.x;
		fy1 = pt1.y;

		CSVGPointF pt2(fx2, fy2);
		pt2 = pt2.matrixTransform(gradientmat);
		fx2 = pt2.x;
		fy2 = pt2.y;
		gradient.setStart(QPointF(fx1, fy1));
		gradient.setFinalStop(QPointF(fx2, fy2));
	}
	else
	{
		CSVGPointF pt1(bbox.x, bbox.y);
		CSVGPointF pt2(bbox.x + bbox.width, bbox.y);
		CSVGPointF pt3(bbox.x, bbox.y + bbox.height);
		CSVGPointF pt4(bbox.x + bbox.width, bbox.y + bbox.height);

		float x1 = m_x1;
		float x2 = m_x2;
		float y1 = m_y1;
		float y2 = m_y2;
		CSVGPointF ptstart(pt1.x, pt1.y);
		CSVGPointF ptend;

		if ((DBEQUAL(x1, 0)) && (DBEQUAL(x2, 0)) && (DBEQUAL(y1, 0)) && (DBEQUAL(y2, 0)))
		{//x1=0;x2=0;y1=0;y2=0;	(0,0)(0,0)
			ptstart = pt1;
			ptend = pt2;
		}
		else if ((DBEQUAL(x1, 0)) && (DBEQUAL(x2, 0)) && (DBEQUAL(y1, 0)) && (DBEQUAL(y2, 1)))
		{//x1=0;x2=0;y1=0;y2=1; (0,0) (0,1)
			ptstart = pt1;
			ptend = pt3;
		}
		else if ((DBEQUAL(x1, 0)) && (DBEQUAL(x2, 0)) && (DBEQUAL(y1, 1)) && (DBEQUAL(y2, 0)))
		{//x1=0;x2=0;y1=1;y2=0; (0,1)(0,0)
			ptstart = pt3;
			ptend = pt1;
		}
		else if ((DBEQUAL(x1, 0)) && (DBEQUAL(x2, 0)) && (DBEQUAL(y1, 1)) && (DBEQUAL(y2, 1)))
		{//x1=0;x2=0;y1=1;y2=1; (0,1)(0,1)
			ptstart = pt1;
			ptend = pt2;
		}
		else if ((DBEQUAL(x1, 0)) && (DBEQUAL(x2, 1)) && (DBEQUAL(y1, 0)) && (DBEQUAL(y2, 0)))
		{//x1=0;x2=1;y1=0;y2=0; (0,0)(1,0)
			ptstart = pt1;
			ptend = pt2;
		}
		else if ((DBEQUAL(x1, 0)) && (DBEQUAL(x2, 1)) && (DBEQUAL(y1, 0)) && (DBEQUAL(y2, 1)))
		{//x1=0;x2=1;y1=0;y2=1;(0,0)(1,1)
		 //////////////////////////////////////////////////////////////////////////
			float x[4], y[4];
			x[0] = pt1.x;
			y[0] = pt1.y;
			x[1] = pt4.x;
			y[1] = pt4.y;
			x[2] = pt2.x;
			y[2] = pt2.y;
			x[3] = pt3.x;
			y[3] = pt3.y;
			float xc = (x[0] + x[1]) / 2;
			float yc = (y[0] + y[1]) / 2;
			//////////////////////////////////////////////////////////////////////////
			float muldata = x[2] - x[3];
			if (DBEQUAL(muldata, 0))
				muldata = (float)0.0001;
			float k = (y[2] - y[3]) / (muldata);
			if (DBEQUAL(k, 0))
				k = (float)0.0001;
			float kt = -1.0f / k;
			float xa = (yc - y[0] + k * x[0] - kt * xc) / (k - kt);
			float ya = k * (xa - x[0]) + y[0];
			float xb = (yc - y[1] + k * x[1] - kt * xc) / (k - kt);
			float yb = k * (xb - x[1]) + y[1];
			ptstart.x = xa;
			ptstart.y = ya;
			ptend.x = xb;
			ptend.y = yb;
		}
		else if ((DBEQUAL(x1, 1)) && (DBEQUAL(x2, 0)) && (DBEQUAL(y1, 1)) && (DBEQUAL(y2, 0)))
		{//x1=1;x2=0;y1=1;y2=0;(1,1)(0,0)
		 //////////////////////////////////////////////////////////////////////////
			float x[4], y[4];
			x[0] = pt1.x;
			y[0] = pt1.y;
			x[1] = pt4.x;
			y[1] = pt4.y;
			x[2] = pt2.x;
			y[2] = pt2.y;
			x[3] = pt3.x;
			y[3] = pt3.y;
			float xc = (x[0] + x[1]) / 2;
			float yc = (y[0] + y[1]) / 2;
			//////////////////////////////////////////////////////////////////////////
			float muldata = x[2] - x[3];
			if (DBEQUAL(muldata, 0))
				muldata = (float)0.0001;
			float k = (y[2] - y[3]) / (muldata);
			if (DBEQUAL(k, 0))
				k = (float)0.0001;
			float kt = -1.0f / k;
			float xa = (yc - y[0] + k * x[0] - kt * xc) / (k - kt);
			float ya = k * (xa - x[0]) + y[0];
			float xb = (yc - y[1] + k * x[1] - kt * xc) / (k - kt);
			float yb = k * (xb - x[1]) + y[1];
			ptstart.x = xb;
			ptstart.y = yb;
			ptend.x = xa;
			ptend.y = ya;
		}
		else if ((DBEQUAL(x1, 0)) && (DBEQUAL(x2, 1)) && (DBEQUAL(y1, 1)) && (DBEQUAL(y2, 0)))
		{//x1=0;x2=1;y1=1;y2=0;(0,1)(1,0)
		 //////////////////////////////////////////////////////////////////////////
			float x[4], y[4];
			x[0] = pt1.x;
			y[0] = pt1.y;
			x[2] = pt2.x;
			y[2] = pt2.y;
			x[3] = pt3.x;
			y[3] = pt3.y;
			x[1] = pt4.x;
			y[1] = pt4.y;
			float xc = (x[0] + x[1]) / 2;
			float yc = (y[0] + y[1]) / 2;
			float muldata = x[1] - x[0];
			if (DBEQUAL(muldata, 0))
				muldata = (float)0.0001;
			float k = (y[1] - y[0]) / (muldata);
			if (DBEQUAL(k, 0))
				k = (float)0.0001;
			float kt = -1 / k;
			float xa = (kt*xc - k * x[2] + y[2] - yc) / (kt - k);
			float ya = k * (xa - x[2]) + y[2];
			float xb = (kt*xc - k * x[3] + y[3] - yc) / (kt - k);
			float yb = k * (xb - x[3]) + y[3];

			ptstart.x = xb;
			ptstart.y = yb;
			ptend.x = xa;
			ptend.y = ya;
		}
		else if ((DBEQUAL(x1, 1)) && (DBEQUAL(x2, 0)) && (DBEQUAL(y1, 0)) && (DBEQUAL(y2, 1)))
		{//x1=1;x2=0;y1=0;y2=1;(1,0)(0,1)
		 ////////////////////////////////////////////////////////////////////////////
			float x[4], y[4];
			x[0] = pt1.x;
			y[0] = pt1.y;
			x[2] = pt2.x;
			y[2] = pt2.y;
			x[3] = pt3.x;
			y[3] = pt3.y;
			x[1] = pt4.x;
			y[1] = pt4.y;
			float xc = (x[0] + x[1]) / 2;
			float yc = (y[0] + y[1]) / 2;
			float muldata = x[1] - x[0];
			if (DBEQUAL(muldata, 0))
				muldata = (float)0.0001;
			float k = (y[1] - y[0]) / (muldata);
			if (DBEQUAL(k, 0))
				k = (float)0.0001;
			float kt = -1 / k;
			float xa = (kt*xc - k * x[2] + y[2] - yc) / (kt - k);
			float ya = k * (xa - x[2]) + y[2];
			float xb = (kt*xc - k * x[3] + y[3] - yc) / (kt - k);
			float yb = k * (xb - x[3]) + y[3];

			ptstart.x = xa;
			ptstart.y = ya;
			ptend.x = xb;
			ptend.y = yb;
		}
		else if ((DBEQUAL(x1, 0)) && (DBEQUAL(x2, 1)) && (DBEQUAL(y1, 1)) && (DBEQUAL(y2, 1)))
		{//x1=0;x2=1;y1=1;y2=1; (0,1)(1,1)
			ptstart = pt3;
			ptend = pt4;
		}
		else if ((DBEQUAL(x1, 1)) && (DBEQUAL(x2, 0)) && (DBEQUAL(y1, 0)) && (DBEQUAL(y2, 0)))
		{//x1=1;x2=0;y1=0;y2=0;(1,0)(0,0)
			ptstart = pt2;
			ptend = pt1;
		}
		else if ((DBEQUAL(x1, 1)) && (DBEQUAL(x2, 0)) && (DBEQUAL(y1, 1)) && (DBEQUAL(y2, 1)))
		{//x1=1;x2=0;y1=1;y2=1;(1,1)(0,1)					
			ptstart = pt4;
			ptend = pt3;
		}
		else if ((DBEQUAL(x1, 1)) && (DBEQUAL(x2, 1)) && (DBEQUAL(y1, 0)) && (DBEQUAL(y2, 0)))
		{//x1=1;x2=1;y1=0;y2=0;(1,0)(1,0)
			ptstart = pt1;
			ptend = pt2;
		}
		else if ((DBEQUAL(x1, 1)) && (DBEQUAL(x2, 1)) && (DBEQUAL(y1, 0)) && (DBEQUAL(y2, 1)))
		{//x1=1;x2=1;y1=0;y2=1;(1,0)(1,1)
			ptstart = pt2;
			ptend = pt4;
		}
		else if ((DBEQUAL(x1, 1)) && (DBEQUAL(x2, 1)) && (DBEQUAL(y1, 1)) && (DBEQUAL(y2, 0)))
		{//x1=1;x2=1;y1=1;y2=0;(1,1)(1,0)
			ptstart = pt4;
			ptend = pt2;
		}
		else if ((DBEQUAL(x1, 1)) && (DBEQUAL(x2, 1)) && (DBEQUAL(y1, 1)) && (DBEQUAL(y2, 1)))
		{//x1=1;x2=1;y1=1;y2=1(1,1)(1,1)
			ptstart = pt1;
			ptend = pt2;
		}

		fx1 = ptstart.x;
		fy1 = ptstart.y;
		fx2 = ptend.x;
		fy2 = ptend.y;

		gradient.setStart(QPointF(fx1, fy1));
		gradient.setFinalStop(QPointF(fx2, fy2));
	}
	return gradient;
}
