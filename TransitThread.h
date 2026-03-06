#ifndef transitTHREAD_H
#define transitTHREAD_H

#include <QThread>
#include "comm.h"
#include "./mainwindow.h"
#include <QDateTime>
#include <qdebug.h>

class TransitThread:public QThread
{
    Q_OBJECT
public:
	TransitThread();
	QSqlQuery		*m_pSqlQuery = nullptr;
	volatile bool	m_stopped    = false;	// 设为true可干净退出run()循环
signals:
	void receviePlaneMsg(tag_PlaneMessage *);	//ע�ᷢ�����˻���Ϣ���źţ��������ڵ�registerPlane�ۺ���    
    void receviePictureMessage(double leftupY, double leftupX,
                                   double rightupY, double rightupX,
                                   double leftdownY, double leftdownX,
                                   double rightdownY, double rightdownX, QString filename);
    void recevieLittlePictureMessage(const QByteArray);
public:
	void run();
};

#endif // DDSTHREAD_H




