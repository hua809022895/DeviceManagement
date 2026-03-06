#pragma once

#include <QDialog>
#include "ui_DlgAreaRadarList.h"

class DlgAreaRadarList : public QDialog
{
	Q_OBJECT

public:
	DlgAreaRadarList(QWidget *parent = nullptr);
	~DlgAreaRadarList();

	int		m_devID=0;//筛选出来的雷达设备id,默认值0表示不筛选,显示全部雷达探测到的无人机
	//void insert(QString airID, QString radarID);
public slots:
	void onItemClicked(QListWidgetItem* item);//表格的单击事件
	void on_exitBtn_click();
private:
public:
	Ui::DlgAreaRaderList ui;
};
