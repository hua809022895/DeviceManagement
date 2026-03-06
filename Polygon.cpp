// Polygon.cpp: implementation of the CPolygon class.
#include "stdafx.h"
#include "Polygon.h"

#include "global.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace std;
//////////////////////////////////////////////////////////////////////////
PointGps::PointGps()
{
	lng = 0;
	lat = 0;
	hit=0;
}

PointGps::PointGps(double longitude, double latitude,double hieght)
{
	lng = longitude;
	lat = latitude;
	hit	=hieght;
}

bool PointGps::operator == ( PointGps pt){
	return pt.lng == lng && pt.lat == lat && pt.hit == hit;
}


double PointGps::GetDistance(PointGps pt){	//单位毫米
	double x = (pt.lng - lng)  * MM_PER_LONGITUDE_ZJR(lat);
	double y = (pt.lat - lat ) * MM_PER_LATITUDE;
	double dis = sqrt(x*x + y*y);
	return dis;
}

double PointGps::GetBearing(PointGps pt){//°
	double x = (pt.lng - lng)	* MM_PER_LONGITUDE_ZJR(lat);
	double y = ( pt.lat - lat ) * MM_PER_LATITUDE;
	double bearing = atan2(y, x) / M_PI_PRE_DEGREE;
	return bearing;
}

double PointGps::Get3DDistance(PointGps pt){	//单位毫米

	double d=(pt.lng - lng)*(pt.lng - lng) +(pt.lat - lat)*(pt.lat - lat) + (pt.hit -pt.hit)*(pt.hit -pt.hit);
	d=abs(sqrt(d));
	return d;
}

//PointRay//////////////////////////////////////////////////////////////////////////
PointRay::PointRay()
{
	lenth = 0;
	angle = 0;
}

PointRay::PointRay(double len, double ang)
{
	lenth = len;
	angle = ang;
}

//极坐标转换为平面坐标
Point2D PointRay::Point2DByCenterAngle( Point2D ptCenter, double heading )
{
	Point2D pt;
	double bearing = ( heading + angle ) * M_PI_PRE_DEGREE;
	pt.x = ptCenter.x + lenth * sin( bearing );
	pt.y = ptCenter.y + lenth * cos( bearing );
	return pt;
}

//////////////////////////////////////////////////////////////////////////
//Point2D
Point2D::Point2D()
{ 
	x = 0; 
	y = 0;
}

Point2D::Point2D( PointGps ptGps, PointGps ptCenter)//给定原点，需要每次重新计算
{
	x = (ptGps.lng - ptCenter.lng) * MM_PER_LONGITUDE_BYLAT(ptCenter.lat);
	y = (ptGps.lat - ptCenter.lat) * MM_PER_LATITUDE;
}

Point2D::Point2D(PointGps ptGps)//采用默认原点，需要每次重新计算
{
	x = ptGps.lng;// -LONGITUDE_ORG)* MM_PER_LONGITUDE_ZJR(lat);
	y = ptGps.lat;// -LATITUDE_ORG ) * MM_PER_LATITUDE;
}

Point2D::Point2D(Point2D ptOrg, PointRay ptRay, double Angle)//通过原点，和相对位置，和当前的角度，计算计算当前的平面点
{
	double bearing = ( Angle + ptRay.angle ) * M_PI_PRE_DEGREE;
	x = ptOrg.x + ptRay.lenth * sin( bearing );
	y = ptOrg.y + ptRay.lenth * cos( bearing );
}

bool Point2D::operator == ( Point2D pt)
{
	return pt.x == x && pt.y == y;
}

//Line//////////////////////////////////////////////////////////////////////////
Line::Line()
{	
}

Line::Line(Point2D ptA, Point2D ptB)
{
	pointA = ptA;
	pointB = ptB;
}

Line::Line( double left, double top, double right, double bottom)
{
	pointA.x = left;
	pointA.y = top;
	pointB.x = right;
	pointB.y = bottom;
}

