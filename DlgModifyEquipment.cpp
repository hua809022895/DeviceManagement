#include "stdafx.h"
#include <QFileDialog>
#include "DlgModifyEquipment.h"
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

DlgModifyEquipment::DlgModifyEquipment(int id, bool bDuo, QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.okBtn,		&QAbstractButton::clicked, this, &DlgModifyEquipment::on_okBtn_click);			//筛选命令按钮
	connect(ui.pushButton,	&QAbstractButton::clicked, this, &DlgModifyEquipment::on_addImageBtn_click);	//添加图片按钮
		
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
	
	ui.tabWidget->setTabText(0, QString::fromLocal8Bit("多旋翼")); // 设置第一个页面的标题
	ui.tabWidget->setTabText(1, QString::fromLocal8Bit("固定翼")); // 设置第一个页面的标题

	m_bDuo = bDuo;

	if (m_bDuo)
		ui.tabWidget->setCurrentIndex(0);
	else
		ui.tabWidget->setCurrentIndex(1);

	QStringList sortedOptions;
	sortedOptions <<"";
	sortedOptions.sort();
	foreach(const QString &option, sortedOptions) {
		//ui.comboBox->addItem(option);
	}

	m_id = id;
	this->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint);
	
	QSqlQuery query(g_sqliteDbase);
	QString sql,sid = QString("%1").arg(m_id);
	sql = "select * from Equipment";
	if (m_bDuo)
		sql += "A";
	else
		sql += "B";

	sql += " where Id=" + sid;

	query.exec(sql);
	if (query.next())
	{
		m_jpg = query.value("image").toByteArray();
		if (m_jpg.size()> 100)
		{
			int width = ui.labelImage->width();
			int height = ui.labelImage->height();
			QPixmap pixmap;
			pixmap.loadFromData(m_jpg, "jpg");
			QPixmap pixmapcopy = pixmap.scaled(width, height);
			ui.labelImage->setPixmap(pixmapcopy);
			m_strFile = "AAA";//设置为有图像状态，只要不为空就行
		}

		QString  type = query.value("type").toString();
		QString  num = query.value("num").toString();
		QString  lookWidth = query.value("lookWidth").toString();		
		QString  len = query.value("len").toString();
		QString  width = query.value("width").toString();
		QString  height = query.value("height").toString();
		QString  bj = query.value("ctrlRadius").toString();				

		if (m_bDuo)
		{
			QString  maxSpd = query.value("MaxSpeed").toString();
			QString  flyTime = query.value("xhTime").toString();
			QString  workHeight = query.value("workHeight").toString();			
			QString  maxWeight = query.value("maxWeight").toString();
			QString  keJian1 = query.value("keJian1").toString();
			QString  keJian2 = query.value("keJian2").toString();
			QString  keJian3 = query.value("keJian3").toString();

			ui.comboBox->setCurrentText(type);
			ui.textEdit_1->setText(num);		//在库数量
			ui.textEdit_11->setText(lookWidth);	//侦查带宽
			ui.textEdit_2->setText(len);		//长度
			ui.textEdit_3->setText(width);		//宽度
			ui.textEdit_4->setText(height);		//高度
			ui.textEdit_5->setText(bj);			//飞行控制半径
			ui.textEdit_6->setText(maxSpd);		//最大飞行速度
			ui.textEdit_7->setText(flyTime);	//最大飞行，续航时间
			ui.textEdit_8->setText(workHeight);	//工作高度			
			ui.textEdit_10->setText(maxWeight);	//最大载重
			ui.textEdit_12->setText(keJian1);	//
			ui.textEdit_13->setText(keJian2);	//
			ui.textEdit_14->setText(keJian3);	//
		}
		else
		{
			QString  flyWeight = query.value("flyWeight").toString();	//起飞重量
			QString  flyHeight = query.value("flyHeight").toString();	//飞行高度
			QString  flyTime = query.value("flyTime").toString();		//空载飞行时间
			QString  maxWeight = query.value("maxWeight").toString();	//最大任务载荷
			QString  flyType = query.value("flyType").toString();		//起飞方式
			QString  backType = query.value("backType").toString();		//回收方式
			QString  yiZhan = query.value("yiZhan").toString();			//翼展
			QString  sSpeed = query.value("other").toString();			//速度

			ui.comboBox_2->setCurrentText(type);
			ui.textEdit_17->setText(num);		//在库数量
			ui.textEdit_20->setText(lookWidth);	//侦查带宽
			ui.textEdit_16->setText(len);		//长度
			ui.textEdit_18->setText(width);		//宽度
			ui.textEdit_19->setText(height);	//高度+
			ui.textEdit_21->setText(flyWeight);	//起飞重量
			ui.textEdit_22->setText(flyHeight);	//起飞
			ui.textEdit_23->setText(bj);		//飞行控制半径
			ui.textEdit_24->setText(flyTime);
			ui.textEdit_15->setText(maxWeight);
			ui.textEdit_25->setText(flyType);
			ui.textEdit_26->setText(backType);
			ui.textEdit_27->setText(yiZhan);
			ui.textEdit_28->setText(sSpeed);
		}		
	}
}

DlgModifyEquipment::~DlgModifyEquipment(){}

//添加图片
void DlgModifyEquipment::on_addImageBtn_click()
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
		ui.labelImage->setGeometry(340, 5, width, height+15); // 设置label的大小和位置

		QBuffer		buffer(&m_jpg);
		buffer.open(QIODevice::WriteOnly);
		pixmap.save(&buffer, "JPG");
	}
}

