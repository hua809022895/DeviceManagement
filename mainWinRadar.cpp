#include "stdafx.h"
#include "mainWindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstring>

#include <filesystem>
#include <iostream>
//namespace fs = std::filesystem;

#include <QComboBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextcodec.h>

#include <qgsVectorLayer.h>
#include <qgsTransaction.h>
#include <qgsTransactionGroup.h>
#include <qgsMessageViewer.h>
#include <qgsLayerTreemodel.h>
#include <qgsLayerTreenode.h>
#include <qgsLayerTreegroup.h>
#include <qgsLayerTreemapCanvasBridge.h>
#include <qgsDataitem.h>
#include <qgsVectorLayerutils.h>
#include <qgsProviderRegistry.h>
#include <qgsSymbolSelectorDialog.h>
#include <qgsSingleSymbolRenderer.h>
#include <qgsSymbol.h>
#include <qgsStyle.h>
#include <qgsFileutils.h>
#include <qgsZiputils.h>
#include <qgsEditorWidgetsetup.h>
#include <qgsFieldFormatter.h>
#include <qgsEditorWidgetRegistry.h>
#include <qgsProviderMetadata.h>
#include <qgsFieldFormatterRegistry.h>
#include <qgsVectorLayerlabeling.h>
#include <qgsMarkerSymbolLayer.h>
#include <qgsExpressionContext.h>
#include <qgsFilterLineedit.h>
#include <qgsHeatMapRenderer.h>
#include <qgslinesymbollayer.h>
#include <qgslayertree.h>
#include <qgsAnnotationManager.h>
#include <qgsAnnotationMarkerItem.h>

#include "qSetLayerDialog.h"
#include "comm.h"
#include "global.h"
#include "QsetRadarDlg.h"
#include "radarTip.h"
#include "Polygon.h"
#include "Dialog_AirList.h"
#include "recMavlinkThread.h"
#include "clipper2/include/clipper.h"

