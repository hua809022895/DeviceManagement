#include "stdafx.h"

#include <qgis.h>
#include <QgsProject.h>
#include <qgsApplication.h>  
#include <qgsRectangle.h>
#include <QgsPoint.h>
#include <QgsProviderRegistry.h>  
#include <Qpolygon.h>
#include "global.h"
#include "Polygon.h"

QString g_sTestID="";//需要隐藏的图层id

QgsPointXY wgs4326To3857(QgsPointXY p) {

	double earthRad = 6378137.0;
	double x = p.x() * M_PI / 180 * earthRad;
	double a = p.y() * M_PI / 180;
	double y = earthRad / 2 * log((1.0 + sin(a)) / (1.0 - sin(a)));

	QgsPointXY r(x, y);
	return r;
}

QgsPointXY wgs3857To4326(QgsPointXY p)
{
	double x = p.x() / 20037508.34 * 180;
	double y = p.y() / 20037508.34 * 180;
	y = 180 / M_PI * (2 * atan(exp(y * M_PI / 180)) - M_PI / 2);

	QgsPointXY r(x, y);
	return r;
}

//返回两个点之间的距离，//单位米
double GetDistance(QgsPoint pt1, QgsPoint pt2)
{	
	double x = (pt1.x() -pt2.x()) * MM_PER_LONGITUDE_ZJR(pt1.y());
	double y = (pt1.y() -pt2.y()) * MM_PER_LATITUDE;
	double dis = sqrt(x*x + y*y);
	return dis /1000.0;
}

//根据图元当前的角度，要移动的距离和原坐标点，返回图元新坐标点信息
QgsPoint GetResult(double Longitude, double Latitude, double dAngle, double pitch, double nLen)
{
	double zWeidu = dAngle * M_PI / 180.0;
	nLen=  nLen* cos(pitch  * M_PI_PRE_DEGREE);
	double dx = (Longitude+ (nLen *sin(zWeidu)) / MM_PER_LONGITUDE_BYLAT(Latitude));	//经度		
	double dy = (Latitude + (nLen *cos(zWeidu)) / MM_PER_LATITUDE);						//纬度

	QgsPoint rst(dx, dy);
	return rst;
}

//返回两个点的角度
double GetBearing(QgsPoint pt1, QgsPoint pt2)
{//°
	double x = (pt1.x() -pt2.x()) * MM_PER_LONGITUDE_ZJR(pt1.y());
	double y = (pt1.y() -pt2.y()) * MM_PER_LATITUDE;
	double bearing = atan2(y, x) / M_PI_PRE_DEGREE;
	return bearing;
}

void sleep(unsigned int msec) 
{ 
	QTime dieTime = QTime::currentTime().addMSecs(msec); 
	while( QTime::currentTime()<dieTime) 
		QCoreApplication::processEvents(QEventLoop::AllEvents, 20); 
}

//得到当前时间相对于1980年1月6日经过的时间周剩余秒数
std::string GetTimeSecond(DWORD iWeek,UINT nWeekofSecond)
{
	UINT nSecondpreweek = 7*24*60*60;//7天
	QString strBuffer = "1980-01-06 00:00:00";
	QDateTime gpsTime = QDateTime::fromString(strBuffer, "yyyy-MM-dd hh:mm:ss");

	time_t tmt =gpsTime.toTime_t();
	tmt += iWeek * nSecondpreweek + nWeekofSecond ;//tmt -= m_iLeapSecond ;//去掉润秒
	tmt += 8* 60 *60;	//换算成本地时间

	QDateTime rst;
	rst.setTime_t(tmt);

	std::string str;// = rst.toString("yyyy-MM-dd").toAscii().constData();
	return str;
}

std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
	std::vector<std::string> tokens;
	size_t start = 0, end = 0;
	while ((end = str.find(delimiter, start)) != std::string::npos) {
		tokens.push_back(str.substr(start, end - start));
		start = end + delimiter.length();
	}
	tokens.push_back(str.substr(start));
	return tokens;
}

//取多边形外围顶点
/*std::vector<Point> convexHull(std::vector<Point> points) 
{
	if (points.size() <= 1) return points;

	sort(points.begin(), points.end()); //排序
	std::vector<Point> lower, upper;

	for (int i = 0; i < points.size(); i++) {
		while (lower.size() >= 2 && cross(lower[lower.size() - 2], lower[lower.size() - 1], points[i]) <= 0) {
			lower.pop_back();
		}

		while (upper.size() >= 2 && cross(upper[upper.size() - 2], upper[upper.size() - 1], points[i]) >= 0) {
			upper.pop_back();
		}
		lower.push_back(points[i]);
		upper.push_back(points[i]);
	}

	lower.pop_back(); // Remove duplicates
	//upper.pop_back(); // Remove duplicates

	lower.insert(lower.end(), upper.rbegin(), upper.rend()); // Merge lower and upper hulls, skipping duplicates
	
	std::vector<Point>::iterator pbeing= lower.begin();
	std::vector<Point>::iterator pend  = lower.end();
	std::vector<Point>::iterator unList=unique(pbeing, pend);

	lower.erase(unList, pend); // Remove 指定范围的元素
	return lower;
}
// 向量叉积
double cross(const Point O, const Point A, const Point B) {
	return (A.x - O.x) * (B.y - O.y) - (A.y - O.y) * (B.x - O.x);
}*/

// 将扇形转换为多边形近似
cl::PathsD sector_to_polygon(const Sector& sector, int segments) 
{
	cl::PathsD	result;
	cl::PathD	path;

	// 添加圆心
	path.push_back(cl::PointD(static_cast<double>(sector.center.x),static_cast<double>(sector.center.y)));

	// 添加弧线上的点
	double angle_step = (sector.end_angle - sector.start_angle) / segments;
	for (int i = 0; i <= segments; ++i) {
		double angle = sector.start_angle + i * angle_step;
		double x = sector.center.x + sector.radius * cos(angle);
		double y = sector.center.y + sector.radius * sin(angle);
		path.push_back(cl::PointD(static_cast<double>(x),static_cast<double>(y)));
	}

	result.push_back(path);
	return result;
}

//将扇形转换为多边形近似
cl::PathsD sector_to_polygon(QList<QgsPointXY> Set)		
{
	cl::PathsD	result;
	cl::PathD	path;

	foreach(QgsPointXY p, Set) {
		path.push_back(cl::PointD(p.x(), p.y()));
	}

	result.push_back(path);
	return result;
}

//将矩形转换为多边形
cl::PathsD rectangle_to_polygon(QPolygonF p)
{
	cl::PathsD result;
	cl::PathD path;

	cl::PointD p1(p[0].x(), p[0].y());	
	cl::PointD p2(p[1].x(), p[1].y());
	cl::PointD p3(p[2].x(), p[2].y());
	cl::PointD p4(p[3].x(), p[3].y());

	path.push_back(p1);	
	path.push_back(p2);
	path.push_back(p3);
	path.push_back(p4);
	
	result.push_back(path);
	return result;
}

//计算多边形面积
double calculate_area(const cl::PathsD & paths) 
{
	double total_area = 0;
	for (const auto& path : paths) {
		total_area += Area(path);
	}
	return total_area;// / 1000000.0; // 除以1000*1000，因为我们之前乘以了1000
}
