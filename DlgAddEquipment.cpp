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

	connect(ui.okBtn,		&QAbstractButton::clicked, this, &DlgAddEquipment::on_okBtn_click);			//ɸѡ���ť
	connect(ui.pushButton,	&QAbstractButton::clicked, this, &DlgAddEquipment::on_addImageBtn_click);	//����ͼƬ��ť
	
	ui.comboBox->addItem(QStringLiteral("\u591a\u65cb\u7ffc\u0041\u578b")); // 多旋翼A型
	ui.comboBox->addItem(QStringLiteral("\u591a\u65cb\u7ffc\u0042\u578b")); // 多旋翼B型
	ui.comboBox->addItem(QStringLiteral("\u591a\u65cb\u7ffc\u0043\u578b")); // 多旋翼C型
	ui.comboBox->addItem(QStringLiteral("\u591a\u65cb\u7ffc\u0044\u578b")); // 多旋翼D型

	ui.comboBox_2->addItem(QStringLiteral("\u56fa\u5b9a\u7ffc\u0041\u578b")); // 固定翼A型
	ui.comboBox_2->addItem(QStringLiteral("\u56fa\u5b9a\u7ffc\u0042\u578b")); // 固定翼B型
	ui.comboBox_2->addItem(QStringLiteral("\u56fa\u5b9a\u7ffc\u0043\u578b")); // 固定翼C型
	ui.comboBox_2->addItem(QStringLiteral("\u56fa\u5b9a\u7ffc\u0044\u578b")); // 固定翼D型
	
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

	ui.tabWidget->setTabText(0, QStringLiteral("\u591a\u65cb\u7ffc")); // 多旋翼
	ui.tabWidget->setTabText(1, QStringLiteral("\u56fa\u5b9a\u7ffc")); // 固定翼
	ui.tabWidget->setCurrentIndex(0);
}

DlgAddEquipment::~DlgAddEquipment()
{}


//����ͼƬ
void DlgAddEquipment::on_addImageBtn_click()
{	
	m_strFile = QFileDialog::getOpenFileName(this, QStringLiteral("\u6253\u5f00\u6587\u4ef6"), QCoreApplication::applicationDirPath(),
		"image files (*jpg);;All files (*.*)");

	if (!m_strFile.isEmpty()) {
		//QRect rc = ui.labelImage->rect();
		int width = ui.labelImage->width();
		int height = ui.labelImage->height();
		QPixmap pixmap(m_strFile); // ʹ����Դ�ļ�·�������·��
		QPixmap scaledPixmap = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation); // ����ͼƬ

		ui.labelImage->setPixmap(scaledPixmap);
		ui.labelImage->setGeometry(348, 15, width, height+15); // ����label�Ĵ�С��λ��
	}
}

void DlgAddEquipment::on_okBtn_click()
{
	if (ui.tabWidget->currentIndex() == 0)
	{
		insert0();//���Ӷ�����
		return;
	}
	
	//���ӹ̶���
	QString type=ui.comboBox_2->currentText();
	QString num = ui.textEdit_17->toPlainText();		//�ڿ�����
	QString lookWidth = ui.textEdit_20->toPlainText();	//������
	QString len = ui.textEdit_16->toPlainText();		//����
	QString width = ui.textEdit_18->toPlainText();		//����
	QString height = ui.textEdit_19->toPlainText();		//�߶�
	QString yiZhan = ui.textEdit_27->toPlainText();		//��չ
	QString flyWeight = ui.textEdit_21->toPlainText();	//�������
	QString flyHeight = ui.textEdit_22->toPlainText();	//��ɹ����߶�
	QString bj = ui.textEdit_23->toPlainText();			//���а뾶	
	QString xhTime = ui.textEdit_24->toPlainText();		//���ط�������ʱ��
	QString maxWeight = ui.textEdit_15->toPlainText();	//�����������
	QString flyType = ui.textEdit_25->toPlainText();	//��ɷ�ʽ
	QString backType = ui.textEdit_26->toPlainText();	//���շ�ʽ
	QString sSpeed = ui.textEdit_28->toPlainText();	//�ٶ�
	
	if (type == "")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u9009\u62e9\u7c7b\u578b"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (num == "")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u5728\u5e93\u6570\u91cf"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (len == "")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u957f\u5ea6"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (width == "")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u5bbd\u5ea6"), QMessageBox::StandardButton::Yes);
		return;
	}

	if (height == "")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u9ad8\u5ea6"), QMessageBox::StandardButton::Yes);
		return;
	}
	

	QString sql = "insert into EquipmentB(type,num,lookWidth,len,width,height,yiZhan,flyWeight,flyHeight,ctrlRadius,flyTime,maxWeight,flyType,backType,other";

	if (!m_strFile.isEmpty())
		sql += ",image";

	sql+=")values (:type,:num,:lookWidth,:len,:width,:height,:yiZhan,:flyWeight,:flyHeight,:ctrlRadius,:flyTime,:maxWeight,:flyType,:backType,:other";

	if (!m_strFile.isEmpty())
		sql += ",:image";

	sql += ")";

	QSqlQuery sql_query(g_sqliteDbase);
	sql_query.prepare(sql);
	sql_query.bindValue(":type", type);			//�ַ��������ִ�Сд
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
		sql_query.bindValue(":image", array);		//���˻�ͼ�񣬶���������
	}


	bool b = sql_query.exec();
	if (!b)
	{
		qDebug() << "Insert fail:" << sql_query.lastError().text() << endl;
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"),
			QStringLiteral("\u6dfb\u52a0\u88c5\u5907\u5931\u8d25: ") + sql_query.lastError().text(),
			QMessageBox::StandardButton::Yes);
	}
	else
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u6dfb\u52a0\u88c5\u5907\u6210\u529f"), QMessageBox::StandardButton::Yes);
	}
}

