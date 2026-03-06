#include "stdafx.h"
#include "DlgTaskGuihua.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <qgsProject.h>
#include "comm.h"
#include "DlgAddEquipment.h"
#include "DlgModifyEquipment.h"

DlgTaskGuihua::DlgTaskGuihua(QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);
	//connect(ui.okButton, &QAbstractButton::clicked, this, &DlgTaskGuihua::on_exitBtn_click);//退出命令按钮
	
	//连接点击项的槽函数	
	connect(ui.mWidget, &QTableWidget::itemClicked, this, &DlgTaskGuihua::onItemClicked);

	//设置无人机表格控件
	int i = 0;
	ui.mWidget->setColumnCount(5);
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("任务ID")));
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("长度")));
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("宽度")));
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("高度")));
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("飞行时间")));
	
	ui.mWidget->setColumnWidth(0, 70);
	ui.mWidget->setColumnWidth(1, 50);
	ui.mWidget->setColumnWidth(2, 50);
	ui.mWidget->setColumnWidth(3, 50);
	ui.mWidget->setColumnWidth(4, 70);

	ui.mWidget->verticalHeader()->setVisible(false);
	ui.mWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择行为，以行为单位
	ui.mWidget->setSelectionMode(QAbstractItemView::SingleSelection); //设置选择模式，选择单行

	i = 0;
	ui.mWidget_2->setColumnCount(9);
	ui.mWidget_2->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("装备id")));
	ui.mWidget_2->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("类型")));
	ui.mWidget_2->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("侦查带宽")));
	ui.mWidget_2->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("长度")));
	ui.mWidget_2->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("宽度")));
	ui.mWidget_2->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("高度")));	
	ui.mWidget_2->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("续航时间")));	
	ui.mWidget_2->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("速度")));
	ui.mWidget_2->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("需用数量")));
	
	ui.mWidget_2->setColumnWidth(0, 0);
	ui.mWidget_2->setColumnWidth(1, 80);//类型
	ui.mWidget_2->setColumnWidth(2, 70);//侦查带宽
	ui.mWidget_2->setColumnWidth(3, 60);
	ui.mWidget_2->setColumnWidth(4, 60);
	ui.mWidget_2->setColumnWidth(5, 60);
	ui.mWidget_2->setColumnWidth(6, 60);
	ui.mWidget_2->setColumnWidth(7, 60); //速度
	ui.mWidget_2->setColumnWidth(8, 60); //需要架数

	ui.mWidget_2->verticalHeader()->setVisible(false);
	ui.mWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择行为，以行为单位
	ui.mWidget_2->setSelectionMode(QAbstractItemView::SingleSelection); //设置选择模式，选择单行	

	//this->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint);
	//遍历全部雷达设备
	QgsFeature feat;
	QgsFeatureIterator fit = g_pAirTaskPolyLayer->getFeatures();
	while (fit.nextFeature(feat))
	{
		QString		sID		= feat.attribute(0).toString();		
		QString		sWidth	= feat.attribute(3).toString();		
		QString		sLen	= feat.attribute(4).toString();
		QString		sHeight = feat.attribute(5).toString();
		QString		sZ		= feat.attribute(6).toString();
		QString		sAngle	= feat.attribute(7).toString();
		QString		sFlyTime= feat.attribute(8).toString();

		int row = ui.mWidget->rowCount();
		ui.mWidget->insertRow(row);

		ui.mWidget->setItem(row, 0, new QTableWidgetItem(sID));	//ID
		ui.mWidget->item(row, 0)->setTextAlignment(Qt::AlignTop);
		ui.mWidget->setItem(row, 1, new QTableWidgetItem(sLen));
		ui.mWidget->setItem(row, 2, new QTableWidgetItem(sWidth));
		ui.mWidget->setItem(row, 3, new QTableWidgetItem(sHeight));
		ui.mWidget->setItem(row, 4, new QTableWidgetItem(sFlyTime));
	}
}

DlgTaskGuihua::~DlgTaskGuihua()
{}