//�Ƿ���ʾ�״�ͶӰ����ʼ����ʾ
void MainWindow::RadarTouying()
{
	QString mPath = QCoreApplication::applicationDirPath();
	QString file = mPath + "/recjpg11/image2189.jpg";

	bool b = ui->actionRadarGo->isChecked();
	qDebug() << "Plane key:" << b << endl;
	//m_mapCanvas->setCurrentLayer(g_pRadarLayer);
	g_pRadarTyLayer->startEditing();//�״�ͶӰͼ��

	if (b)
	{
		//����ͼ����ȫ���״�ͼԪ
		QgsFeature feat;
		for (int i = 0; i < gRadarLayerList.size(); i++)
		{
			QgsFeatureIterator fit = gRadarLayerList[i]->getFeatures();
			while (fit.nextFeature(feat))
			{
				QgsPointXY devPt = feat.geometry().asPoint();
				//����ÿ���״�ͼԪ��Ϣ������һ���µ�ͶӰͼԪ
				QString ID = feat.attribute(0).toString();
				//QString sAngle	= feat.attribute(5).toString();//��ʼ��λ�Ƕ�
				QString tAngle = feat.attribute(6).toString();//ͶӰ�Ƕ�
				QString sAzimuth = feat.attribute(7).toString();//��ʼ��λ�Ǧ�
				QString sElevation = feat.attribute(8).toString();//�����Ǧ�
				QString svWidth = feat.attribute(9).toString();//ˮƽ��������
				QString shWidth = feat.attribute(10).toString();//��ֱ��������
				QString length = feat.attribute(11).toString();//ͶӰ����

				//�����豸��Ϣ�Զ�����ͶӰ�����ȫ����
				QList<QgsPointXY> PointSet;
				PointSet = GetTYPolygon(devPt, sAzimuth.toFloat(), tAngle.toInt(), sElevation.toFloat(), length.toInt());

				QgsPolygonXY pxy = QgsPolygonXY() << PointSet.toVector();
				QgsGeometry	Geometry = QgsGeometry::fromPolygonXY(pxy);//���ݶ���εĵ���Ϣ���ɼ���ͼ�ζ���

				QgsFeature f = QgsFeature();
				f.setGeometry(Geometry);
				f.setAttributes(QgsAttributes() << ID);
				b = g_pRadarTyLayer->addFeature(f);
			}
		}
	}
	else
	{		
		//����ͼ����ȫ���״�ͼԪ
		QgsFeature feat;
		QgsFeatureIterator fit = g_pRadarTyLayer->getFeatures();
		while (fit.nextFeature(feat))
		{
			g_pRadarTyLayer->deleteFeature(feat.id());
		}		
	}
	g_pRadarTyLayer->triggerRepaint();
}
//������ʼ�Ƕȣ�̽��Ƕȣ�̽����룬����̽��������������꼯��
QList<QgsPointXY> MainWindow::GetTYPolygon(QgsPointXY devPt, float sAngle, float tAngle, float pitch, int length)
{
	QList<QgsPointXY> pset;
	pset.append(devPt);

	for (int i = 0; i < tAngle; i ++)
	{
		QgsPoint xy = GetResult(devPt.x(), devPt.y(), sAngle + i,pitch, length * 1000);
		pset.append(QgsPointXY(xy.x(), xy.y()));
	}

	return pset;
}
//�����״���Ϣ��������ʾͼԪ
void MainWindow::ShowRadarTip()
{
	foreach(RadarTip *w, m_radarTipList) {
		delete w;		
	}
	m_radarTipList.clear();

	QString mPath = QCoreApplication::applicationDirPath();
	QSettings settings(mPath + "/config.ini", QSettings::IniFormat);
	QString slist = settings.value("DeviceType/list").toString();
	QStringList list = slist.split("|");
	//����ͼ����ȫ���״�ͼԪ
	QgsFeature feat;

	for (int i = 0; i < gRadarLayerList.size(); i++)
	{
		QgsFeatureIterator fit = gRadarLayerList[i]->getFeatures();
		while (fit.nextFeature(feat))
		{
			QgsPointXY	p = feat.geometry().asPoint();
			QString		ID = feat.attribute(0).toString();
			int			type = feat.attribute(3).toInt();	//����

			RadarTip* pPai = new RadarTip((QgsMapCanvas*)m_mapCanvas);
			pPai->setString(list[type] + ID);
			pPai->setType(type);
			pPai->setPos(p);
			pPai->m_id = ID.toInt();

			m_radarTipList.append(pPai);
		}
	}
}

