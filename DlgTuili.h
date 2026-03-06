#pragma once

#include <QDialog>
#include "ui_DlgTuili.h"
#include "comm.h"

class TuiliThread;
class DlgTuili : public QDialog
{
	Q_OBJECT

public:
	DlgTuili(QWidget *parent = nullptr);
	~DlgTuili();

	TuiliThread* m_pTuiliThread = nullptr;
	//int		m_devID=0;	//筛选出来的雷达设备id,默认值0表示不筛选,显示全部雷达探测到的无人机
	QTimer	*m_timer=nullptr;	//定时器对象
	void ModifyTable(tag_PlaneMessage* p);
	void insertToTable(tag_PlaneMessage* p);

public slots:	
	void on_queryBtn_click();
	void UpdateInterface();
private:
public:
	Ui::DlgTuili ui;
};
