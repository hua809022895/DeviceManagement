#if !defined(AFX_POLYGON_H__CCFE9149_5BA1_423F_90A2_4B32D13CC8D4__INCLUDED_)
#define AFX_POLYGON_H__CCFE9149_5BA1_423F_90A2_4B32D13CC8D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <MATH.H>

struct Point2D;

//Gps坐标点
struct PointGps  
{
	PointGps::PointGps();	
	PointGps::PointGps(double longitude, double latitude,double hieght);
	bool PointGps::operator == ( PointGps pt);
	double GetDistance(PointGps pt);		//获取两点之间的距离，单位毫米
	double Get3DDistance(PointGps pt);		//获取两点之间的3D距离，单位毫米	
	double GetBearing(PointGps pt);			//获取两点之间的角度，pt到当前点的向量角度°
	//Point2D GetPoint2D();					//通过默认的坐标源点转换为

	double lng;		//经度
	double lat;		//纬度
	double hit;		//高度
};

//射线坐标点结构
struct PointRay  
{
	PointRay();
	PointRay(double len, double ang);	
	//PointGps PointGpsByCenterAngle( PointGps ptCenter, double heading );//极坐标转换为gps坐标	
	Point2D Point2DByCenterAngle( Point2D ptCenter, double heading );	//极坐标转换为平面坐标

	double lenth; //长度，单位毫米
	double angle; //角度
};

//平面点结构
struct Point2D
{
	Point2D();
	Point2D(PointGps ptGps, PointGps ptCenter);	//给定原点，需要每次重新计算;
	Point2D(PointGps ptGps);					//采用默认原点，需要每次重新计算;
	Point2D(Point2D ptOrg, PointRay ptRay, double Angle);//通过原点，和相对位置，和当前的角度，计算计算当前的平面点;	
	bool operator == (Point2D pt);
	double x; //单位毫米
	double y; //单位毫米
};

//直线
struct Line 
{
	Line();
	Line( Point2D ptA, Point2D ptB );
	Line( double left, double top, double right, double bottom );	
	bool IsRectCross(const Line& cl);//排斥实验 看以线段为对角线的矩形是否相交	
	bool IsLineSegmentCross(const Line& cl);//跨立判断,是否相交	
	bool GetCrossPoint( const Line& cl, Point2D& pt );//求解两个线段的交点

	Point2D pointA;
	Point2D pointB;
};

//矩形
struct PgnRect
{
	PgnRect();
	PgnRect(double left, double top, double right, double bottom);
	void Merger( const PgnRect& rt );	//合并两个矩形，为一个包含两个矩形的矩形
	bool IsCross( const PgnRect& rt );	//必须是平面矩形才可使用，小区域的球面可以看作平面
	bool PtInRect( double x, double y );//点是否在矩形之内

	Line	diagonal;	//矩形对角线
	float	m_height;	//门框高度，单位m，gps模块测试出来的
};

//多边形
struct PgnPolygon
{
	PgnPolygon( bool bReInit = true );//是否允许后面，重新初始化存储.如果为false，后面只允许改变形状和位置，但是不允许改变顶点的个数,这样做主要是防止重新分配内存
	virtual ~PgnPolygon();
	//bool Init( Point2D* pPoints, int nPointCount );	//通过点集初始化多边形
	PgnRect GetContainRect() const;						//返回包络矩形
	bool GetPointByIndex(int Index, Point2D& pt) const;	//通过索引返回顶点

	bool PtInPolygon(Point2D pt,float fAlt=0);		//点是否在多边形内,参数fAlt为无人机高度信息，必须大于小车高度，小于小车高度+7米，才算在多边形内
	bool ContainPolygon(const PgnPolygon& cpgn );	//包含
	bool AntiContainPolygon( PgnPolygon& cpgn);		//反包含	
	bool IntersectPolygon( PgnPolygon& cpgn);		//多边形是否相交，返回false不相交，返回true表示相交

	QList	<Point2D>	m_pPts;//点集指针(不要出现相同的点，不然可能有问题)
	void InitContainRect();//初始化包络矩形
private:
	void Reset();//重置
	
	bool PtInPolygonWindingNumber(Point2D pt);	//回转数法判断点是否在多边形内，实测慢	
	bool PtInPolygonRayCasting(Point2D pt);		//射线法判断点是否在多边形内部,实测快

	float	m_Height;			//小车高度，单位米	
	PgnRect m_containRect;		//外接矩形，简化判别过程
	bool	m_bReInit;
};



#endif // !defined(AFX_POLYGON_H__CCFE9149_5BA1_423F_90A2_4B32D13CC8D4__INCLUDED_)
