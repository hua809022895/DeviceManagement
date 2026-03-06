#include "stdafx.h"
#include "DlgModifyTaskArea.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <qgsProject.h>
#include "comm.h"
#include "global.h"

DlgModifyTaskArea::DlgModifyTaskArea(QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.okButton,	&QAbstractButton::clicked, this, &DlgModifyTaskArea::on_queryBtn_click);//筛选命令按钮
	connect(ui.cancelButton,&QAbstractButton::clicked, this, &DlgModifyTaskArea::on_exitBtn_click);	//取消命令按钮
	
	//ui.lineEdit_4->setText("0");
	
	this->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint);
}

DlgModifyTaskArea::~DlgModifyTaskArea()
{}

//退出
void DlgModifyTaskArea::on_exitBtn_click()
{
	MainWindow *pDlg = (MainWindow*)this->parentWidget();
}

//确认修改
void DlgModifyTaskArea::on_queryBtn_click()
{
	QString  spt= ui.lineEdit_1->text();	//经纬度值
	QString  sAirID = ui.lineEdit_2->text();//区域id
	QString  sW=ui.lineEdit_3->text();		//宽度
	QString  sLength=ui.lineEdit_4->text();	//长度
	QString  sHeight= ui.lineEdit_5->text();//高度
	QString  sTime	= ui.lineEdit_6->text();//飞行时间
	QString  sAngle = ui.lineEdit_7->text();//角度
	QString  sRemark= ui.lineEdit_8->text();//备注
	QString  sZ = ui.lineEdit_9->text();		//相对高度
	
	if (sW.toInt() <1)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("任务区域宽度值不能小于100 ！！！"));
		return;
	}
	if (sLength.toInt() < 1)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("任务区域长度值不能小于100 ！！！"));
		return;
	}

	if (sHeight.toInt() <2)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入 正确区域高度值 ！！！"));
		return;
	}

	if (sAngle=="")
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入任务区域角度值 ！！！"));
		return;
	}
		
	if (sAirID == "")
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入任务区域 无人机编号  ！！！"));
		return;
	}

	if (sTime.toInt()<4)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入任务区域 飞行时间  ！！！"));
		return;
	}

	g_pAirTaskPolyLayer->startEditing();
	g_pAirTaskPolyLayer->deleteFeature(m_fid);
	g_pAirTaskPolyLayer->commitChanges();

	g_pAirTaskPolyLayer->startEditing();
	QList<QgsPointXY>	PointSet	= GetPolygon();
	QgsPolygonXY		pxy			= QgsPolygonXY() << PointSet.toVector();
	QgsGeometry			Geometry	= QgsGeometry::fromPolygonXY(pxy);//根据多边形的点信息生成几何图形对象

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

	QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("修改任务区域属性完成 ！！！"));
	this->close();
}

QList<QgsPointXY> DlgModifyTaskArea::GetPolygon()//根据矩形原点，宽度，长度，角度，算出一个四个点列表
{
	QList<QgsPointXY> list;
	QString  spt	= ui.lineEdit_1->text();		//矩形原点,经纬度值
	QString  sw		= ui.lineEdit_3->text();		//宽度,单位米
	QString  sLength= ui.lineEdit_4->text();		//长度,单位米
	QString  sAngle = ui.lineEdit_7->text();		//角度

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

//根据角度进行旋转
void DlgModifyTaskArea::Rotate(QList<QgsPointXY> list)
{
	QString  spt	= ui.lineEdit_1->text();	//经纬度值
	QString  sAngle = ui.lineEdit_7->text();	//角度

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