void DlgTaskGuihua::onItemClicked(QTableWidgetItem *item)
{
	if (item == nullptr)
		return;

	QList<QTableWidgetItem*> pitem =ui.mWidget->selectedItems();
	if (pitem.size() < 1)
		return;

loop1:
	for (int i =0; i < ui.mWidget_2->rowCount(); i++)
	{
		ui.mWidget_2->removeRow(0);
		goto loop1;
	}
	int row = item->row();

	// 获取单元格内容
	//QTableWidgetItem *qim = ui.mWidget->item(row, 0);
	QTableWidgetItem *qFlyTime	= ui.mWidget->item(row, 4);//飞行时间
	QTableWidgetItem *qLen		= ui.mWidget->item(row, 1);//长度
	QTableWidgetItem *qW		= ui.mWidget->item(row, 2);//宽度
	if (item) {
		QString text = qFlyTime->text();
		QString tLen = qLen->text();
		QString tWidth = qW->text();
		//qDebug() << "iCount text=" << text << endl;
		
		refreshA(tLen.toInt(), tWidth.toInt(),text.toInt()); //根据飞行时间，判断，装备是否可用
		refreshB(tLen.toInt(), tWidth.toInt(),text.toInt()); //根据飞行时间，判断，装备是否可用		
	}
}

//重新刷新表格数据，根据飞行时间，判断装备需要用几个
void DlgTaskGuihua::refreshA(int len, int w, int flyTime)
{
	QString sql = "select * from EquipmentA order by id";
	QSqlQuery query(g_sqliteDbase);
	bool b = query.exec(sql);

	while (query.next())
	{		
		QString ID			= query.value("ID").toString();
		QString type		= query.value("type").toString();
		QString lookWidth	= query.value("lookWidth").toString();		//侦查带宽
		QString slen		= query.value("len").toString();
		QString width	= query.value("width").toString();
		QString height	= query.value("height").toString();
		QString xhTime	= query.value("xhTime").toString();
		QString sSpeed = query.value("MaxSpeed").toString();  //速度

		int row = ui.mWidget_2->rowCount();
		ui.mWidget_2->insertRow(row);
		
		ui.mWidget_2->setItem(row, 0, new QTableWidgetItem(ID));	//ID
		ui.mWidget_2->item(row, 0)->setTextAlignment(Qt::AlignTop);
		ui.mWidget_2->setItem(row, 1, new QTableWidgetItem(type));
		ui.mWidget_2->setItem(row, 2, new QTableWidgetItem(lookWidth)); //侦查带宽
		ui.mWidget_2->setItem(row, 3, new QTableWidgetItem(slen));
		ui.mWidget_2->setItem(row, 4, new QTableWidgetItem(width));
		ui.mWidget_2->setItem(row, 5, new QTableWidgetItem(height));
		ui.mWidget_2->setItem(row, 6, new QTableWidgetItem(xhTime));
		ui.mWidget_2->setItem(row, 7, new QTableWidgetItem(sSpeed));
		
		int time =  (len * w) / lookWidth.toInt() / sSpeed.toInt()/ flyTime /60; 
		QString sNum = QString::number(time+1);
		ui.mWidget_2->setItem(row, 8, new QTableWidgetItem(sNum));//需要个数
	}
}

void DlgTaskGuihua::refreshB(int len, int w, int flyTime)
{
	QString sql = "select * from EquipmentB order by id";
	QSqlQuery query(g_sqliteDbase);
	bool b = query.exec(sql);

	while (query.next())
	{
		QString ID = query.value("ID").toString();
		QString type = query.value("type").toString();
		QString lookWidth = query.value("lookWidth").toString();		//侦查带宽
		QString slen = query.value("len").toString();
		QString width = query.value("width").toString();
		QString height = query.value("height").toString();
		QString xhTime = query.value("flyTime").toString();
		int iSpeed = query.value("other").toInt();

		int row = ui.mWidget_2->rowCount();
		ui.mWidget_2->insertRow(row);

		ui.mWidget_2->setItem(row, 0, new QTableWidgetItem(ID));	//ID
		ui.mWidget_2->item(row, 0)->setTextAlignment(Qt::AlignTop);
		ui.mWidget_2->setItem(row, 1, new QTableWidgetItem(type));
		ui.mWidget_2->setItem(row, 2, new QTableWidgetItem(lookWidth)); //侦查带宽
		ui.mWidget_2->setItem(row, 3, new QTableWidgetItem(slen));
		ui.mWidget_2->setItem(row, 4, new QTableWidgetItem(width));
		ui.mWidget_2->setItem(row, 5, new QTableWidgetItem(height));
		ui.mWidget_2->setItem(row, 6, new QTableWidgetItem(xhTime));
		ui.mWidget_2->setItem(row, 7, new QTableWidgetItem(QString::number(iSpeed)));

		int time = (len * w) / lookWidth.toInt() / flyTime /iSpeed /60;
		QString sNum = QString::number(time+1);
		ui.mWidget_2->setItem(row, 8, new QTableWidgetItem(sNum));//需要个数
	}
}

