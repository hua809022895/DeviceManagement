#include "stdafx.h"
#include <QThread>
#include "comm.h"
#include <QDateTime>
#include <qdebug.h>
//#include <Python.h>
#include <string>
#include <vector>
#include <chrono>
#include <thread>
#include <algorithm>

#include <qgsMapCanvas.h>
#include <qgsAnnotationManager.h>
#include <qgsAnnotationMarkerItem.h>
#include "qgsMapCanvasAnnotationitem2.h"

#include "global.h"
#include "LineThread.h"
#include "qgsLineAnnotation.h"
#include "qgsAnnotation2.h"

using namespace std::chrono;

LineThread::LineThread(QgsMapCanvas* _canvas)
{
	m_pCanvas = _canvas;
}

void LineThread::run()
{
	QString mPath= QCoreApplication::applicationDirPath();
	setPriority(QThread::HighPriority);//-- > 最高的优先级
	qDebug() << ("FixPlane in run start: ") << m_plane.ID << endl;	
	while (true)
	{
		this->msleep(10);
	}
}

void LineThread::addPoint(tag_PlaneMessage *p)//修改图层中飞机位置
{
	if (m_id != p->ID.toInt())
		return;

	//m_pCar->setString("ring");
	double x = p->planeX.toDouble();
	double y = p->planeY.toDouble();

	m_pA->m_PtList.append(QgsPointXY(x, y));
	
	//qDebug() << ("FixPlane in m_plane: ") << m_plane.ID <<endl;
	// 使用 Qt 的 foreach 宏遍历
	/*foreach(const int &key, g_mapAnnoLayer.keys()) {	
		if (key == p->ID.toInt())
		{
			qDebug() << "key in:" << key << ":" << g_mapAnnoLayer.value(key);
			QgsAnnotationLayer* pLayer = g_mapAnnoLayer.value(key);
			pLayer->removeItem(p->ID);
			pLayer->addItem(pMarkerItem);
			break;
		}
	}*/
}