//�ж����˻���ǰ���Ƿ���ĳ���״����������ڣ�����ֵ����0��ʾ�����������ڣ������������״������豸id�б�
QList <int> MainWindow::isRadarPolygon(tag_PlaneMessage p)
{
	QString	mPath = QCoreApplication::applicationDirPath();
	//�����״�ͼ����ȫ���״�ͼԪ
	QgsFeature			f;
	QList	<int>	list;			//�����״������豸id�б�

	for (int i = 0; i < gRadarLayerList.size(); i++)
	{
		QgsFeatureIterator	fit = gRadarLayerList[i]->getFeatures();
		double x = p.planeX.toDouble();	//��ǰ���˻�����
		double y = p.planeY.toDouble();	//��ǰ���˻�����
		double z = p.xZ.toDouble();		//��Ը߶�
		while (fit.nextFeature(f))
		{
			//����ÿ���״�ͼԪ��Ϣ������һ���µ�����ͶӰͼԪ
			QgsPointXY devPt = f.geometry().asPoint();
			//QString sAngle = feat.attribute(5).toString();//��ʼ�Ƕ�
			//QString tAngle = feat.attribute(6).toString();//ͶӰ�Ƕ�	
			QString heigth = f.attribute(4).toString();		//�豸�߶�
			QString sAzimuth = f.attribute(7).toString();		//��λ�Ǧ�
			QString sElevation = f.attribute(8).toString();		//�����Ǧ�
			QString svWidth = f.attribute(9).toString();	//ˮƽ��������
			QString shWidth = f.attribute(10).toString();	//��ֱ��������
			QString sR_max = f.attribute(11).toString();	//̽�����
			double x0 = devPt.x();
			double y0 = devPt.y();
			double z0 = heigth.toFloat();

			//��һ�����������˻�������״������		
			double dx = (x - x0); 	double dy = y - y0;  double dz = z - z0;

			double R = sqrt(dx * dx + dy * dy + dz * dz);
			R = fabs(R);

			//�ڶ���������������
			if (R > sR_max.toDouble())
				continue;

			//�����������㷽λ��ƫ��(Azimuth Deviation)
			//1:)����ˮƽͶӰ�ķ�λ�� ��_p:
			double phi_az_p = atan2(dy, dx);   // �����λΪ���ȣ���Χ [-��, ��]

			//�����ת��Ϊ�Ƕ��ƣ��������� [0��, 360��) ��Χ��
			double phi_az_p_deg = RAD_TO_DEG(phi_az_p);
			if (phi_az_p_deg < 0)
				phi_az_p_deg += 360;

			double phi_az_delta = fabs(phi_az_p_deg - sAzimuth.toFloat());

			if (phi_az_delta > 180)
				phi_az_delta = 360 - phi_az_delta;

			//��� ����> (��_az / 2)�������˻���ˮƽ������ƫ���˲��������ڷ��䷶Χ�⡣
			if (phi_az_delta > (svWidth.toFloat() / 2))
				continue;

			//���Ĳ������㸩����ƫ��(Elevation Deviation)
			//1:)�������� V �ĸ����� ��_p:
			double phi_el_p_deg = RAD_TO_DEG(asin(dz / R));	//�����ת��Ϊ�Ƕ��� phi_el_p_deg��

			//2:)���㸩����ƫ�� ����:
			double phi_el_delta = fabs(phi_el_p_deg - sElevation.toFloat());
			//3:)��� ���� > (��_el / 2)�������˻��ڴ�ֱ������ƫ���˲��������ڷ��䷶Χ�⡣
			if (phi_el_delta > (shWidth.toFloat() / 2.0))
				continue;
			else
			{
				int  devID = f.attribute(0).toInt();		//�״��豸id
				list.append(devID);
			}
		}
	}

	return list;
}

