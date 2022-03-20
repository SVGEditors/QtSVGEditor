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
#include "SVGElementPath.h"
#include "SVGDocument.h"

#define CTRL_PT1 10000001
#define CTRL_PT2 10000002

CSVGElementPath::CSVGElementPath(CSVGDocument* ownerDoc)
	:CSVGElementShape(ownerDoc, L"path", SVG_PATH)
{
}

CSVGElementPath::~CSVGElementPath()
{
	clear();
}

bool CSVGElementPath::IsPtInShape(float fx, int fy, std::wstring &retgroupid, std::wstring &retshapeid)
{
	Q_UNUSED(retgroupid);
	CSVGPointF pt(fx, fy);
	CSVGMatrix mat = m_usedmatrix;
	mat.Inverse();
	pt = pt.matrixTransform(mat);
	float fwidth = global.atof_3(GetUseStyle()[ATTR_STROKE_WIDTH].c_str());

	float fval = global.atof_3(GetUseStyle()[ATTR_FILL_OPACITY].c_str());
	bool bret = false;
	if (fval > 0.1)
	{
		bret = global.PtInPolygonEx(m_pathpts, pt);
		if (bret)
		{
			retshapeid = getAttributeId();
		}
		return bret;
	}
	else
	{
		int i = 0;
		CSVGPointF prept(0, 0);
		for (std::vector<CSVGPointF>::iterator iter = m_pathpts.begin(); iter != m_pathpts.end(); ++iter)
		{
			if (i != 0)
			{
				float halfpenwidth = fwidth;
				CSVGPointF pt1, pt2, pt3, pt4;
				global.GetparallelLine(prept, *iter, pt1, pt2, -2 - halfpenwidth);
				global.GetparallelLine(prept, *iter, pt3, pt4, 2 + halfpenwidth);
				std::vector<CSVGPointF>vcpt;
				vcpt.push_back(pt1);
				vcpt.push_back(pt2);
				vcpt.push_back(pt4);
				vcpt.push_back(pt3);
				if (global.PtInPolygonEx(vcpt, pt))
				{
					retshapeid = getAttributeId();
					bret = true;
					break;
				}
			}
			prept.x = iter->x;
			prept.y = iter->y;
			++i;
		}
		return bret;
	}
}

void CSVGElementPath::clear()
{
	std::list<CSVGPathSeg*>::iterator iter = vcpathseg.begin();
	for (; iter != vcpathseg.end(); ++iter)
	{
		delete *iter;
	}
	vcpathseg.clear();
}