//保存修改
void DlgModifyEquipment::on_okBtn_click()
{
	if (!m_bDuo)
	{
		on_Bclick(); //是添加固定翼
		return;
	}

	//以下是添加多旋翼
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

	if (num == "")
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入在库数量   ！！"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (lookWidth == "")
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入侦查带宽   ！！"), QMessageBox::StandardButton::Yes);
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
	if (bj == "")
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入无人机飞行半径   ！！"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (maxSpd == "")
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入无人机 最大速度   ！！"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (xhTime == "")
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入无人机 续航时间   ！！"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (workHeight == "")
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入无人机 工作高度   ！！"), QMessageBox::StandardButton::Yes);
		return;
	}
	
	if (maxWeight == "")
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入无人机 最大载重   ！！"), QMessageBox::StandardButton::Yes);
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
	QString sid =QString("%1").arg(m_id);
	QString sqlDel = "Delete from EquipmentA where id=" + sid;

	sql_query.prepare(sqlDel);
	bool b = sql_query.exec();
	qDebug() << "delete DBThread ...."<<b << endl;

	sql_query.prepare(sql);
	sql_query.bindValue(":type", type);
	sql_query.bindValue(":len", len);
	sql_query.bindValue(":num", num);
	sql_query.bindValue(":lookWidth", lookWidth);

	sql_query.bindValue(":width", width);
	sql_query.bindValue(":height",height);
	sql_query.bindValue(":ctrlRadius", bj);
	sql_query.bindValue(":MaxSpeed", maxSpd);
	sql_query.bindValue(":xhTime", xhTime);
	sql_query.bindValue(":workHeight", workHeight);
	sql_query.bindValue(":maxWeight", maxWeight);
	sql_query.bindValue(":kejian1", kjg1);
	sql_query.bindValue(":kejian2", kjg2);
	sql_query.bindValue(":kejian3", kjg3);

	if (!m_strFile.isEmpty())
	{		
		sql_query.bindValue(":image", m_jpg);		//无人机图像，二进制数据
	}

	b = sql_query.exec();
	if (!b)
	{
		qDebug() << "InsertDBThread in: fail...." << endl;
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("保存 装备 失败   ！！"), QMessageBox::StandardButton::Yes);
	}
	else
	{
		qDebug() << "InsertDBThread in: success...." << endl;
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("保存无人机装备 成功    ！！"), QMessageBox::StandardButton::Yes);
		this->close();
	}
}

void DlgModifyEquipment::on_Bclick()//保存固定翼，b表
{
	//以下是添加固定翼，复合翼
	QString type = ui.comboBox_2->currentText();
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

	if (num == "")
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入在库数量   ！！"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (lookWidth == "")
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入侦查带宽   ！！"), QMessageBox::StandardButton::Yes);
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
	if (bj == "")
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入无人机飞行半径   ！！"), QMessageBox::StandardButton::Yes);
		return;
	}
	
	if (maxWeight == "")
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请输入无人机 最大载重   ！！"), QMessageBox::StandardButton::Yes);
		return;
	}

	QString sql = "insert into EquipmentB(type,num,lookWidth,len,width,height,yiZhan,flyWeight,flyHeight,ctrlRadius,flyTime, \
			maxWeight,flyType,backType,other";

	if (!m_strFile.isEmpty())
		sql += ",image";

	sql += ")values (:type,:num,:lookWidth,:len,:width,:height,:yiZhan,:flyWeight,:flyHeight,:ctrlRadius,:flyTime,:maxWeight,:flyType,:backType,:other";

	if (!m_strFile.isEmpty())
		sql += ",:image";

	sql += ")";

	QSqlQuery sql_query(g_sqliteDbase);

	QString sid = QString("%1").arg(m_id);
	QString sqlDel = "Delete from EquipmentB where id=" + sid;

	sql_query.prepare(sqlDel);
	bool b = sql_query.exec();
	qDebug() << "delete DBThread ...." << b << endl;

	sql_query.prepare(sql);
	sql_query.bindValue(":type", type);			//字符串，区分大小写
	sql_query.bindValue(":num", num);
	sql_query.bindValue(":lookWidth", lookWidth);
	sql_query.bindValue(":len", len);
	sql_query.bindValue(":width", width);
	sql_query.bindValue(":height", height);
	sql_query.bindValue(":yiZhan", yiZhan);
	sql_query.bindValue(":flyWeight", flyWeight);
	sql_query.bindValue(":flyHeight", flyHeight);
	sql_query.bindValue(":ctrlRadius", bj);
	sql_query.bindValue(":flyTime", xhTime);
	sql_query.bindValue(":maxWeight", maxWeight);
	sql_query.bindValue(":flyType", flyType);
	sql_query.bindValue(":backType", backType);
	sql_query.bindValue(":other", sSpeed);
	
	if (m_jpg.size()>100)
	{		
		sql_query.bindValue(":image", m_jpg);		//无人机图像，二进制数据
	}


	b = sql_query.exec();
	if (!b)
	{
		qDebug() << "InsertDBThread in: fail...." << endl;
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("保存 装备 失败   ！！"), QMessageBox::StandardButton::Yes);
	}
	else
	{
		qDebug() << "InsertDBThread in: success...." << endl;
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("保存无人机装备 成功    ！！"), QMessageBox::StandardButton::Yes);
	}
}
