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

#include "qgsAnnotationMarkerItem2.h"
#include <qgsAnnotationMarkerItem.h>

#include "global.h"
#include "FixPlaneThread.h"

using namespace std::chrono;

FixPlaneThread::FixPlaneThread()
{
}

void FixPlaneThread::run()
{
	QString mPath= QCoreApplication::applicationDirPath();
	setPriority(QThread::HighPriority);//-- > ��ߵ����ȼ�
	//qDebug() << ("FixPlane in run start: ") << m_plane.ID << endl;

	while (true)
	{		
		this->msleep(10);
	}
}

void FixPlaneThread::FixPlane(tag_PlaneMessage *p)//修改图层中飞机位置
{
	if (m_id != p->ID.toInt())
		return;

	// planeX 为空表示 msg199 模式帧，不含坐标，跳过几何更新
	if (p->planeX.isEmpty())
		return;

	// 每50ms最多更新一次（20fps），减少QGIS写锁频率，降低与渲染线程的锁竞争
	DWORD now = GetTickCount();
	if (now - m_lastUpdateTick < 50)
		return;
	m_lastUpdateTick = now;

	if (!g_pAirLayer)
		return;

	double x = p->planeX.toDouble();
	double y = p->planeY.toDouble();
	if (!g_pAirLayer->isEditable())
		g_pAirLayer->startEditing();

	// 首次调用时缓存feature id，后续直接用id取feature（O(1)），避免每次字符串过滤查询（O(N)）
	if (m_cachedFid == -1)
	{
		QgsFeatureIterator featureit = g_pAirLayer->getFeatures("\"electircid\"=" + p->ID);
		QgsFeature f;
		if (featureit.nextFeature(f))
			m_cachedFid = f.id();
		else
			return;
	}

	QgsFeature feature = g_pAirLayer->getFeature(m_cachedFid);
	if (feature.isValid())
	{
		QgsGeometry geometry = QgsGeometry::fromPointXY(QgsPointXY(x, y));
		feature.setGeometry(geometry);

		QgsAttributes attribues;
		attribues << p->ID << p->planeY << p->planeX << p->hZ << p->Yaw;
		feature.setAttributes(attribues);
		g_pAirLayer->updateFeature(feature);
		// triggerRepaint 已移除: 改由 MainWindow::m_pAirLayerTimer 每100ms统一刷新一次
	}
}