void CSVGElementPath::parseD(const wchar_t* value)
{
	clear();
	if (!value)
	{
		return;
	}
	int nLen = std::wcslen(value);
	wchar_t *pChar = new wchar_t[nLen + 1];
	std::wmemset(pChar, 0, nLen + 1);
	wchar_t *pTemp = pChar;
	std::wcscpy(pChar, value);

	int nfdatasize = 56;
	float *fdata = new float[nfdatasize];
	char cType = -1;
	int nNumPos = 0;

	float lastMX = 0;
	float lastMY = 0;
	float lastx = 0;
	float lasty = 0;
	while (*pChar)
	{
		memset(fdata, 0, nfdatasize * sizeof(float));
		cType = -1;
		nNumPos = 0;
		while (*pChar && !((*pChar >= L'a' && *pChar <= L'z') || (*pChar >= L'A' && *pChar <= L'Z')))
		{
			pChar++;
		}
		if (*pChar)
		{//M0.47 88.05 A32.9981 30.8159 -180 0 0 13.55 94.77 
			cType = *pChar;
			pChar++;
			while (*pChar != 0 && !((*pChar >= L'a' && *pChar <= L'z') || (*pChar >= L'A' && *pChar <= L'Z')))
			{
				while (*pChar != 0 && !((*pChar >= '0' && *pChar <= '9') || (*pChar >= L'a' && *pChar <= L'z') || (*pChar >= L'A' && *pChar <= L'Z')
					|| *pChar == '.' || *pChar == '-' || *pChar == '+'))
				{
					pChar++;
				}

				wchar_t *pTemp = pChar;

				bool bhaspoint = false;
				while ((*pChar >= '0' &&*pChar <= '9')
					|| *pChar == '.'
					|| (*(pChar - 1) == 'E' && *pChar == '-')
					|| (*(pChar - 1) == 'e' && *pChar == '-')
					|| (pTemp == pChar && *pChar == '-')
					|| (pTemp == pChar && *pChar == '+')
					|| *pChar == 'e'
					|| *pChar == 'E')
				{
					if (bhaspoint && (*pChar == '.'))
						break;
					if (*pChar == '.')
						bhaspoint = true;
					pChar++;
				}

				wchar_t c = *pChar;
				*pChar = 0;
				if (nNumPos >= nfdatasize)
				{
					float *fnewdata = new float[nfdatasize * 2];
					memset(fnewdata, 0, sizeof(float)*nfdatasize * 2);
					memcpy(fnewdata, fdata, nfdatasize * sizeof(float));
					nfdatasize = nfdatasize * 2;
					delete[]fdata;
					fdata = fnewdata;
				}
				fdata[nNumPos] = global.atof_3(pTemp);
				nNumPos++;
				*pChar = c;

				while (*pChar != 0 && !((*pChar >= '0' && *pChar <= '9') || (*pChar >= L'a' && *pChar <= L'z') || (*pChar >= L'A' && *pChar <= L'Z')
					|| *pChar == '.' || *pChar == '-' || *pChar == '+'))
				{
					pChar++;
				}
			}
		}
		switch (cType)
		{
		case L'M':
		case L'm':
		{
			if (cType == L'M')
			{
				lastx = 0;
				lasty = 0;
				lastMX = fdata[0];
				lastMY = fdata[1];
			}
			if (vcpathseg.size() > 0)
			{
				CSVGPathSegClosePath *pClose = new CSVGPathSegClosePath();
				vcpathseg.push_back(pClose);
			}
			CSVGPathSegMoveto *pMoveto = new CSVGPathSegMoveto();
			pMoveto->x = fdata[0];
			pMoveto->y = fdata[1];
			if (cType == L'm')
			{
				pMoveto->x += lastx;
				pMoveto->y += lasty;
				lastMX = pMoveto->x;
				lastMY = pMoveto->y;
			}
			lastx = pMoveto->x;
			lasty = pMoveto->y;
			vcpathseg.push_back(pMoveto);
			for (int n = 2; n < nNumPos; n += 2)
			{
				CSVGPathSegLineto *pLineto = new  CSVGPathSegLineto();
				pLineto->x = fdata[n];
				pLineto->y = fdata[n + 1];
				if (cType == L'm')
				{
					pLineto->x += lastx;
					pLineto->y += lasty;
				}
				lastx = pLineto->x;
				lasty = pLineto->y;
				vcpathseg.push_back(pLineto);
			}
			break;
		}
		case L'L':
		case L'l':
		{
			for (int n = 0; n < nNumPos; n += 2)
			{
				CSVGPathSegLineto *pLineto = new  CSVGPathSegLineto();
				pLineto->x = fdata[n];
				pLineto->y = fdata[n + 1];
				if (cType == L'l')
				{
					pLineto->x += lastx;
					pLineto->y += lasty;
				}
				lastx = pLineto->x;
				lasty = pLineto->y;
				vcpathseg.push_back(pLineto);
			}
			break;
		}
		case L'C':
		case L'c':
		{
			for (int n = 0; n < nNumPos; n += 6)
			{
				CSVGPathSegCurvetoCubic *pCurveTo = new CSVGPathSegCurvetoCubic();
				pCurveTo->x1 = fdata[n];
				pCurveTo->y1 = fdata[n + 1];
				pCurveTo->x2 = fdata[n + 2];
				pCurveTo->y2 = fdata[n + 3];
				pCurveTo->x = fdata[n + 4];
				pCurveTo->y = fdata[n + 5];

				if (cType == L'c')
				{
					pCurveTo->x1 += lastx;
					pCurveTo->y1 += lasty;
					pCurveTo->x2 += lastx;
					pCurveTo->y2 += lasty;
					pCurveTo->x += lastx;
					pCurveTo->y += lasty;
				}
				lastx = pCurveTo->x;
				lasty = pCurveTo->y;

				vcpathseg.push_back(pCurveTo);
			}
			break;
		}
		case L'Q':
		case L'q':
		{
			for (int n = 0; n < nNumPos; n += 4)
			{
				CSVGPathSegCurvetoQuadratic *pQuadratic = new CSVGPathSegCurvetoQuadratic();
				pQuadratic->x1 = fdata[n + 0];
				pQuadratic->y1 = fdata[n + 1];
				pQuadratic->x = fdata[n + 2];
				pQuadratic->y = fdata[n + 3];

				if (cType == L'q')
				{
					pQuadratic->x1 += lastx;
					pQuadratic->y1 += lasty;
					pQuadratic->x += lastx;
					pQuadratic->y += lasty;
				}
				lastx = pQuadratic->x;
				lasty = pQuadratic->y;
				vcpathseg.push_back(pQuadratic);
			}
			break;
		}
		case L'A':
		case L'a':
		{
			if (nNumPos == 6)
			{
				////rx ry x-axis-rotation large-arc-flag sweep-flag x y	
				fdata[6] = fdata[5];
				fdata[5] = fdata[4];
				fdata[4] = 0;
			}
			for (int n = 0; n < nNumPos; n += 7)
			{
				CSVGPathSegArc* pArc = new CSVGPathSegArc();
				pArc->r1 = fdata[n];
				pArc->r2 = fdata[n + 1];
				pArc->angle = fdata[n + 2];
				pArc->largeArcFlag = ((int)(fdata[n + 3]) != 0);
				pArc->sweepFlag = ((int)(fdata[n + 4]) != 0);
				pArc->x = fdata[n + 5];
				pArc->y = fdata[n + 6];
				if (cType == 'a')
				{
					pArc->x += lastx;
					pArc->y += lasty;
				}
				lastx = pArc->x;
				lasty = pArc->y;
				vcpathseg.push_back(pArc);
			}
			break;
		}
		case L'H':
		case L'h':
		{
			for (int n = 0; n < nNumPos; n += 1)
			{
				CSVGPathSegLineto *pLineto = new CSVGPathSegLineto();
				pLineto->x = fdata[n];
				pLineto->y = lasty;
				if (cType == 'h')
				{
					pLineto->x += lastx;
				}
				lastx = pLineto->x;
				vcpathseg.push_back(pLineto);
			}
			break;
		}
		case L'V':
		case L'v':
		{
			for (int n = 0; n < nNumPos; n += 1)
			{
				CSVGPathSegLineto *pLineto = new CSVGPathSegLineto();
				pLineto->y = fdata[n];
				pLineto->x = lastx;
				if (cType == 'v')
				{
					pLineto->y += lasty;
				}
				lasty = pLineto->y;
				vcpathseg.push_back(pLineto);
			}
			break;
		}
		case L'S':
		case L's':
		{
			for (int n = 0; n < nNumPos; n += 4)
			{
				CSVGPathSegCurvetoCubicSmooth *pSmoothS = new CSVGPathSegCurvetoCubicSmooth();
				pSmoothS->x2 = fdata[n + 0];
				pSmoothS->y2 = fdata[n + 1];
				pSmoothS->x = fdata[n + 2];
				pSmoothS->y = fdata[n + 3];
				if (cType == 's')
				{
					pSmoothS->x2 += lastx;
					pSmoothS->y2 += lasty;
					pSmoothS->x += lastx;
					pSmoothS->y += lasty;
				}
				lastx = pSmoothS->x;
				lasty = pSmoothS->y;
				vcpathseg.push_back(pSmoothS);
			}
			break;
		}
		case L'T':
		case L't':
		{
			for (int n = 0; n < nNumPos; n += 2)
			{
				CSVGPathSegCurvetoQuadraticSmooth *pSmoothT = new CSVGPathSegCurvetoQuadraticSmooth();
				pSmoothT->x = fdata[n + 0];
				pSmoothT->y = fdata[n + 1];
				if (cType == L't')
				{
					pSmoothT->x += lastx;
					pSmoothT->y += lasty;
				}
				lastx = pSmoothT->x;
				lasty = pSmoothT->y;
				vcpathseg.push_back(pSmoothT);
			}
			break;
		}
		case L'Z':
		case L'z':
		{
			CSVGPathSegClosePath *pClose = new CSVGPathSegClosePath();
			vcpathseg.push_back(pClose);

			lastx = lastMX;
			lasty = lastMY;
			break;
		}
		default:
			break;
		}
	}
	delete[]fdata;
	delete pTemp;
}


