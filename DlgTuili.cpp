#include "stdafx.h"
#include "DlgTuili.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <qgsProject.h>
#include "comm.h"

#include "TuiliThread.h"

DlgTuili::DlgTuili(QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.ok_Button, &QAbstractButton::clicked, this, &DlgTuili::on_queryBtn_click);	//筛选命令按钮
	//connect(ui.pushExitBtn, &QAbstractButton::clicked, this, &DlgTuili::on_exitBtn_click);	//定位命令按钮
	//this->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint);
	//central->addWidget(ui.mAirTable, 2, 0, 1, 1, Qt::AlignBottom);
	//MainWindow *pDlg = (MainWindow*)this->parentWidget();
	//m_pModel = new QStandardItemModel();
	//ui.listView->setModel(m_pModel);
	m_pTuiliThread =new TuiliThread();

	m_timer = new QTimer(this);
	connect(m_timer, &QTimer::timeout, this, &DlgTuili::UpdateInterface);
	
	//设置无人机表格控件
	int i = 0;
	ui.mAirTable->setColumnCount(9);	
	ui.mAirTable->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("飞机ID")));
	ui.mAirTable->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("经度")));
	ui.mAirTable->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("纬度")));
	ui.mAirTable->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("相对高度")));
	ui.mAirTable->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("海拔高度")));
	ui.mAirTable->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("Pitch")));
	ui.mAirTable->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("Roll")));
	ui.mAirTable->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("Yaw")));
	ui.mAirTable->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("状态")));

	ui.mAirTable->setColumnWidth(0, 50);
	ui.mAirTable->setColumnWidth(1, 90);
	ui.mAirTable->setColumnWidth(2, 90);
	ui.mAirTable->setColumnWidth(3, 90);
	ui.mAirTable->setColumnWidth(4, 90);
	ui.mAirTable->setColumnWidth(5, 70);
	ui.mAirTable->setColumnWidth(6, 70);
	ui.mAirTable->setColumnWidth(7, 70);
	ui.mAirTable->setColumnWidth(8, 90);
	
	ui.mAirTable->verticalHeader()->setVisible(false);
	ui.mAirTable->setSelectionBehavior(QAbstractItemView::SelectRows);	//设置选择行为，以行为单位
	ui.mAirTable->setSelectionMode(QAbstractItemView::SingleSelection); //设置选择模式，选择单行
}

DlgTuili::~DlgTuili()
{}

void DlgTuili::on_queryBtn_click()
{
	if (m_pTuiliThread->m_isRun)
	{
		ui.ok_Button->setText(QString::fromLocal8Bit("开始推理"));
		ui.label->setText(QString::fromLocal8Bit("停止推理了..."));
		m_pTuiliThread->m_isRun = false;
		m_timer->stop();
		//ui.label_2->setText("");
	}
	else
	{
		ui.ok_Button->setText(QString::fromLocal8Bit("停止推理"));
		ui.label->setText(QString::fromLocal8Bit("正在推理中....."));
		m_pTuiliThread->start();
		m_pTuiliThread->m_isRun = true;
		m_timer->start(1000);
	}
}

void DlgTuili::UpdateInterface()
{
	int count = ui.label->text().count();
	if (count <= 10)
	{
		QString s = ui.label->text() + ".";
		ui.label->setText(s);
	}
	else
		ui.label->setText(QString::fromLocal8Bit("正在推理中"));
	
}

