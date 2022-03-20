
#include "SVGGDIPath.h"
#include "GdiPlus.h"
#include <atlconv.h>
#include "SVGBase.h"
#include "SVGDOM.h"
#include "XMLDOM.h"


void DrawCurveCubicImp(float x1,float y1,float x2,float y2,
								   float x3,float y3,float x4,float y4,
								   Gdiplus::GraphicsPath &path)
{
	//x1 y1 starting point of the Bezier spline. 
	//x2 y2 first control point of the Bezier spline. 
	//x3 y3 second control point of the Bezier spline. 
	//x4 y4 ending point of the Bezier spline. 
	Gdiplus::REAL rx1 = float(x1);
	Gdiplus::REAL ry1 = float(y1);
	Gdiplus::REAL rx2 = float(x2);
	Gdiplus::REAL ry2 = float(y2);
	Gdiplus::REAL rx3 = float(x3);
	Gdiplus::REAL ry3 = float(y3);
	Gdiplus::REAL rx4 = float(x4);
	Gdiplus::REAL ry4 = float(y4);
	path.AddBezier(rx1,ry1,rx2,ry2,rx3,ry3,rx4,ry4);
}

void AddSVGArc(Gdiplus::GraphicsPath *tagPath,Gdiplus::PointF startPoint,Gdiplus::PointF overPoint, double r1, double r2,
			   double angle, bool largeArcFlag, bool sweepFlag)
{
	//1.计算圆心坐标x0, y0
	float x1 = startPoint.X;
	float y1 = startPoint.Y;
	float x2 = overPoint.X;
	float y2 = overPoint.Y;

	float x3 = x1;
	float y3 = y1;
	float x4 = x2;
	float y4 = y2;

	float a = (float)r1;
	float b = (float)r2;
	float x0 = 0;
	float y0 = 0;

	if(abs(y1-y2) < pow(0.1,6))
	{//y1 == y2
		if(abs(x1-x2) < pow(0.1,6))
		{//x1 == x2 两个坐标不能相同
			return;
		}
		x0 = (float)((x1*x1-x2*x2)/(2.0*(x1-x2)));
		y0 = y1;
	}	
	else
	{
		float A =( b*b*(x2*x2-x1*x1)+(a*a*(y2*y2-y1*y1)) )/(2*a*a*(y2-y1));
		float B = (b*b*(x1-x2))/(a*a*(y2-y1));
		float C = b*b+a*a*B*B;
		float D = -2*a*a*(y1-A)*B-2*b*b*x1;
		float E = a*a*(y1-A)*(y1-A)+b*b*x1*x1-a*a*b*b;
		float deta = D*D-4*C*E;
		if(deta < pow(0.1,6))
			deta = 0;

		float x00 = (float)((-D+ pow((double)(deta),(double)0.5))/(2.0*C));
		float x11 = (float)((-D- pow((double)(deta),(double)0.5))/(2.0*C));

		x0 = x00;
		y0 = (A+B*x0);

		//确定截线两点N(x1,y1),M(x2,y2)的位置关系,以o为圆心的位置关系,绕圆心的左右关系
		//可以确定角NOM肯定是锐角.根据N绕O逆时针旋转到M的角度可以判断N,M之间的位置关系.
		//如果N绕O旋转到M是正,则M在N的逆时针方向.反之在正时针方向
		POINT o = {(int)x0,(int)y0};
		POINT pt1 = {(int)x1,(int)y1};
		POINT pt2 = {(int)x2,(int)y2};
		float angle1 = g_angle(o,pt1,pt2);

		if(angle1 > 0)
		{//N->M ==>逆时针
			if(largeArcFlag)
			{//大圆弧
				if(sweepFlag)
				{//逆时针
					x0 = x11;

					x3 = x1;
					y3 = y1;
					x4 = x2;
					y4 = y2;
				}
				else
				{
					x0 = x00;

					x3 = x2;
					y3 = y2;
					x4 = x1;
					y4 = y1;
				}
			}
			else
			{//小圆弧
				if(sweepFlag)
				{
					x0 = x00;

					x3 = x1;
					y3 = y1;
					x4 = x2;
					y4 = y2;
				}
				else
				{
					x0 = x11;

					x3 = x2;
					y3 = y2;
					x4 = x1;
					y4 = y1;
				}
			}
		}
		else
		{//N->M ==>顺时针
			if(largeArcFlag)
			{//大圆弧
				if(sweepFlag)
				{//逆时针
					x0 = x00;

					x3 = x1;
					y3 = y1;
					x4 = x2;
					y4 = y2;
				}
				else
				{
					x0 = x11;

					x3 = x2;
					y3 = y2;
					x4 = x1;
					y4 = y1;
				}
			}
			else
			{//小圆弧
				if(sweepFlag)
				{
					x0 = x11;

					x3 = x1;
					y3 = y1;
					x4 = x2;
					y4 = y2;
				}
				else
				{
					x0 = x00;

					x3 = x2;
					y3 = y2;
					x4 = x1;
					y4 = y1;
				}
			}
		}
		y0 = (A+B*x0);//y向下为正
	}

	float fstartangle = 0;
	float fendangle = 0;
	//有旋转的情况 ,没经过测试 todo
	if(abs(angle) > pow(0.1,6))
	{
		//计算旋转问题.
		//计算弦长
		float L = (float)pow((double)((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)),(double)0.5);
		//旋转前直线的斜率
		float angle1 = 0;
		if(x1-x2 > pow(0.1,6))
		{
			float k1 = (y1-y2)/(x1-x2);
			angle1 = atan(k1);
		}
		else
			angle1 = 0;
		//旋转后的斜率
		float k2 = 0;
		if( abs(angle1+angle - pow(0.1,6)) <= 90)
		{//==90度时

		}
		else
		{
			k2 = (float)tan(angle1+angle);
		}
		float F = b*b+a*a*k2*k2;
		float G = -2*b*b*x0-2*a*a*k2*y0;
		float H = b*b*x0*x0-a*a*b*b;
		float I = 4*a*a*a*a*k2*k2-4*F*a*a;
		float J = 4*G*a*a*k2+8*y0*F*a*a;
		float K = G*G-4*F*H-4*F*a*a*y0*y0-L*L*F*F/(1+k2);


		float t = 0;
		if(largeArcFlag)
		{//往哪个方向转	
			t =(float)(-J-pow((double)(J*J-4*I*K),(double)0.5))/(2*I);
		}
		else
		{
			t =(float)(-J+pow((double)(J*J-4*I*K),(double)0.5))/(2*I);
		}
		double deta2 = (double)((G+2*a*a*k2*t)*(G+2*a*a*k2*t)-4*F*(H+a*a*(t-y0)*(t-y0)) );
		if(!sweepFlag)
		{
			x3 = (float)(( -(G+2*a*a*k2*t)-pow(deta2,(double)0.5 ))/(2*F));
			x4 = (float)(( -(G+2*a*a*k2*t)+pow(deta2,(double)0.5 ))/(2*F));
		}
		else
		{
			x3 = (float)(( -(G+2*a*a*k2*t)+pow(deta2,(double)0.5 ))/(2*F));
			x4 = (float)(( -(G+2*a*a*k2*t)-pow(deta2,(double)0.5 ))/(2*F));
		}
		y3 = k2*x3+t;
		y4 = k2*x4+t;

		Matrix matrix;
		matrix = matrix.Rotate((float)angle);
		Point pt(x3,y3);
		pt = pt.matrixTransform(matrix);
		matrix = matrix.Translate(pt.getX()-x1,pt.getY()-y1);

		Gdiplus::Matrix matrix2(matrix.GetA(),matrix.GetB(),matrix.GetC(),matrix.GetD(),matrix.GetE(),matrix.GetF());
		Gdiplus::RectF rect(x0-a,y0-b,2*a,2*b);
		tagPath->AddArc(rect,fstartangle,fendangle);
		tagPath->Transform(&matrix2);
	}
	else
	{//没旋转的情况.
		POINT o1 = {(int)x0,(int)y0};
		POINT s1= {(int)x0+100,(int)y0};
		POINT e1= {(int)x3,(int)y3};
		fstartangle = (float)(g_angle(o1,s1,e1)*180/M_PI);
		if(fstartangle < 0)
			fstartangle = (float)((int)(g_angle(o1,s1,e1)*180/M_PI+360)%360);
		POINT e2= {(int)x4,(int)y4};
		fendangle = (float)(g_angle(o1,e1,e2)*180/M_PI);
		if(fendangle < 0)
			fendangle = (float)((int)(g_angle(o1,e1,e2)*180/M_PI + 360)%360);
		Gdiplus::RectF rect(x0-a,y0-b,2*a,2*b);

		//判断是否需要反转路径
		Gdiplus::GraphicsPath temppath;
		temppath.AddArc(rect,fstartangle,fendangle);
		Gdiplus::PointF lspt1,lspt2;
		temppath.GetLastPoint(&lspt1);
		tagPath->GetLastPoint(&lspt2);
		if(DB_EQUAL(lspt1.X,lspt2.X)&&DB_EQUAL(lspt1.Y,lspt2.Y))
		{//尾部都相同就说明要反转.
			tagPath->Reverse();
			tagPath->AddArc(rect,fstartangle,fendangle);
			tagPath->Reverse();
		}
		else
		{
			tagPath->AddArc(rect,fstartangle,fendangle);
		}
	}
}

