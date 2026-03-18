#include "stdafx.h"
#include "DlgModifyTaskArea.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <qgsProject.h>
#include "comm.h"
#include "global.h"
#include "mainWindow.h"

DlgModifyTaskArea::DlgModifyTaskArea(QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.okButton,	&QAbstractButton::clicked, this, &DlgModifyTaskArea::on_queryBtn_click);
	connect(ui.cancelButton,&QAbstractButton::clicked, this, &DlgModifyTaskArea::on_exitBtn_click);

	//ui.lineEdit_4->setText("0");

	this->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint);
}

DlgModifyTaskArea::~DlgModifyTaskArea()
{}

void DlgModifyTaskArea::on_exitBtn_click()
{
	MainWindow *pDlg = (MainWindow*)this->parentWidget();
}

void DlgModifyTaskArea::on_queryBtn_click()
{
	QString  spt= ui.lineEdit_1->text();
	QString  sAirID = ui.lineEdit_2->text();
	QString  sW=ui.lineEdit_3->text();
	QString  sLength=ui.lineEdit_4->text();
	QString  sHeight= ui.lineEdit_5->text();
	QString  sTime	= ui.lineEdit_6->text();
	QString  sAngle = ui.lineEdit_7->text();
	QString  sRemark= ui.lineEdit_8->text();
	QString  sZ = ui.lineEdit_9->text();

	if (sW.toInt() <1)
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u533a\u57df\u5bbd\u5ea6\u503c\u4e0d\u80fd\u5c0f\u4e8e100 \uff01\uff01\uff01"));
		return;
	}
	if (sLength.toInt() < 1)
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u533a\u57df\u957f\u5ea6\u503c\u4e0d\u80fd\u5c0f\u4e8e100 \uff01\uff01\uff01"));
		return;
	}

	if (sZ.isEmpty() || sZ.toInt() <= sHeight.toInt())
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u9ad8\u5ea6\u4e0a\u9650\u5fc5\u987b\u5927\u4e8e\u9ad8\u5ea6\u4e0b\u9650\uff01"));
		return;
	}

	if (sAngle=="")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u6b63\u786e\u533a\u57df\u89d2\u5ea6\u503c \uff01\uff01\uff01"));
		return;
	}

	if (sAirID == "")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u6b63\u786e\u533a\u57df \u65e0\u4eba\u673a\u7f16\u53f7 \uff01\uff01\uff01"));
		return;
	}

	if (sTime.toInt()<4)
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u6b63\u786e\u533a\u57df \u98de\u884c\u65f6\u95f4 \uff01\uff01\uff01"));
		return;
	}

	g_pAirTaskPolyLayer->startEditing();
	g_pAirTaskPolyLayer->deleteFeature(m_fid);
	g_pAirTaskPolyLayer->commitChanges();

	g_pAirTaskPolyLayer->startEditing();
	QList<QgsPointXY>	PointSet	= GetPolygon();
	QgsPolygonXY		pxy			= QgsPolygonXY() << PointSet.toVector();
	QgsGeometry			Geometry	= QgsGeometry::fromPolygonXY(pxy);

	QgsFeature f = QgsFeature();
	f.setGeometry(Geometry);
	f.setAttributes(QgsAttributes() << sAirID<<"NAME"<< spt << sW << sLength << sHeight <<sZ<< sAngle << sTime << sRemark);
	bool b = g_pAirTaskPolyLayer->addFeature(f);
	g_pAirTaskPolyLayer->commitChanges();

	QgsFeatureIterator fit = g_pAirTaskPolyLayer->getFeatures();
	while (fit.nextFeature(f))
	{
		qDebug() << f.id() << endl;
	}
	m_fid = f.id();

	g_pAirTaskPolyLayer->startEditing();
	Rotate(PointSet);

	MainWindow *pDlg = (MainWindow*)this->parentWidget();
	pDlg->ShowTaskAreaTip();
	pDlg->syncLeafletAll();
	pDlg->sync3DAll();
	m_pMapCanvas->refresh();

	QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u4fee\u6539\u65e0\u4eba\u673a\u4efb\u52a1\u533a\u57df\u6210\u529f \uff01\uff01\uff01"));
	this->close();
}

QList<QgsPointXY> DlgModifyTaskArea::GetPolygon()
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

	double banWidth = (sw.toInt()/2 * 0.00001);
	double banLngth = sLength.toInt()/2 * 1000 * LATITUDE_PER_MM;
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

void DlgModifyTaskArea::Rotate(QList<QgsPointXY> list)
{
	QString  spt	= ui.lineEdit_1->text();
	QString  sAngle = ui.lineEdit_7->text();

	int k = spt.indexOf(",");
	QString x = spt.mid(0, k);
	QString y = spt.mid(k + 1);
	QgsPointXY cp(x.toDouble(), y.toDouble());

	QgsVectorLayer* vlayer = g_pAirTaskPolyLayer;
	float m_Rotation = sAngle.toFloat();
	//calculations for affine transformation
	double angle = -1 * m_Rotation * (M_PI / 180);
	QgsPointXY anchorPoint = m_pMapCanvas->mapSettings().mapToLayerCoordinates(vlayer, cp);
	double a = cos(angle);
	double b = -1 * sin(angle);
	double c = anchorPoint.x() - cos(angle) * anchorPoint.x() + sin(angle) * anchorPoint.y();
	double d = sin(angle);
	double ee = cos(angle);
	double f = anchorPoint.y() - sin(angle) * anchorPoint.x() - cos(angle) * anchorPoint.y();

	vlayer->beginEditCommand(tr("Features Rotated"));

	QgsFeature		feat;
	QgsFeatureId	fid;
	QgsFeatureIterator fit = g_pAirTaskPolyLayer->getFeatures();
	while (fit.nextFeature(feat))
	{
		qDebug() << feat.id() << endl;
	}
	fid = feat.id();
	int i = 0;
	foreach(QgsPointXY p, list)
	{
		QgsPoint vertex(p.x(), p.y());
		double newX = a * vertex.x() + b * vertex.y() + c;
		double newY = d * vertex.x() + ee * vertex.y() + f;
		vlayer->moveVertex(newX, newY, fid, i);
		i++;
	}

	vlayer->endEditCommand();
	g_pAirTaskPolyLayer->commitChanges();
}
