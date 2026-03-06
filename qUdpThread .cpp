#include "stdafx.h"
#include <QThread>
#include <QDateTime>
#include <qdebug.h>
//#include <Python.h>
#include <string>
#include <vector>
#include <QUdpSocket>

#include "comm.h"
#include "qUdpThread.h"

qUdpThread::qUdpThread()
{
	m_buf = new byte[BUFFER_LENGTH];
}

//uint32_t	g_tbTime = 0;
void qUdpThread::run()
{		
	QString mPath= QCoreApplication::applicationDirPath();
	mPath = QCoreApplication::applicationDirPath();

	m_ioffset = 0;
	//如果还没有创建这个socket就创建一个
	if (m_pUdp == nullptr)
	{
		m_pUdp = new QUdpSocket(this);
		m_pUdp->bind(QHostAddress(localIP), localPlanePort.toInt());

		//设置接收缓冲区的大小为64KB
		//m_pUdp->setReadBufferSize(BUFFER_LENGTH);
		//connect(m_pUdpPlaneNumber, SIGNAL(readyRead()), this, SLOT(processPendingDatagram()));//设置udp协议回调函数
	}

	while (true)
	{
		//循环判断是否接收到了udp数据
		if (m_pUdp->hasPendingDatagrams())
		{
			//initTick = GetTickCount();
			//array.resize(m_pUdpPlaneNumber->pendingDatagramSize());//读取udp协议接收到的udp数据
			int recsize = m_pUdp->pendingDatagramSize();			//包大小=41,有效数据包大小33						
			//
			uint8_t buf[1024] = {0};//	memset(buf, 0, BUFFER_LENGTH);

			m_pUdp->readDatagram((char*)buf, recsize);

			m_Mutex.lock();
			for (int i = 0; i < recsize; i++)
			{
				m_buf[m_ioffset] = buf[i];
				m_ioffset++;
			}

			if (m_ioffset > (BUFFER_LENGTH - 500))
				m_ioffset = 0;
			m_Mutex.unlock();
			qDebug() << "qUdpThread run m_ioffset= " << m_ioffset << endl;
		}
		this->msleep(10);
	}
}
