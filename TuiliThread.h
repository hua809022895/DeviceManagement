#ifndef FlineTHREAD_H
#define FlineTHREAD_H

#include <QThread>
#include <QDateTime>
#include <qdebug.h>
#include <chrono>
#include "comm.h"

class QgsMapCanvas;
class QgsAnnotationMarkerItem;

using namespace std::chrono;
//无人机集群推理,线程类
class TuiliThread:public QThread
{
    Q_OBJECT
public:
	TuiliThread();
	
	int		m_iJQactive=0;	//集群目前动作，初始值0表示没有做任何动作，1表示集群起飞，2表示绕着一个点盘旋为空中集结
	std::chrono::steady_clock::time_point m_activeTime;//集群动作时间，超过11秒钟还是同一个动作，就设置为3诱饵欺骗
	//3表示诱饵欺骗，4表示低空突防
	bool	m_isRun = false;//标记是否在运行
	//tag_PlaneMessage			m_plane;
	//QVector<QLineGuiji*>		m_ptList;	//全部线段列表	
	QVector<tag_PlaneMessage>   m_planeVec;
	QMutex                      m_Mutex;
	int		m_iPlaneCount =1;	//集群无人机个数，限定值
	//1,集群起飞逻辑，用变量
	std::chrono::steady_clock::time_point  m_start99;//都在99米以上开始计时
	bool	m_bStart100 = false;	//都在100米以上开始计时了，初始值false表示没有计时
	bool	m_is100Down=false;		//是否都在100米以下
	bool	m_is2000up = false;		//是否都在2000米以上，

	//4表示低空突防,用变量
	bool	m_b2000mi = false;	//是否都在2000米左右
	bool	m_bDown500 = false;	//是否都降低了500米，
	bool	m_bStart = false;	//降高后是否开始计时了，初始值false表示没有计时
	std::chrono::steady_clock::time_point  m_start;
signals:
	void addPlaneMsg(tag_PlaneMessage *);	//修改无人机位置信号
public:
	void run();

	void jqStart();		//1，判断集群起飞
	void jqAirjijie();	//2，空中集结
	void jqYouerqp();	//3，表示诱饵欺骗
	void jqdikong();	//4，表示低空突防
public slots://定义信号槽,回调函数
	//void addPoint(tag_PlaneMessage *planeMessage);//添加飞机位置点
};

#endif //




