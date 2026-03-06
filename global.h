#ifndef Qglobal_H
#define Qglobal_H

#include <Windows.h>
#include <MATH.h>
#include <random>
#include <cmath>
#include <algorithm>

#include <QMessageBox>
#include <QListWidgetItem>
#include <QTreeView>
#include <QDockWidget>
#include <QIcon>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QDialog>
#include <QFileDialog.h>
#include <Qevent.h>
#include <QWidget>
#include <QMainWindow>
#include <QDialog>
#include <QMap>
#include <QString>
#include <QSettings>
#include <QcoreApplication>
#include <Qfile>
#include <QMessageBox>
#include <QTextStream>
#include <Qdebug>
#include <Qnamespace.h>
#include <QMenu>
#include <QMATH.h>


#include "qgsMaptoPixel.h"
#include <qgis.h>
#include <QgsProject.h>
#include <qgsApplication.h>  
#include <qgsRectangle.h>
#include <QgsPoint.h>
#include <qgsProviderRegistry.h>  
#include "qgsmapCanvas.h"  

#include <qgsfeature.h> //we will need to pass a bunch of these for each rec
#include <qgsfield.h>
#include <qgsRubberBand.h>

#include "qgsVectorLayer.h"
#include <qgsVectorDataProvider.h>
#include <qgsLogger.h>
#include <qgsCoordinateReferenceSystem.h>

#include "clipper2/include/clipper.h"

namespace cl=Clipper2Lib;

//角度转弧度
#define DEG_TO_RAD(ang) ((ang)*M_PI/180.0)

//弧度转角度
#define RAD_TO_DEG(rad) ((rad)*180.0/M_PI)

//取得x~y之间任意一个值
#define RAND_RANGE(x,y) ((x) + rand() % ((y) - (x) + 1)))

#define EARTH_RADIUS	6371004000.0 // According to Wikipedia, the Earth's radius is about 6,371km 地球半径，单位=毫米												
#define M_PI_PRE_DEGREE (M_PI/180.0)				//每度对应的弧度

#define MM_PER_LATITUDE ( EARTH_RADIUS * M_PI_PRE_DEGREE )										//一度纬度对应的毫米数
#define MM_PER_LONGITUDE_EQUATORIAL ( EARTH_RADIUS * M_PI_PRE_DEGREE ) 							//赤道上一度经度对应的毫米数
#define MM_PER_LONGITUDE_BYLAT(A) (MM_PER_LONGITUDE_EQUATORIAL * cos( (A)* M_PI_PRE_DEGREE))	//一度经度对应的毫米数
#define LATITUDE_PER_MM (1.0/MM_PER_LATITUDE)													//一毫米对应的纬度
//每1米对应0.00001度
#define MM_PER_LONGITUDE_ZJR(A) (MM_PER_LONGITUDE_EQUATORIAL * cos( (A)* M_PI / 180.0))			//一度经度对应的毫米数，参数A表示纬度
//

struct CarInfo//
{
	double Length;    //x长度数据
	double angle;     //y角度数据
};  //车点位置信息x,y数据结构，单位（米）

struct ThreeObj
{
	double                  m_x;       //经度
	double                  m_y;       //纬度
	double                  m_a;       //角度
	ThreeObj()
	{
		m_x=m_y=m_a=0;
	}
};  //经纬度，角度类型结构

QgsPoint GetResult(double Longitude, double Latitude, double dAngle, double pitch,double nLen);
double	GetDistance(QgsPoint pt1, QgsPoint pt2);
double	GetBearing(QgsPoint pt1, QgsPoint pt2);
void	sleep(unsigned int msec) ;

//得到当前时间相对于1980年1月6日经过的时间周剩余秒数
std::string GetTimeSecond(DWORD iWeek,UINT nWeekofSecond);
std::vector<std::string> split(const std::string& str, const std::string& delimiter);

//点结构
struct Point {
	double x, y;
	Point(double _x, double _y) : x(_x), y(_y) {}

	bool operator == (Point pt) {
		return pt.x == x && pt.y == y;
	}
	bool operator<(const Point &p) const {
		return x < p.x || (x == p.x && y < p.y);
	}
};

//扇形结构
struct Sector {
	cl::PointD center;	//扇形原点，中心点坐标
	double radius;		//扇形半径，单位米
	double start_angle;	//起始角度
	double end_angle;	//结束角度
};

// 定义多个扇形
/*vector<Sector> sectors = {
	{{2, 2}, 3, 0, M_PI / 2},          // 圆心(2,2)，半径3，0-90度
	{{7, 7}, 4, M_PI / 2, M_PI},       // 圆心(7,7)，半径4，90-180度
	{{5, 3}, 2.5, M_PI, 3 * M_PI / 2}  // 圆心(5,3)，半径2.5，180-270度
};*/

//double cross(const Point O, const Point A, const Point B);
//std::vector<Point> convexHull(std::vector<Point> points);
/*总共有四种裁剪运算类型 AND, OR, NOT和XOR
他们的类型主要取决于他们的点集信息和填充规则，规则如下：
AND(Intersection求交) 获取两者相交的部分；
OR(Union求并) 获取两者并集部分；
NOT(difference求异) 获取Clip区域以外的区域；
XOR(exclusive求异或) 获取两个区域互相不重复的区域；*/
cl::PathsD sector_to_polygon(const Sector& sector, int segments = 36);		//将扇形转换为多边形近似
cl::PathsD sector_to_polygon(QList<QgsPointXY> Set);		//将扇形转换为多边形近似
cl::PathsD rectangle_to_polygon(QPolygonF p);				//将矩形转换为多边形
double calculate_area(const cl::PathsD & paths);			//计算多边形面积

QgsPointXY wgs4326To3857(QgsPointXY p);
QgsPointXY wgs3857To4326(QgsPointXY p);
#endif // QDLGRULER_H