void CSVGElementPath::setAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
{
	if (addorsetAttribute(attrname, value, bSetModfiyFlag))
	{
		CSVGElement::setAttribute(attrname, value, bSetModfiyFlag);
	}
	else
	{
		CSVGElementStyleable::setAttribute(attrname, value, bSetModfiyFlag);
	}
}


bool  CSVGElementPath::addorsetAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
{
	if (attrname == L"d")
	{
		QString strValue = QString::fromStdWString(value);
		strValue.replace("&#xD;", "");//n
		strValue.replace("&#xA;", "");//r
		strValue.replace("&#x9;", "");//Tab 
		strValue.replace("&#20;", "");//space 
		parseD(strValue.toStdWString().c_str());
	}
	else
	{
		return false;
	}
	if (bSetModfiyFlag)
	{
		CSVGElement::pathHasChange();
	}
	return true;
}

void  CSVGElementPath::calBBox()
{
	m_bbox.Empty();
	float x1 = 0;
	float y1 = 0;
	float x2 = 0;
	float y2 = 0;
	for (size_t i = 0; i < m_pathpts.size(); ++i)
	{
		if (i == 0)
		{
			x1 = m_pathpts[i].x;
			y1 = m_pathpts[i].y;
			x2 = m_pathpts[i].x;
			y2 = m_pathpts[i].y;
		}
		else
		{
			x1 = SVGmin(x1, m_pathpts[i].x);
			y1 = SVGmin(y1, m_pathpts[i].y);
			x2 = SVGmax(x2, m_pathpts[i].x);
			y2 = SVGmax(y2, m_pathpts[i].y);
		}
	}
	m_bbox.x = x1;
	m_bbox.y = y1;
	m_bbox.width = x2 - x1;
	m_bbox.height = y2 - y1;
}

