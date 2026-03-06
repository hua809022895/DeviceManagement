#include "stdafx.h"
#include "DlgEquipment.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <qgsProject.h>
#include "comm.h"
#include "DlgAddEquipment.h"
#include "DlgModifyEquipment.h"

DlgEquipment::DlgEquipment(QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.pushAddBtn,&QAbstractButton::clicked, this, &DlgEquipment::on_addBtn_click);//添加命令按钮
	connect(ui.pushModifyBtn, &QAbstractButton::clicked, this, &DlgEquipment::on_Midify_click);//修改命令按钮
	connect(ui.pushDeleteBtn, &QAbstractButton::clicked, this, &DlgEquipment::on_deleteBtn_click);//删除命令按钮
	connect(ui.pushAllDelBtn, &QAbstractButton::clicked, this, &DlgEquipment::on_AllDeleteBtn_click);//全部删除命令按钮
	connect(ui.okButton, &QAbstractButton::clicked, this, &DlgEquipment::on_exitBtn_click);//退出命令按钮
	
	//连接点击项的槽函数
	connect(ui.mWidget, &QTableWidget::itemClicked, this, &DlgEquipment::onItemClicked);

	//设置无人机装备表格控件
	int i = 0;
	ui.mWidget->setColumnCount(11); //设置列个数
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("id")));
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("类型")));
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("在库数量")));
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("侦查带宽")));
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("长度")));
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("宽度")));
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("高度")));
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("控制半径")));
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("续航时间")));	
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("最大载重")));
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("起飞重量")));//固定翼专用字段
	
	ui.mWidget->setColumnWidth(0, 0);
	ui.mWidget->setColumnWidth(1, 80);
	ui.mWidget->setColumnWidth(2, 70);
	ui.mWidget->setColumnWidth(3, 70);
	ui.mWidget->setColumnWidth(4, 50);
	ui.mWidget->setColumnWidth(5, 60);
	ui.mWidget->setColumnWidth(6, 60);
	ui.mWidget->setColumnWidth(7, 60);
	ui.mWidget->setColumnWidth(8, 60);
	ui.mWidget->setColumnWidth(9, 60);
	ui.mWidget->setColumnWidth(10,60);

	ui.mWidget->verticalHeader()->setVisible(false);
	ui.mWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择行为，以行为单位
	ui.mWidget->setSelectionMode(QAbstractItemView::SingleSelection); //设置选择模式，选择单行	

	//this->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint);
	refreshA();
	refreshB();
}

DlgEquipment::~DlgEquipment()
{}

void DlgEquipment::onItemClicked(QTableWidgetItem *item)
{
	if (item == nullptr)
		return;

	QList<QTableWidgetItem*> pitem =ui.mWidget->selectedItems();
	if (pitem.size() < 1)
		return;

	int row = item->row();

	// 获取单元格内容
	QTableWidgetItem *qim0 = ui.mWidget->item(row, 0);
	QTableWidgetItem *qim1 = ui.mWidget->item(row, 1);
	if (item) {

		ui.labelImage->clear();
		QString sql,text = qim0->text();
		QString name= qim1->text();
		qDebug() << "iCount text=" << text << endl;

		QSqlQuery query(g_sqliteDbase);

		sql = "select image from Equipment";

		if (name.indexOf(QString::fromLocal8Bit("多旋")) >= 0)
			sql += "A";
		if (name.indexOf(QString::fromLocal8Bit("固定")) >= 0 || name.indexOf(QString::fromLocal8Bit("复合")) >= 0)
			sql += "B";

		sql += " where Id=" + text;
		query.exec(sql);
		if (query.next())
		{
			QByteArray jpg = query.value("image").toByteArray();
			if (jpg.size() > 100)
			{
				QPixmap pixmap;
				pixmap.loadFromData(jpg, "jpg");
				QPixmap pixmapcopy = pixmap.scaled(ui.frame->width() - 10, ui.frame->height() - 10);
				ui.labelImage->setPixmap(pixmapcopy);
			}
		}
	}
}
//重新刷新,多旋翼表格数据，
void DlgEquipment::refreshA()
{
loop1:
	for (int i = 0; i < ui.mWidget->rowCount(); i++)
	{
		ui.mWidget->removeRow(0);
		goto loop1;		
	}

	QString sql = "select * from EquipmentA order by id";
	QSqlQuery query(g_sqliteDbase);
	bool b = query.exec(sql);

	while (query.next())
	{		
		QString ID = query.value("ID").toString();
		QString type = query.value("type").toString();
		QString num = query.value("num").toString(); 
		QString lookWidth = query.value("lookWidth").toString();
		QString len = query.value("len").toString();
		QString width = query.value("width").toString();
		QString height = query.value("height").toString();
		QString ctrlRadius = query.value("ctrlRadius").toString();		
		QString xhTime = query.value("xhTime").toString();			//续航时间
		QString maxWeight = query.value("maxWeight").toString();	//最大载重		
		//QString flyWeight = query.value("flyWeight").toString();	//固定翼专用字段
		int row = ui.mWidget->rowCount();
		ui.mWidget->insertRow(row);
		
		ui.mWidget->setItem(row, 0, new QTableWidgetItem(ID));	//ID
		ui.mWidget->item(row, 0)->setTextAlignment(Qt::AlignTop);		
		ui.mWidget->setItem(row, 1, new QTableWidgetItem(type));
		ui.mWidget->setItem(row, 2, new QTableWidgetItem(num));
		ui.mWidget->setItem(row, 3, new QTableWidgetItem(lookWidth));
		ui.mWidget->setItem(row, 4, new QTableWidgetItem(len));
		ui.mWidget->setItem(row, 5, new QTableWidgetItem(width));
		ui.mWidget->setItem(row, 6, new QTableWidgetItem(height));
		ui.mWidget->setItem(row, 7, new QTableWidgetItem(ctrlRadius));		
		ui.mWidget->setItem(row, 8, new QTableWidgetItem(xhTime));
		ui.mWidget->setItem(row, 9, new QTableWidgetItem(maxWeight));		
	}
}

