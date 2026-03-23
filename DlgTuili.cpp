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

	connect(ui.ok_Button, &QAbstractButton::clicked, this, &DlgTuili::on_queryBtn_click);	//ɸѡ���ť
	//connect(ui.pushExitBtn, &QAbstractButton::clicked, this, &DlgTuili::on_exitBtn_click);	//��λ���ť
	//this->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint);
	//central->addWidget(ui.mAirTable, 2, 0, 1, 1, Qt::AlignBottom);
	//MainWindow *pDlg = (MainWindow*)this->parentWidget();
	//m_pModel = new QStandardItemModel();
	//ui.listView->setModel(m_pModel);
	m_pTuiliThread =new TuiliThread();

	m_timer = new QTimer(this);
	connect(m_timer, &QTimer::timeout, this, &DlgTuili::UpdateInterface);
	
	// 初始化推理表格控件
	int i = 0;
	ui.mAirTable->setColumnCount(9);
	ui.mAirTable->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u98de\u673aID")));
	ui.mAirTable->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u7ecf\u5ea6")));
	ui.mAirTable->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u7eac\u5ea6")));
	ui.mAirTable->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u76ee\u5bf9\u9ad8\u5ea6")));
	ui.mAirTable->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u6d77\u62d4\u9ad8\u5ea6")));
	ui.mAirTable->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("Pitch")));
	ui.mAirTable->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("Roll")));
	ui.mAirTable->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("Yaw")));
	ui.mAirTable->setHorizontalHeaderItem(i++, new QTableWidgetItem(QStringLiteral("\u72b6\u6001")));

	ui.mAirTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.mAirTable->verticalHeader()->setVisible(false);
	ui.mAirTable->setSelectionBehavior(QAbstractItemView::SelectRows);
	ui.mAirTable->setSelectionMode(QAbstractItemView::SingleSelection);
}

DlgTuili::~DlgTuili()
{}

void DlgTuili::on_queryBtn_click()
{
	if (m_pTuiliThread->m_isRun)
	{
		ui.ok_Button->setText(QStringLiteral("\u5f00\u59cb\u63a8\u7406"));
		ui.label->setText(QStringLiteral("\u5df2\u505c\u6b62\u63a8\u7406"));
		m_pTuiliThread->m_isRun = false;
		m_timer->stop();
		//ui.label_2->setText("");
	}
	else
	{
		ui.ok_Button->setText(QStringLiteral("\u505c\u6b62\u63a8\u7406"));
		ui.label->setText(QStringLiteral("\u6b63\u5728\u63a8\u7406\u4e2d."));
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
		ui.label->setText(QStringLiteral("\u6b63\u5728\u63a8\u7406\u4e2d"));
	
}

//�������˻�������ؼ���
void DlgTuili::insertToTable(tag_PlaneMessage* p)
{
	int c = 2;
	int row = ui.mAirTable->rowCount();
	ui.mAirTable->insertRow(row);

	ui.mAirTable->setItem(row, 0, new QTableWidgetItem(p->ID));//ID��
	ui.mAirTable->item(row, 0)->setTextAlignment(Qt::AlignTop);

	//���ȣ�γ�ȣ���Ը߶ȣ����θ߶�
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
//ʵʱ�޸ı������˻�����
void DlgTuili::ModifyTable(tag_PlaneMessage* p)
{
	int row = ui.mAirTable->rowCount();

	for (int i = 0; i < row; i++)
	{
		if (p->ID == ui.mAirTable->item(i, 0)->text())
		{
			//���ȣ�γ�ȣ���Ը߶ȣ����θ߶�
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
				s = QStringLiteral("\u653b\u51fb\u4e2d");
				auto end = steady_clock::now(); // ��ȡ����ʱ���
				int dur = duration_cast<seconds>(end - m_pTuiliThread->m_activeTime).count(); // �������ʱ����

				if(dur>11)
					s= QStringLiteral("\u6b3a\u9a97");
			}

			if (m_pTuiliThread->m_iJQactive == 2)
			{
				s = QStringLiteral("\u8fdb\u884c\u653b\u51fb");
				auto end = steady_clock::now(); // ��ȡ����ʱ���
				int dur = duration_cast<seconds>(end - m_pTuiliThread->m_activeTime).count(); // �������ʱ����

				if (dur > 11)
					s = QStringLiteral("\u6b3a\u9a97");
			}

			if (m_pTuiliThread->m_iJQactive == 3)
				s = QStringLiteral("\u6b3a\u9a97");
			
			if (m_pTuiliThread->m_iJQactive == 4)
			{
				s = QStringLiteral("\u951f\u9175\u5321\u62f7\u7a81\u951f\u65a4\u62f7");
				auto end = steady_clock::now(); // ��ȡ����ʱ���
				int dur = duration_cast<seconds>(end - m_pTuiliThread->m_activeTime).count(); // �������ʱ����

				if (dur > 11)
					s = QStringLiteral("\u6b3a\u9a97");
			}
			
			ui.mAirTable->setItem(i, 8, new QTableWidgetItem(s));
			break;
		}
	}

	//�������˻�����
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