void CSVGElementPath::resetPath()
{
	if (m_bPathHasChanged)
	{
		m_Path = QPainterPath();
		m_pathpts.clear();
		getPathpoints();
		bool  bSet = false;
		float firx = 0;
		float firy = 0;

		float lstx = 0;
		float lsty = 0;

		float ctrlx1 = 0;
		float ctrly1 = 0;

		CSVGPointF ptstart;
		CSVGPointF ptend;
		std::list<CSVGPathSeg*>::iterator iter = vcpathseg.begin();
		for (; iter != vcpathseg.end(); ++iter)
		{
			CSVGPathSeg* pPathSeg = *iter;
			if (pPathSeg)
			{
				unsigned short Segtype = pPathSeg->getType();
				switch (Segtype)
				{
				case PATH_Z:
				{
					m_Path.closeSubpath();
					bSet = false;
				}
				break;
				case PATH_M:
				{
					CSVGPathSegMoveto * pPath = (CSVGPathSegMoveto*)pPathSeg;
					float x = pPath->x;
					float y = pPath->y;
					lstx = x;
					lsty = y;
					if (!bSet)
					{
						ptstart.x = x;
						ptstart.y = y;
						ptend = ptstart;

						firx = x;
						firy = y;
						ctrlx1 = x;
						ctrly1 = y;
						bSet = true;
					}
					m_Path.moveTo(x, y);
				}
				break;
				case PATH_L:
				{
					CSVGPathSegLineto * pPath = (CSVGPathSegLineto*)pPathSeg;
					float x = pPath->x;
					float y = pPath->y;
					m_Path.lineTo(x, y);
					lstx = x;
					lsty = y;
				}
				break;
				case PATH_C:
				{
					CSVGPathSegCurvetoCubic * pPath = (CSVGPathSegCurvetoCubic*)pPathSeg;
					float x1 = pPath->x1;
					float y1 = pPath->y1;
					float x2 = pPath->x2;
					float y2 = pPath->y2;
					float x = pPath->x;
					float y = pPath->y;
					m_Path.cubicTo(x1, y1, x2, y2, x, y);
					ctrlx1 = 2 * x - x2;
					ctrly1 = 2 * y - y2;
					lstx = x;
					lsty = y;
				}
				break;
				case PATH_Q:
				{
					CSVGPathSegCurvetoQuadratic * pPath = (CSVGPathSegCurvetoQuadratic*)pPathSeg;
					float x1 = pPath->x1;;
					float y1 = pPath->y1;
					float x = pPath->x;
					float y = pPath->y;

					m_Path.cubicTo(x1, y1, x1, y1, x, y);

					ctrlx1 = 2 * x - x1;
					ctrly1 = 2 * y - y1;
					//
					lstx = x;
					lsty = y;
				}
				break;
				case PATH_A:
				{
					CSVGPathSegArc * pPath = (CSVGPathSegArc*)pPathSeg;
					float x = pPath->x;
					float y = pPath->y;
					float angle = pPath->angle;
					float r1 = pPath->r1;
					float r2 = pPath->r2;
					bool blar = pPath->largeArcFlag;
					bool bflag = pPath->sweepFlag;

					std::vector<BezierData> vcBeziers = arctobezier(lstx, lsty, r1, r2, angle, blar, bflag, x, y);
					for (size_t i = 0; i < vcBeziers.size(); ++i)
					{
						m_Path.cubicTo(vcBeziers[i].pt[1].x, vcBeziers[i].pt[1].y,
							vcBeziers[i].pt[2].x, vcBeziers[i].pt[2].y, vcBeziers[i].pt[3].x, vcBeziers[i].pt[3].y);
					}
					lstx = x;
					lsty = y;
				}
				break;
				case PATH_S:
				{
					CSVGPathSegCurvetoCubicSmooth * pPath = (CSVGPathSegCurvetoCubicSmooth*)pPathSeg;
					float x = pPath->x;
					float y = pPath->y;
					float x2 = pPath->x2;
					float y2 = pPath->y2;

					m_Path.cubicTo(ctrlx1, ctrly1, x2, y2, x, y);

					ctrlx1 = 2 * x - x2;
					ctrly1 = 2 * y - y2;
					lstx = x;
					lsty = y;
				}
				break;
				case PATH_H:
				{
					CSVGPathSegLineto * pPath = (CSVGPathSegLineto*)pPathSeg;
					float x = pPath->x;
					float y = pPath->y;
					m_Path.lineTo(x, y);
					lstx = x;
					lsty = y;
				}
				break;
				case PATH_V:
				{
					CSVGPathSegLineto * pPath = (CSVGPathSegLineto*)pPathSeg;
					float x = pPath->x;
					float y = pPath->y;
					m_Path.lineTo(x, y);
					lstx = x;
					lsty = y;
				}
				break;
				case PATH_T:
				{
					CSVGPathSegCurvetoQuadraticSmooth* pPath = (CSVGPathSegCurvetoQuadraticSmooth*)pPathSeg;
					float x = pPath->x;
					float y = pPath->y;

					float x1 = ctrlx1;
					float y1 = ctrly1;

					m_Path.cubicTo(x1, y1, x1, y1, x, y);

					ctrlx1 = 2 * x - x1;
					ctrly1 = 2 * y - y1;

					lstx = x;
					lsty = y;
				}
				break;
				}
			}
		}
		CSVGElementShape::resetPath();
	}
}

std::vector<BezierData> CSVGElementPath::arctobezier(float startx, float starty, float rX, float rY, float xaxisrotation,
	unsigned short largearcflag, unsigned short sweepflag, float endx, float endy)
{
	std::vector<BezierData> vcBezierdata;

	if (rX == 0.0f && rY == 0.0f)
	{
		return vcBezierdata;
	}

	double RadiansPerDegree = SVGPI / 180.0;
	double sinPhi = sin(xaxisrotation * RadiansPerDegree);
	double cosPhi = cos(xaxisrotation * RadiansPerDegree);

	double x1dash = cosPhi * (startx - endx) / 2.0 + sinPhi * (starty - endy) / 2.0;
	double y1dash = -sinPhi * (startx - endx) / 2.0 + cosPhi * (starty - endy) / 2.0;

	double root = 0;
	double numerator = rX * rX * rY * rY - rX * rX * y1dash * y1dash - rY * rY * x1dash * x1dash;

	double rx = (double)rX;
	double ry = (double)rY;

	if (numerator < 0.0)
	{
		double s = (double)sqrt((double)1.0 - numerator / (rX * rX * rY * rY));
		rx *= s;
		ry *= s;
		root = 0.0;
	}
	else
	{
		root = (double)((largearcflag == 1 && sweepflag == 1) || (largearcflag == 0 && sweepflag == 0) ? -1.0 : 1.0) * sqrt(numerator / (rX * rX * y1dash * y1dash + rY * rY * x1dash * x1dash));
	}

	double cxdash = root * rx * y1dash / ry;
	double cydash = -root * ry * x1dash / rx;

	double cx = cosPhi * cxdash - sinPhi * cydash + (startx + endx) / 2.0;
	double cy = sinPhi * cxdash + cosPhi * cydash + (starty + endy) / 2.0;

	double theta1 = (double)calculatevectorangle((double)1.0, (double)0.0, (x1dash - cxdash) / rx, (y1dash - cydash) / ry);
	double dtheta = (double)calculatevectorangle((x1dash - cxdash) / rx, (y1dash - cydash) / ry, (-x1dash - cxdash) / rx, (-y1dash - cydash) / ry);

	if (sweepflag == 0 && dtheta > 0)
	{
		dtheta -= 2.0 * SVGPI;
	}
	else if (sweepflag == 1 && dtheta < 0)
	{
		dtheta += 2.0 * SVGPI;
	}

	int segments = (int)ceil((double)abs(dtheta / (SVGPI / 2.0)));
	double delta = dtheta / segments;
	double t = 8.0 / 3.0 * sin(delta / 4.0) * sin(delta / 4.0) / sin(delta / 2.0);

	double startX = startx;
	double startY = starty;
	for (int i = 0; i < segments; ++i)
	{
		double cosTheta1 = cos(theta1);
		double sinTheta1 = sin(theta1);
		double theta2 = theta1 + delta;
		double cosTheta2 = cos(theta2);
		double sinTheta2 = sin(theta2);

		double endpointX = cosPhi * rx * cosTheta2 - sinPhi * ry * sinTheta2 + cx;
		double endpointY = sinPhi * rx * cosTheta2 + cosPhi * ry * sinTheta2 + cy;

		double dx1 = t * (-cosPhi * rx * sinTheta1 - sinPhi * ry * cosTheta1);
		double dy1 = t * (-sinPhi * rx * sinTheta1 + cosPhi * ry * cosTheta1);

		double dxe = t * (cosPhi * rx * sinTheta2 + sinPhi * ry * cosTheta2);
		double dye = t * (sinPhi * rx * sinTheta2 - cosPhi * ry * cosTheta2);

		BezierData bezier;
		bezier.pt[0] = CSVGPointF((float)startX, (float)startY);
		bezier.pt[1] = CSVGPointF((float)(startX + dx1), (float)(startY + dy1));
		bezier.pt[2] = CSVGPointF((float)(endpointX + dxe), (float)(endpointY + dye));
		bezier.pt[3] = CSVGPointF((float)endpointX, (float)endpointY);
		vcBezierdata.push_back(bezier);

		theta1 = theta2;
		startX = (float)endpointX;
		startY = (float)endpointY;
	}
	return vcBezierdata;
}

