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
#include "SVGElementImage.h"
#include "SVGDocument.h"
#include <QFile>

CSVGElementImage::CSVGElementImage(CSVGDocument* ownerDoc)
	:CSVGElementShape(ownerDoc, L"image", SVG_IMAGE)
{
	x = 0;
	y = 0;
	width = 0;
	height = 0;
}

CSVGElementImage::~CSVGElementImage()
{
}

void CSVGElementImage::setAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
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

bool CSVGElementImage::addorsetAttribute(const std::wstring& attrname, const std::wstring& value, bool bSetModfiyFlag)
{
	if (attrname == L"x")
	{
		x = global.atof_3(value);
	}
	else if (attrname == L"y")
	{
		y = global.atof_3(value);
	}
	else if (attrname == L"width")
	{
		width = global.atof_3(value);
	}
	else if (attrname == L"height")
	{
		height = global.atof_3(value);
	}
	else if (attrname == L"xlink:href")
	{
		xlinkhref = value;
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

std::set<std::wstring> CSVGElementImage::getAttrNames()
{
	std::set<std::wstring>vcName = CSVGElementShape::getAttrNames();
	vcName.insert(vcName.begin(), L"height");
	vcName.insert(vcName.begin(), L"width");
	vcName.insert(vcName.begin(), L"y");
	vcName.insert(vcName.begin(), L"x");
	return vcName;
}

void  CSVGElementImage::calBBox()
{
	m_bbox.x = x;
	m_bbox.y = y;
	m_bbox.width = width;
	m_bbox.height = height;
}

void CSVGElementImage::resetPath()
{
	if (m_bPathHasChanged)
	{
		m_Path = QPainterPath();
		if (!IsEmbedImage())
		{
			std::wstring fullpath;

			if (QFile::exists(QString::fromStdWString(xlinkhref)))
			{
				fullpath = xlinkhref;
			}
			else
			{
				std::wstring folder = m_pownDocument->getLocalFolder();
				std::wstring strpath = folder + xlinkhref;
				if (QFile::exists(QString::fromStdWString(strpath)))
				{
					fullpath = strpath;
				}
			}
			if (fullpath.length() > 0)
			{
				m_Image.load(QString::fromStdWString(fullpath));
			}
		}
		else
		{
			std::wstring strdata = GetEmbedImageData();
			m_Image = LoadFromBase64(strdata);
		}
		if (width > 0 && height > 0 && !m_Image.isNull())
		{
			int nOrgWidth = m_Image.width();
			int nOrgHeight = m_Image.height();
			if (width != nOrgWidth || height != nOrgHeight)
			{
				float fscalex = width * 1.0 / nOrgWidth;
				float fscaley = height * 1.0 / nOrgHeight;
				float fscale = fscalex;
				int nxpos = 0;
				int nypos = 0;
				if (fscalex >= fscaley)
				{
					fscale = fscaley;
					nxpos = (width - nOrgWidth * fscale)*1.0 / 2;
					if (nxpos < 0)
						nxpos = 0;
				}
				else
				{
					fscale = fscalex;
					nypos = (height - nOrgHeight * fscale)*1.0 / 2;
					if (nypos < 0)
						nypos = 0;
				}
				int nNewWidth = nOrgWidth * fscale;
				int nNewHeight = nOrgHeight * fscale;
				QImage newImage = QImage(width, height, QImage::Format_ARGB32);
				newImage.fill(0);
				QPainter painter(&newImage);
				m_Image = m_Image.scaled(QSize(nNewWidth, nNewHeight), Qt::IgnoreAspectRatio, Qt::FastTransformation);
				painter.drawImage(nxpos, nypos, m_Image);
				m_Image = newImage;
			}
		}
		CSVGElementShape::resetPath();
	}
}

QImage CSVGElementImage::LoadFromBase64(const std::wstring &strdata)
{
	QString qstr = QString::fromStdWString(strdata);
	QByteArray imagedata = qstr.toLatin1();
	QByteArray byteArr = QByteArray::fromBase64(imagedata);
	QImage image;
	image.loadFromData(byteArr);
	return image;
}

bool CSVGElementImage::IsEmbedImage()
{
	if (xlinkhref.find(L"data:image") != std::wstring::npos
		|| xlinkhref.find(L"data:metafile") != std::wstring::npos)
	{
		return true;
	}
	return false;
}

std::wstring CSVGElementImage::GetEmbedImageData()
{
	std::wstring strdata;
	if (IsEmbedImage())
	{
		size_t nfind = xlinkhref.find(L"base64,");
		if (nfind != std::wstring::npos)
		{
			int noffset = nfind + 7;
			strdata = xlinkhref.substr(noffset, xlinkhref.length() - noffset);
		}
	}
	return strdata;
}

void  CSVGElementImage::resetpatternPath(int nNewWidth, int nNewHeight)
{
	if (m_bPathHasChanged)
	{
		calBBox();
		m_bPathHasChanged = false;
		float scalex = nNewWidth * 1.0 / m_bbox.width + 0.00001;
		float scaley = nNewHeight * 1.0 / m_bbox.height + 0.00001;

		m_Path = QPainterPath();
		if (!IsEmbedImage())
		{
			std::wstring fullpath;
			if (QFile::exists(QString::fromStdWString(xlinkhref)))
			{
				fullpath = xlinkhref;
			}
			else
			{
				std::wstring folder = m_pownDocument->getLocalFolder();
				std::wstring strpath = folder + xlinkhref;
				if (QFile::exists(QString::fromStdWString(strpath)))
				{
					fullpath = strpath;
				}
			}
			if (fullpath.length() > 0)
			{
				m_Image.load(QString::fromStdWString(fullpath));
			}
		}
		else
		{
			std::wstring strdata = GetEmbedImageData();
			m_Image = LoadFromBase64(strdata);
		}

		float fnewwidth = width * scalex;
		float fnewheight = height * scaley;
		if (fnewwidth > 0 && height > 0 && !m_Image.isNull())
		{
			int nOrgWidth = m_Image.width();
			int nOrgHeight = m_Image.height();
			if (fnewwidth != nOrgWidth || fnewheight != nOrgHeight)
			{
				float fscalex = fnewwidth * 1.0 / nOrgWidth;
				float fscaley = fnewheight * 1.0 / nOrgHeight;
				float fscale = fscalex;
				int nxpos = 0;
				int nypos = 0;
				if (fscalex >= fscaley)
				{
					fscale = fscaley;
					nxpos = (fnewwidth - nOrgWidth * fscale)*1.0 / 2;
					if (nxpos < 0)
						nxpos = 0;
				}
				else
				{
					fscale = fscalex;
					nypos = (height - nOrgHeight * fscale)*1.0 / 2;
					if (nypos < 0)
						nypos = 0;
				}
				int nNewWidth = nOrgWidth * fscale;
				int nNewHeight = nOrgHeight * fscale;
				QImage newImage = QImage(fnewwidth, fnewheight, QImage::Format_ARGB32);
				newImage.fill(0);
				QPainter painter(&newImage);
				m_Image = m_Image.scaled(QSize(nNewWidth, nNewHeight), Qt::IgnoreAspectRatio, Qt::FastTransformation);
				painter.drawImage(nxpos, nypos, m_Image);
				m_Image = newImage;
			}
		}
	}
}

bool CSVGElementImage::IsPtInShape(float fx, int fy, std::wstring &retgroupid, std::wstring &retshapeid)
{
	Q_UNUSED(retgroupid);
	CSVGPointF pt(fx, fy);
	CSVGMatrix mat = m_usedmatrix;
	mat.Inverse();
	pt = pt.matrixTransform(mat);
	float fwidth = global.atof_3(GetUseStyle()[ATTR_STROKE_WIDTH].c_str());
	CSVGRectF rectf = m_bbox.Inflate(fwidth, fwidth);
	bool bRet = rectf.PtInRect(pt);
	if (bRet)
	{
		retshapeid = getAttributeId();
	}
	return bRet;
}

void  CSVGElementImage::SizeTo(int nptIndex, int nx, int ny, const CSVGMatrix *screenmat)
{
	CSVGPointF pt((float)nx, (float)ny);
	CSVGMatrix matrix = *screenmat;
	matrix.Add(m_usedmatrix);
	matrix.Inverse();
	pt = pt.matrixTransform(matrix);
	int x2 = x + width;
	int y2 = y + height;

	switch (nptIndex)
	{
	case LEFT_TOP:
	{
		x = pt.x;
		y = pt.y;
		width = x2 - x;
		height = y2 - y;
		if (width < MIN_SIZE)
		{
			width = MIN_SIZE;
			x = x2 - MIN_SIZE;
		}
		if (height <= MIN_SIZE)
		{
			height = MIN_SIZE;
			y = y2 - MIN_SIZE;
		}
		break;
	}
	case RIGHT_TOP:
	{
		y = pt.y;
		width = pt.x - x;
		height = y2 - y;
		if (width < MIN_SIZE)
		{
			width = MIN_SIZE;
		}
		if (height < MIN_SIZE)
		{
			height = MIN_SIZE;
			y = y2 - MIN_SIZE;
		}
		break;
	}
	case LEFT_BOTTOM:
	{
		x = pt.x;
		width = x2 - x;
		height = pt.y - y;
		if (x2 - x < MIN_SIZE)
		{
			x = x2 - MIN_SIZE;
			width = MIN_SIZE;
		}
		if (height < MIN_SIZE)
		{
			height = MIN_SIZE;
		}
		break;
	}
	case RIGHT_BOTTOM:
	{
		width = pt.x - x;
		height = pt.y - y;
		if (width < MIN_SIZE)
		{
			width = MIN_SIZE;
		}
		if (height < MIN_SIZE)
		{
			height = MIN_SIZE;
		}
		break;
	}
	case LEFT_MID:
	{
		x = pt.x;
		width = x2 - x;
		if (width < MIN_SIZE)
		{
			x = x2 - MIN_SIZE;
			width = MIN_SIZE;
		}
		break;
	}
	case TOP_MID:
	{
		y = pt.y;
		height = y2 - y;
		if (height < MIN_SIZE)
		{
			height = MIN_SIZE;
			y = y2 - MIN_SIZE;
		}
		break;
	}
	case RIGHT_MID:
	{
		width = pt.x - x;
		if (width < MIN_SIZE)
		{
			width = MIN_SIZE;
		}
		break;
	}
	case BOTTOM_MID:
	{
		height = pt.y - y;
		if (height < MIN_SIZE)
		{
			height = MIN_SIZE;
		}
		break;
	}
	default:
		break;
	}
}
