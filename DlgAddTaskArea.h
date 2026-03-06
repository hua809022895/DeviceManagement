#pragma once

#include <QDialog>
#include "ui_DlgAddTaskArea.h"
#include "qgis.h"

class DlgAddTaskArea : public QDialog
{
	Q_OBJECT
public:
	DlgAddTaskArea(QWidget *parent = nullptr);
	~DlgAddTaskArea();

	QList<QgsPointXY> GetPolygon();	//根据矩形原点，宽度，长度，角度，算出一个四个点列表
public slots:
	void on_queryBtn_click();
	void on_exitBtn_click();
private:
public:
	Ui::DlgAddTaskArea	 ui;
};