//添加无人机到表格控件中
void DlgTuili::insertToTable(tag_PlaneMessage* p)
{
	int c = 2;
	int row = ui.mAirTable->rowCount();
	ui.mAirTable->insertRow(row);

	ui.mAirTable->setItem(row, 0, new QTableWidgetItem(p->ID));//ID号
	ui.mAirTable->item(row, 0)->setTextAlignment(Qt::AlignTop);

	//经度，纬度，相对高度，海拔高度
	QString s = QString("%1").arg(QString::number(p->planeX.toDouble(), 'f', 5));
	ui.mAirTable->setItem(row, 1, new QTableWidgetItem(s));

	s = QString("%1").arg(QString::number(p->planeY.toDouble(), 'f', 5));
	ui.mAirTable->setItem(row, c++, new QTableWidgetItem(s));

	s = QString("%1").arg(QString::number(p->xZ.toDouble(), 'f', 2));
	ui.mAirTable->setItem(row, c++, new QTableWidgetItem(s));

	s = QString("%1").arg(QString::number(p->hZ.toDouble(), 'f', 2));
	ui.mAirTable->setItem(row, c++, new QTableWidgetItem(s));

	s = QString("%1").arg(QString::number(p->Pitch.toDouble(), 'f', 2));
	ui.mAirTable->setItem(row, c++, new QTableWidgetItem(s));

	s = QString("%1").arg(QString::number(p->Roll.toDouble(), 'f', 2));
	ui.mAirTable->setItem(row, c++, new QTableWidgetItem(s));

	s = QString("%1").arg(QString::number(p->Yaw.toDouble(), 'f', 2));
	ui.mAirTable->setItem(row, c++, new QTableWidgetItem(s));

	s = QString("%1").arg(QString::number(p->airSpeed.toDouble(), 'f', 2));
	ui.mAirTable->setItem(row, c++, new QTableWidgetItem(s));	
}
//实时修改表格无人机数据
void DlgTuili::ModifyTable(tag_PlaneMessage* p)
{
	int row = ui.mAirTable->rowCount();

	for (int i = 0; i < row; i++)
	{
		if (p->ID == ui.mAirTable->item(i, 0)->text())
		{
			//经度，纬度，相对高度，海拔高度
			QString s = QString("%1").arg(QString::number(p->planeX.toDouble(), 'f', 5));
			ui.mAirTable->setItem(i, 1, new QTableWidgetItem(s));

			s = QString("%1").arg(QString::number(p->planeY.toDouble(), 'f', 5));
			ui.mAirTable->setItem(i, 2, new QTableWidgetItem(s));

			s = QString("%1").arg(QString::number(p->xZ.toDouble(), 'f', 2));
			ui.mAirTable->setItem(i, 3, new QTableWidgetItem(s));

			s = QString("%1").arg(QString::number(p->hZ.toDouble(), 'f', 2));
			ui.mAirTable->setItem(i, 4, new QTableWidgetItem(s));

			s = QString("%1").arg(QString::number(p->Pitch.toDouble(), 'f', 2));
			ui.mAirTable->setItem(i, 5, new QTableWidgetItem(s));

			s = QString("%1").arg(QString::number(p->Roll.toDouble(), 'f', 2));
			ui.mAirTable->setItem(i, 6, new QTableWidgetItem(s));

			s = QString("%1").arg(QString::number(p->Yaw.toDouble(), 'f', 2));
			ui.mAirTable->setItem(i, 7, new QTableWidgetItem(s));

			s = "";
			if (m_pTuiliThread->m_iJQactive == 1)
			{
				s = QString::fromLocal8Bit("起飞");
				auto end = steady_clock::now(); // 获取结束时间点
				int dur = duration_cast<seconds>(end - m_pTuiliThread->m_activeTime).count(); // 计算持续时间秒

				if(dur>11)
					s= QString::fromLocal8Bit("诱饵欺骗");
			}

			if (m_pTuiliThread->m_iJQactive == 2)
			{
				s = QString::fromLocal8Bit("空中集结");
				auto end = steady_clock::now(); // 获取结束时间点
				int dur = duration_cast<seconds>(end - m_pTuiliThread->m_activeTime).count(); // 计算持续时间秒

				if (dur > 11)
					s = QString::fromLocal8Bit("诱饵欺骗");
			}

			if (m_pTuiliThread->m_iJQactive == 3)
				s = QString::fromLocal8Bit("诱饵欺骗");
			
			if (m_pTuiliThread->m_iJQactive == 4)
			{
				s = QString::fromLocal8Bit("低空突防");
				auto end = steady_clock::now(); // 获取结束时间点
				int dur = duration_cast<seconds>(end - m_pTuiliThread->m_activeTime).count(); // 计算持续时间秒

				if (dur > 11)
					s = QString::fromLocal8Bit("诱饵欺骗");
			}
			
			ui.mAirTable->setItem(i, 8, new QTableWidgetItem(s));
			break;
		}
	}

	//更新无人机数据
	m_pTuiliThread->m_Mutex.lock();
	for (int i = 0; i < m_pTuiliThread->m_planeVec.size(); i++)
	{
		if (p->ID == m_pTuiliThread->m_planeVec[i].ID)
		{
			m_pTuiliThread->m_planeVec[i].planeX = p->planeX;
			m_pTuiliThread->m_planeVec[i].planeY = p->planeY;
			m_pTuiliThread->m_planeVec[i].xZ = p->xZ;
			m_pTuiliThread->m_planeVec[i].airSpeed = p->airSpeed;
			break;
		}		
	}
	m_pTuiliThread->m_Mutex.unlock();
}