//排斥实验 看以线段为对角线的矩形是否相交
bool Line::IsRectCross(const Line& cl)
{
	bool ret = min(pointA.x, pointB.x) <= max(cl.pointA.x, cl.pointB.x) &&
		min(cl.pointA.x, cl.pointB.x) <= max(pointA.x, pointB.x) &&
		min(pointA.y, pointB.y) <= max(cl.pointA.y, cl.pointB.y) &&
		min(cl.pointA.y, cl.pointB.y) <= max(pointA.y, pointB.y);
	
	return ret;
}

//跨立判断,是否相交
bool Line::IsLineSegmentCross(const Line& cl)
{
	//端点相同
	if ( pointA == cl.pointA || pointB == cl.pointB || pointA == cl.pointB || pointB == cl.pointA ) {
		return true;
	}
	
	double f1 = pointA.x * ( cl.pointA.y - pointB.y ) + pointB.x * ( pointA.y - cl.pointA.y ) + cl.pointA.x * ( pointB.y - pointA.y );
	double f2 = pointA.x * ( cl.pointB.y - pointB.y ) + pointB.x * ( pointA.y - cl.pointB.y ) + cl.pointB.x * ( pointB.y - pointA.y );		
	INT64 line1 = (INT64)f1;
	INT64 line2 = (INT64)f2;
	if (((line1 ^ line2) >= 0) && !(line1 == 0 && line2 == 0)) {
		return false;
	}
	
	f1 = cl.pointA.x * ( pointA.y - cl.pointB.y ) + cl.pointB.x * ( cl.pointA.y - pointA.y ) + pointA.x * ( cl.pointB.y - cl.pointA.y );
	f2 = cl.pointA.x * ( pointB.y - cl.pointB.y ) + cl.pointB.x * ( cl.pointA.y - pointB.y ) + pointB.x * ( cl.pointB.y - cl.pointA.y );
	line1 = (INT64)f1;
	line2 = (INT64)f2;
	if (((line1 ^ line2) >= 0) && !(line1 == 0 && line2 == 0)) {
	   return false;
	}

	return true;
}

//求解两个线段的交点
bool Line::GetCrossPoint( const Line& cl, Point2D& pt )
{
	if ( IsRectCross(cl)) {//判断矩形是否相交
		if ( IsLineSegmentCross(cl)) {//判断两个线段是否横跨
			//求交点
			double tmpLeft = ( cl.pointB.x - cl.pointA.x ) * ( pointA.y - pointB.y ) - ( pointB.x - pointA.x ) * ( cl.pointA.y - cl.pointB.y );
			double tmpRight = ( pointA.y - cl.pointA.y ) * ( pointB.x - pointA.x ) * ( cl.pointB.x - cl.pointA.x ) 
				+ cl.pointA.x * ( cl.pointB.y - cl.pointA.y ) * ( pointB.x - pointA.x ) 
				- pointA.x * ( pointB.y - pointA.y ) * ( cl.pointB.x - cl.pointA.x );
			pt.x = tmpRight / tmpLeft;
			
			tmpLeft = ( pointA.x - pointB.x ) * ( cl.pointB.y - cl.pointA.y ) 
				-  ( pointB.y - pointA.y ) * ( cl.pointA.x - cl.pointB.x );
			tmpRight = pointB.y * ( pointA.x - pointB.x ) * ( cl.pointB.y - cl.pointA.y )
				+ ( cl.pointB.x - pointB.x ) * ( cl.pointB.y - cl.pointA.y ) * ( pointA.y - pointB.y ) 
				- ( cl.pointB.y * ( cl.pointA.x - cl.pointB.x ) * ( pointB.y - pointA.y ) );
			pt.y = tmpRight / tmpLeft;
			
			return true;
		}
	}
	
	return false;
}

//PgnRect
PgnRect::PgnRect()
{
	m_height=0;
	diagonal.pointA.x=0;
}

//保证left、right、top、bottom含义
PgnRect::PgnRect(double left, double top, double right, double bottom)
{
	m_height=0;
	diagonal.pointA.x = min( left, right);
	diagonal.pointA.y = min( top, bottom);
	diagonal.pointB.x = max( left, right);
	diagonal.pointB.y = max( top, bottom);
}

