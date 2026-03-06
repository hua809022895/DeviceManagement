#ifndef FlineTHREAD_H
#define FlineTHREAD_H

#include <QThread>
#include <QDateTime>
#include <qdebug.h>
#include "comm.h"

class QgsMapCanvas;
class QgsAnnotationMarkerItem;
class QLineGuiji;
class QgsLineAnnotation;
//class QgsAnnotation2;

//无人机轨迹线类
class LineThread:public QThread
{
    Q_OBJECT
public:
	LineThread(QgsMapCanvas* _canvas);
	QgsMapCanvas				*m_pCanvas=nullptr;
	int							m_id		=-1;		//无人机id
	tag_PlaneMessage			m_plane;
	//QVector<QLineGuiji*>		m_ptList;	//全部线段列表	
	//QLineGuiji					*m_gjLine;	//轨迹对象
	QgsLineAnnotation			*m_pA = nullptr;
signals:
	void addPlaneMsg(tag_PlaneMessage *);	//修改无人机位置信号
public:
	void run();

public slots://定义信号槽,回调函数
	void addPoint(tag_PlaneMessage *planeMessage);//添加飞机位置点
};

#endif //