QList <int> MainWindow::isRadarInPoly(tag_PlaneMessage p)	//�жϵ�ǰ���Ƿ����״����������ڣ�����ֵ����0��ʾ�����������ڣ��������״��豸id
{
	//�����״�ͼ����ȫ���״�ͼԪ
	QgsFeature		f;
	QList	<int>	list;			//�����״������豸id�б�

	for (int i = 0; i < gRadarLayerList.size(); i++)
	{
		QgsFeatureIterator	fit = gRadarLayerList[i]->getFeatures();
		double x = p.planeX.toDouble();	//��ǰ���˻�����
		double y = p.planeY.toDouble();	//��ǰ���˻�����
		double z = p.xZ.toDouble();		//��Ը߶�


		while (fit.nextFeature(f))
		{
			//����ÿ���״�ͼԪ��Ϣ������һ���µ�����ͶӰͼԪ
			QgsPointXY pt = f.geometry().asPoint();
			QString tAngle = f.attribute(6).toString();		//ͶӰ��Χ�Ƕ�	
			QString heigth = f.attribute(4).toString();		//�豸�߶�
			QString sAzimuth = f.attribute(7).toString();		//��ʼ��λ�Ǧ�
			QString sElevation = f.attribute(8).toString();		//�����Ǧ�
			QString svWidth = f.attribute(9).toString();	//ˮƽ��������
			QString shWidth = f.attribute(10).toString();	//��ֱ��������
			QString sR_max = f.attribute(11).toString();	//̽�����
			//double x0 = pt.x();	double y0 = pt.y();
			double z0 = heigth.toFloat();

			//�жϸ߶�
			if (abs(z - z0) > 150)
				continue;

			//�ж����θ���
			//�����豸��Ϣ�Զ�����ͶӰ�����ȫ����
			QList<QgsPointXY>	set = GetTYPolygon(pt, sAzimuth.toInt(), tAngle.toFloat(), sElevation.toFloat(), sR_max.toInt());
			cl::PathD			path;

			foreach(QgsPointXY p, set) {
				path.push_back(cl::PointD(p.x(), p.y()));
			}

			cl::PointD			p1(x, y);

			cl::PointInPolygonResult bin = PointInPolygon(p1, path);//�жϵ��Ƿ��ڶ������

			if (bin == cl::PointInPolygonResult::IsInside)
			{
				list.append(f.attribute(0).toInt());
			}
		}
	}
	return list;
}
//װ���ƶ�
void MainWindow::OutfitMove()
{
	QString mPath = QCoreApplication::applicationDirPath();

	//������ڽ������ݣ��͸�����ʾ�������������
	//�жϸ�Ŀ¼���Ƿ����豸�켣�����ı��ļ���radar1.txt
	if (m_DevList.size() == 0)
	{
		//����ȫ���״��豸	
		QgsFeature			feat;

		for (int i = 0; i < gRadarLayerList.size(); i++)
		{
			QgsFeatureIterator	fit = gRadarLayerList[i]->getFeatures();
			while (fit.nextFeature(feat))
			{
				QString		sID = feat.attribute(0).toString();
				QString		fileName = mPath + "/radar" + sID + ".txt";
				QFileInfo	fileInfo(fileName);
				if (!fileInfo.exists())
				{
					continue;
				}

				m_DevList.append(sID);
				QFile file(fileName); //�滻Ϊ����ļ�·��  
				if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
					qDebug() << "Unable to open file for reading";
					return;
				}

				RadarInfo info;
				info.id = sID;

				QTextStream in(&file);
				bool b = true;
				QgsPoint prePt;

				while (!in.atEnd()) {
					QString line = in.readLine();

					if (b)
					{
						info.speed = line.toInt();	//��ȡ��һ�У��ٶ�ֵ
						b = false;
					}
					else
					{
						int k = line.indexOf(",");
						QString x = line.mid(0, k);
						QString y = line.mid(k + 1);
						info.plist.append(QgsPointXY(x.toDouble(), y.toDouble()));

						if (!prePt.isEmpty())
						{
							float dist = GetDistance(prePt, QgsPoint(x.toDouble(), y.toDouble()));
							qDebug() << dist << endl; // �����ȡ��ÿһ��  

							//����߶γ��ȴ���100��
							if (dist > 50)
							{
								QList	<QgsPointXY> list = getTrack(QgsPointXY(prePt.x(), prePt.y()), QgsPoint(x.toDouble(), y.toDouble()), info.speed);

								foreach(QgsPointXY ip, list) {
									info.tlist.append(ip);
								}
							}
						}
						prePt = QgsPoint(x.toDouble(), y.toDouble());
					}
				}
				m_RadarTrack.append(info);
				file.close();
			}
		}
	}

	if (m_DevList.size() == 0)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("该目录下没有可控制的地图数据文件 radar1.txt，无法加载"));
		return;
	}

	if (m_pTimer1)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("装备已经开始移动了，请注意"));	
		//m_pTimer1->start(1000);
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("装备即将开始移动........."));

		//m_pTimer1 = new QTimer();
		//connect(m_pTimer1, &QTimer::timeout, this, &MainWindow::timer1_timeout);
		//m_pTimer1->start(1000);
	}	
}