//合并两个矩形，为一个包含两个矩形的矩形
void PgnRect::Merger( const PgnRect& rt )
{
	diagonal.pointA.x = min( diagonal.pointA.x, rt.diagonal.pointA.x);
	diagonal.pointA.y = min( diagonal.pointA.y, rt.diagonal.pointA.y);
	diagonal.pointB.x = max( diagonal.pointB.x, rt.diagonal.pointB.x);
	diagonal.pointB.y = max( diagonal.pointB.y, rt.diagonal.pointB.y);
}

//判断两个矩形是否相交
bool PgnRect::IsCross( const PgnRect& rt )
{
	return diagonal.IsRectCross( rt.diagonal );
}

//点是否在矩形之内
bool PgnRect::PtInRect( double x, double y )
{
	bool bRet = ( x >= diagonal.pointA.x && x <= diagonal.pointB.x ) 
				&& ( y >= diagonal.pointA.y && y <= diagonal.pointB.y );
	return bRet;
}
//////////////////////////////////////////////////////////////////////////
//PgnPolygon
//////////////////////////////////////////////////////////////////////////
PgnPolygon::PgnPolygon(bool bReInit)
{
	m_bReInit	= bReInit;
	m_Height=0;
}

PgnPolygon::~PgnPolygon()
{
	Reset();
}


PgnRect PgnPolygon::GetContainRect() const
{
	return m_containRect;
}

bool PgnPolygon::GetPointByIndex(int Index, Point2D& pt) const
{
	if (m_pPts.isEmpty() || Index >= m_pPts.size())
		return false;

	pt = m_pPts[Index];
	return true;
}

bool PgnPolygon::PtInPolygon(Point2D pt, float fAlt)
{
	BOOL b=PtInPolygonRayCasting(pt);
	//BOOL h=fAlt>m_Height && fAlt<m_Height;
	return b;
}

bool PgnPolygon::ContainPolygon(const PgnPolygon& cpgn)
{
	int i;
	int nCount = cpgn.m_pPts.size();
	Point2D pt;
	
	if (!m_containRect.IsCross(cpgn.GetContainRect())) { //包络盒必须相交
		return false;
	}

	for (i = 0; i < nCount  ; i++) {
		if( !cpgn.GetPointByIndex(i, pt) )
		{
			return false;
		}

		if ( !PtInPolygon(pt)) {
			return false;
		}
	}
	return true;
}

//反包含
bool PgnPolygon::AntiContainPolygon(PgnPolygon& cpgn) {
	return cpgn.ContainPolygon(*this);
}

//多边形是否相交，false不相交，true相交
bool PgnPolygon::IntersectPolygon(PgnPolygon& cpgn){
	int i,j;
	int k,l;
	Point2D pt,ptA,ptB;		
	int ptPgnCount = m_pPts.size();
	int ptCPgnCount = cpgn.m_pPts.size();
	
	if ( !m_containRect.IsCross(cpgn.GetContainRect())) { //包络盒必须相交
		return false;
	}
	
	//正包含点
	for (i = 0; i < ptCPgnCount  ; i++) {
		if( cpgn.GetPointByIndex(i, pt) )
		{
			if ( PtInPolygon(pt)) {
				return true;
			}
		}
	}	
	
	//反包含点
	for (i = 0; i < ptPgnCount  ; i++) {
		if ( cpgn.PtInPolygon(m_pPts[i])) {
			return true;
		}
	}
	
	//线段相交
	i = 0;
	j = ptPgnCount - 1;
	for ( ; i < ptPgnCount;) 
	{
		Line ln( m_pPts[i].x, m_pPts[i].y, m_pPts[j].x, m_pPts[j].y );
		
		k = 0;
		l = ptCPgnCount - 1;
		for (; k < ptCPgnCount ;) 
		{
			if ( ( cpgn.GetPointByIndex( k, ptA ) && cpgn.GetPointByIndex( l, ptB )) )
			{
				Line cln( ptA , ptB );
				
				if ( ln.IsLineSegmentCross( cln ) ){ //相交
					return true;
				}
			}		
			
			l = k;
			k++;
		}
		j = i;
		i++;
	}
	
	return false;
}

