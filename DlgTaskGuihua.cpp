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
	//connect(ui.okButton, &QAbstractButton::clicked, this, &DlgTaskGuihua::on_exitBtn_click);//�˳����ť
	
	//���ӵ����Ĳۺ���	
	connect(ui.mWidget, &QTableWidget::itemClicked, this, &DlgTaskGuihua::onItemClicked);

	//�������˻�����ؼ�
	int i = 0;
	ui.mWidget->setColumnCount(5);
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u4efb\u52a1ID")));    // 任务ID
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u957f\u5ea6")));      // 长度
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u5bbd\u5ea6")));      // 宽度
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u9ad8\u5ea6")));      // 高度
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u884c\u65f6")));      // 行时
	
	ui.mWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.mWidget->verticalHeader()->setVisible(false);
	ui.mWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.mWidget->setSelectionMode(QAbstractItemView::SingleSelection);

	i = 0;
	ui.mWidget_2->setColumnCount(9);
	ui.mWidget_2->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u88c5\u5907id")));   // 装备id
	ui.mWidget_2->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u7c7b\u578b")));    // 类型
	ui.mWidget_2->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u67e5\u5e26\u5bbd")));// 查带宽
	ui.mWidget_2->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u957f\u5ea6")));    // 长度
	ui.mWidget_2->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u5bbd\u5ea6")));    // 宽度
	ui.mWidget_2->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u9ad8\u5ea6")));    // 高度
	ui.mWidget_2->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u822a\u65f6")));    // 航时
	ui.mWidget_2->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u901f\u5ea6")));    // 速度
	ui.mWidget_2->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u7528\u6570")));    // 用数
	
	ui.mWidget_2->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.mWidget_2->setColumnHidden(0, true);  // 隐藏装备id列
	ui.mWidget_2->verticalHeader()->setVisible(false);
	ui.mWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.mWidget_2->setSelectionMode(QAbstractItemView::SingleSelection);

	//this->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint);
	//����ȫ���״��豸
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

	// ��ȡ��Ԫ������
	//QTableWidgetItem *qim = ui.mWidget->item(row, 0);
	QTableWidgetItem *qFlyTime	= ui.mWidget->item(row, 4);//����ʱ��
	QTableWidgetItem *qLen		= ui.mWidget->item(row, 1);//����
	QTableWidgetItem *qW		= ui.mWidget->item(row, 2);//����
	if (item) {
		QString text = qFlyTime->text();
		QString tLen = qLen->text();
		QString tWidth = qW->text();
		//qDebug() << "iCount text=" << text << endl;
		
		refreshA(tLen.toInt(), tWidth.toInt(),text.toInt()); //���ݷ���ʱ�䣬�жϣ�װ���Ƿ����
		refreshB(tLen.toInt(), tWidth.toInt(),text.toInt()); //���ݷ���ʱ�䣬�жϣ�װ���Ƿ����		
	}
}

//����ˢ�±������ݣ����ݷ���ʱ�䣬�ж�װ����Ҫ�ü���
void DlgTaskGuihua::refreshA(int len, int w, int flyTime)
{
	QString sql = "select * from EquipmentA order by id";
	QSqlQuery query(g_sqliteDbase);
	bool b = query.exec(sql);

	while (query.next())
	{		
		QString ID			= query.value("ID").toString();
		QString type		= query.value("type").toString();
		QString lookWidth	= query.value("lookWidth").toString();		//������
		QString slen		= query.value("len").toString();
		QString width	= query.value("width").toString();
		QString height	= query.value("height").toString();
		QString xhTime	= query.value("xhTime").toString();
		QString sSpeed = query.value("MaxSpeed").toString();  //�ٶ�

		int row = ui.mWidget_2->rowCount();
		ui.mWidget_2->insertRow(row);
		
		ui.mWidget_2->setItem(row, 0, new QTableWidgetItem(ID));	//ID
		ui.mWidget_2->item(row, 0)->setTextAlignment(Qt::AlignTop);
		ui.mWidget_2->setItem(row, 1, new QTableWidgetItem(type));
		ui.mWidget_2->setItem(row, 2, new QTableWidgetItem(lookWidth)); //������
		ui.mWidget_2->setItem(row, 3, new QTableWidgetItem(slen));
		ui.mWidget_2->setItem(row, 4, new QTableWidgetItem(width));
		ui.mWidget_2->setItem(row, 5, new QTableWidgetItem(height));
		ui.mWidget_2->setItem(row, 6, new QTableWidgetItem(xhTime));
		ui.mWidget_2->setItem(row, 7, new QTableWidgetItem(sSpeed));
		
		int time =  (len * w) / lookWidth.toInt() / sSpeed.toInt()/ flyTime /60; 
		QString sNum = QString::number(time+1);
		ui.mWidget_2->setItem(row, 8, new QTableWidgetItem(sNum));//��Ҫ����
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
		QString lookWidth = query.value("lookWidth").toString();		//������
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
		ui.mWidget_2->setItem(row, 2, new QTableWidgetItem(lookWidth)); //������
		ui.mWidget_2->setItem(row, 3, new QTableWidgetItem(slen));
		ui.mWidget_2->setItem(row, 4, new QTableWidgetItem(width));
		ui.mWidget_2->setItem(row, 5, new QTableWidgetItem(height));
		ui.mWidget_2->setItem(row, 6, new QTableWidgetItem(xhTime));
		ui.mWidget_2->setItem(row, 7, new QTableWidgetItem(QString::number(iSpeed)));

		int time = (len * w) / lookWidth.toInt() / flyTime /iSpeed /60;
		QString sNum = QString::number(time+1);
		ui.mWidget_2->setItem(row, 8, new QTableWidgetItem(sNum));//��Ҫ����
	}
}

