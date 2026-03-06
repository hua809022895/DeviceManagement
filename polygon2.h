#pragma once

#include <iostream>
#include <MATH.H>
#include <qgsSettings.h>
#include <qgsGui.h>
#include <qgsProject.h>

using namespace std;

//////////////////////////点集排序////////////////////////
//若点a大于点b,即点a在点b顺时针方向,返回true,否则返回false
bool PointCompare(QgsPointXY a, QgsPointXY b, QgsPointXY center)
{
	if (a.x() >= 0 && b.x()< 0)
		return true;
	if (a.x() == 0 && b.x() == 0)
		return a.y() > b.y();

	//向量OA和向量OB的叉积
	int det = (a.x() - center.x()) * (b.y() - center.y()) - (b.x() - center.x()) * (a.y() - center.y());
	if (det < 0)
		return true;
	if (det > 0)
		return false;
	//向量OA和向量OB共线，以距离判断大小
	int d1 = (a.x() - center.x()) * (a.x() - center.x()) + (a.y() - center.y()) * (a.y() - center.y());
	int d2 = (b.x() - center.x()) * (b.x() - center.y()) + (b.y() - center.y()) * (b.y() - center.y());

	return d1 > d2;
}
// 顺时针方向排序
void ClockwiseSortPoints(std::vector<QgsPointXY> &vPoints)
{
	//计算重心
	QgsPointXY center;
	int count_size = vPoints.size();
	double x = 0, y = 0;
	for (int i = 0; i < count_size; i++)
	{
		x += vPoints[i].x();
		y += vPoints[i].y();
	}

	center.setX((int)x / count_size);
	center.setY((int)y / count_size);

	//冒泡排序
	for (int i = 0; i < count_size - 1; i++)
	{
		for (int j = 0; j < count_size - i - 1; j++)
		{
			if (PointCompare(vPoints[j], vPoints[j + 1], center))
			{
				QgsPointXY tmp = vPoints[j];
				vPoints[j] = vPoints[j + 1];
				vPoints[j + 1] = tmp;
			}
		}
	}
	return;
}

//////////////////////////判断点是否在多边形内部/////////////////////////////
bool IsPointInPolygon(const std::vector<QgsPointXY> poly, QgsPointXY pt)
{
	int i, j;
	bool c = false;
	int count = poly.size();
	for (i = 0, j = count - 1; i < count; j = i++)
	{
		if ((((poly[i].y() <= pt.y()) && (pt.y() < poly[j].y())) ||
			((poly[j].y() <= pt.y()) && (pt.y() < poly[i].y())))
			&& (pt.x() < (poly[j].x() - poly[i].x()) * (pt.y() - poly[i].y()) / (poly[j].y() - poly[i].y()) + poly[i].x()))
		{
			c = !c;
		}
	}
	return c;
}

///////////////////////////////线段相交判断//////////////////////////////////////////
//排斥实验
bool IsRectCross(QgsPointXY &p1, QgsPointXY &p2, QgsPointXY &q1, QgsPointXY &q2)
{
	bool ret = min(p1.x(), p2.x()) <= max(q1.x(), q2.x()) &&
		min(q1.x(), q2.x()) <= max(p1.x(), p2.x()) &&
		min(p1.y(), p2.y()) <= max(q1.y(), q2.y()) &&
		min(q1.y(), q2.y()) <= max(p1.y(), p2.y());

	return ret;
}
//跨立判断
bool IsLineSegmentCross(QgsPointXY &pFirst1, QgsPointXY &pFirst2, QgsPointXY &pSecond1, QgsPointXY &pSecond2)
{
	long line1, line2;
	line1 = pFirst1.x() * (pSecond1.y() - pFirst2.y()) +
		pFirst2.x() * (pFirst1.y() - pSecond1.y()) +
		pSecond1.x() * (pFirst2.y() - pFirst1.y());
	line2 = pFirst1.x() * (pSecond2.y() - pFirst2.y()) +
		pFirst2.x() * (pFirst1.y() - pSecond2.y()) +
		pSecond2.x() * (pFirst2.y() - pFirst1.y());
	if (((line1 ^ line2) >= 0) && !(line1 == 0 && line2 == 0))
		return false;

	line1 = pSecond1.x() * (pFirst1.y() - pSecond2.y()) +
		pSecond2.x() * (pSecond1.y() - pFirst1.y()) +
		pFirst1.x() * (pSecond2.y() - pSecond1.y());
	line2 = pSecond1.x() * (pFirst2.y() - pSecond2.y()) +
		pSecond2.x() * (pSecond1.y() - pFirst2.y()) +
		pFirst2.x() * (pSecond2.y() - pSecond1.y());
	
	if (((line1 ^ line2) >= 0) && !(line1 == 0 && line2 == 0))
		return false;

	return true;
}

bool GetCrossPoint(QgsPointXY &p1, QgsPointXY &p2, QgsPointXY &q1, QgsPointXY &q2, double &x, double &y)
{
	if (IsRectCross(p1, p2, q1, q2))
	{
		if (IsLineSegmentCross(p1, p2, q1, q2))
		{
			//求交点
			long tmpLeft, tmpRight;
			tmpLeft = (q2.x() - q1.x()) * (p1.y() - p2.y()) - (p2.x() - p1.x()) * (q1.y() - q2.y());
			tmpRight =(p1.y() - q1.y()) * (p2.x() - p1.x()) * (q2.x() - q1.x()) + q1.x() * (q2.y() - q1.y()) * (p2.x()- p1.x()) - p1.x()* (p2.y() - p1.y()) * (q2.x() - q1.x());

			x = (int)((double)tmpRight / (double)tmpLeft);

			tmpLeft =(p1.x() - p2.x()) * (q2.y() - q1.y()) - (p2.y() - p1.y()) * (q1.x() - q2.x());
			tmpRight=p2.y() * (p1.x() - p2.x()) * (q2.y() - q1.y()) + (q2.x() - p2.x()) * (q2.y() - q1.y()) * (p1.y() - p2.y()) -q2.y() * (q1.x()- q2.x()) * (p2.y() - p1.y());
			y = (int)((double)tmpRight / (double)tmpLeft);
			return true;
		}
	}
	return false;
}

//////////////////////////线段相交结束/////////////////////
//////////////////////////多边形交集////////////////////////
bool PolygonClip(std::vector<QgsPointXY> &poly1, std::vector<QgsPointXY> &poly2, std::vector<QgsPointXY> &interPoly)
{
	if (poly1.size() < 3 || poly2.size() < 3)
	{
		return false;
	}

	double x=0, y=0;
	//计算多边形交点
	int count1 = poly1.size();
	int count2 = poly2.size();
	for (int i = 0; i < count1; i++)
	{
		int poly1_next_idx = (i + 1) % count1;
		for (int j = 0; j < count2; j++)
		{
			int poly2_next_idx = (j + 1) % count2;

			if (GetCrossPoint(poly1[i], poly1[poly1_next_idx],	poly2[j], poly2[poly2_next_idx], x, y))
			{
				interPoly.push_back(QgsPointXY(x, y));
			}
		}
	}

	//计算多边形内部点
	for (int i = 0; i < count1; i++)
	{
		if (IsPointInPolygon(poly2, poly1[i]))
		{
			interPoly.push_back(poly1[i]);
		}
	}
	for (int i = 0; i < count2; i++)
	{
		if (IsPointInPolygon(poly1, poly2[i]))
		{
			interPoly.push_back(poly2[i]);
		}
	}

	if (interPoly.size() <= 0)
		return false;

	//点集排序
	ClockwiseSortPoints(interPoly);

	return true;
}

