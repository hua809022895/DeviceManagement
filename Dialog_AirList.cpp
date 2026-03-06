#include "stdafx.h"
#include "dialog_AirList.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <qgsProject.h>
#include "comm.h"
#include <QHeaderView>

Dialog_AirList::Dialog_AirList(QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.ok_Button, &QAbstractButton::clicked, this, &Dialog_AirList::on_queryBtn_click);	// 筛选按钮
	connect(ui.pushExitBtn, &QAbstractButton::clicked, this, &Dialog_AirList::on_exitBtn_click);	// 退出按钮

	// 初始化无人机列表控件
	int i = 0;
	ui.mAirTable->setColumnCount(3);
	ui.mAirTable->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("时间")));
	ui.mAirTable->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("雷达ID")));
	ui.mAirTable->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("无人机ID")));

	// 列宽自适应：时间列拉伸填满，ID列按内容自适应
	QHeaderView *hHeader = ui.mAirTable->horizontalHeader();
	hHeader->setSectionResizeMode(0, QHeaderView::Stretch);
	hHeader->setSectionResizeMode(1, QHeaderView::ResizeToContents);
	hHeader->setSectionResizeMode(2, QHeaderView::ResizeToContents);
	hHeader->setMinimumSectionSize(60);

	ui.mAirTable->verticalHeader()->setVisible(false);
	ui.mAirTable->verticalHeader()->setDefaultSectionSize(22);	// 行高紧凑
	ui.mAirTable->setSelectionBehavior(QAbstractItemView::SelectRows);		// 整行选中
	ui.mAirTable->setSelectionMode(QAbstractItemView::SingleSelection);		// 单选模式
	ui.mAirTable->setAlternatingRowColors(true);							// 交替行颜色
	ui.mAirTable->setEditTriggers(QAbstractItemView::NoEditTriggers);		// 只读
	ui.mAirTable->setWordWrap(false);

	ui.comboBox->addItem(QString::fromLocal8Bit("全部"));
	QgsFeature feat;
	QStringList sortedOptions;

	for (int i = 0; i < gRadarLayerList.size(); i++)
	{
		QgsFeatureIterator fit = gRadarLayerList[i]->getFeatures();
		while (fit.nextFeature(feat))
		{
			QString		sID = feat.attribute(0).toString();
			sortedOptions << sID;
		}
	}

	sortedOptions.sort();
	foreach(const QString &option, sortedOptions) {
		ui.comboBox->addItem(option);
	}

	this->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
}

Dialog_AirList::~Dialog_AirList()
{}

// 添加到列表控件中
void Dialog_AirList::insert(QString airID,QString radarID)
{
	QDateTime time = QDateTime::currentDateTime();
	QString st = time.toString("yyyy-MM-dd hh:mm:ss.zzz");
	if (m_devID > 0)
	{
		if (m_devID == radarID.toInt())
		{
			int row = ui.mAirTable->rowCount();
			ui.mAirTable->insertRow(row);
			// 当前时间
			ui.mAirTable->setItem(row, 0, new QTableWidgetItem(st));
			ui.mAirTable->item(row, 0)->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
			ui.mAirTable->setItem(row, 1, new QTableWidgetItem(radarID));
			ui.mAirTable->item(row, 1)->setTextAlignment(Qt::AlignCenter);
			ui.mAirTable->setItem(row, 2, new QTableWidgetItem(airID));
			ui.mAirTable->item(row, 2)->setTextAlignment(Qt::AlignCenter);
		}
	}
	else
	{
		int row = ui.mAirTable->rowCount();
		ui.mAirTable->insertRow(row);

		ui.mAirTable->setItem(row, 0, new QTableWidgetItem(st));	// 当前时间
		ui.mAirTable->item(row, 0)->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
		ui.mAirTable->setItem(row, 1, new QTableWidgetItem(radarID));
		ui.mAirTable->item(row, 1)->setTextAlignment(Qt::AlignCenter);
		ui.mAirTable->setItem(row, 2, new QTableWidgetItem(airID));
		ui.mAirTable->item(row, 2)->setTextAlignment(Qt::AlignCenter);
	}

	ui.mAirTable->scrollToBottom();
}

// 退出
void Dialog_AirList::on_exitBtn_click()
{
	MainWindow *pDlg = (MainWindow*)this->parentWidget();
}

void Dialog_AirList::on_queryBtn_click()
{
	ui.mAirTable->sortItems(1, Qt::AscendingOrder);	// 按雷达设备id排序

	// 列表中只显示指定雷达设备id的记录
	m_devID = ui.comboBox->currentText().toInt();

	if (m_devID == 0)
	{
		return;
	}

loop1:
	for (int i = 0; i < ui.mAirTable->rowCount(); i++)
	{
		QTableWidgetItem *pitem = ui.mAirTable->item(i, 1);
		if (m_devID != pitem->text().toInt())
		{
			ui.mAirTable->removeRow(i);
			goto loop1;
		}
	}
}
