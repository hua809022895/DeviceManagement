#ifndef recMavlinkTHREAD_H
#define recMavlinkTHREAD_H

#include <QThread>
#include <QDateTime>
#include <qdebug.h>
#include "qUdpThread.h"
#include "mainWindow.h"

class RecMavlinkThread :public QThread
{
    Q_OBJECT
public:
	//QUdpSocket	*m_pUdpPlaneNumber = nullptr;	//����mavlinkЭ�����˻�����udp�׽���
	MainWindow	*m_pDlg=nullptr;				//������ָ��
	RecMavlinkThread();
	~RecMavlinkThread()
	{
		// terminate() 强杀时 run() 来不及 close，析构函数兜底
		if (m_pUdp) { m_pUdp->close(); delete m_pUdp; m_pUdp = nullptr; }
	}
	bool			m_paused  = false;//�߳��Ƿ�����ִͣ�У�false��ʾû����ͣ��true��ʾ��ͣ��
	volatile bool	m_stopped = false;
	QUdpSocket		*m_pUdp = nullptr;			//����mavlinkЭ�����˻�����udp�׽���	
	//DWORD DoData(uint8_t *, DWORD iCount);
signals:
    void receviePlaneMsg(tag_PlaneMessage *);	//ע�ᷢ�����˻���Ϣ���źţ��������ڵ�registerPlane�ۺ���    
public:
	void run();
};

#endif // DDSTHREAD_H




