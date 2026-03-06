#pragma once

#include <QDialog>
#include "ui_Dialog_AirList.h"

class Dialog_AirList : public QDialog
{
	Q_OBJECT

public:
	Dialog_AirList(QWidget *parent = nullptr);
	~Dialog_AirList();

	int		m_devID=0;//筛选出来的雷达设备id,默认值0表示不筛选,显示全部雷达探测到的无人机
	void insert(QString airID, QString radarID);
public slots:
	//void on_setTitle();
	//void on_addBtn_click();
	//void on_deleteBtn_click();
	void on_queryBtn_click();
	void on_exitBtn_click();
private:
public:
	Ui::DialogAirList	 ui;
};
