#include "stdafx.h"
#include "dataBackDlg.h"
#include <QShowEvent>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include "comm.h"
#include "mainWindow.h"
#include "mavlink5Thread.h"

dataBackDlg::dataBackDlg(QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.addBtn,        &QAbstractButton::clicked, this, &dataBackDlg::on_addBtn_click);
	connect(ui.deleteBtn,     &QAbstractButton::clicked, this, &dataBackDlg::on_deleteBtn_click);
	connect(ui.queryIdBtn,    &QAbstractButton::clicked, this, &dataBackDlg::on_queryIdBtn_click);
	connect(ui.pushQueryBtn,  &QAbstractButton::clicked, this, &dataBackDlg::on_queryBtn_click);
	connect(ui.pushExitBtn,   &QAbstractButton::clicked, this, &dataBackDlg::on_exitBtn_click);
	connect(ui.pushPauseBtn,  &QAbstractButton::clicked, this, &dataBackDlg::on_pauseBtn_click);
	connect(ui.pushExportBtn, &QAbstractButton::clicked, this, &dataBackDlg::on_exportBtn_click);

	// Dynamic speed: update thread's speed factor immediately when combo changes
	connect(ui.speedCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
		this, [this](int idx) {
			static const double kSpeeds[] = {0.5, 1.0, 2.0, 5.0, 10.0};
			if (m_pMavThread && idx >= 0 && idx < 5)
				m_pMavThread->m_speedFactor = kSpeeds[idx];
		});

	ui.mDateTimeEditbegin->setDateTime(QDateTime::currentDateTime().addDays(-2));
	ui.mDateTimeEditEnd->setDateTime(QDateTime::currentDateTime());

	m_pModel = new QStandardItemModel();
	ui.listView->setModel(m_pModel);
}

dataBackDlg::~dataBackDlg()
{}

// Refresh end-time to "now" each time the dialog becomes visible,
// so newly recorded data is always within the default query range.
void dataBackDlg::showEvent(QShowEvent *e)
{
	QDialog::showEvent(e);
	ui.mDateTimeEditEnd->setDateTime(QDateTime::currentDateTime());
}

void dataBackDlg::on_addBtn_click()
{
	if (ui.lineEdit->text() == "")
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle(QString::fromUtf8("\346\217\220\347\244\272"));  // 提示
		msgBox.setText(QString::fromUtf8("  \350\257\267\350\276\223\345\205\245\351\243\236\346\234\272ID\357\274\201 "));  // 请输入飞机ID！
		msgBox.exec();
		return;
	}

	QString s = ui.lineEdit->text();
	bool b = false;
	for (int i = 0; i < m_pModel->rowCount(); ++i)
	{
		if (m_pModel->item(i)->text() == s)
			b = true;
	}

	if (b)
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle(QString::fromUtf8("\346\217\220\347\244\272"));  // 提示
		msgBox.setText(QString::fromUtf8("  \345\267\262\345\255\230\345\234\250\350\257\245\351\243\236\346\234\272ID\357\274\201 "));  // 已存在该飞机ID！
		msgBox.exec();
		return;
	}
	m_pModel->appendRow(new QStandardItem(s));
}

void dataBackDlg::on_deleteBtn_click()
{
	QItemSelectionModel *sel = ui.listView->selectionModel();
	QModelIndexList selected = sel->selectedIndexes();

	foreach(const QModelIndex &index, selected)
		m_pModel->removeRow(index.row());
}

// Query available AirIDs from the database within the current time range
// and append any not already in the list.
void dataBackDlg::on_queryIdBtn_click()
{
	QString begin = ui.mDateTimeEditbegin->dateTime().toString("yyyy-MM-dd hh:mm:ss");
	QString end   = ui.mDateTimeEditEnd->dateTime().toString("yyyy-MM-dd hh:mm:ss");

	QSqlDatabase db = QSqlDatabase::database("sqlite3");
	QSqlQuery query(db);
	bool ok = query.exec(
		QString("SELECT DISTINCT AirID FROM mavlink "
			"WHERE time>='%1' AND time<='%2' ORDER BY AirID").arg(begin).arg(end));
	if (!ok)
		return;

	while (query.next())
	{
		QString id = query.value(0).toString();
		bool exists = false;
		for (int i = 0; i < m_pModel->rowCount(); ++i)
		{
			if (m_pModel->item(i)->text() == id)
			{
				exists = true;
				break;
			}
		}
		if (!exists)
			m_pModel->appendRow(new QStandardItem(id));
	}
}

