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
	ui.mAirTable->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("装备")));
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

	// 读取设备类型名称列表（光学|雷达|遥测|微波…）
	QString mPath = QCoreApplication::applicationDirPath();
	QSettings cfg(mPath + "/config.ini", QSettings::IniFormat);
	QStringList typeNames = cfg.value("DeviceType/list").toString().split("|");

	// 按类别排列：逐类型层遍历，每层内按 ID 数值升序，类型之间保持原始顺序
	for (int i = 0; i < gRadarLayerList.size(); i++)
	{
		QString typeName = (i < typeNames.size()) ? typeNames[i] : QString::number(i);
		QgsFeatureIterator fit = gRadarLayerList[i]->getFeatures();
		QList<int> ids;
		while (fit.nextFeature(feat))
			ids << feat.attribute(0).toInt();
		std::sort(ids.begin(), ids.end());
		for (int id : ids)
			ui.comboBox->addItem(QString("%1 (%2)").arg(id).arg(typeName));
	}

	this->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
}

Dialog_AirList::~Dialog_AirList()
{}

// 查找雷达设备对应的类型名，返回 "ID (类型名)" 格式字符串
static QString radarDisplayName(const QString &radarID)
{
	int rid = radarID.toInt();
	QString mPath = QCoreApplication::applicationDirPath();
	QSettings cfg(mPath + "/config.ini", QSettings::IniFormat);
	QStringList typeNames = cfg.value("DeviceType/list").toString().split("|");
	for (int i = 0; i < gRadarLayerList.size(); i++)
	{
		QgsFeatureIterator fit = gRadarLayerList[i]->getFeatures(
			QString("\"electircid\"=%1").arg(rid));
		QgsFeature f;
		if (fit.nextFeature(f))
		{
			QString typeName = (i < typeNames.size()) ? typeNames[i] : QString::number(i);
			return QString("%1 (%2)").arg(rid).arg(typeName);
		}
	}
	return radarID;  // 找不到类型则原样显示
}

// 添加到列表控件中；按无人机ID从小到大排序插入
void Dialog_AirList::insert(QString airID, QString radarID)
{
	QDateTime time = QDateTime::currentDateTime();
	QString st = time.toString("yyyy-MM-dd hh:mm:ss.zzz");
	QString radarDisplay = radarDisplayName(radarID);
	int aid = airID.toInt();

	// 找到第一行中无人机ID >= aid 的位置，保持升序
	int row = ui.mAirTable->rowCount();
	for (int i = 0; i < ui.mAirTable->rowCount(); i++)
	{
		QTableWidgetItem *it = ui.mAirTable->item(i, 2);
		if (it && it->text().toInt() >= aid) { row = i; break; }
	}

	ui.mAirTable->insertRow(row);
	ui.mAirTable->setItem(row, 0, new QTableWidgetItem(st));
	ui.mAirTable->item(row, 0)->setTextAlignment(Qt::AlignVCenter | Qt::AlignLeft);
	ui.mAirTable->setItem(row, 1, new QTableWidgetItem(radarDisplay));
	ui.mAirTable->item(row, 1)->setTextAlignment(Qt::AlignCenter);
	ui.mAirTable->setItem(row, 2, new QTableWidgetItem(airID));
	ui.mAirTable->item(row, 2)->setTextAlignment(Qt::AlignCenter);

	// 按当前筛选隐藏不匹配的行（按ID比较，不依赖显示字符串格式）
	if (m_devID > 0 && radarID.toInt() != m_devID)
		ui.mAirTable->setRowHidden(row, true);

	if (!ui.mAirTable->isRowHidden(row))
		ui.mAirTable->scrollToBottom();
}

// 退出
void Dialog_AirList::on_exitBtn_click()
{
	MainWindow *pDlg = (MainWindow*)this->parentWidget();
}

void Dialog_AirList::on_queryBtn_click()
{
	QString sel = ui.comboBox->currentText();
	if (sel == QString::fromLocal8Bit("全部"))
	{
		m_devID = 0;
	}
	else
	{
		// comboBox 文本格式为 "100 (光学)"，取空格前的数字部分
		m_devID = sel.section(' ', 0, 0).toInt();
	}

	// 按隐藏/显示过滤行（按装备ID数值比较，不依赖显示字符串格式）
	int visCount = 0;
	for (int i = 0; i < ui.mAirTable->rowCount(); i++)
	{
		QTableWidgetItem *pitem = ui.mAirTable->item(i, 1);
		int cellDevID = pitem ? pitem->text().section(' ', 0, 0).toInt() : 0;
		bool show = (m_devID == 0) || (cellDevID == m_devID);
		ui.mAirTable->setRowHidden(i, !show);
		if (show) visCount++;
	}
	// 若筛选后无匹配行，回退到"全部"并提示
	if (m_devID != 0 && visCount == 0)
	{
		m_devID = 0;
		for (int i = 0; i < ui.mAirTable->rowCount(); i++)
			ui.mAirTable->setRowHidden(i, false);
		ui.comboBox->setCurrentIndex(0);  // 切回"全部"
	}
}
