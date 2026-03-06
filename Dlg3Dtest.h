#pragma once

#include <QDialog>
#include "ui_Dlg3Dtest.h"
#include "comm.h"

#include <QtWebEngineWidgets/QWebEngineView>
#include <QtWidgets/QApplication>

class Dlg3Dtest : public QDialog
{
	Q_OBJECT

public:
	Dlg3Dtest(QWidget *parent = nullptr);
	~Dlg3Dtest();

	//QWebEngineView* m_pWebEngineView = nullptr;
public slots:	
	//void on_queryBtn_click();
	//void UpdateInterface();
	void resizeEvent(QResizeEvent* event);		//系统自带窗口大小变化事件
private:
public:
	Ui::Dlg3Dtest ui;
};