// 回放查询
void dataBackDlg::on_queryBtn_click()
{
	QString begin = ui.mDateTimeEditbegin->dateTime().toString("yyyy-MM-dd hh:mm:ss");
	QString end   = ui.mDateTimeEditEnd->dateTime().toString("yyyy-MM-dd hh:mm:ss");

	QString timesection = " and (time>='%1' and time<='%2')";
	timesection = timesection.arg(begin).arg(end);
	QString sql = "select * from mavlink where (1 = 1)" + timesection;

	QString sqlID;
	for (int i = 0; i < m_pModel->rowCount(); ++i)
		sqlID += " AirID='" + m_pModel->item(i)->text() + "' OR";

	if (!sqlID.isEmpty())
	{
		sqlID = sqlID.left(sqlID.length() - 3);
		sqlID = " and (" + sqlID + ")";
	}

	sql += sqlID + " order by time, Id";

	MainWindow *pDlg = (MainWindow*)this->parentWidget();

	if (pDlg->m_bBacking)
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle(QString::fromUtf8("\346\217\220\347\244\272"));  // 提示
		msgBox.setText(QString::fromUtf8("  \345\275\223\345\211\215\346\255\243\345\234\250\345\233\236\346\224\276\346\227\240\344\272\272\346\234\272\346\225\260\346\215\256\357\274\201 "));  // 当前正在回放无人机数据！
		msgBox.exec();
		return;
	}

	pDlg->m_bBacking = true;
	this->setWindowTitle(QString::fromUtf8("\346\227\240\344\272\272\346\234\272\346\225\260\346\215\256\345\233\236\346\224\276\344\270\255...."));  // 无人机数据回放中....

	// Reset pause state for the new replay session
	m_pMavThread->m_bPaused = false;
	ui.pushPauseBtn->setText(QString::fromUtf8("\346\232\202\345\201\234"));  // 暂停

	// Pass speed factor from combo (0.5x/1x/2x/5x/10x)
	static const double kSpeeds[] = {0.5, 1.0, 2.0, 5.0, 10.0};
	int idx = ui.speedCombo->currentIndex();
	m_pMavThread->m_speedFactor = (idx >= 0 && idx < 5) ? kSpeeds[idx] : 1.0;

	m_pMavThread->m_sql = sql;
	m_pMavThread->start();
}

// 退出，停止回放
void dataBackDlg::on_exitBtn_click()
{
	// Stop the replay thread gracefully via the stop flag
	m_pMavThread->m_bStopped = true;
	if (!m_pMavThread->wait(2000))
	{
		m_pMavThread->terminate();
		m_pMavThread->wait();
	}

	// Drain any signal_modifyPlane events already queued in the Qt event loop
	// while m_bBacking is still true so registerPlane() handles them safely.
	QCoreApplication::processEvents(QEventLoop::AllEvents, 300);

	m_pMavThread->m_bStopped = false;

	MainWindow *pDlg = (MainWindow*)this->parentWidget();
	pDlg->m_bBacking = false;

	this->hide();
}

void dataBackDlg::on_setTitle()
{
	setWindowTitle(QString::fromUtf8("\346\227\240\344\272\272\346\234\272\346\225\260\346\215\256\345\233\236\346\224\276\345\256\214...."));  // 无人机数据回放完....
}

// Toggle replay pause / resume
void dataBackDlg::on_pauseBtn_click()
{
	if (!m_pMavThread->m_bPaused)
	{
		m_pMavThread->m_bPaused = true;
		ui.pushPauseBtn->setText(QString::fromUtf8("\347\273\247\347\273\255"));  // 继续
	}
	else
	{
		m_pMavThread->m_bPaused = false;
		ui.pushPauseBtn->setText(QString::fromUtf8("\346\232\202\345\201\234"));  // 暂停
	}
}