//��ʱ�����̺���
void MainWindow::timer1_timeout()
{
	qDebug() << QString::fromLocal8Bit("每隔一定时间执行.....");
	QString mPath = QCoreApplication::applicationDirPath();

	for (int i = 0; i < gRadarLayerList.size(); i++)
	{
		if (!gRadarLayerList[i]->isEditable())
			gRadarLayerList[i]->startEditing();

		foreach(RadarInfo info, m_RadarTrack) {
			if (m_iRaderTckID >= info.tlist.size())
			{
				m_iRaderTckID = -1;
				m_pTimer1->stop();
				break;
			}

			QgsFeatureIterator featureit = gRadarLayerList[i]->getFeatures("\"electircid\"=" + info.id);
			QgsFeature	f;
			QgsPointXY	pt;
			if (featureit.nextFeature(f))
			{
				QString sType = f.attribute(3).toString();	//�豸����
				QString heigth = f.attribute(4).toString();	//�豸�߶�
				QString sAngle = f.attribute(5).toString();	//��ʼ�Ƕ�
				QString tAngle = f.attribute(6).toString();	//̽��Ƕ�
				QString length = f.attribute(11).toString();	//̽�����

				pt = info.tlist[m_iRaderTckID];
				QgsGeometry geometry = QgsGeometry::fromPointXY(pt);
				f.setGeometry(geometry);

				QgsAttributes attribues;
				attribues << info.id << 1 << 1 << sType.toInt() << heigth << sAngle << tAngle << length;
				f.setAttributes(attribues);
				gRadarLayerList[i]->updateFeature(f);
			}

			//��ʼ�ƶ��״���ʾͼԪ
			foreach(RadarTip * r, m_radarTipList) {
				if (r->m_id == info.id.toInt())
				{
					r->setPos(pt);
					break;
				}
			}
		}
		m_iRaderTckID++;
		gRadarLayerList[i]->triggerRepaint();
	}
}

// 无人机图层定时刷新（100ms=10fps），由 m_pAirLayerTimer 驱动。
// 职责：
//   1. processAllPlaneUpdates() — 批量处理 registerPlane() 缓冲的飞机位置
//      （canvas 空闲时才执行，避免渲染期间写锁竞争，彻底解决缩放卡顿）
//   2. triggerRepaint() — 触发图层重绘
//      （isDrawing() 检查，不中断正在进行的缩放/平移渲染任务）
void MainWindow::onAirLayerRefreshTimer()
{
	processAllPlaneUpdates();

	// 仅当有新飞机位置数据时才触发重绘，避免无变化时持续驱动全图层渲染导致地图闪烁
	if (m_bAirLayerDirty && g_pAirLayer && m_mapCanvas && !m_mapCanvas->isDrawing()) {
		g_pAirLayer->triggerRepaint();
		m_bAirLayerDirty = false;
	}

	// 轨迹记录开启时，每 2s 自动将缓冲点渲染到图层（20 × 100ms）
	static int s_trackTick = 0;
	if (m_bTrackEnabled && ++s_trackTick >= 20) {
		s_trackTick = 0;
		RefreshGj();
	}
}

//��������������,�豸�ƶ��ٶȣ�����������֮�䣬ÿ�ξ�������
QList <QgsPointXY> MainWindow::getTrack(QgsPointXY p1, QgsPointXY p2, int speed)
{
	QList <QgsPointXY> list;
	float dist = GetDistance(QgsPoint(p1.x(), p1.y()), QgsPoint(p2.x(), p2.y()));

	int iCount = dist / speed;
	for (int i = 0; i <iCount; i++)
	{
		double lat = p1.y() + (p2.y()- p1.y()) * i/ iCount;
		double lon = p1.x() + (p2.x()- p1.x()) * i/ iCount;

		list.append(QgsPointXY(lon, lat));
	}

	return list;
}
//����Ҽ����ӿ�ݲ˵�
void MainWindow::AddMenu1()
{
	QString s = m_lblCoordinate->text();
	int k = s.indexOf(",");
	QString x = s.mid(0, k);
	QString y = s.mid(k + 1);
	//QgsPointXY px(x.toDouble(), y.toDouble());
	//addSvgAnnotation(px, px);
	//return;

	if (m_pSetRadarDlg)
		delete m_pSetRadarDlg;

	m_pSetRadarDlg = new QsetRadarDlg(this, 0);
	m_pSetRadarDlg->m_bInsert = true;//�½��豸
	
	m_pSetRadarDlg->m_point = QgsPointXY(x.toDouble(), y.toDouble());
	m_pSetRadarDlg->ui.textEdit_4->setText(m_lblCoordinate->text());
	m_pSetRadarDlg->show();
	m_pSetRadarDlg->activateWindow();	
}

