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

	connect(ui.pushAddBtn,&QAbstractButton::clicked, this, &DlgEquipment::on_addBtn_click);//�������ť
	connect(ui.pushModifyBtn, &QAbstractButton::clicked, this, &DlgEquipment::on_Midify_click);//�޸����ť
	connect(ui.pushDeleteBtn, &QAbstractButton::clicked, this, &DlgEquipment::on_deleteBtn_click);//ɾ�����ť
	connect(ui.pushAllDelBtn, &QAbstractButton::clicked, this, &DlgEquipment::on_AllDeleteBtn_click);//ȫ��ɾ�����ť
	connect(ui.okButton, &QAbstractButton::clicked, this, &DlgEquipment::on_exitBtn_click);//�˳����ť
	
	//���ӵ����Ĳۺ���
	connect(ui.mWidget, &QTableWidget::itemClicked, this, &DlgEquipment::onItemClicked);

	//�������˻�װ������ؼ�
	int i = 0;
	ui.mWidget->setColumnCount(11); //�����и���
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("id")));
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u88c5\u5907\u7c7b\u578b")));          // 装备类型
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u5728\u5e93\u6570\u91cf")));          // 在库数量
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u4fa6\u5bdf\u5e26\u5bbd")));          // 侦察带宽
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u88c5\u5907\u957f\u5ea6")));          // 装备长度
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u88c5\u5907\u5bbd\u5ea6")));          // 装备宽度
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u88c5\u5907\u9ad8\u5ea6")));          // 装备高度
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u63a7\u5236\u534a\u5f84")));          // 控制半径
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u98de\u884c\u65f6\u95f4")));          // 飞行时间
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u6700\u5927\u8f7d\u91cd")));          // 最大载重
	ui.mWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u5de5\u4f5c\u9ad8\u5ea6")));          // 工作高度

	ui.mWidget->setColumnHidden(0, true);
	ui.mWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.mWidget->verticalHeader()->setVisible(false);
	ui.mWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.mWidget->setSelectionMode(QAbstractItemView::SingleSelection);

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

	// ��ȡ��Ԫ������
	QTableWidgetItem *qim0 = ui.mWidget->item(row, 0);
	QTableWidgetItem *qim1 = ui.mWidget->item(row, 1);
	if (item) {

		ui.labelImage->clear();
		QString sql,text = qim0->text();
		QString name= qim1->text();
		qDebug() << "iCount text=" << text << endl;

		QSqlQuery query(g_sqliteDbase);

		sql = "select image from Equipment";

		if (name.indexOf(QStringLiteral("\u591a\u65cb")) >= 0)
			sql += "A";
		else
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
				QPixmap pixmapcopy = pixmap.scaled(ui.frame->width() - 10, ui.frame->height() - 10, Qt::KeepAspectRatio, Qt::SmoothTransformation);
				ui.labelImage->setPixmap(pixmapcopy);
				ui.labelImage->setAlignment(Qt::AlignCenter);
			}
		}
	}
}
//����ˢ��,�������������ݣ�
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
		QString xhTime = query.value("xhTime").toString();
		QString maxWeight = query.value("maxWeight").toString();
		QString workHeight = query.value("workHeight").toString();
		int row = ui.mWidget->rowCount();
		ui.mWidget->insertRow(row);

		ui.mWidget->setItem(row, 0, new QTableWidgetItem(ID));
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
		ui.mWidget->setItem(row, 10, new QTableWidgetItem(workHeight));		
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
		QString xhTime		= query.value("flyTime").toString();	//����ʱ��
		QString maxWeight	= query.value("maxWeight").toString();	//�������		
		QString flyWeight	= query.value("flyWeight").toString();	//�̶���ר���ֶ�

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
//����װ��
void DlgEquipment::on_addBtn_click()
{
	DlgAddEquipment dlg;
	dlg.exec();
	refreshA();
	refreshB();
}
//�޸�װ��
void DlgEquipment::on_Midify_click()
{
	QList<QTableWidgetItem*> pitem = ui.mWidget->selectedItems();
	if (pitem.size() < 1)
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u5148\u9009\u62e9\u4e00\u4e2a\u88c5\u5907\u518d\u4fee\u6539"), QMessageBox::StandardButton::Yes);
		return;
	}

	int row = pitem[0]->row();

	// ��ȡ��Ԫ������
	QTableWidgetItem *qim0 = ui.mWidget->item(row, 0);
	QTableWidgetItem *qim1 = ui.mWidget->item(row, 1); //����
	QString text = qim0->text();
	QString name = qim1->text();	

	bool bDuo = true;
	if(name.indexOf(QStringLiteral("\u591a\u65cb"))>=0)
		bDuo = true;
	else
		bDuo = false;

	DlgModifyEquipment* dlg = new DlgModifyEquipment(text.toInt(), bDuo);
	dlg->exec();
	refreshA();
	refreshB();
	delete dlg;
}
//ɾ��װ��
void DlgEquipment::on_deleteBtn_click()
{
	QList<QTableWidgetItem*> pitem = ui.mWidget->selectedItems();
	if (pitem.size() < 1)
		return;

	int row = pitem[0]->row();

	// ��ȡ��Ԫ������
	QTableWidgetItem *qim0 = ui.mWidget->item(row, 0);
	QTableWidgetItem *qim1 = ui.mWidget->item(row, 1);
	QMessageBox::StandardButton id = QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u786e\u5b9e\u8981\u5220\u9664\u5f53\u524d\u88c5\u5907\u5417\uff1f"), QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No);
	if (id == QMessageBox::StandardButton::Yes)
	{
		QString text = qim0->text();
		QString name = qim1->text();
		QString sql = "Delete from Equipment";

		if (name.indexOf(QStringLiteral("\u591a\u65cb")) >= 0)
			sql += "A";
		else
			sql += "B";

		sql+=" where id = " +text;
		QSqlQuery query(g_sqliteDbase);
		bool b = query.exec(sql);
		refreshA();
		refreshB();
		ui.labelImage->clear();
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u5220\u9664\u88c5\u5907\u6210\u529f"), QMessageBox::StandardButton::Yes);
	}
}

//�˳�
void DlgEquipment::on_exitBtn_click()
{
	MainWindow *pDlg = (MainWindow*)this->parentWidget();
}

//ȫ��ɾ��װ��
void DlgEquipment::on_AllDeleteBtn_click()
{
	//ui.mAirTable->sortItems(1, Qt::AscendingOrder);//���״��豸id����		
	QMessageBox::StandardButton id = QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u786e\u5b9e\u8981\u5220\u9664\u5168\u90e8\u88c5\u5907\u5417\uff1f"), QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No);
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
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u5220\u9664\u5168\u90e8\u88c5\u5907\u6210\u529f"), QMessageBox::StandardButton::Yes);
	}
}
