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
#include <QList>

#include <qgsMapCanvas.h>
#include "global.h"
#include "tuiliThread.h"

using namespace std::chrono;

TuiliThread::TuiliThread()
{
	//m_pCanvas = _canvas;
}

void TuiliThread::run()
{
	QString mPath= QCoreApplication::applicationDirPath();
	//setPriority(QThread::HighPriority);//-- > 最高的优先级
	//qDebug() << ("FixPlane in run start: ") << 111 << endl;	

	connect(this, &QThread::finished, this, &QObject::deleteLater);

	while (true)
	{
		this->msleep(200);

		if (!m_isRun)
			continue;

		//判断是否为集群起飞阶段
		//qDebug() << ("TuiliThread in run start: ") << 111 << endl;
		jqStart();		//1判断集群起飞
		jqAirjijie();	//2空中集结
		jqYouerqp();	//3表示诱饵欺骗
		jqdikong();		//4表示低空突防
	}
}

void TuiliThread::jqStart()//判断集群起飞
{
	if (m_planeVec.size() < m_iPlaneCount)
		return;

	if (m_iJQactive == 1) //如果已经是集群起飞了，就返回
		return;

	double h = 0;
	m_Mutex.lock();
	foreach(tag_PlaneMessage  r, m_planeVec) {
		if (m_planeVec.size() < m_iPlaneCount)
			break;
		h += r.xZ.toDouble();
	}
	m_Mutex.unlock();

	if (!m_is100Down)
	{
		if (h / m_iPlaneCount < 20)
		{
			m_is100Down = true;			
		}
	}

	if (m_is100Down && !m_bStart100)
	{
		if (h / m_iPlaneCount >20)
		{
			m_bStart100 = true;
			m_start99 = steady_clock::now(); // 获取开始时间点
		}
	}

	if (h / m_iPlaneCount > 90)
		m_is2000up = true;

	if (m_is100Down && m_is2000up && m_bStart100)
	{
		auto end = steady_clock::now(); // 获取结束时间点
		auto dur = duration_cast<seconds>(end - m_start99).count(); // 计算持续时间秒

		if (dur >= 10)//集群起飞时间持续10秒以上，即可
		{
			m_iJQactive =1;
			m_activeTime = steady_clock::now(); // 获取开始时间点
		}		
	}
}

void TuiliThread::jqAirjijie() //空中集结
{
	if (m_planeVec.size() < m_iPlaneCount)
		return;

	if(m_iJQactive == 2)
		return;

	//求中心点坐标
	double x = 0, y = 0; 
	QList<float> disList; //所有点到中心点的距离列表

	m_Mutex.lock();
	foreach(tag_PlaneMessage  r, m_planeVec) {		
		x += r.planeX.toDouble();
		y += r.planeY.toDouble();
	}
	x = x / m_iPlaneCount;
	y = y / m_iPlaneCount;

	QgsPoint cpt(x, y); //中心点坐标	
	foreach(tag_PlaneMessage  r, m_planeVec) {
		x = r.planeX.toDouble();
		y = r.planeY.toDouble();

		QgsPoint p(x, y);
		float dist = GetDistance(cpt, p);
		disList.append(dist);
	}
	m_Mutex.unlock();
	qSort(disList.begin(), disList.end());

	//如果当前最大点减去最小点距离小于5米，就说明是围绕一个点绕圈
	if ((disList[m_iPlaneCount - 1] - disList[0]) < 5)
	{
		m_iJQactive = 2;
		m_activeTime = steady_clock::now(); // 获取开始时间点
	}

}

void TuiliThread::jqYouerqp()	//3表示诱饵欺骗
{

}

void TuiliThread::jqdikong()	// 4表示低空突防
{
	if (m_planeVec.size() < m_iPlaneCount)
		return;

	if (m_iJQactive == 4) //如果已经是低空突防了，就返回
		return;

	if (!m_b2000mi)
	{
		foreach(tag_PlaneMessage  r, m_planeVec) {
			if (r.xZ < 1950)
				return;
		}
		m_b2000mi = true;
	}

	if (!m_bDown500)
	{
		foreach(tag_PlaneMessage  r, m_planeVec) {
			if (r.xZ >500)
				return;
		}
		m_bDown500 = true;
	}

	//1，飞机全部降高
	if (m_b2000mi && m_bDown500)
	{
		if (!m_bStart)
		{
			m_start	= steady_clock::now(); // 获取开始时间点
			m_bStart= true;
		}
		else
		{
			auto end = steady_clock::now(); // 获取结束时间点
			auto dur = duration_cast<seconds>(end - m_start).count(); // 计算持续时间秒

			if (dur >= 15)//低空突防时间持续15秒以上，即可
			{
				m_iJQactive = 4;
				m_activeTime = steady_clock::now(); // 获取开始时间点
			}
		}
	}
}