//���Ӷ�����
void DlgAddEquipment::insert0()
{
	QString type = ui.comboBox->currentText();
	QString num = ui.textEdit_1->toPlainText();			//�ڿ�����
	QString lookWidth = ui.textEdit_11->toPlainText();	//������
	QString len = ui.textEdit_2->toPlainText();			//����
	QString width = ui.textEdit_3->toPlainText();		//����
	QString height = ui.textEdit_4->toPlainText();		//�߶�
	QString bj = ui.textEdit_5->toPlainText();			//���а뾶
	QString maxSpd = ui.textEdit_6->toPlainText();		//����ٶ�
	QString xhTime = ui.textEdit_7->toPlainText();		//�����У�����ʱ��
	QString workHeight = ui.textEdit_8->toPlainText();	//�����߶�	
	QString maxWeight = ui.textEdit_10->toPlainText();	//�������
	if (type.isEmpty()) {
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u9009\u62e9\u7c7b\u578b"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (num.isEmpty()) {
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u5728\u5e93\u6570\u91cf"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (len.isEmpty()) {
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u957f\u5ea6"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (width.isEmpty()) {
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u5bbd\u5ea6"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (height.isEmpty()) {
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u9ad8\u5ea6"), QMessageBox::StandardButton::Yes);
		return;
	}

	QByteArray imgArray;
	bool hasImage = !m_strFile.isEmpty();
	if (hasImage) {
		QPixmap pixmap(m_strFile);
		QBuffer buffer(&imgArray);
		buffer.open(QIODevice::WriteOnly);
		pixmap.save(&buffer, "JPG");
	}

	QString sql = "insert into EquipmentA(type,Num,lookWidth,len,width,height,ctrlRadius,MaxSpeed,xhTime,workHeight,maxWeight";
	if (hasImage) sql += ",image";
	sql += ") values (?,?,?,?,?,?,?,?,?,?,?";
	if (hasImage) sql += ",?";
	sql += ")";

	QSqlQuery sql_query(g_sqliteDbase);
	bool prepOk = sql_query.prepare(sql);
	if (!prepOk) {
		QString err = QString("prepare fail: %1\nSQL: %2\nDB open: %3")
			.arg(sql_query.lastError().text()).arg(sql).arg(g_sqliteDbase.isOpen()?"yes":"no");
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), err, QMessageBox::StandardButton::Yes);
		return;
	}
	sql_query.addBindValue(type);
	sql_query.addBindValue(num);
	sql_query.addBindValue(lookWidth);
	sql_query.addBindValue(len);
	sql_query.addBindValue(width);
	sql_query.addBindValue(height);
	sql_query.addBindValue(bj);
	sql_query.addBindValue(maxSpd);
	sql_query.addBindValue(xhTime);
	sql_query.addBindValue(workHeight);
	sql_query.addBindValue(maxWeight);
	if (hasImage) sql_query.addBindValue(imgArray);

	bool b = sql_query.exec();
	if (!b)
	{
		QString err = QString("exec fail: %1\nSQL: %2").arg(sql_query.lastError().text()).arg(sql);
		qDebug() << err;
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"),
			QStringLiteral("\u6dfb\u52a0\u88c5\u5907\u5931\u8d25: ") + sql_query.lastError().text(),
			QMessageBox::StandardButton::Yes);
	}
	else
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u6dfb\u52a0\u88c5\u5907\u6210\u529f"), QMessageBox::StandardButton::Yes);
	}
}