void PgnPolygon::Reset()
{
	if (m_pPts.size()>0)
	{
		//m_nPtsCount = 0;
		m_pPts.clear();		
	}
}	

void PgnPolygon::InitContainRect()
{
	int i;
	m_containRect.diagonal.pointA = m_pPts[0];
	m_containRect.diagonal.pointB = m_pPts[0];
	for( i = 0 ; i < m_pPts.size(); i ++)
	{
		if ( m_containRect.diagonal.pointA.x > m_pPts[i].x )
		{
			m_containRect.diagonal.pointA.x = m_pPts[i].x;
		}

		if ( m_containRect.diagonal.pointA.y > m_pPts[i].y )
		{
			m_containRect.diagonal.pointA.y = m_pPts[i].y;
		}

		if ( m_containRect.diagonal.pointB.x < m_pPts[i].x )
		{
			m_containRect.diagonal.pointB.x = m_pPts[i].x;
		}

		if ( m_containRect.diagonal.pointB.y < m_pPts[i].y )
		{
			m_containRect.diagonal.pointB.y = m_pPts[i].y;
		}
	}
}
//回转数法判断点是否在多边形内，实测慢
bool PgnPolygon::PtInPolygonWindingNumber(Point2D pt)
{
	double px = pt.x, py = pt.y;
	double sx,sy, tx,ty, angle;
	double sum = 0.0;
	int ptCount = m_pPts.size();
	int i = 0, j = ptCount - 1;
	
	for ( ; i < ptCount;)
	{
		sx = m_pPts[i].x;	sy = m_pPts[i].y;
		tx = m_pPts[j].x;	ty = m_pPts[j].y;
		
		// 点与多边形顶点重合或在多边形的边上
		if ((sx-px)*(px-tx) >= 0 && (sy-py)*(py-ty) >= 0 && (px-sx)*(ty-sy) == (py-sy)*(tx-sx)) {
			return true;
		}
		
		// 点与相邻顶点连线的夹角
		angle = atan2(sy-py, sx-px) - atan2(ty-py, tx-px);
		
		// 确保夹角不超出取值范围（-π 到 π）
		if (angle >= M_PI) {
			angle = angle - M_PI*2;
		} 
		else if (angle <= -(M_PI)) {
			angle = angle + M_PI*2;
		}
		
		sum += angle;
		j = i;
		i++;
	}
	
	// 计算回转数并判断点和多边形的几何关系
	//如果在外部值接近于0，大于0一般是内部
	return ( sum / M_PI ) > 0.001;//实验测试这个值没有发现问题，但不排除有问题
}
	
//射线法判断点是否在多边形内部,实测快
bool PgnPolygon::PtInPolygonRayCasting(Point2D pt){
	double px = pt.x;
	double py = pt.y;
	double sx;
	double sy;
	double tx;
	double ty;
	double x;
	bool flag = false;
	int ptCount = m_pPts.size();
	int i = 0;
	int j = ptCount - 1;
	
	for ( ;i < ptCount; ){
		sx = m_pPts[i].x;
		sy = m_pPts[i].y;
		tx = m_pPts[j].x;
		ty = m_pPts[j].y;
		
		// 点与多边形顶点重合
		if ((sx == px && sy == py) || (tx == px && ty == py)) {
			return true;
		}
		
		// 判断线段两端点是否在射线两侧
		if ((sy < py && ty >= py) || (sy >= py && ty < py)) {
			// 线段上与射线 Y 坐标相同的点的 X 坐标
			x = sx + (py-sy)*(tx-sx)/(ty-sy);
			
			// 点在多边形的边上
			if( fabs( x - px ) < 0.00000000001 ){//单位毫米
				return true;
			}
			
			// 射线穿过多边形的边界
			if ( x > px ){
				flag = !flag;
			}
		}
		j = i;
		i++;
	}
	
	// 射线穿过多边形边界的次数为奇数时点在多边形内
	return flag;
}

//////////////////////////////////////////////////////////////////////////
//Polygon
//////////////////////////////////////////////////////////////////////////
