#ifndef qudpTHREAD_H
#define qudpTHREAD_H

#include <QThread>
#include <QDateTime>
#include <qdebug.h>
#include <QUdpSocket>
#include "comm.h"

class qUdpThread :public QThread
{
    Q_OBJECT
public:	
	qUdpThread();
	QUdpSocket	*m_pUdp =nullptr;			//请求mavlink协议无人机个数udp套接字	
	uint8_t		*m_buf  =nullptr;			//udp网口接收缓冲区
	DWORD		m_ioffset=0;				//当前处理位置
	QMutex		m_Mutex;					//互斥对象，用于保护m_buf，和m_ioffset
signals:
    //void receviePlaneMsg(tag_PlaneMessage *);	//注册发送无人机消息，信号，给主窗口的registerPlane槽函数    
public:
	void run();
};

#endif // DDSTHREAD_H