void MainWindow::AddMenu2()
{
	/*if (m_pRecThread->m_paused == false)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("无人机数据接收时，不能进行此操作！"));
		return;
	}*/
	if (m_pSetRadarDlg)
		delete m_pSetRadarDlg;

	m_pSetRadarDlg = new QsetRadarDlg(this,1);
	m_pSetRadarDlg->m_bInsert = true;//�½��豸
	QString s = m_lblCoordinate->text();
	int k = s.indexOf(",");
	QString x = s.mid(0, k);
	QString y = s.mid(k + 1);
	m_pSetRadarDlg->m_point = QgsPointXY(x.toDouble(), y.toDouble());
	m_pSetRadarDlg->ui.textEdit_4->setText(m_lblCoordinate->text());
	m_pSetRadarDlg->show();
	m_pSetRadarDlg->activateWindow();
}

void MainWindow::AddMenu3()
{
	if (m_pRecThread != nullptr && m_pRecThread->m_paused == false)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("无人机数据接收时，不能进行此操作！"));
		return;
	}
	if (m_pSetRadarDlg)
		delete m_pSetRadarDlg;

	m_pSetRadarDlg = new QsetRadarDlg(this,2);
	m_pSetRadarDlg->m_bInsert =true;//�½��豸
	QString s = m_lblCoordinate->text();
	int k = s.indexOf(",");
	QString x = s.mid(0, k);
	QString y = s.mid(k + 1);
	m_pSetRadarDlg->m_point = QgsPointXY(x.toDouble(), y.toDouble());
	m_pSetRadarDlg->ui.textEdit_4->setText(m_lblCoordinate->text());
	m_pSetRadarDlg->show();
	m_pSetRadarDlg->activateWindow();
}

void MainWindow::AddMenu4()
{
	if (m_pSetRadarDlg)
		delete m_pSetRadarDlg;

	m_pSetRadarDlg = new QsetRadarDlg(this,3);
	m_pSetRadarDlg->m_bInsert = true;//�½��豸
	QString s = m_lblCoordinate->text();
	int k = s.indexOf(",");
	QString x = s.mid(0, k);
	QString y = s.mid(k + 1);
	m_pSetRadarDlg->m_point = QgsPointXY(x.toDouble(), y.toDouble());
	m_pSetRadarDlg->ui.textEdit_4->setText(m_lblCoordinate->text());
	m_pSetRadarDlg->show();
	m_pSetRadarDlg->activateWindow();
}

void MainWindow::AddMenu5()
{
	if (m_pSetRadarDlg)
		delete m_pSetRadarDlg;

	m_pSetRadarDlg = new QsetRadarDlg(this,4);
	m_pSetRadarDlg->m_bInsert = true;//�½��豸
	QString s = m_lblCoordinate->text();
	int k = s.indexOf(",");
	QString x = s.mid(0, k);
	QString y = s.mid(k + 1);
	m_pSetRadarDlg->m_point = QgsPointXY(x.toDouble(), y.toDouble());
	m_pSetRadarDlg->ui.textEdit_4->setText(m_lblCoordinate->text());
	m_pSetRadarDlg->show();
	m_pSetRadarDlg->activateWindow();
}
//ѡ���״�ͼ��
void MainWindow::selectRadarLayer()
{
	if (gRadarLayerList.size() > 0)
	{
		m_mapCanvas->setCurrentLayer(gRadarLayerList[0]);
		for (int i = 0; i < gRadarLayerList.size(); i++)
		{
			gRadarLayerList[i]->startEditing();
		}

		m_mapCanvas->unsetMapTool(mToolPan);
	}
}

