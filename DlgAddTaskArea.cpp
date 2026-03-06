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

	connect(ui.okButton,	&QAbstractButton::clicked, this, &DlgAddTaskArea::on_queryBtn_click);	//添加命令按钮
	connect(ui.cancelButton,&QAbstractButton::clicked, this, &DlgAddTaskArea::on_exitBtn_click);	//取消命令按钮
	
	ui.lineEdit_5->setText("100");//高度
	
	this->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint);
}

DlgAddTaskArea::~DlgAddTaskArea()
{}

//退出
void DlgAddTaskArea::on_exitBtn_click()
{
	MainWindow *pDlg = (MainWindow*)this->parentWidget();
}

//添加命令按钮
void DlgAddTaskArea::on_queryBtn_click()
{
	QString  spt		= ui.lineEdit_1->text();//经纬度值
	QString  sAreaID	= ui.lineEdit_2->text();//任务区域编号id
	QString  sW			=ui.lineEdit_3->text();	//宽度
	QString  sLength	=ui.lineEdit_4->text();	//长度
	QString  sHeight	= ui.lineEdit_5->text();//高度
	QString  sflyTime= ui.lineEdit_6->text();	//飞行时间
	QString  sAngle = ui.lineEdit_7->text();	//角度
	QString  sRemarks= ui.lineEdit_8->text();	//备注
	QString  sZ			= ui.lineEdit_9->text();//相对高度
	
	if (sW.toInt() <100)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("任务区域宽度值不能小于100 ！！！"));
		return;
	}
	if (sLength.toInt() <100)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("任务区域长度值不能小于100 ！！！"));
		return;
	}

	if (sAngle=="")
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入任务区域 角度值 ！！！"));
		return;
	}
	
	if (sHeight.toInt()<2)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入任务区域 高度值 ！！！"));
		return;
	}

	if (sAreaID == "")
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入任务区域 编号  ！！！"));
		return;
	}

	if (sflyTime.toInt()<2)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入任务区域 飞行时间  ！！！"));
		return;
	}

	//检查任务图层中是否已经有此无人机编号的任务区域
	//遍历全部雷达设备	
	QgsFeature feat;
	QgsFeatureIterator fit = g_pAirTaskPolyLayer->getFeatures();
	while (fit.nextFeature(feat))
	{
		QString		sID = feat.attribute(0).toString();
		if (sID == sAreaID)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("您已经添加了此无人机的任务区域编号  ！！！"));
			return;
		}
	}
	g_pAirTaskPolyLayer->startEditing();

	QList<QgsPointXY> PointSet = GetPolygon();
	QgsPolygonXY	pxy		= QgsPolygonXY() << PointSet.toVector();
	QgsGeometry		Geometry= QgsGeometry::fromPolygonXY(pxy);//根据多边形的点信息生成几何图形对象

	QgsFeature f = QgsFeature();
	f.setGeometry(Geometry);
	f.setAttributes(QgsAttributes() << sAreaID <<"name"<< spt << sW << sLength << sHeight <<sZ << sAngle << sflyTime << sRemarks);
	bool b = g_pAirTaskPolyLayer->addFeature(f);

	qDebug() << "b=" << b << endl;
	g_pAirTaskPolyLayer->commitChanges();

	MainWindow *pDlg = (MainWindow*)this->parentWidget();
	pDlg->ShowTaskAreaTip();
}

QList<QgsPointXY> DlgAddTaskArea::GetPolygon()//根据矩形原点，宽度，长度，角度，算出一个四个点列表
{
	QList<QgsPointXY> list;
	QString  spt	= ui.lineEdit_1->text();	//矩形原点,经纬度值
	QString  sw		= ui.lineEdit_3->text();	//宽度,单位米
	QString  sLength= ui.lineEdit_4->text();	//长度,单位米
	QString  sAngle = ui.lineEdit_7->text();	//角度

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