void GetLinePath(Gdiplus::GraphicsPath &path,LineElement* pElement)
{
	if(!pElement)
	{
		return;
	}
	path.Reset();
	Gdiplus::REAL x1 = pElement->getX1();
	Gdiplus::REAL y1 = pElement->getY1();
	Gdiplus::REAL x2 = pElement->getX2();
	Gdiplus::REAL y2 = pElement->getY2();
	path.AddLine(x1,y1,x2,y2);
	GetMarkPath(path,pElement);
	ReSetMarkPath(pElement);
}

void GetMarkPath(Gdiplus::GraphicsPath &path,LineElement* pElement)
{
	/*DOM::DOMDocument *pDoc = pElement->getOwnerDocument();
	if(!pDoc)
		return;
	MarkerElement *pMarker = NULL;
	char patternid[256];
	memset(patternid,0,256);

	if(pElement)
	{
		float x1 = pElement->getX1().getBaseVal().getValue();
		float y1 = pElement->getY1().getBaseVal().getValue();
		float x2 = pElement->getX2().getBaseVal().getValue();
		float y2 = pElement->getY2().getBaseVal().getValue();
	
		//获得尾部的mark
		float angle = 0;
		POINT Po={(int)x1,(int)y1};
		POINT Ps={(int)x1,(int)y1+100};
		POINT Pe={(int)x2,(int)y2};
		angle = g_angle(Po,Ps,Pe)*180/PI + 90;
		const char *pId = pElement->getAttribute("marker-end");
		if(pId)
		{
			std::string str = pId;
			std::string strid = UrlStringGetId(str);
			Element* pElement = (Element*)pDoc->getElementById(strid.c_str());	
			if(pElement && pElement->getNodeType() == SVG_MARKER_NODE)
			{
				pMarker = (MarkerElement*)pElement;
			}
		}
		if(pMarker)
		{
			float refx = pMarker->GetRefX();
			float refy = pMarker->GetRefY();
			CDOMNodeList* nodelist = pMarker->getChildNodes();
			if(nodelist)
			{
				nodelist->SaveCurrentPointer();
				CDOMNode *pNode = nodelist->getFirstNode();
				while (pNode)
				{
					if(pNode->getNodeType() == SVG_POLYLINE_NODE)
					{
						PolylineElement* pPolylineElement = (PolylineElement*)pNode;
						pPolylineElement->ReMoveFromeArea();
						Gdiplus::GraphicsPath polypath;
						GetPolylinePath(polypath,pPolylineElement);
						Gdiplus::Matrix mat(1,0,0,1,x2,y2);
						mat.Rotate(angle);
						mat.Translate(-refx,-refy);						
						polypath.Transform(&mat);
						path.AddPath(&polypath,false);
					}
					pNode =nodelist->getNextNode();
				}
				nodelist->ReCoverCurrentPointer();
			}
		}
		pMarker = NULL;
		//获得头部的mark
		pId  = pElement->getAttribute("marker-start");
		if(pId)
		{
			std::string str = pId;
			std::string strid = UrlStringGetId(str);
			Element* pElement = (Element*)pDoc->getElementById(strid.c_str());	
			if(pElement && pElement->getNodeType() == SVG_MARKER_NODE)
			{
				pMarker = (MarkerElement*)pElement;
			}
		}
		if(pMarker)
		{			
			float refx = pMarker->GetRefX();
			float refy = pMarker->GetRefY();
			CDOMNodeList* nodelist = pMarker->getChildNodes();
			if(nodelist)
			{
				nodelist->SaveCurrentPointer();
				CDOMNode *pNode = nodelist->getFirstNode();
				while (pNode)
				{
					if(pNode->getNodeType() == SVG_POLYLINE_NODE)
					{
						PolylineElement* pPolylineElement = (PolylineElement*)pNode;
						pPolylineElement->ReMoveFromeArea();
						Gdiplus::GraphicsPath polypath;
						GetPolylinePath(polypath,pPolylineElement);
						Gdiplus::Matrix mat(1,0,0,1,x1,y1);
						mat.Rotate(angle);
						mat.Translate(-refx,-refy);						
						polypath.Transform(&mat);
						path.AddPath(&polypath,false);							
					}
					pNode =nodelist->getNextNode();
				}
				nodelist->ReCoverCurrentPointer();
			}
		}
	}*/
}