double CSVGElementPath::calculatevectorangle(double ux, double uy, double vx, double vy)
{
	double ta = (double)atan2(uy, ux);
	double tb = (double)atan2(vy, vx);
	if (tb >= ta)
	{
		return (double)(tb - ta);
	}
	return (double)(SVGPI * 2 - (ta - tb));
}

CSVGPointF CSVGElementPath::GetPt(CSVGPointF pt0, CSVGPointF pt1, CSVGPointF pt2, CSVGPointF pt3, float t)
{
	CSVGPointF pt;
	pt.x = pt0.x*(1 - t)*(1 - t)*(1 - t) + 3 * pt1.x*(1 - t)*(1 - t)*t + 3 * pt2.x*t*t*(1 - t) + pt3.x*t*t*t;
	pt.y = pt0.y*(1 - t)*(1 - t)*(1 - t) + 3 * pt1.y*(1 - t)*(1 - t)*t + 3 * pt2.y*t*t*(1 - t) + pt3.y*t*t*t;
	return pt;
}

CSVGRectF CSVGElementPath::GetBezierBox(CSVGPointF &pt_0, CSVGPointF &pt_1
	, CSVGPointF &pt_2, CSVGPointF& pt_3)
{
	CSVGRectF rectf;
	float x0 = pt_0.x;
	float y0 = pt_0.y;
	float x1 = pt_1.x;
	float y1 = pt_1.y;
	float x2 = pt_2.x;
	float y2 = pt_2.y;
	float x3 = pt_3.x;
	float y3 = pt_3.y;
	CSVGPointF pt1 = pt_0;
	CSVGPointF pt2 = pt_0;
	CSVGPointF pt3 = pt_0;
	CSVGPointF pt4 = pt_0;
	if (std::abs(y0 - 3 * y1 + 3 * y2 - y3) > 0)
	{
		float t1 = (y0 - 2 * y1 + y2 - sqrt(y1*y1 - y0*y2 - y1*y2 + y2*y2 + y0*y3 - y1*y3)) / (y0 - 3 * y1 + 3 * y2 - y3);
		float t2 = (y0 - 2 * y1 + y2 + sqrt(y1*y1 - y0*y2 - y1*y2 + y2*y2 + y0*y3 - y1*y3)) / (y0 - 3 * y1 + 3 * y2 - y3);
		if (t1 > 0 && t1 < 1)
			pt1 = GetPt(pt_0, pt_1, pt_2, pt_3, t1);
		if (t2 > 0 && t2 < 1)
			pt2 = GetPt(pt_0, pt_1, pt_2, pt_3, t2);
	}
	if (std::abs(x0 - 3 * x1 + 3 * x2 - x3) > 0)
	{
		float t3 = (x0 - 2 * x1 + x2 - sqrt(x1*x1 - x0*x2 - x1*x2 + x2*x2 + x0*x3 - x1*x3)) / (x0 - 3 * x1 + 3 * x2 - x3);
		float t4 = (x0 - 2 * x1 + x2 + sqrt(x1*x1 - x0*x2 - x1*x2 + x2*x2 + x0*x3 - x1*x3)) / (x0 - 3 * x1 + 3 * x2 - x3);
		if (t3 > 0 && t3 < 1)
			pt3 = GetPt(pt_0, pt_1, pt_2, pt_3, t3);
		if (t4 > 0 && t4 < 1)
			pt4 = GetPt(pt_0, pt_1, pt_2, pt_3, t4);
	}
	rectf.x = SVGmin(SVGmin(SVGmin(SVGmin(SVGmin(pt_0.x, pt_3.x), pt1.x), pt2.x), pt3.x), pt4.x);
	rectf.y = SVGmin(SVGmin(SVGmin(SVGmin(SVGmin(pt_0.y, pt_3.y), pt1.y), pt2.y), pt3.y), pt4.y);
	float maxx = SVGmax(SVGmax(SVGmax(SVGmax(SVGmax(pt_0.x, pt_3.x), pt1.x), pt2.x), pt3.x), pt4.x);
	float maxy = SVGmax(SVGmax(SVGmax(SVGmax(SVGmax(pt_0.y, pt_3.y), pt1.y), pt2.y), pt3.y), pt4.y);
	rectf.width = maxx - rectf.x;
	rectf.height = maxy - rectf.y;
	return rectf;
}