void DlgEquipment::refreshB()
{
	QString sql = "select * from EquipmentB order by id";
	QSqlQuery query(g_sqliteDbase);
	bool b = query.exec(sql);

	while (query.next())
	{
		QString ID = query.value("ID").toString();
		QString type	= query.value("type").toString();
		QString num		= query.value("num").toString();
		QString lookWidth = query.value("lookWidth").toString();
		QString len		= query.value("len").toString();
		QString width	= query.value("width").toString();
		QString height	= query.value("height").toString();
		QString ctrlRadius	= query.value("ctrlRadius").toString();
		QString xhTime		= query.value("flyTime").toString();	//续航时间
		QString maxWeight	= query.value("maxWeight").toString();	//最大载重		
		QString flyWeight	= query.value("flyWeight").toString();	//固定翼专用字段

		int row = ui.mWidget->rowCount();
		ui.mWidget->insertRow(row);

		ui.mWidget->setItem(row, 0, new QTableWidgetItem(ID));	//ID
		ui.mWidget->item(row, 0)->setTextAlignment(Qt::AlignTop);
		ui.mWidget->setItem(row, 1, new QTableWidgetItem(type));
		ui.mWidget->setItem(row, 2, new QTableWidgetItem(num));
		ui.mWidget->setItem(row, 3, new QTableWidgetItem(lookWidth));
		ui.mWidget->setItem(row, 4, new QTableWidgetItem(len));
		ui.mWidget->setItem(row, 5, new QTableWidgetItem(width));
		ui.mWidget->setItem(row, 6, new QTableWidgetItem(height));
		ui.mWidget->setItem(row, 7, new QTableWidgetItem(ctrlRadius));
		ui.mWidget->setItem(row, 8, new QTableWidgetItem(xhTime));
		ui.mWidget->setItem(row, 9, new QTableWidgetItem(maxWeight));
		ui.mWidget->setItem(row, 10, new QTableWidgetItem(flyWeight));
	}
}
//添加装备
void DlgEquipment::on_addBtn_click()
{
	DlgAddEquipment dlg;
	dlg.exec();
	refreshA();
	refreshB();
}
//修改装备
void DlgEquipment::on_Midify_click()
{
	QList<QTableWidgetItem*> pitem = ui.mWidget->selectedItems();
	if (pitem.size() < 1)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请首先选中 一个装备 完成 ！！"), QMessageBox::StandardButton::Yes);
		return;
	}

	int row = pitem[0]->row();

	// 获取单元格内容
	QTableWidgetItem *qim0 = ui.mWidget->item(row, 0);
	QTableWidgetItem *qim1 = ui.mWidget->item(row, 1); //类型
	QString text = qim0->text();
	QString name = qim1->text();	

	bool bDuo = true;
	if(name.indexOf(QString::fromLocal8Bit("多旋"))>=0)
		bDuo = true;
	else
		bDuo = false;

	DlgModifyEquipment* dlg = new DlgModifyEquipment(text.toInt(), bDuo);
	dlg->exec();
	refreshA();
	refreshB();
	delete dlg;
}
//删除装备
void DlgEquipment::on_deleteBtn_click()
{
	QList<QTableWidgetItem*> pitem = ui.mWidget->selectedItems();
	if (pitem.size() < 1)
		return;

	int row = pitem[0]->row();

	// 获取单元格内容
	QTableWidgetItem *qim0 = ui.mWidget->item(row, 0);
	QTableWidgetItem *qim1 = ui.mWidget->item(row, 1);
	QMessageBox::StandardButton id = QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("您确实想删除当前 装备吗？？"), QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No);
	if (id == QMessageBox::StandardButton::Yes)
	{
		QString text = qim0->text();
		QString name = qim1->text();
		QString sql = "Delete from Equipment";

		if (name.indexOf(QString::fromLocal8Bit("多旋")) >= 0)
			sql += "A";
		if (name.indexOf(QString::fromLocal8Bit("固定")) >= 0 || name.indexOf(QString::fromLocal8Bit("复合")) >= 0)
			sql += "B";

		sql+=" where id = " +text; 
		QSqlQuery query(g_sqliteDbase);
		bool b = query.exec(sql);
		refreshA();
		refreshB();
		ui.labelImage->clear();
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("删除一个装备 完成 ！！"), QMessageBox::StandardButton::Yes);
	}
}

//退出
void DlgEquipment::on_exitBtn_click()
{
	MainWindow *pDlg = (MainWindow*)this->parentWidget();
}

//全部删除装备
void DlgEquipment::on_AllDeleteBtn_click()
{
	//ui.mAirTable->sortItems(1, Qt::AscendingOrder);//按雷达设备id排序		
	QMessageBox::StandardButton id = QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("您确实想删除全部 装备吗？？"), QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No);
	if (id == QMessageBox::StandardButton::Yes)
	{
	loop1:
		for (int i = 0; i < ui.mWidget->rowCount(); i++)
		{
			ui.mWidget->removeRow(0);
			goto loop1;			
		}
		
		QString sql = "Delete from EquipmentA";
		QSqlQuery query(g_sqliteDbase);
		bool b = query.exec(sql);

		sql = "Delete from EquipmentB";
		QSqlQuery queryB(g_sqliteDbase);
		b = queryB.exec(sql);

		refreshA();
		refreshB();
		ui.labelImage->clear();
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("删除 全部装备 完成 ！！"), QMessageBox::StandardButton::Yes);
	}
}
