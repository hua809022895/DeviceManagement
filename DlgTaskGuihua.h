#pragma once

#include <QDialog>
#include "ui_DlgTaskGuihua.h"

class DlgTaskGuihua : public QDialog
{
	Q_OBJECT
public:
	DlgTaskGuihua(QWidget *parent = nullptr);
	~DlgTaskGuihua();

	void refreshA(int len,int w,int flyTime);//重新刷新表格数据
	void refreshB(int len,int w, int flyTime);//重新刷新表格数据
public slots:
	void onItemClicked(QTableWidgetItem *item);
	//void on_exitBtn_click();
private:
public:
	Ui::DlgTaskGuihua	 ui;
};