//ѡ���״�
void MainWindow::selectRadarDevice(QString s)
{
	QString mPath = QCoreApplication::applicationDirPath();
	QSettings settings(mPath + "/config.ini", QSettings::IniFormat);
	QStringList list = settings.value("DeviceType/list").toString().split("|");

	// 按类型名找图层索引
	int index = list.indexOf(s);
	if (index < 0 || index >= gRadarLayerList.size())
		return;

	// 切换到对应类型图层，激活选择工具，由用户在地图上点选具体设备
	m_mapCanvas->setCurrentLayer(gRadarLayerList[index]);
	gRadarLayerList[index]->startEditing();
	m_mapCanvas->setMapTool(mToolSelect);
}
//�ƶ��״�
void MainWindow::moveRadarDevice()		
{	
	int iCount = 0;
	for (int i = 0; i < gRadarLayerList.size(); i++)
	{
		iCount += gRadarLayerList[i]->selectedFeatures().count();
	}

	if (iCount == 0)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请先选择一个探测光学设备！"));
		return;
	}

	QString mPath = QCoreApplication::applicationDirPath();
	QSettings settings(mPath + "/config.ini", QSettings::IniFormat);
	QString slist = settings.value("DeviceType/list").toString();
	QStringList list = slist.split("|");
	int index = -1;
	QString s=m_pRaderLayerBox->currentText();
	for (int i = 0; i < list.size(); i++)
	{
		s = s.left(2);
		if (list[i] == s)
		{
			index = i;
			break;
		}
	}

	m_mapCanvas->setMapTool(mMoveFeature);

	bool b = ui->mActionMoveRadarDev->isChecked();
	if (index>=0 && !b)//�����ѡ���ˣ���˵���Ѿ��ƶ������
	{
		gRadarLayerList[index]->commitChanges();

		//����ͶӰͼ����ȫ���״�ͼԪ,��ȫ��ɾ��
		g_pRadarTyLayer->startEditing();
		QgsFeature feat;
		QgsFeatureIterator fit = g_pRadarTyLayer->getFeatures();
		while (fit.nextFeature(feat))
		{
			g_pRadarTyLayer->deleteFeature(feat.id());
		}
		g_pRadarTyLayer->commitChanges();
		ShowRadarTip();//�����״���ʾ��ǩ
	}
}

void MainWindow::deleteRadarDevice()	//ɾ���״�
{
	int index = -1;
	QString mPath = QCoreApplication::applicationDirPath();
	QSettings settings(mPath + "/config.ini", QSettings::IniFormat);
	QString slist = settings.value("DeviceType/list").toString();
	QStringList list = slist.split("|");
	QString s = m_pRaderLayerBox->currentText();
	for (int i = 0; i < list.size(); i++)
	{
		s = s.left(2);
		if (list[i] == s)
		{
			index = i;
			break;
		}
	}

	int iCount = gRadarLayerList[index]->selectedFeatures().count();
	if (iCount<0)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请先选择一个探测光学设备！"));
		return;
	}

	QMessageBox::StandardButton idd = QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("确实要删除选中的设备吗？"), QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No);

	if (idd == QMessageBox::StandardButton::Yes)
	{
		m_mapCanvas->setCurrentLayer(gRadarLayerList[index]);
		gRadarLayerList[index]->startEditing();

		gRadarLayerList[index]->deleteSelectedFeatures();
		gRadarLayerList[index]->commitChanges();
		gRadarLayerList[index]->startEditing();
		
		//����ͶӰͼ����ȫ���״�ͼԪ,��ȫ��ɾ��
		g_pRadarTyLayer->startEditing();
		QgsFeature feat;
		QgsFeatureIterator fit = g_pRadarTyLayer->getFeatures();
		while (fit.nextFeature(feat))
		{
			g_pRadarTyLayer->deleteFeature(feat.id());
		}
		g_pRadarTyLayer->commitChanges();

		ShowRadarTip();

		refreshRadarCombox();
	}
}

