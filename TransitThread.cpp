#include "stdafx.h"
#include <QThread>
#include "comm.h"
#include <QDateTime>
#include <qdebug.h>
#include "TransitThread.h"
//#include <Python.h>
#include <string>
#include <vector>
#include "global.h"

using namespace std;

TransitThread::TransitThread()
{
}

void TransitThread::run()
{
	setPriority(QThread::HighestPriority);
	while (!m_stopped)
	{
		// 先加锁再检查，消除 isEmpty() 在锁外的 TOCTOU 竞态
		g_transitMutex.lock();
		if (!g_transitList.isEmpty())
		{
			tag_PlaneMessage pm = g_transitList.first();	// 锁内拷贝
			g_transitList.removeFirst();
			g_transitMutex.unlock();

			tag_PlaneMessage *p = new tag_PlaneMessage(pm);
			emit receviePlaneMsg(p);	// 主线程 registerPlane() 负责 delete p
		}
		else
		{
			g_transitMutex.unlock();
			this->msleep(5);	// 队列空才休眠，有数据时不停歇
		}
	}
}