void ReSetMarkPath(LineElement* pElement)
{
	/*Document *pDoc = (Document*)pElement->getOwnerDocument();
	if(!pDoc)
		return;
	MarkerElement *pMarker = NULL;
	char patternid[256];
	memset(patternid,0,256);

	if(pElement)
	{
		float x1 = pElement->getX1().getBaseVal().getValue();
		float y1 = pElement->getY1().getBaseVal().getValue();
		float x2 = pElement->getX2().getBaseVal().getValue();
		float y2 = pElement->getY2().getBaseVal().getValue();

		//获得尾部的mark
		float angle = 0;
		POINT Po={(int)x1,(int)y1};
		POINT Ps={(int)x1,(int)y1+100};
		POINT Pe={(int)x2,(int)y2};
		angle = g_angle(Po,Ps,Pe)*180/PI + 90;
		const char *pId = pElement->getAttribute("marker-end");
		if(pId)
		{
			std::string str = pId;
			std::string strid = UrlStringGetId(str);
			Element* pElement = (Element*)pDoc->getElementById(strid.c_str());	
			if(pElement && pElement->getNodeType() == SVG_MARKER_NODE)
			{
				pMarker = (MarkerElement*)pElement;
			}
		}
		if(pMarker)
		{
			if(!pElement->pMarkEndpath)
				pElement->pMarkEndpath = new Gdiplus::GraphicsPath;
			pElement->pMarkEndpath->Reset();

			float refx = pMarker->GetRefX();
			float refy = pMarker->GetRefY();
			CDOMNodeList* nodelist = pMarker->getChildNodes();
			if(nodelist)
			{
				nodelist->SaveCurrentPointer();
				CDOMNode *pNode = nodelist->getFirstNode();
				while (pNode)
				{
					if(pNode->getNodeType() == SVG_POLYLINE_NODE)
					{
						PolylineElement* pPolylineElement = (PolylineElement*)pNode;
						pPolylineElement->ReMoveFromeArea();
						GetPolylinePath(*pElement->pMarkEndpath,pPolylineElement);
						Gdiplus::RectF bounds;
						pElement->pMarkEndpath->GetBounds(&bounds);
						Gdiplus::Matrix mat(1,0,0,1,x2,y2);
						mat.Rotate(angle);
						mat.Translate(-refx,-refy);						
						pElement->pMarkEndpath->Transform(&mat);

						const char *pfill = pPolylineElement->getAttribute("fill");
						if(pfill)
						{
							std::string str = pfill;
							COLORREF color = StrToColor(pfill);
							pElement->endfillclr = color;
						}
						const char *fopacity = pPolylineElement->getAttribute("fill-opacity");
						if(fopacity)
						{
							pElement->endfillopacity = atof_3(fopacity);
						}												
					}
					pNode =nodelist->getNextNode();
				}
				nodelist->ReCoverCurrentPointer();
			}
		}
		pMarker = NULL;
		//获得头部的mark
		pId  = pElement->getAttribute("marker-start");
		if(pId)
		{
			std::string str = pId;
			std::string strid = UrlStringGetId(str);
			Element* pElement = (Element*)pDoc->getElementById(strid.c_str());	
			if(pElement && pElement->getNodeType() == SVG_MARKER_NODE)
			{
				pMarker = (MarkerElement*)pElement;
			}
		}
		if(pMarker)
		{
			if(!pElement->pMarkStartpath)
				pElement->pMarkStartpath = new Gdiplus::GraphicsPath;
			pElement->pMarkStartpath->Reset();

			float refx = pMarker->GetRefX();
			float refy = pMarker->GetRefY();
			CDOMNodeList* nodelist = pMarker->getChildNodes();
			if(nodelist)
			{
				nodelist->SaveCurrentPointer();
				CDOMNode *pNode = nodelist->getFirstNode();
				while (pNode)
				{
					if(pNode->getNodeType() == SVG_POLYLINE_NODE)
					{
						PolylineElement* pPolylineElement = (PolylineElement*)pNode;
						pPolylineElement->ReMoveFromeArea();
						GetPolylinePath(*pElement->pMarkStartpath,pPolylineElement);
						Gdiplus::RectF bounds;
						pElement->pMarkStartpath->GetBounds(&bounds);
						Gdiplus::Matrix mat(1,0,0,1,x1,y1);
						mat.Rotate(angle);
						mat.Translate(-refx,-refy);						
						pElement->pMarkStartpath->Transform(&mat);
						const char *pfill = pPolylineElement->getAttribute("fill");
						if(pfill)
						{
							std::string str = pfill;
							COLORREF color = StrToColor(pfill);
							pElement->startfillclr = color;
						}
						const char *fopacity = pPolylineElement->getAttribute("fill-opacity");
						if(fopacity)
						{
							pElement->startfillopacity = atof_3(fopacity);
						}				
					}
					pNode =nodelist->getNextNode();
				}
				nodelist->ReCoverCurrentPointer();
			}
		}
	}*/
}

