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

	// Hide kejian fields (removed from UI)
	ui.textEdit_12->hide(); ui.textEdit_13->hide(); ui.textEdit_14->hide();
	ui.label_24->hide(); ui.label_26->hide(); ui.label_25->hide();

	connect(ui.okBtn,		&QAbstractButton::clicked, this, &DlgModifyEquipment::on_okBtn_click);			//ɸѡ���ť
	connect(ui.pushButton,	&QAbstractButton::clicked, this, &DlgModifyEquipment::on_addImageBtn_click);	//����ͼƬ��ť
		
	ui.comboBox->addItem(QStringLiteral("\u591a\u65cb\u7ffc\u0041\u578b"));
	ui.comboBox->addItem(QStringLiteral("\u591a\u65cb\u7ffc\u0042\u578b"));
	ui.comboBox->addItem(QStringLiteral("\u591a\u65cb\u7ffc\u0043\u578b"));
	ui.comboBox->addItem(QStringLiteral("\u591a\u65cb\u7ffc\u0044\u578b"));

	ui.comboBox_2->addItem(QStringLiteral("\u56fa\u5b9a\u7ffc\u0041\u578b"));
	ui.comboBox_2->addItem(QStringLiteral("\u56fa\u5b9a\u7ffc\u0042\u578b"));
	ui.comboBox_2->addItem(QStringLiteral("\u56fa\u5b9a\u7ffc\u0043\u578b"));
	ui.comboBox_2->addItem(QStringLiteral("\u56fa\u5b9a\u7ffc\u0044\u578b"));

	ui.tabWidget->setTabText(0, QStringLiteral("\u591a\u65cb\u7ffc"));
	ui.tabWidget->setTabText(1, QStringLiteral("\u56fa\u5b9a\u7ffc"));

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
			QPixmap pixmap;
			pixmap.loadFromData(m_jpg, "jpg");
			ui.labelImage->setScaledContents(true);
			ui.labelImage->setPixmap(pixmap);
			m_strFile = "AAA";//����Ϊ��ͼ��״̬��ֻҪ��Ϊ�վ���
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
			ui.textEdit_1->setText(num);		//�ڿ�����
			ui.textEdit_11->setText(lookWidth);	//������
			ui.textEdit_2->setText(len);		//����
			ui.textEdit_3->setText(width);		//����
			ui.textEdit_4->setText(height);		//�߶�
			ui.textEdit_5->setText(bj);			//���п��ư뾶
			ui.textEdit_6->setText(maxSpd);		//�������ٶ�
			ui.textEdit_7->setText(flyTime);	//�����У�����ʱ��
			ui.textEdit_8->setText(workHeight);	//�����߶�			
			ui.textEdit_10->setText(maxWeight);	//�������
			ui.textEdit_12->setText(keJian1);	//
			ui.textEdit_13->setText(keJian2);	//
			ui.textEdit_14->setText(keJian3);	//
		}
		else
		{
			QString  flyWeight = query.value("flyWeight").toString();	//�������
			QString  flyHeight = query.value("flyHeight").toString();	//���и߶�
			QString  flyTime = query.value("flyTime").toString();		//���ط���ʱ��
			QString  maxWeight = query.value("maxWeight").toString();	//��������غ�
			QString  flyType = query.value("flyType").toString();		//��ɷ�ʽ
			QString  backType = query.value("backType").toString();		//���շ�ʽ
			QString  yiZhan = query.value("yiZhan").toString();			//��չ
			QString  sSpeed = query.value("other").toString();			//�ٶ�

			ui.comboBox_2->setCurrentText(type);
			ui.textEdit_17->setText(num);		//�ڿ�����
			ui.textEdit_20->setText(lookWidth);	//������
			ui.textEdit_16->setText(len);		//����
			ui.textEdit_18->setText(width);		//����
			ui.textEdit_19->setText(height);	//�߶�+
			ui.textEdit_21->setText(flyWeight);	//�������
			ui.textEdit_22->setText(flyHeight);	//���
			ui.textEdit_23->setText(bj);		//���п��ư뾶
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

//����ͼƬ
void DlgModifyEquipment::on_addImageBtn_click()
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
		ui.labelImage->setGeometry(340, 5, width, height+15); // ����label�Ĵ�С��λ��

		QBuffer		buffer(&m_jpg);
		buffer.open(QIODevice::WriteOnly);
		pixmap.save(&buffer, "JPG");
	}
}

