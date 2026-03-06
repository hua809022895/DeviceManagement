#pragma once

#include <QDialog>
#include "ui_DlgModifyTaskArea.h"
#include "qgis.h"
#include <qgsProject.h>
#include <qgsMapCanvas.h>

class DlgModifyTaskArea : public QDialog
{
	Q_OBJECT
public:
	DlgModifyTaskArea(QWidget *parent = nullptr);
	~DlgModifyTaskArea();

	QgsMapCanvas		*m_pMapCanvas = nullptr;
	void				Rotate(QList<QgsPointXY> list);
	QgsFeatureId		m_fid;			//要修改的图元id
	QList<QgsPointXY>	GetPolygon();	//根据矩形原点，宽度，长度，角度，算出一个四个点列表
public slots:
	void on_queryBtn_click();
	void on_exitBtn_click();
private:
public:	
	Ui::DlgModifyTaskArea	 ui;
};
