#include "stdafx.h"
#include <QFileDialog>
#include "DlgAddEquipment.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <qgsProject.h>
#include "comm.h"
#include <string>
#include <vector>

using namespace std::chrono;
using namespace std;

DlgAddEquipment::DlgAddEquipment(QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.okBtn,		&QAbstractButton::clicked, this, &DlgAddEquipment::on_okBtn_click);			//筛选命令按钮
	connect(ui.pushButton,	&QAbstractButton::clicked, this, &DlgAddEquipment::on_addImageBtn_click);	//添加图片按钮
	
	ui.comboBox->addItem(QString::fromLocal8Bit("多旋翼A型"));
	ui.comboBox->addItem(QString::fromLocal8Bit("多旋翼B型"));
	ui.comboBox->addItem(QString::fromLocal8Bit("多旋翼C型"));
	ui.comboBox->addItem(QString::fromLocal8Bit("多旋翼D型"));


	ui.comboBox_2->addItem(QString::fromLocal8Bit("固定翼A型"));
	ui.comboBox_2->addItem(QString::fromLocal8Bit("固定翼B型"));
	ui.comboBox_2->addItem(QString::fromLocal8Bit("固定翼C型"));
	ui.comboBox_2->addItem(QString::fromLocal8Bit("固定翼D型"));
	ui.comboBox_2->addItem(QString::fromLocal8Bit("复合翼A型"));
	ui.comboBox_2->addItem(QString::fromLocal8Bit("复合翼B型"));
	ui.comboBox_2->addItem(QString::fromLocal8Bit("复合翼C型"));
	ui.comboBox_2->addItem(QString::fromLocal8Bit("复合翼D型"));
	
	QStringList sortedOptions;
	sortedOptions <<"";
	sortedOptions.sort();
	foreach(const QString &option, sortedOptions) {
		//ui.comboBox->addItem(option);
	}

	this->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint);
	//central->addWidget(ui.mAirTable, 2, 0, 1, 1, Qt::AlignBottom);
	//MainWindow *pDlg = (MainWindow*)this->parentWidget();
	//m_pModel = new QStandardItemModel();
	//ui.listView->setModel(m_pModel);

	ui.tabWidget->setTabText(0, QString::fromLocal8Bit("多旋翼")); // 设置第一个页面的标题
	ui.tabWidget->setTabText(1, QString::fromLocal8Bit("固定翼")); // 设置第一个页面的标题
	ui.tabWidget->setCurrentIndex(0);
}

DlgAddEquipment::~DlgAddEquipment()
{}


//添加图片
void DlgAddEquipment::on_addImageBtn_click()
{	
	m_strFile = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("打开文件"), QCoreApplication::applicationDirPath(),
		"image files (*jpg);;All files (*.*)");

	if (!m_strFile.isEmpty()) {
		//QRect rc = ui.labelImage->rect();
		int width = ui.labelImage->width();
		int height = ui.labelImage->height();
		QPixmap pixmap(m_strFile); // 使用资源文件路径或绝对路径
		QPixmap scaledPixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation); // 缩放图片

		ui.labelImage->setPixmap(scaledPixmap);
		ui.labelImage->setGeometry(348, 15, width, height+15); // 设置label的大小和位置
	}
}

void DlgAddEquipment::on_okBtn_click()
{
	if (ui.tabWidget->currentIndex() == 0)
	{
		insert0();//添加多旋翼
		return;
	}
	
	//添加固定翼
	QString type=ui.comboBox_2->currentText();
	QString num = ui.textEdit_17->toPlainText();		//在库数量
	QString lookWidth = ui.textEdit_20->toPlainText();	//侦查带宽
	QString len = ui.textEdit_16->toPlainText();		//长度
	QString width = ui.textEdit_18->toPlainText();		//宽度
	QString height = ui.textEdit_19->toPlainText();		//高度
	QString yiZhan = ui.textEdit_27->toPlainText();		//翼展
	QString flyWeight = ui.textEdit_21->toPlainText();	//起飞重量
	QString flyHeight = ui.textEdit_22->toPlainText();	//起飞工作高度
	QString bj = ui.textEdit_23->toPlainText();			//飞行半径	
	QString xhTime = ui.textEdit_24->toPlainText();		//空载飞行续航时间
	QString maxWeight = ui.textEdit_15->toPlainText();	//最大任务载重
	QString flyType = ui.textEdit_25->toPlainText();	//起飞方式
	QString backType = ui.textEdit_26->toPlainText();	//回收方式
	QString sSpeed = ui.textEdit_28->toPlainText();	//速度
	
	if (type == "")
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请选择类型   ！！"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (num == "")
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入在库数量   ！！"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (len == "")
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入长度   ！！"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (width == "")
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入无人机宽度   ！！"), QMessageBox::StandardButton::Yes);
		return;
	}

	if (height == "")
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入无人机高度   ！！"), QMessageBox::StandardButton::Yes);
		return;
	}
	

	QString sql = "insert into EquipmentB(type,num,lookWidth,len,width,height,yiZhan,flyWeight,flyHeight,ctrlRadius,flyTime, \
			maxWeight,flyType,backType,other";

	if (!m_strFile.isEmpty())
		sql += ",image";

	sql+=")values (:type,:num,:lookWidth,:len,:width,:height,:yiZhan,:flyWeight,:flyHeight,:ctrlRadius,:flyTime,:maxWeight,:flyType,:backType,:other";

	if (!m_strFile.isEmpty())
		sql += ",:image";

	sql += ")";

	QSqlQuery sql_query(g_sqliteDbase);
	sql_query.prepare(sql);
	sql_query.bindValue(":type", type);			//字符串，区分大小写
	sql_query.bindValue(":num", num);
	sql_query.bindValue(":lookWidth", lookWidth);
	sql_query.bindValue(":len", len);
	sql_query.bindValue(":width", width);
	sql_query.bindValue(":height",height);
	sql_query.bindValue(":yiZhan", yiZhan);	
	sql_query.bindValue(":flyWeight", flyWeight);
	sql_query.bindValue(":flyHeight", flyHeight);
	sql_query.bindValue(":ctrlRadius", bj);
	sql_query.bindValue(":flyTime", xhTime);	
	sql_query.bindValue(":maxWeight", maxWeight);	
	sql_query.bindValue(":flyType", flyType);
	sql_query.bindValue(":backType", backType);
	sql_query.bindValue(":other", sSpeed);

	QByteArray	array;
	if (!m_strFile.isEmpty())
	{
		QPixmap		pixmap(m_strFile);
		QBuffer		buffer(&array);
		buffer.open(QIODevice::WriteOnly);
		pixmap.save(&buffer, "JPG");
		sql_query.bindValue(":image", array);		//无人机图像，二进制数据
	}


	bool b = sql_query.exec();
	if (!b)
	{
		qDebug() << "InsertDBThread in: fail...." << endl;
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("添加装备 失败   ！！"), QMessageBox::StandardButton::Yes);
	}
	else
	{
		qDebug() << "InsertDBThread in: success...." << endl;
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("添加无人机装备 成功    ！！"), QMessageBox::StandardButton::Yes);
	}
}