//�����޸�
void DlgModifyEquipment::on_okBtn_click()
{
	if (!m_bDuo)
	{
		on_Bclick(); //�����ӹ̶���
		return;
	}

	//���������Ӷ�����
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
	QString kjg1 = ui.textEdit_12->toPlainText();		//�ɼ���1
	QString kjg2 = ui.textEdit_13->toPlainText();		//�ɼ���2
	QString kjg3 = ui.textEdit_14->toPlainText();		//�ɼ���3

	if (num == "")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u5728\u5e93\u6570\u91cf"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (lookWidth == "")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u4fa6\u5bdf\u5e26\u5bbd"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (len == "")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u957f\u5ea6"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (width == "")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u65e0\u4eba\u673a\u5bbd\u5ea6"), QMessageBox::StandardButton::Yes);
		return;
	}

	if (height == "")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u65e0\u4eba\u673a\u9ad8\u5ea6"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (bj == "")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u65e0\u4eba\u673a\u63a7\u5236\u534a\u5f84"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (maxSpd == "")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u65e0\u4eba\u673a\u6700\u5927\u901f\u5ea6"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (xhTime == "")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u65e0\u4eba\u673a\u98de\u884c\u65f6\u95f4"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (workHeight == "")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u65e0\u4eba\u673a\u5de5\u4f5c\u9ad8\u5ea6"), QMessageBox::StandardButton::Yes);
		return;
	}

	if (maxWeight == "")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u65e0\u4eba\u673a\u6700\u5927\u8f7d\u91cd"), QMessageBox::StandardButton::Yes);
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
		sql_query.bindValue(":image", m_jpg);		//���˻�ͼ�񣬶���������
	}

	b = sql_query.exec();
	if (!b)
	{
		qDebug() << "InsertDBThread in: fail...." << endl;
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u4fee\u6539\u88c5\u5907\u5931\u8d25"), QMessageBox::StandardButton::Yes);
	}
	else
	{
		qDebug() << "InsertDBThread in: success...." << endl;
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u4fee\u6539\u65e0\u4eba\u673a\u88c5\u5907\u6210\u529f"), QMessageBox::StandardButton::Yes);
		this->close();
	}
}

void DlgModifyEquipment::on_Bclick()//����̶�����b��
{
	//���������ӹ̶�����������
	QString type = ui.comboBox_2->currentText();
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

	if (num == "")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u5728\u5e93\u6570\u91cf"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (lookWidth == "")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u4fa6\u5bdf\u5e26\u5bbd"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (len == "")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u957f\u5ea6"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (width == "")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u65e0\u4eba\u673a\u5bbd\u5ea6"), QMessageBox::StandardButton::Yes);
		return;
	}

	if (height == "")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u65e0\u4eba\u673a\u9ad8\u5ea6"), QMessageBox::StandardButton::Yes);
		return;
	}
	if (bj == "")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u65e0\u4eba\u673a\u63a7\u5236\u534a\u5f84"), QMessageBox::StandardButton::Yes);
		return;
	}

	if (maxWeight == "")
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u8f93\u5165\u65e0\u4eba\u673a\u6700\u5927\u8f7d\u91cd"), QMessageBox::StandardButton::Yes);
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
	sql_query.bindValue(":type", type);			//�ַ��������ִ�Сд
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
		sql_query.bindValue(":image", m_jpg);		//���˻�ͼ�񣬶���������
	}


	b = sql_query.exec();
	if (!b)
	{
		qDebug() << "InsertDBThread in: fail...." << endl;
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u4fee\u6539\u88c5\u5907\u5931\u8d25"), QMessageBox::StandardButton::Yes);
	}
	else
	{
		qDebug() << "InsertDBThread in: success...." << endl;
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u4fee\u6539\u65e0\u4eba\u673a\u88c5\u5907\u6210\u529f"), QMessageBox::StandardButton::Yes);
	}
}
