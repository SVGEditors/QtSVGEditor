
#include "SVGElementGTemp.h"
#include "SVGDocument.h"

CSVGElementGTemp::CSVGElementGTemp(CSVGDocument* ownerDoc)
	:CSVGElementShape(ownerDoc,"gtemp",SVG_GTEMP)
{
}

CSVGElementGTemp::~CSVGElementGTemp()
{
}

void  CSVGElementGTemp::calBBox()
{
	m_bbox.Empty();
	if (m_ChildNodes.size() == 0)
		return;
	for (size_t i = 0; i < m_ChildNodes.size(); ++i)
	{
		if (m_ChildNodes[i]->IsShapeElement())
		{
			CSVGElementShape *pShape = (CSVGElementShape*)m_ChildNodes[i];
			CSVGRect childRect = pShape->getBBox();

			CSVGMatrix *pMat = pShape->getMatrix();
			CSVGMatrix mat = m_usedmatrix;
			mat.Inverse();
			mat.Add(*pMat);
			childRect = childRect.matrixTransform(mat);
			if (i == 0)
			{
				m_bbox = childRect;
			}
			else
			{
				CSVGPoint rectRb(m_bbox.x + m_bbox.width, m_bbox.y + m_bbox.height);
				CSVGPoint childRb(childRect.x + childRect.width, childRect.y + childRect.height);
				float minx = min(min(min(m_bbox.x, childRect.x), rectRb.x), childRb.x);
				float miny = min(min(min(m_bbox.y, childRect.y), rectRb.y), childRb.y);
				float maxx = max(max(max(m_bbox.x, childRect.x), rectRb.x), childRb.x);
				float maxy = max(max(max(m_bbox.y, childRect.y), rectRb.y), childRb.y);

				m_bbox.x = minx;
				m_bbox.y = miny;
				m_bbox.width = maxx - minx;
				m_bbox.height = maxy - miny;
			}
		}
	}
}

bool CSVGElementGTemp::IsPtInShape(float fx, int fy)
{
	for (size_t i = 0; i < m_ChildNodes.size(); ++i)
	{
		if (m_ChildNodes[i]->IsShapeElement())
		{
			CSVGElementShape *pShape = (CSVGElementShape*)m_ChildNodes[i];
			if (pShape->IsPtInShape(fx, fy))
			{
				return true;
			}
		}
	}
	return  false;
}

bool CSVGElementGTemp::IsShapeInRect(CSVGRect rectf)
{
	for (size_t i = 0; i < m_ChildNodes.size(); ++i)
	{
		if (m_ChildNodes[i]->IsShapeElement())
		{
			CSVGElementShape *pShape = (CSVGElementShape*)m_ChildNodes[i];
			if (pShape->IsShapeInRect(rectf))
			{
				return true;
			}
		}
	}
	return false;
}

void  CSVGElementGTemp::Move(int deltax, int deltay, const CSVGMatrix *screenmat)
{
	CSVGElementShape::Move(deltax, deltay, screenmat);
	matrixToChild();
	m_lstMat = m_matrix;
}

void CSVGElementGTemp::SizeTo(int nptIndex, int nx, int ny, const CSVGMatrix *screenmat)
{
	CSVGElementShape::SizeTo(nptIndex, nx, ny, screenmat);
	matrixToChild();
	m_lstMat = m_matrix;
}

void  CSVGElementGTemp::Rotate(float angle, float cx, float cy)
{
	CSVGElementShape::Rotate(angle, cx, cy);
	matrixToChild();
	m_lstMat = m_matrix;
}

void   CSVGElementGTemp::matrixToChild()
{
	if (m_pownDocument->IsSelectG(this))
	{//把自身的矩阵给子节点
		for (size_t i = 0; i < m_ChildNodes.size(); ++i)
		{
			if (m_ChildNodes[i]->IsShapeElement())
			{
				CSVGElementShape *pChildShape = (CSVGElementShape*)m_ChildNodes[i];
				CSVGMatrix *gMat = getOwnerMatrix();
				CSVGMatrix rMat = m_lstMat;
				rMat.Inverse();
				CSVGMatrix *ownerMat = pChildShape->getOwnerMatrix();
				CSVGMatrix newMat = *gMat;
				newMat.Add(rMat);
				newMat.Add(*ownerMat);
				pChildShape->setOwnerMatrix(&newMat);
				pChildShape->resetUseMatrix();
			}
		}
	}
}

void   CSVGElementGTemp::addChildOnly(CSVGElement *pChild)
{
	if (pChild == this)
	{
		_ASSERT(FALSE);
	}
	if (pChild)
	{
		m_ChildNodes.push_back(pChild);
	}
}

void   CSVGElementGTemp::removeAllChildOnly()
{
	m_ChildNodes.clear();
	clearFocus();
	m_lstMat.SetEmpty();
	m_matrix.SetEmpty();
	m_usedmatrix.SetEmpty();
}

void   CSVGElementGTemp::removeChildOnly(CSVGElement *pChild)
{
	std::vector<CSVGElement*>::iterator iter = m_ChildNodes.begin();
	for (; iter != m_ChildNodes.end();)
	{//迭代器删除必须全部走完,否则下次迭代器使用会出现:iterator not incrementable
		if (*iter == pChild)
		{
			iter = m_ChildNodes.erase(iter);
		}
		else
		{
			++iter;
		}
	}
	if (m_ChildNodes.size() <= 1)
	{
		removeAllChildOnly();
	}
}