// Export mavlink table data (matching current time/ID filters) to CSV or TXT
void dataBackDlg::on_exportBtn_click()
{
	// Snap end time to now so any data recorded since the dialog was opened is included
	ui.mDateTimeEditEnd->setDateTime(QDateTime::currentDateTime());

	QString begin = ui.mDateTimeEditbegin->dateTime().toString("yyyy-MM-dd hh:mm:ss");
	QString end   = ui.mDateTimeEditEnd->dateTime().toString("yyyy-MM-dd hh:mm:ss");

	QString timesection = " and (time>='%1' and time<='%2')";
	timesection = timesection.arg(begin).arg(end);
	QString sql = "select * from mavlink where (1 = 1)" + timesection;

	QString sqlID;
	for (int i = 0; i < m_pModel->rowCount(); ++i)
		sqlID += " AirID='" + m_pModel->item(i)->text() + "' OR";

	if (!sqlID.isEmpty())
	{
		sqlID = sqlID.left(sqlID.length() - 3);
		sqlID = " and (" + sqlID + ")";
	}
	sql += sqlID + " order by time, Id";

	QString filePath = QFileDialog::getSaveFileName(
		this,
		QString::fromUtf8("\345\257\274\345\207\272\346\227\245\345\277\227"),  // 导出日志
		QString(),
		"CSV Files (*.csv);;Text Files (*.txt);;All Files (*)");
	if (filePath.isEmpty())
		return;

	QSqlDatabase db = QSqlDatabase::database("sqlite3");
	QSqlQuery query(db);
	if (!query.exec(sql))
	{
		QMessageBox::warning(this, "Error",
			QString::fromUtf8("\346\237\245\350\257\242\345\244\261\350\264\245"));  // 查询失败
		return;
	}

	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QMessageBox::warning(this, "Error",
			QString::fromUtf8("\346\211\223\345\274\200\346\226\207\344\273\266\345\244\261\350\264\245"));  // 打开文件失败
		return;
	}
	QTextStream out(&file);
	out.setCodec("UTF-8");
	out << "\xEF\xBB\xBF";  // UTF-8 BOM for Excel compatibility

	bool isCsv = filePath.endsWith(".csv", Qt::CaseInsensitive);
	QString sep = isCsv ? "," : "\t";

	// Header row — matches the live display table columns
	out << "Id"                                                              << sep
		<< QString::fromUtf8("\351\243\236\346\234\272ID")                   << sep  // 飞机ID
		<< QString::fromUtf8("\347\273\217\345\272\246")                     << sep  // 经度
		<< QString::fromUtf8("\347\272\254\345\272\246")                     << sep  // 纬度
		<< QString::fromUtf8("\347\233\270\345\257\271\351\253\230\345\272\246") << sep  // 相对高度
		<< QString::fromUtf8("\346\265\267\346\213\224\351\253\230\345\272\246") << sep  // 海拔高度
		<< "Pitch"                                                           << sep
		<< "Roll"                                                            << sep
		<< "Yaw"                                                             << sep
		<< QString::fromUtf8("\347\251\272\351\200\237")                     << sep  // 空速
		<< QString::fromUtf8("\345\234\260\351\200\237")                     << sep  // 地速
		<< QString::fromUtf8("\345\214\227\351\200\237vN")                   << sep  // 北速
		<< QString::fromUtf8("\344\270\234\351\200\237vE")                   << sep  // 东速
		<< QString::fromUtf8("\345\244\251\351\200\237vU")                   << sep  // 天速
		<< QString::fromUtf8("gps\347\212\266\346\200\201")                  << sep  // gps状态
		<< QString::fromUtf8("\347\224\265\345\216\213")                     << sep  // 电压
		<< QString::fromUtf8("\346\230\237\346\225\260")                     << sep  // 星数
		<< QString::fromUtf8("\344\273\273\345\212\241\347\261\273\345\236\213") << sep  // 任务类型
		<< QString::fromUtf8("\350\275\250\350\277\271\346\250\241\345\274\217")  << sep  // 轨迹模式
		<< QString::fromUtf8("\347\276\244\346\216\247\350\277\220\350\241\214\346\250\241\345\274\217") << sep  // 群控运行模式
		<< QString::fromUtf8("\347\276\244\346\216\247\346\214\207\344\273\244\346\250\241\345\274\217") << sep  // 群控指令模式
		<< QString::fromUtf8("\351\243\236\346\216\247\350\277\220\350\241\214\346\250\241\345\274\217") << sep  // 飞控运行模式
		<< QString::fromUtf8("\351\233\206\347\276\244\345\220\214\346\255\245\346\227\266\351\227\264") << sep  // 集群同步时间
		<< QString::fromUtf8("\346\227\266\351\227\264")                     << "\n";  // 时间

	int rowCount = 0;
	while (query.next())
	{
		out << query.value("Id").toString()        << sep
			<< query.value("AirID").toString()     << sep
			<< query.value("X").toString()         << sep
			<< query.value("Y").toString()         << sep
			<< query.value("xZ").toString()        << sep
			<< query.value("Z").toString()         << sep
			<< query.value("pitch").toString()     << sep
			<< query.value("roll").toString()      << sep
			<< query.value("yaw").toString()       << sep
			<< query.value("airSpeed").toString()  << sep
			<< query.value("groundSpd").toString() << sep
			<< query.value("vN").toString()        << sep
			<< query.value("vE").toString()        << sep
			<< query.value("vU").toString()        << sep
			<< query.value("gpsStatus").toString() << sep
			<< query.value("voltage").toString()   << sep
			<< query.value("gpsNum").toString()    << sep
			<< query.value("taskType").toString()  << sep
			<< query.value("gjMode").toString()    << sep
			<< query.value("qkRunMode").toString() << sep
			<< query.value("qkCmdMode").toString() << sep
			<< query.value("fkRunMode").toString() << sep
			<< query.value("jqtbTime").toString()  << sep
			<< query.value("time").toString()      << "\n";
		++rowCount;
	}
	file.close();

	QMessageBox::information(this,
		QString::fromUtf8("\345\257\274\345\207\272\346\210\220\345\212\237"),  // 导出成功
		QString::fromUtf8("\345\205\261\345\257\274\345\207\272 %1 \346\235\241\350\256\260\345\275\225\343\200\202\n")  // 共导出 %1 条记录。\n
			.arg(rowCount)
			+ filePath);
}