//添加多旋翼
void DlgAddEquipment::insert0()
{
	QString type = ui.comboBox->currentText();
	QString num = ui.textEdit_1->toPlainText();			//在库数量
	QString lookWidth = ui.textEdit_11->toPlainText();	//侦查带宽
	QString len = ui.textEdit_2->toPlainText();			//长度
	QString width = ui.textEdit_3->toPlainText();		//宽度
	QString height = ui.textEdit_4->toPlainText();		//高度
	QString bj = ui.textEdit_5->toPlainText();			//飞行半径
	QString maxSpd = ui.textEdit_6->toPlainText();		//最大速度
	QString xhTime = ui.textEdit_7->toPlainText();		//最大飞行，续航时间
	QString workHeight = ui.textEdit_8->toPlainText();	//工作高度	
	QString maxWeight = ui.textEdit_10->toPlainText();	//最大载重
	QString kjg1 = ui.textEdit_12->toPlainText();		//可见光1
	QString kjg2 = ui.textEdit_13->toPlainText();		//可见光2
	QString kjg3 = ui.textEdit_14->toPlainText();		//可见光3

	if (type == "")
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请选择类型   ！！"), QMessageBox::StandardButton::Yes);
		return;
	}

	if (num == "")
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入在库数量   ！！"), QMessageBox::StandardButton::Yes);
		return;
	}

	if (len == "")
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入长度   ！！"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (width == "")
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入无人机宽度   ！！"), QMessageBox::StandardButton::Yes);
		return;
	}

	if (height == "")
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入无人机高度   ！！"), QMessageBox::StandardButton::Yes);
		return;
	}

	QString sql = "insert into EquipmentA(type,Num,lookWidth,len,width,height,ctrlRadius,MaxSpeed,xhTime,workHeight, \
		maxWeight,kejian1,kejian2,kejian3";

	if (!m_strFile.isEmpty())
		sql += ",image";

	sql += ") values (:type,:num,:lookWidth,:len,:width,:height,:ctrlRadius,:MaxSpeed,:xhTime,:workHeight,:maxWeight,\
		:kejian1,:kejian2,:kejian3";

	if (!m_strFile.isEmpty())
		sql += ",:image";

	sql += ")";

	QSqlQuery sql_query(g_sqliteDbase);
	sql_query.prepare(sql);
	sql_query.bindValue(":type", type);
	sql_query.bindValue(":num", num);
	sql_query.bindValue(":lookWidth", lookWidth);	
	sql_query.bindValue(":len", len);
	sql_query.bindValue(":width", width);
	sql_query.bindValue(":height", height);
	sql_query.bindValue(":ctrlRadius", bj);
	sql_query.bindValue(":MaxSpeed", maxSpd);
	sql_query.bindValue(":xhTime", xhTime);
	sql_query.bindValue(":workHeight", workHeight);	
	sql_query.bindValue(":maxWeight", maxWeight);
	sql_query.bindValue(":kejian1", kjg1);
	sql_query.bindValue(":kejian2", kjg2);
	sql_query.bindValue(":kejian3", kjg3);

	QByteArray	array;
	if (!m_strFile.isEmpty())
	{
		QPixmap		pixmap(m_strFile);
		QBuffer		buffer(&array);
		buffer.open(QIODevice::WriteOnly);
		pixmap.save(&buffer, "JPG");
		sql_query.bindValue(":image", array);		//无人机图像，二进制数据
	}


	bool b = sql_query.exec();
	if (!b)
	{
		qDebug() << "InsertDBThread in: fail...." << endl;
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("添加装备 失败   ！！"), QMessageBox::StandardButton::Yes);
	}
	else
	{
		qDebug() << "InsertDBThread in: success...." << endl;
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("添加无人机装备 成功    ！！"), QMessageBox::StandardButton::Yes);
	}
}