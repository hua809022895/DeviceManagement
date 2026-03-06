#pragma once
#include <qmainwindow.h>
#include <qgsProject.h>
#include "ui_SetRadarDlg.h"

class QsetRadarDlg :public QMainWindow
{
	Q_OBJECT
public:
	QsetRadarDlg(QWidget *parent = nullptr, int itype=0);
	~QsetRadarDlg();

	QgsFeatureId m_fid;			//要修改的图元id
	bool		m_bInsert=true;//默认值true表示，是新建设备，false表示是修改设备
	QgsPointXY	m_point;
	int			m_iType = 0;//设置类型，1光学探测器，2雷达，3遥感
	Ui::Form	ui;
public slots:
	void on_handSet();			//手动设置
	void on_saveBtn_click();	//设置保存
	void on_exitBtn_click();	//退出
//private:
	
};

