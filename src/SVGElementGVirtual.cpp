
#include "SVGElementGVirtual.h"
#include "SVGDocument.h"

CSVGElementGVirtual::CSVGElementGVirtual(CSVGDocument* ownerDoc)
	:CSVGElementShape(ownerDoc,"gvirtual",SVG_GVIRTUAL)
{
}

CSVGElementGVirtual::~CSVGElementGVirtual()
{
}

bool CSVGElementGVirtual::IsPtInShape(float fx, int fy)
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

bool CSVGElementGVirtual::IsShapeInRect(CSVGRect rectf)
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

void  CSVGElementGVirtual::Move(int deltax, int deltay, const CSVGMatrix *screenmat)
{
	CSVGElementShape::Move(deltax, deltay, screenmat);
	matrixToChild();
	m_lstMat = m_matrix;
}

void CSVGElementGVirtual::SizeTo(int nptIndex, int nx, int ny, const CSVGMatrix *screenmat)
{
	CSVGElementShape::SizeTo(nptIndex, nx, ny, screenmat);
	matrixToChild();
	m_lstMat = m_matrix;
}

void  CSVGElementGVirtual::Rotate(float angle, float cx, float cy)
{
	CSVGElementShape::Rotate(angle, cx, cy);
	matrixToChild();
	m_lstMat = m_matrix;
}

void   CSVGElementGVirtual::matrixToChild()
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
				pChildShape->ClearXML();
			}
		}
	}
}

void   CSVGElementGVirtual::addChildOnly(CSVGElement *pChild)
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

void   CSVGElementGVirtual::removeAllChildOnly()
{
	m_ChildNodes.clear();
	clearFocus();
	m_lstMat.SetEmpty();
	m_matrix.SetEmpty();
	m_usedmatrix.SetEmpty();
}

void   CSVGElementGVirtual::removeChildOnly(CSVGElement *pChild)
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