void GetRectPath(Gdiplus::GraphicsPath &path,RectElement* pElement)
{
	if(!pElement)
	{
		return;
	}
	Gdiplus::RectF rect2((Gdiplus::REAL)pElement->getX()
		,(Gdiplus::REAL)pElement->getY()
		,(Gdiplus::REAL)pElement->getWidth()
		,(Gdiplus::REAL)pElement->getHeight());
	path.Reset();
	path.AddRectangle(rect2);
}
void GetCirclePath(Gdiplus::GraphicsPath &path,CircleElement* pElement)
{
	if(!pElement)
	{
		return;
	}
	float x = pElement->getCx();
	float y = pElement->getCy();
	float rx = pElement->getR();
	float ry = rx;
	Gdiplus::RectF rect2((Gdiplus::REAL)(x-rx),(Gdiplus::REAL)(y-ry),(Gdiplus::REAL)(2*rx),(Gdiplus::REAL)(2*ry));
	path.Reset();
	path.AddEllipse(rect2);
}
void GetEllipsePath(Gdiplus::GraphicsPath &path,EllipseElement* pElement)
{
	if(!pElement)
	{
		return;
	}
	float x = pElement->getCx();
	float y = pElement->getCy();
	float rx = pElement->getRx();
	float ry = pElement->getRy();
	Gdiplus::RectF rect2((Gdiplus::REAL)(x-rx),(Gdiplus::REAL)(y-ry),(Gdiplus::REAL)(2*rx),(Gdiplus::REAL)(2*ry));
	path.Reset();
	path.AddEllipse(rect2);
}
void GetImagePath(Gdiplus::GraphicsPath &path,ImageElement* pElement)
{
	if(!pElement)
	{
		return;
	}
	Gdiplus::RectF rect2((Gdiplus::REAL)pElement->getX()
		,(Gdiplus::REAL)pElement->getY()
		,(Gdiplus::REAL)pElement->getwidth()
		,(Gdiplus::REAL)pElement->getheight());
	path.Reset();
	path.AddRectangle(rect2);
}
void GetPolylinePath(Gdiplus::GraphicsPath &path,PolylineElement* pElement)
{
	if(!pElement)
	{
		return;
	}
	std::list<Point>&_items = pElement->getPointList();

	int nNum = _items.size();
	Gdiplus::PointF *pPoint = new Gdiplus::PointF[nNum];
	std::list<Point>::iterator iter = _items.begin();
	int pos = 0;
	for (; iter != _items.end(); iter ++,pos ++)
	{
		pPoint[pos].X = (float)((Point)(*iter)).getX();
		pPoint[pos].Y = (float)((Point)(*iter)).getY();
	}
	path.Reset();
	path.AddLines(pPoint,nNum);
	delete []pPoint;

}
void GetPolygonPath(Gdiplus::GraphicsPath &path,PolygonElement* pElement)
{
	if(!pElement)
	{
		return;
	}
	std::list<Point>&_items = pElement->getPointList();
	int nNum = _items.size();
	if(nNum > 0)
	{
		Gdiplus::PointF *pPoint = new Gdiplus::PointF[nNum+1];
		std::list<Point>::iterator iter = _items.begin();
		Point ptlst = *iter;
		int pos = 0;
		for (; iter != _items.end(); iter ++,pos ++)
		{
			pPoint[pos].X = (float)((Point)(*iter)).getX();
			pPoint[pos].Y = (float)((Point)(*iter)).getY();
		}
		pPoint[nNum].X = (Gdiplus::REAL)ptlst.getX();
		pPoint[nNum].Y = (Gdiplus::REAL)ptlst.getY();
		path.Reset();
		path.AddLines(pPoint,nNum+1);
		delete []pPoint;
	}
}
void GetPathPath(Gdiplus::GraphicsPath &path,PathElement* pElement)
{
	if(!pElement)
	{
		return;
	}
	const PathSegList &cPathSegList = pElement->getPathSeglist();
	PathSegList &SegList = const_cast<PathSegList&>(cPathSegList);
	int nNum = SegList.getNumberOfItems();

	//起点坐标
	bool  bSet = false; //起点坐标是否已经设置过
	float firx = 0;
	float firy = 0;

	//上一次的坐标点
	float lstx = 0;
	float lsty = 0;

	//给光滑三次贝赛尔曲线的第一控制点
	float ctrlx1 = 0;
	float ctrly1 = 0;

	path.Reset();
	for (int i = 0; i < nNum; i ++)
	{
		const PathSeg* pPathSeg = SegList.getItem(i);
		if(pPathSeg)
		{
			unsigned short Segtype = pPathSeg->GetPathSegType();
			switch (Segtype)
			{
			case PATHSEG_CLOSEPATH:
				{
					PathSegClosePath * pPath = (PathSegClosePath*)pPathSeg;
					path.AddLine((Gdiplus::REAL)firx,(Gdiplus::REAL)firy,(Gdiplus::REAL)lstx,(Gdiplus::REAL)lsty);
					path.CloseFigure();
					bSet = false;
				}
				break;
			case PATHSEG_MOVETO_ABS:
				{
					PathSegMovetoAbs * pPath = (PathSegMovetoAbs*)pPathSeg;
					float x = pPath->getX();
					float y = pPath->getY();
					lstx = x;
					lsty = y;	
					if(!bSet)
					{
						firx = x;
						firy = y;
						ctrlx1 = x;
						ctrly1 = y;
						bSet = true;
					}
				}
				break;
			case PATHSEG_MOVETO_REL:
				{
					PathSegMovetoRel * pPath = (PathSegMovetoRel*)pPathSeg;
					float x = pPath->getX();
					float y = pPath->getY();
					lstx = x;
					lsty = y;
					if(!bSet)
					{
						firx = x;
						firy = y;
						ctrlx1 = x;
						ctrly1 = y;
						bSet = true;
					}
				}
				break;
			case PATHSEG_LINETO_ABS:
				{
					PathSegLinetoAbs * pPath = (PathSegLinetoAbs*)pPathSeg;
					float x = pPath->getX();
					float y = pPath->getY();
					path.AddLine((Gdiplus::REAL)lstx,(Gdiplus::REAL)lsty,(Gdiplus::REAL)x,(Gdiplus::REAL)y);
					lstx = x;
					lsty = y;
				}
				break;
			case PATHSEG_LINETO_REL:
				{
					PathSegLinetoRel * pPath = (PathSegLinetoRel*)pPathSeg;
					float x = pPath->getX();
					float y = pPath->getY();
					x +=lstx;
					y += lsty;
					path.AddLine((Gdiplus::REAL)lstx,(Gdiplus::REAL)lsty,(Gdiplus::REAL)x,(Gdiplus::REAL)y);
					lstx = x;
					lsty = y;
				}
				break;
			case PATHSEG_CURVETO_CUBIC_ABS:
				{
					PathSegCurvetoCubicAbs * pPath = (PathSegCurvetoCubicAbs*)pPathSeg;
					float x1 = pPath->getX1();
					float y1 = pPath->getY1();
					float x2 = pPath->getX2();
					float y2 = pPath->getY2();
					float x = pPath->getX();
					float y = pPath->getY();
					DrawCurveCubicImp(lstx,lsty,x1,y1,x2,y2,x,y,path);
					ctrlx1 = 2*x - x2;
					ctrly1 = 2*y - y2;
					lstx = x;
					lsty = y;
				}
				break;
			case PATHSEG_CURVETO_CUBIC_REL:
				{
					PathSegCurvetoCubicRel * pPath = (PathSegCurvetoCubicRel*)pPathSeg;
					float x1 = pPath->getX1();
					float y1 = pPath->getY1();
					float x2 = pPath->getX2();
					float y2 = pPath->getY2();
					float x = pPath->getX();
					float y = pPath->getY();
					x1 += lstx;
					y1 += lsty;
					x2 += lstx;
					y2 += lsty;
					x += lstx;
					y += lsty;
					DrawCurveCubicImp(lstx,lsty,x1,y1,x2,y2,x,y,path);
					ctrlx1 = 2*x - x2;
					ctrly1 = 2*y - y2;
					lstx = x;
					lsty = y;
				}
				break;
			case PATHSEG_CURVETO_QUADRATIC_ABS:
				{
					PathSegCurvetoQuadraticAbs * pPath = (PathSegCurvetoQuadraticAbs*)pPathSeg;
					float x1 = pPath->getX1();
					float y1 = pPath->getY1();
					float x = pPath->getX();
					float y = pPath->getY();					
					//
					lstx = x;
					lsty = y;
				}
				break;
			case PATHSEG_CURVETO_QUADRATIC_REL:
				{
					PathSegCurvetoQuadraticRel * pPath = (PathSegCurvetoQuadraticRel*)pPathSeg;
					float x = pPath->getX();
					float y = pPath->getY();
					float x1 = pPath->getX1();
					float y1 = pPath->getY1();
					x1 += lstx;
					y1 += lsty;
					x += lstx;
					y += lsty;
					//
					lstx = x;
					lsty = y;
				}
				break;
			case PATHSEG_ARC_ABS:
				{//椭圆弧
					PathSegArcAbs * pPath = (PathSegArcAbs*)pPathSeg;
					float x = pPath->getX();
					float y = pPath->getY();
					float angle = pPath->getAngle();
					float r1 = pPath->getR1();
					float r2 = pPath->getR2();
					bool blar = pPath->getLargeArcFlag();
					bool bflag = pPath->getSweepFlag();

					AddSVGArc(&path,Gdiplus::PointF(lstx,lsty),Gdiplus::PointF(x,y),
						r1,r2,angle,blar,bflag);

					lstx = x;
					lsty = y;
				}
				break;
			case PATHSEG_ARC_REL:
				{
					PathSegArcRel * pPath = (PathSegArcRel*)pPathSeg;
					float x = pPath->getX();
					float y = pPath->getY();
					float angle = pPath->getAngle();
					float r1 = pPath->getR1();
					float r2 = pPath->getR2();
					bool blar = pPath->getLargeArcFlag();
					bool bflag = pPath->getSweepFlag();
					x += lstx;
					y += lsty;
					//todo
					lstx = x;
					lsty = y;
				}
				break;
			case PATHSEG_LINETO_HORIZONTAL_ABS:
				{
					PathSegLinetoHorizontalAbs * pPath = (PathSegLinetoHorizontalAbs*)pPathSeg;
					float x = pPath->getX();
					path.AddLine((Gdiplus::REAL)lstx,(Gdiplus::REAL)lsty,(Gdiplus::REAL)x,(Gdiplus::REAL)lsty);
					lstx = x;					
				}
				break;
			case PATHSEG_LINETO_HORIZONTAL_REL:
				{
					PathSegLinetoHorizontalRel * pPath = (PathSegLinetoHorizontalRel*)pPathSeg;
					float x = pPath->getX();
					x += lstx;
					path.AddLine((Gdiplus::REAL)lstx,(Gdiplus::REAL)lsty,(Gdiplus::REAL)x,(Gdiplus::REAL)lsty);
					lstx = x;
				}
				break;
			case PATHSEG_LINETO_VERTICAL_ABS:
				{
					PathSegLinetoVerticalAbs * pPath = (PathSegLinetoVerticalAbs*)pPathSeg;
					float y = pPath->getY();
					path.AddLine((Gdiplus::REAL)lstx,(Gdiplus::REAL)lsty,(Gdiplus::REAL)lstx,(Gdiplus::REAL)y);
					lsty = y;
				}
				break;
			case PATHSEG_LINETO_VERTICAL_REL:
				{
					PathSegLinetoVerticalRel * pPath = (PathSegLinetoVerticalRel*)pPathSeg;
					float y = pPath->getY();
					y += lsty;
					path.AddLine((Gdiplus::REAL)lstx,(Gdiplus::REAL)lsty,(Gdiplus::REAL)lstx,(Gdiplus::REAL)y);
					lsty = y;
				}
				break;
			case PATHSEG_CURVETO_CUBIC_SMOOTH_ABS:
				{
					PathSegCurvetoCubicSmoothAbs * pPath = (PathSegCurvetoCubicSmoothAbs*)pPathSeg;
					float x = pPath->getX();
					float y = pPath->getY();
					float x2 = pPath->getX2();
					float y2 = pPath->getY2();					
					DrawCurveCubicImp(lstx,lsty,ctrlx1,ctrly1,x2,y2,x,y,path);
					ctrlx1 = 2*x - x2;
					ctrly1 = 2*y - y2;
					lstx = x;
					lsty = y;
				}
				break;
			case PATHSEG_CURVETO_CUBIC_SMOOTH_REL:
				{
					PathSegCurvetoCubicSmoothRel * pPath = (PathSegCurvetoCubicSmoothRel*)pPathSeg;
					float x = pPath->getX();
					float y = pPath->getY();
					float x2 = pPath->getX2();
					float y2 = pPath->getY2();
					x2 += lstx;
					y2 += lsty;
					x += lstx;
					y += lsty;
					DrawCurveCubicImp(lstx,lsty,ctrlx1,ctrly1,x2,y2,x,y,path);
					ctrlx1 = 2*x - x2;
					ctrly1 = 2*y - y2;
					lstx = x;
					lsty = y;
				}
				break;
			case PATHSEG_CURVETO_QUADRATIC_SMOOTH_ABS:
				{
					PathSegCurvetoQuadraticSmoothAbs* pPath = (PathSegCurvetoQuadraticSmoothAbs*)pPathSeg;
					float x = pPath->getX();
					float y = pPath->getY();					
					//todo
					lstx = x;
					lsty = y;
				}
				break;
			case PATHSEG_CURVETO_QUADRATIC_SMOOTH_REL:
				{
					PathSegCurvetoQuadraticSmoothRel * pPath = (PathSegCurvetoQuadraticSmoothRel*)pPathSeg;
					float x = pPath->getX();
					float y = pPath->getY();
					//todo
					x += lstx;
					y += lsty;

					lstx = x;
					lsty = y;
				}
				break;
			}
		}
	}
}