inline CSVGPointF GetBezierPoint(CSVGPointF A, CSVGPointF B, CSVGPointF C, CSVGPointF D, double t)
{
	CSVGPointF P;
	P.x = (float)(pow((1 - t), 3)*A.x + 3 * t*pow((1 - t), 2)*B.x + 3 * (1 - t)*pow(t, 2)*C.x + pow(t, 3)*D.x);
	P.y = (float)(pow((1 - t), 3)*A.y + 3 * t*pow((1 - t), 2)*B.y + 3 * (1 - t)*pow(t, 2)*C.y + pow(t, 3)*D.y);
	return P;
}

void CSVGElementPath::getBezierpoints(CSVGPointF &pt_0, CSVGPointF &pt_1, CSVGPointF &pt_2, CSVGPointF& pt_3, std::vector<CSVGPointF>&vcpoint)
{
	vcpoint.push_back(pt_0);
	CSVGPointF lastpt = pt_0;
	for (double t = 0.0; t <= 1.0; t += 0.05)
	{
		CSVGPointF getP = GetBezierPoint(pt_0, pt_1, pt_2, pt_3, t);
		if (std::abs(lastpt.x - getP.x) >= 1 || std::abs(lastpt.y - getP.y) >= 1)
		{
			vcpoint.push_back(getP);
			lastpt = getP;
		}
	}
}

std::vector<CSVGPointF> CSVGElementPath::getPathpoints()
{
	if (m_pathpts.size() > 0)
		return m_pathpts;

	bool  bSet = false;
	float firx = 0;
	float firy = 0;

	float lstx = 0;
	float lsty = 0;

	float ctrlx1 = 0;
	float ctrly1 = 0;

	CSVGPointF ptstart;
	CSVGPointF ptend;
	std::list<CSVGPathSeg*>::iterator iter = vcpathseg.begin();
	for (; iter != vcpathseg.end(); ++iter)
	{
		CSVGPathSeg* pPathSeg = *iter;
		if (pPathSeg)
		{
			unsigned short Segtype = pPathSeg->getType();
			switch (Segtype)
			{
			case PATH_Z:
			{
				m_pathpts.push_back(CSVGPointF(firx, firy));
				m_pathpts.push_back(CSVGPointF(lstx, lsty));
				bSet = false;
			}
			break;
			case PATH_M:
			{
				CSVGPathSegMoveto * pPath = (CSVGPathSegMoveto*)pPathSeg;
				float x = pPath->x;
				float y = pPath->y;
				lstx = x;
				lsty = y;
				if (!bSet)
				{
					ptstart.x = x;
					ptstart.y = y;
					ptend = ptstart;

					firx = x;
					firy = y;
					ctrlx1 = x;
					ctrly1 = y;
					bSet = true;
				}
			}
			break;
			case PATH_L:
			{
				CSVGPathSegLineto * pPath = (CSVGPathSegLineto*)pPathSeg;
				float x = pPath->x;
				float y = pPath->y;
				m_pathpts.push_back(CSVGPointF(lstx, lsty));
				m_pathpts.push_back(CSVGPointF(x, y));
				lstx = x;
				lsty = y;
			}
			break;
			case PATH_C:
			{
				CSVGPathSegCurvetoCubic * pPath = (CSVGPathSegCurvetoCubic*)pPathSeg;
				float x1 = pPath->x1;
				float y1 = pPath->y1;
				float x2 = pPath->x2;
				float y2 = pPath->y2;
				float x = pPath->x;
				float y = pPath->y;

				CSVGPointF pt1(lstx, lsty);
				CSVGPointF pt2(x1, y1);
				CSVGPointF pt3(x2, y2);
				CSVGPointF pt4(x, y);
				getBezierpoints(pt1, pt2, pt3, pt4, m_pathpts);

				ctrlx1 = 2 * x - x2;
				ctrly1 = 2 * y - y2;
				lstx = x;
				lsty = y;
			}
			break;
			case PATH_Q:
			{
				CSVGPathSegCurvetoQuadratic * pPath = (CSVGPathSegCurvetoQuadratic*)pPathSeg;
				float x1 = pPath->x1;;
				float y1 = pPath->y1;
				float x = pPath->x;
				float y = pPath->y;

				CSVGPointF pt1(lstx, lsty);
				CSVGPointF pt2(x1, y1);
				CSVGPointF pt3(x1, x1);
				CSVGPointF pt4(x, y);
				getBezierpoints(pt1, pt2, pt3, pt4, m_pathpts);

				//
				lstx = x;
				lsty = y;
			}
			break;
			case PATH_A:
			{
				CSVGPathSegArc * pPath = (CSVGPathSegArc*)pPathSeg;
				float x = pPath->x;
				float y = pPath->y;
				float angle = pPath->angle;
				float r1 = pPath->r1;
				float r2 = pPath->r2;
				bool blar = pPath->largeArcFlag;
				bool bflag = pPath->sweepFlag;

				std::vector<BezierData> vcBeziers = arctobezier(lstx, lsty, r1, r2, angle, blar, bflag, x, y);
				for (size_t i = 0; i < vcBeziers.size(); ++i)
				{
					CSVGPointF pt1(vcBeziers[i].pt[0].x, vcBeziers[i].pt[0].y);
					CSVGPointF pt2(vcBeziers[i].pt[1].x, vcBeziers[i].pt[1].y);
					CSVGPointF pt3(vcBeziers[i].pt[2].x, vcBeziers[i].pt[2].y);
					CSVGPointF pt4(vcBeziers[i].pt[3].x, vcBeziers[i].pt[3].y);
					getBezierpoints(pt1, pt2, pt3, pt4, m_pathpts);
				}
				lstx = x;
				lsty = y;
			}
			break;
			case PATH_S:
			{
				CSVGPathSegCurvetoCubicSmooth * pPath = (CSVGPathSegCurvetoCubicSmooth*)pPathSeg;
				float x = pPath->x;
				float y = pPath->y;
				float x2 = pPath->x2;
				float y2 = pPath->y2;

				CSVGPointF pt1(lstx, lsty);
				CSVGPointF pt2(ctrlx1, ctrly1);
				CSVGPointF pt3(x2, y2);
				CSVGPointF pt4(x, y);

				getBezierpoints(pt1, pt2, pt3, pt4, m_pathpts);

				ctrlx1 = 2 * x - x2;
				ctrly1 = 2 * y - y2;
				lstx = x;
				lsty = y;
			}
			break;
			case PATH_T:
			{
				CSVGPathSegCurvetoQuadraticSmooth* pPath = (CSVGPathSegCurvetoQuadraticSmooth*)pPathSeg;
				float x = pPath->x;
				float y = pPath->y;
				//todo
				lstx = x;
				lsty = y;
			}
			break;
			}
		}
	}
	return m_pathpts;
}