//�޸��״����
int g_index = -1;
void MainWindow::fixRadarDevice()			
{
	g_index = -1;
	QString mPath = QCoreApplication::applicationDirPath();
	QSettings settings(mPath + "/config.ini", QSettings::IniFormat);
	QString slist = settings.value("DeviceType/list").toString();
	QStringList list = slist.split("|");
	QString s = m_pRaderLayerBox->currentText();
	for (int i = 0; i < list.size(); i++)
	{
		s = s.left(2);
		if (list[i] == s)
		{
			g_index = i;
			break;
		}
	}

	if (g_index == -1)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请先选择一个探测设备！"));
		return;
	}

	if (g_index >= gRadarLayerList.size() ||
		gRadarLayerList[g_index]->selectedFeatureCount() == 0)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请先选择一个探测设备！"));
		return;
	}

	m_mapCanvas->setCurrentLayer(gRadarLayerList[g_index]);
	gRadarLayerList[g_index]->startEditing();

	if (m_pSetRadarDlg)
		delete m_pSetRadarDlg;

	QgsFeature f	= gRadarLayerList[g_index]->selectedFeatures().at(0);
	QString	ID		= f.attribute(0).toString();
	int iType		= f.attribute(3).toInt();		//�豸����
	QString heigth	= f.attribute(4).toString();	//�豸�߶�
	//QString sAngle	= f.attribute(5).toString();//��ʼ�Ƕ�
	QString tAngle	= f.attribute(6).toString();	//end��Χ�Ƕ�
	QString sAzimuth		= f.attribute(7).toString();	//��λ�Ǧ�
	QString sElevation		= f.attribute(8).toString();	//�����Ǧ�
	QString svWidth = f.attribute(9).toString();	//ˮƽ��������
	QString shWidth = f.attribute(10).toString();	//��ֱ��������
	QString length	= f.attribute(11).toString();	//̽�����

	m_pSetRadarDlg	= new QsetRadarDlg(this, iType);
	
	m_pSetRadarDlg->m_fid = f.id();
	m_pSetRadarDlg->m_bInsert = false;//�޸��豸��Ϣ
	m_pSetRadarDlg->m_point = f.geometry().asPoint();

	QgsPointXY p= f.geometry().asPoint();
	QString sx = QString("%1").arg(p.x(), 0, 'g',9);
	QString sy = QString("%1").arg(p.y(), 0, 'g',9);
	
	m_pSetRadarDlg->ui.textEdit_1->setText(ID);
	m_pSetRadarDlg->ui.textEdit_2->setText(list[iType]);
	m_pSetRadarDlg->ui.textEdit_3->setText(heigth);
	m_pSetRadarDlg->ui.textEdit_4->setText(sx + "," + sy);
	m_pSetRadarDlg->ui.textEdit_6->setText(tAngle);
	m_pSetRadarDlg->ui.textEdit_7->setText(sAzimuth);
	m_pSetRadarDlg->ui.textEdit_8->setText(sElevation);
	m_pSetRadarDlg->ui.textEdit_9->setText(svWidth);
	m_pSetRadarDlg->ui.textEdit_10->setText(shWidth);
	m_pSetRadarDlg->ui.textEdit_11->setText(length);

	m_pSetRadarDlg->show();
	m_pSetRadarDlg->activateWindow();
}

//�״�̽�����˻��б�
void MainWindow::RadarTestAirList()
{
	if (m_pDlgAirList == nullptr)
	{
		m_pDlgAirList = new Dialog_AirList(this);
	}
	m_pDlgAirList->show();
	//m_pDlgAirList->raise();
	m_pDlgAirList->activateWindow();
}

void MainWindow::refreshRadarCombox()       //ˢ���״���Ͽ�ؼ�
{
	if (m_pRaderLayerBox == nullptr)
	{
		QMessageBox::information(this, "SUCCESS", QString::fromLocal8Bit("已将探测设备从地图上移除成功！"));
		this->close();
		return;
	}
	while (m_pRaderLayerBox->count()>0)
	{
		m_pRaderLayerBox->removeItem(0);
	}

	QString mPath = QCoreApplication::applicationDirPath();
	QSettings settings(mPath + "/config.ini", QSettings::IniFormat);
	QString slist = settings.value("DeviceType/list").toString();
	QStringList list = slist.split("|");

	for (int i = 0; i < gRadarLayerList.size(); i++)
	{
		QgsFeature feat;
		QgsFeatureIterator fit = gRadarLayerList[i]->getFeatures();
		while (fit.nextFeature(feat))
		{
			QString		ID = feat.attribute(0).toString();
			int			type = feat.attribute(3).toInt();	//����

			QString str = list[type] + ID;
			m_pRaderLayerBox->addItem(str);
		}
	}
	
}