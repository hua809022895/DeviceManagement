#pragma once
#include <QThread>
#include <QUdpSocket>
#include "mainWindow.h"

class dataBackDlg;
class mavlink5Thread :	public QThread	//��ȡmysql���ݿ����˻���¼�߳���
{
	Q_OBJECT
public:
	mavlink5Thread();
	QString		m_sql;
	MainWindow	*m_pMainDlg		= nullptr;
	dataBackDlg *m_pdtBackDlg	= nullptr;
	volatile bool m_bStopped	= false;
	volatile bool m_bPaused		= false;
	double        m_speedFactor	= 1.0;

signals:
	void signal_setTitle();
	void signal_modifyPlane(tag_PlaneMessage *plane);	
	
public:
	void run();
};