void  CSVGElementPath::resetpatternPath(int nNewWidth, int nNewHeight)
{
	if (m_bPathHasChanged)
	{
		m_bPathHasChanged = false;
		m_pathpts.clear();
		getPathpoints();
		calBBox();
		float scalex = nNewWidth*1.0 / m_bbox.width + 0.00001;
		float scaley = nNewHeight*1.0 / m_bbox.height + 0.00001;
		m_bbox.x *= scalex;
		m_bbox.y *= scaley;
		m_bbox.width *= scalex;
		m_bbox.height *= scaley;
		m_Path = QPainterPath();

		bool  bSet = false;
		float firx = 0;
		float firy = 0;

		float lstx = 0;
		float lsty = 0;

		float ctrlx1 = 0;
		float ctrly1 = 0;

		CSVGPoint ptstart;
		CSVGPoint ptend;
		std::list<CSVGPathSeg*>::iterator iter = vcpathseg.begin();

		for (; iter != vcpathseg.end(); ++iter)
		{
			CSVGPathSeg* pPathSeg = *iter;
			if (pPathSeg)
			{
				unsigned short Segtype = pPathSeg->getType();
				switch (Segtype)
				{
				case PATH_Z:
				case PATH_z0:
				{
					m_Path.closeSubpath();
					bSet = false;
				}
				break;
				case PATH_M:
				{
					CSVGPathSegMoveto * pPath = (CSVGPathSegMoveto*)pPathSeg;
					float x = pPath->x;
					float y = pPath->y;
					lstx = x;
					lsty = y;
					if (!bSet)
					{
						ptstart.x = x;
						ptstart.y = y;
						ptend = ptstart;

						firx = x;
						firy = y;
						ctrlx1 = x;
						ctrly1 = y;
						bSet = true;
					}
					m_Path.moveTo(x*scalex, y*scaley);
				}
				break;
				case PATH_L:
				{
					CSVGPathSegLineto * pPath = (CSVGPathSegLineto*)pPathSeg;
					float x = pPath->x;
					float y = pPath->y;
					m_Path.lineTo(x*scalex, y*scaley);
					lstx = x;
					lsty = y;
				}
				break;
				case PATH_C:
				{
					CSVGPathSegCurvetoCubic * pPath = (CSVGPathSegCurvetoCubic*)pPathSeg;
					float x1 = pPath->x1;
					float y1 = pPath->y1;
					float x2 = pPath->x2;
					float y2 = pPath->y2;
					float x = pPath->x;
					float y = pPath->y;
					m_Path.cubicTo(x1*scalex, y1*scaley, x2*scalex, y2*scaley, x*scalex, y*scaley);
					ctrlx1 = 2 * x - x2;
					ctrly1 = 2 * y - y2;
					lstx = x;
					lsty = y;
				}
				break;
				case PATH_Q:
				{
					CSVGPathSegCurvetoQuadratic * pPath = (CSVGPathSegCurvetoQuadratic*)pPathSeg;
					float x1 = pPath->x1;;
					float y1 = pPath->y1;
					float x = pPath->x;
					float y = pPath->y;

					m_Path.cubicTo(x1*scalex, y1*scaley, x1*scalex, y1*scaley, x*scalex, y*scaley);

					ctrlx1 = 2 * x - x1;
					ctrly1 = 2 * y - y1;
					//
					lstx = x;
					lsty = y;
				}
				break;
				case PATH_A:
				{
					CSVGPathSegArc * pPath = (CSVGPathSegArc*)pPathSeg;
					float x = pPath->x;
					float y = pPath->y;
					float angle = pPath->angle;
					float r1 = pPath->r1;
					float r2 = pPath->r2;
					bool blar = pPath->largeArcFlag;
					bool bflag = pPath->sweepFlag;

					std::vector<BezierData> vcBeziers = arctobezier(lstx, lsty, r1, r2, angle, blar, bflag, x, y);
					for (size_t i = 0; i < vcBeziers.size(); ++i)
					{
						m_Path.cubicTo(vcBeziers[i].pt[1].x*scalex, vcBeziers[i].pt[1].y*scaley,
							vcBeziers[i].pt[2].x*scalex, vcBeziers[i].pt[2].y*scaley, vcBeziers[i].pt[3].x*scalex, vcBeziers[i].pt[3].y*scaley);
					}
					lstx = x;
					lsty = y;
				}
				break;
				case PATH_S:
				{
					CSVGPathSegCurvetoCubicSmooth * pPath = (CSVGPathSegCurvetoCubicSmooth*)pPathSeg;
					float x = pPath->x;
					float y = pPath->y;
					float x2 = pPath->x2;
					float y2 = pPath->y2;
					m_Path.cubicTo(ctrlx1*scalex, ctrly1*scaley, x2*scalex, y2*scaley, x*scalex, y*scaley);
					ctrlx1 = 2 * x - x2;
					ctrly1 = 2 * y - y2;
					lstx = x;
					lsty = y;
				}
				break;
				case PATH_H:
				{
					CSVGPathSegLineto * pPath = (CSVGPathSegLineto*)pPathSeg;
					float x = pPath->x;
					float y = pPath->y;
					m_Path.lineTo(x*scalex, y*scaley);
					lstx = x;
					lsty = y;
				}
				break;
				case PATH_V:
				{
					CSVGPathSegLineto * pPath = (CSVGPathSegLineto*)pPathSeg;
					float x = pPath->x;
					float y = pPath->y;
					m_Path.lineTo(x*scalex, y*scaley);
					lstx = x;
					lsty = y;
				}
				case PATH_T:
				{
					CSVGPathSegCurvetoQuadraticSmooth* pPath = (CSVGPathSegCurvetoQuadraticSmooth*)pPathSeg;
					float x = pPath->x;
					float y = pPath->y;
					//todo
					lstx = x;
					lsty = y;
				}
				break;
				}
			}
		}
	}
}