void GetTextPath(Gdiplus::GraphicsPath &path,std::string text)
{
	path.Reset();
	WCHAR *pfontfamily = L"宋体";
	float fontsize = 15;
	Gdiplus::FontFamily fontFamily(pfontfamily);
	Gdiplus::PointF pointF(0,0);
	USES_CONVERSION;
	WCHAR *pstr = A2W(text.c_str());
	Gdiplus::StringFormat strformat(0);
	Gdiplus::RectF boundtext;
	//计算文字高度
	Gdiplus::GraphicsPath pathtext;
	int nStyle = Gdiplus::FontStyleRegular;	
	pathtext.AddString(pstr,(INT)wcslen(pstr),&fontFamily,nStyle,fontsize,pointF,&strformat);
	pathtext.GetBounds(&boundtext);
	path.AddString(pstr,(INT)wcslen(pstr),&fontFamily,nStyle,fontsize,pointF,&strformat);
	//delete pstr;
}

void GetTextPath(Gdiplus::GraphicsPath &path,TextElement* pElement)
{
	if(!pElement)
	{
		return;
	}
	path.Reset();
	const char *pValue = pElement->getNodeValue().c_str();
	float x = pElement->getX();
	float y = pElement->getY();
	USES_CONVERSION;
	const char *pFont = pElement->getfamily().c_str();
	WCHAR *pfontfamily = L"宋体";
	if(pFont)
	{
		pfontfamily = A2W(pFont);
	}
	float fontsize = pElement->getsize();
	if(DB_EQUAL(fontsize,0))
	{
		fontsize = 12;
	}
	Gdiplus::FontFamily fontFamily(pfontfamily);
	Gdiplus::PointF pointF(x,y);
	WCHAR *pstr = A2W(pValue);
	Gdiplus::StringFormat strformat(0);
	Gdiplus::RectF boundtext;
	//计算文字高度
	Gdiplus::GraphicsPath pathtext;
	int nStyle = Gdiplus::FontStyleRegular;
	if(pElement->IsItalic())
	{
		nStyle |= Gdiplus::FontStyleItalic;
	}
	if(pElement->IsUnderLine())
	{
		nStyle |= Gdiplus::FontStyleUnderline;
	}
	if(pElement->IsBold())
	{
		nStyle |= Gdiplus::FontStyleBold;
	}
	pathtext.AddString(pstr,(INT)wcslen(pstr),&fontFamily,nStyle,fontsize,pointF,&strformat);
	pathtext.GetBounds(&boundtext);
	pointF.Y -= boundtext.Height;

 	if(pElement->Isvertical())
 	{//垂直
 		strformat.SetFormatFlags(Gdiplus::StringFormatFlagsDirectionVertical);
		pointF.Y += fontsize;
		pointF.X -= (int)(boundtext.Height);
 	}
	
	path.AddString(pstr,(INT)wcslen(pstr),&fontFamily,nStyle,fontsize,pointF,&strformat);

}

Gdiplus::Image *GetImage(ImageElement* pElement,std::string workpath)
{
	Gdiplus::Image *pImage = NULL;
	_ASSERT(pElement);
	std::string href = pElement->getlocalHref();
	//判断文件路径是否是正确.
	FILE* fp = NULL;
	fopen_s(&fp,href.c_str(), "rb");
	if(!fp)
	{
		href = workpath + "\\" + href;
		fopen_s(&fp,href.c_str(), "rb");
		if (!fp)
		{
			return NULL;
		}
		else
		{
			fclose(fp);
		}
	}
	//获得字符个数
	DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, href.c_str(), -1, NULL, 0);
	//分配相应的数组空间:
	wchar_t *pwText;
	pwText = new wchar_t[dwNum];
	MultiByteToWideChar(CP_ACP, 0, href.c_str(), -1, pwText, dwNum);
	pImage = Gdiplus::Image::FromFile(pwText);
	delete []pwText;

	return pImage;
}