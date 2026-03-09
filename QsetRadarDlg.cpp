#include "stdafx.h"
#include "QsetRadarDlg.h"
#include "comm.h"
#include <QgsVectorLayer.h>
#include "mainWindow.h"

QsetRadarDlg::QsetRadarDlg(QWidget *parent, int itype): QMainWindow(parent)
{
	QWidget *w = new QWidget(this);
	ui.setupUi(w);
	setCentralWidget(w);

	//connect(ui.pushButton_1, &QAbstractButton::clicked, this, &QsetRadarDlg::on_handSet);		//�ֶ����ð�ť
	connect(ui.pushButton_2, &QAbstractButton::clicked, this, &QsetRadarDlg::on_saveBtn_click);	//���ð�ť
	connect(ui.pushButton_3, &QAbstractButton::clicked, this, &QsetRadarDlg::on_exitBtn_click);	//�˳���ť

	m_iType = itype;

	QString mPath = QCoreApplication::applicationDirPath();
	QSettings settings(mPath + "/config.ini", QSettings::IniFormat);
	QString slist = settings.value("DeviceType/list").toString();
	QStringList list = slist.split("|");

	if (itype < list.size())
		setWindowTitle(list[itype]);

	//int iCount = g_pRadarLayer->featureCount();
	QString s= QString("%1").arg(100);
	ui.textEdit_1->setText(s);
	ui.textEdit_2->setText(list[itype]);
	ui.textEdit_3->setText("2000");	//�豸���ڸ߶�
	ui.textEdit_7->setText("0");	//��ʼ����λ�Ƕ�
	ui.textEdit_6->setText("45");	//������Χ�Ƕ�
	//ui.textEdit_5->setText("2000"); //̽�����
}

QsetRadarDlg::~QsetRadarDlg()
{}

void QsetRadarDlg::on_handSet()			//�ֶ�����
{
	showMinimized();					//�ѵ�ǰ������С��
}

void QsetRadarDlg::on_saveBtn_click()	//���ñ���
{
	if (ui.textEdit_1->text().toInt() < 1)
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle(QString::fromLocal8Bit("提示"));
		msgBox.setText(QString::fromLocal8Bit("  探测设备ID必须大于0  "));
		msgBox.exec();
		return;
	}

	if (ui.textEdit_3->text().toInt() < 1)
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle(QString::fromLocal8Bit("提示"));
		msgBox.setText(QString::fromLocal8Bit("  探测设备高度必须大于0  "));
		msgBox.exec();
		return;
	}

	if (ui.textEdit_9->text().toInt() < 1)
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle(QString::fromLocal8Bit("提示"));
		msgBox.setText(QString::fromLocal8Bit("  水平波束宽度必须大于0  "));
		msgBox.exec();
		return;
	}

	if (ui.textEdit_10->text().toInt() < 1)
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle(QString::fromLocal8Bit("提示"));
		msgBox.setText(QString::fromLocal8Bit("  垂直波束宽度必须大于0  "));
		msgBox.exec();
		return;
	}
	if (ui.textEdit_11->text().toInt() < 1)
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle(QString::fromLocal8Bit("提示"));
		msgBox.setText(QString::fromLocal8Bit("  探测距离必须大于10  "));
		msgBox.exec();
		return;
	}
	//���״�ͼ��������һ���״�ͼԪ

	gRadarLayerList[m_iType]->startEditing();

	//������޸��豸����ɾ��ԭ�����豸ͼԪ
	if (!m_bInsert)
	{
		gRadarLayerList[m_iType]->deleteFeature(m_fid);
	}
	QgsGeometry geometry = QgsGeometry::fromPointXY(m_point);
	QgsFeature f = QgsFeature();
	f.setGeometry(geometry);
	
	f.setAttributes(QgsAttributes() << ui.textEdit_1->text().toInt()
		<<QString::number(m_point.x(), 'f', 6)
		<<QString::number(m_point.y(), 'f', 6)
		<< m_iType<<
		ui.textEdit_3->text().toInt()<<			//设备高度
		ui.textEdit_7->text() <<				//初始方位角度
		ui.textEdit_6->text() <<					//探测范围角度
		ui.textEdit_7->text()<<					//初始方位角
		ui.textEdit_8->text()<<					//俯仰角
		ui.textEdit_9->text()<<					//水平波束宽度
		ui.textEdit_10->text()<<				//垂直波束宽度
		ui.textEdit_11->text());				//探测距离

	bool b= gRadarLayerList[m_iType]->addFeature(f);

	qDebug() << "b=" << b << endl;
	gRadarLayerList[m_iType]->commitChanges();

	MainWindow *pDlg=(MainWindow*) this->parentWidget();
	pDlg->refreshRadarCombox();

	if (!m_bInsert)
	{
		//����ͶӰͼ����ȫ���״�ͼԪ,��ȫ��ɾ��
		g_pRadarTyLayer->startEditing();
		QgsFeature feat;
		QgsFeatureIterator fit = g_pRadarTyLayer->getFeatures();
		while (fit.nextFeature(feat))
		{
			g_pRadarTyLayer->deleteFeature(feat.id());
		}
		g_pRadarTyLayer->commitChanges();
	}

	// 保存装载无人机机号到 radar_mount.ini
	{
		int radarId = ui.textEdit_1->text().toInt();
		QString uavId = ui.textEdit_12->text().trimmed();
		QString mPath2 = QCoreApplication::applicationDirPath();
		QSettings mountCfg(mPath2 + "/radar_mount.ini", QSettings::IniFormat);
		QString mountKey = QString("RadarMount/radar_%1").arg(radarId);
		if (uavId.isEmpty() || uavId == "0")
			mountCfg.remove(mountKey);
		else
			mountCfg.setValue(mountKey, uavId);
	}

	MainWindow *pWin = (MainWindow*)gMainWindow;
	pWin->loadRadarUavMount();   // 重新加载装载关系到内存
	pWin->ShowRadarTip();

	this->close();
}

void QsetRadarDlg::on_exitBtn_click()	//�˳�
{
	this->close();
}