CSVGPathSegMoveto* CSVGElementPath::createSVGPathSegMoveto(float x, float y)
{
	CSVGPathSegMoveto* seg = new CSVGPathSegMoveto;
	if (seg)
	{
		seg->x = x;
		seg->y = y;
		vcpathseg.push_back(seg);
	}
	return seg;
}


CSVGPathSegLineto* CSVGElementPath::createSVGPathSegLineto(float x, float y)
{
	CSVGPathSegLineto* seg = new CSVGPathSegLineto;
	if (seg)
	{
		seg->x = x;
		seg->y = y;
		vcpathseg.push_back(seg);
	}
	return seg;
}

CSVGPathSegCurvetoCubic* CSVGElementPath::createSVGPathSegCurvetoCubic(float x1, float y1, float x2, float y2, float x, float y)
{
	CSVGPathSegCurvetoCubic* seg = new CSVGPathSegCurvetoCubic;
	if (seg)
	{
		seg->x = x;
		seg->y = y;
		seg->x1 = x1;
		seg->y1 = y1;
		seg->x2 = x2;
		seg->y2 = y2;
		vcpathseg.push_back(seg);
	}
	return seg;
}

CSVGPathSegArc* CSVGElementPath::createSVGPathSegArc(float r1, float r2, float angle, float x, float y)
{
	CSVGPathSegArc * seg = new CSVGPathSegArc;
	if (seg)
	{
		seg->r1 = r1;
		seg->r2 = r2;
		seg->angle = angle;
		seg->x = x;
		seg->y = y;
		seg->largeArcFlag = 1;
		seg->sweepFlag = 1;
		vcpathseg.push_back(seg);
	}
	return seg;
}

void  CSVGElementPath::unClosePath()
{
	CSVGPathSeg *pSeg = getLastSeg(0);
	if (pSeg->getType() == PATH_Z)
	{
		vcpathseg.pop_back();
		delete pSeg;
		resetPath();
	}
}

void  CSVGElementPath::ClosePath()
{
	createSVGPathSegClose();
	resetPath();
}

void  CSVGElementPath::createSVGPathSegClose()
{
	CSVGPathSegClosePath *pClose = new CSVGPathSegClosePath;
	vcpathseg.push_back(pClose);
}

void  CSVGElementPath::changeCurvetoCubic(CSVGPathSegCurvetoCubic *pCubic, float x1, float y1, float x2, float y2)
{
	if (pCubic)
	{
		pCubic->x1 = x1;
		pCubic->y1 = y1;
		pCubic->x2 = x2;
		pCubic->y2 = y2;
	}
}

void  CSVGElementPath::removeSeg(CSVGPathSeg *pPathseg)
{
	std::list<CSVGPathSeg*>::iterator iter = vcpathseg.begin();
	for (; iter != vcpathseg.end();)
	{
		if (*iter == pPathseg)
		{
			delete pPathseg;
			iter = vcpathseg.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

CSVGPathSeg * CSVGElementPath::getBeginSeg(int nIndex)
{
	std::list<CSVGPathSeg *>::iterator iter = vcpathseg.begin();
	for (int i = 0; iter != vcpathseg.end(); ++iter, ++i)
	{
		if (i == nIndex)
		{
			return *iter;
		}
	}
	return NULL;
}

CSVGPathSeg  *CSVGElementPath::getLastSeg(int nIndex)
{
	std::list<CSVGPathSeg *>::reverse_iterator iter = vcpathseg.rbegin();
	for (int i = 0; iter != vcpathseg.rend(); ++iter, ++i)
	{
		if (i == nIndex)
		{
			return *iter;
		}
	}
	return NULL;
}

int CSVGElementPath::GetPathSegSize()
{
	return (int)vcpathseg.size();
}

std::list<CSVGPathSeg*> &CSVGElementPath::GetPathSeg()
{
	return vcpathseg;
}

std::wstring CSVGElementPath::pathToString()
{
	std::wstring str;
	if (vcpathseg.size() > 0)
	{
		wchar_t szbuf[256];
		std::wmemset(szbuf, 0, 256);
		std::list<CSVGPathSeg*>::iterator iter = vcpathseg.begin();
		std::list<CSVGPathSeg*>::iterator iterend = vcpathseg.end();
		(*iter)->Encode(szbuf, 255);
		str += szbuf;
		unsigned short lsttype = PATH_NULL;
		for (iter++; iter != iterend; iter++)
		{
			if (lsttype == PATH_Z && PATH_Z == (*iter)->getType())
			{
				continue;
			}
			(*iter)->Encode(szbuf, 255);
			str += L" ";
			str += szbuf;
			lsttype = (*iter)->getType();
		}
	}
	return str;
}

void CSVGElementPath::SetTwoPrecision()
{
	std::wstring strpath = pathToString();
	setAttribute(L"d", strpath);
}
