#include "stdafx.h"
#include "DlgAddTaskArea.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <qgsProject.h>
#include "comm.h"
#include "global.h"
#include "mainWindow.h"

DlgAddTaskArea::DlgAddTaskArea(QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.okButton,	&QAbstractButton::clicked, this, &DlgAddTaskArea::on_queryBtn_click);
	connect(ui.cancelButton,&QAbstractButton::clicked, this, &DlgAddTaskArea::on_exitBtn_click);

	ui.lineEdit_5->setText("100");

	this->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint);
}

DlgAddTaskArea::~DlgAddTaskArea()
{}

void DlgAddTaskArea::on_exitBtn_click()
{
	MainWindow *pDlg = (MainWindow*)this->parentWidget();
}

void DlgAddTaskArea::on_queryBtn_click()
{
	QString  spt		= ui.lineEdit_1->text();
	QString  sAreaID	= ui.lineEdit_2->text();
	QString  sW			=ui.lineEdit_3->text();
	QString  sLength	=ui.lineEdit_4->text();
	QString  sHeight	= ui.lineEdit_5->text();
	QString  sflyTime= ui.lineEdit_6->text();
	QString  sAngle = ui.lineEdit_7->text();
	QString  sRemarks= ui.lineEdit_8->text();
	QString  sZ			= ui.lineEdit_9->text();

	if (sW.toInt() <100)
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u533a\u57df\u5bbd\u5ea6\u503c\u4e0d\u80fd\u5c0f\u4e8e100 \uff01\uff01\uff01"));
		return;
	}
	if (sLength.toInt() <100)
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u533a\u57df\u957f\u5ea6\u503c\u4e0d\u80fd\u5c0f\u4e8e100 \uff01\uff01\uff01"));
		return;
	}

	if (sAngle=="")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u6b63\u786e\u533a\u57df \u89d2\u5ea6\u503c \uff01\uff01\uff01"));
		return;
	}

	if (sZ.isEmpty() || sZ.toInt() <= sHeight.toInt())
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u9ad8\u5ea6\u4e0a\u9650\u5fc5\u987b\u5927\u4e8e\u9ad8\u5ea6\u4e0b\u9650\uff01"));
		return;
	}

	if (sAreaID == "")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u6b63\u786e\u533a\u57df \u7f16\u53f7 \uff01\uff01\uff01"));
		return;
	}

	if (sflyTime.toInt()<2)
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u6b63\u786e\u533a\u57df \u98de\u884c\u65f6\u95f4 \uff01\uff01\uff01"));
		return;
	}

	QgsFeature feat;
	QgsFeatureIterator fit = g_pAirTaskPolyLayer->getFeatures();
	while (fit.nextFeature(feat))
	{
		QString		sID = feat.attribute(0).toString();
		if (sID == sAreaID)
		{
			QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u5df2\u7ecf\u5b58\u5728\u6b64\u65e0\u4eba\u673a\u4efb\u52a1\u533a\u57df\u7f16\u53f7 \uff01\uff01\uff01"));
			return;
		}
	}
	g_pAirTaskPolyLayer->startEditing();

	QList<QgsPointXY> PointSet = GetPolygon();
	QgsPolygonXY	pxy		= QgsPolygonXY() << PointSet.toVector();
	QgsGeometry		Geometry= QgsGeometry::fromPolygonXY(pxy);

	QgsFeature f = QgsFeature();
	f.setGeometry(Geometry);
	f.setAttributes(QgsAttributes() << sAreaID <<"name"<< spt << sW << sLength << sHeight <<sZ << sAngle << sflyTime << sRemarks);
	bool b = g_pAirTaskPolyLayer->addFeature(f);

	qDebug() << "b=" << b << endl;
	g_pAirTaskPolyLayer->commitChanges();

	MainWindow *pDlg = (MainWindow*)this->parentWidget();
	pDlg->ShowTaskAreaTip();
	pDlg->syncLeafletAll();
	pDlg->sync3DAll();
}

QList<QgsPointXY> DlgAddTaskArea::GetPolygon()
{
	QList<QgsPointXY> list;
	QString  spt	= ui.lineEdit_1->text();
	QString  sw		= ui.lineEdit_3->text();
	QString  sLength= ui.lineEdit_4->text();
	QString  sAngle = ui.lineEdit_7->text();

	int k = spt.indexOf(",");
	QString x = spt.mid(0, k);
	QString y = spt.mid(k + 1);

	QgsPointXY p(x.toDouble(), y.toDouble());

	double banWidth = (sw.toInt() / 2 * 0.00001);
	double banLngth = sLength.toInt() / 2 * 1000 * LATITUDE_PER_MM;
	QgsPointXY  p1(x.toDouble()- banWidth, y.toDouble()+ banLngth);
	QgsPointXY  p2(x.toDouble()+ banWidth, y.toDouble()+ banLngth);
	QgsPointXY  p3(x.toDouble()+ banWidth, y.toDouble()- banLngth);
	QgsPointXY  p4(x.toDouble()- banWidth, y.toDouble()- banLngth);

	list.append(p1);
	list.append(p2);
	list.append(p3);
	list.append(p4);

	return list;
}
