#include "stdafx.h"
#include "mainWindow.h"
#include "ui_mainwindow.h"
#include "Biaopai.h"
#include <qgsNullSymbolRenderer.h>

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstring>

#include <filesystem>
#include <iostream>
//namespace fs = std::filesystem;

#include <QComboBox>
#include <QDateTime>
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
				QString tAngle = feat.attribute(9).toString();//ˮƽ��������
				QString sAzimuth = feat.attribute(7).toString();//��ʼ��λ�Ǧ�
				QString sElevation = feat.attribute(8).toString();//�����Ǧ�
				QString svWidth = feat.attribute(9).toString();//ˮƽ��������
				QString shWidth = feat.attribute(10).toString();//��ֱ��������
				QString length = feat.attribute(11).toString();//ͶӰ����

				//�����豸��Ϣ�Զ�����ͶӰ�����ȫ����
				QList<QgsPointXY> PointSet;
				PointSet = GetTYPolygon(devPt, sAzimuth.toFloat(), tAngle.toInt(), sElevation.toFloat(), length.toInt(), shWidth.toFloat(), feat.attribute(4).toFloat());

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
//根据方位角、水平波束宽度、俯仰角、垂直波束宽度、探测距离，生成地面投影多边形
QList<QgsPointXY> MainWindow::GetTYPolygon(QgsPointXY devPt, float sAngle, float tAngle, float elevation, int length, float vBeam, float devHeight)
{
	QList<QgsPointXY> pset;

	// 向下俯视（elevation < 0）且有设备高度：基于高度的地面投影
	if (elevation < 0 && devHeight > 0 && vBeam > 0)
	{
		double nLen = length * 1000.0;
		double hMm = devHeight * 1000.0;
		double vbh2 = vBeam / 2.0;
		double elevRad = elevation * M_PI_PRE_DEGREE;
		double azCRad = (sAngle + tAngle / 2.0) * M_PI_PRE_DEGREE;
		double kx = -cos(azCRad), kz = sin(azCRad);
		double cosT = cos(elevRad), sinT = sin(elevRad);

		auto projectPt = [&](double azDeg, double eLocalDeg) -> QgsPointXY {
			double a = azDeg * M_PI_PRE_DEGREE;
			double el = eLocalDeg * M_PI_PRE_DEGREE;
			double ce = cos(el), se = sin(el);
			double px = ce * sin(a), py = se, pz = ce * cos(a);
			double kdp = kx * px + kz * pz;
			double rx = px * cosT + (-kz * py) * sinT + kx * kdp * (1 - cosT);
			double ry = py * cosT + (kz * px - kx * pz) * sinT;
			double rz = pz * cosT + (kx * py) * sinT + kz * kdp * (1 - cosT);
			double t;
			if (ry < -0.001)
				t = qMin(hMm / fabs(ry), nLen);
			else
				t = nLen;
			return QgsPointXY(
				devPt.x() + t * rx / MM_PER_LONGITUDE_BYLAT(devPt.y()),
				devPt.y() + t * rz / MM_PER_LATITUDE);
		};

		int N = qMax((int)tAngle, 1);
		int M = qMax((int)vBeam / 3, 2);
		for (int i = 0; i <= N; i++)
			pset.append(projectPt(sAngle + (double)i, -vbh2));
		for (int j = 1; j < M; j++)
			pset.append(projectPt(sAngle + tAngle, -vbh2 + vBeam * j / M));
		for (int i = N; i >= 0; i--)
			pset.append(projectPt(sAngle + (double)i, vbh2));
		for (int j = M - 1; j > 0; j--)
			pset.append(projectPt(sAngle, -vbh2 + vBeam * j / M));
		return pset;
	}

	float effPitch = (vBeam > 0) ? qMax(0.0f, elevation - vBeam / 2.0f) : elevation;
	bool crossZenith = (vBeam > 0) && (elevation + vBeam / 2.0f > 90.0f);

	if (!crossZenith)
	{
		// 波束不过天顶：扇形投影，使用波束下边缘角度做地面距离
		pset.append(devPt);
		for (int i = 0; i <= (int)tAngle; i++)
		{
			QgsPoint xy = GetResult(devPt.x(), devPt.y(), sAngle + i, effPitch, length * 1000);
			pset.append(QgsPointXY(xy.x(), xy.y()));
		}
		return pset;
	}

	// 波束越过天顶：3D旋转投影产生四边形
	double nLen = length * 1000.0;
	double vbh2 = vBeam / 2.0 * M_PI_PRE_DEGREE;
	double elevRad = elevation * M_PI_PRE_DEGREE;
	double azCRad = (sAngle + tAngle / 2.0) * M_PI_PRE_DEGREE;
	double kx = -cos(azCRad), kz = sin(azCRad);
	double cosT = cos(elevRad), sinT = sin(elevRad);

	auto projectPt = [&](double azDeg, double eLocal) -> QgsPointXY {
		double a = azDeg * M_PI_PRE_DEGREE;
		double ce = cos(eLocal), se = sin(eLocal);
		double px = nLen * ce * sin(a);
		double py = nLen * se;
		double pz = nLen * ce * cos(a);
		double kcpx = -kz * py, kcpz = kx * py;
		double kdp = kx * px + kz * pz;
		double rx = px * cosT + kcpx * sinT + kx * kdp * (1 - cosT);
		double rz = pz * cosT + kcpz * sinT + kz * kdp * (1 - cosT);
		return QgsPointXY(
			devPt.x() + rx / MM_PER_LONGITUDE_BYLAT(devPt.y()),
			devPt.y() + rz / MM_PER_LATITUDE);
	};

	int N = qMax((int)tAngle, 1);
	int M = qMax((int)vBeam / 3, 2);
	for (int i = 0; i <= N; i++)
		pset.append(projectPt(sAngle + (double)i, -vbh2));
	for (int j = 1; j < M; j++)
		pset.append(projectPt(sAngle + tAngle, -vbh2 + vBeam * M_PI_PRE_DEGREE * j / M));
	for (int i = N; i >= 0; i--)
		pset.append(projectPt(sAngle + (double)i, vbh2));
	for (int j = M - 1; j > 0; j--)
		pset.append(projectPt(sAngle, -vbh2 + vBeam * M_PI_PRE_DEGREE * j / M));

	return pset;
}
//�����״���Ϣ��������ʾͼԪ
void MainWindow::ShowRadarTip()
{
	// canvas 正在渲染时，setMapPosition→updatePosition→toCanvasCoordinates
	// 访问 canvas 共享 QVector，会触发 isDetached() 断言崩溃 → 延迟执行
	if (m_mapCanvas && m_mapCanvas->isDrawing()) {
		QTimer::singleShot(50, this, &MainWindow::ShowRadarTip);
		return;
	}

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
		if (!gRadarLayerList[i]) continue;
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

QList <int> MainWindow::isRadarInPoly(tag_PlaneMessage p)
{
	QgsFeature		f;
	QList<int>		list;

	double planeLon = p.planeX.toDouble();
	double planeLat = p.planeY.toDouble();
	double planeAlt = p.hZ.toDouble();
	if (planeAlt <= 0) planeAlt = p.xZ.toDouble();

	for (int i = 0; i < gRadarLayerList.size(); i++)
	{
		QgsFeatureIterator	fit = gRadarLayerList[i]->getFeatures();

		while (fit.nextFeature(f))
		{
			QgsPointXY pt = f.geometry().asPoint();
			double radarLon = pt.x();
			double radarLat = pt.y();
			double radarAlt = f.attribute(4).toDouble();		// 设备高度(m)
			double azStart  = f.attribute(7).toDouble();		// 起始方位角(度, 0=北, 顺时针)
			double elevC    = f.attribute(8).toDouble();		// 俯仰角(度)
			double hBeam    = f.attribute(9).toDouble();		// 水平波束宽度(度)
			double vBeam    = f.attribute(10).toDouble();		// 垂直波束宽度(度)
			double range    = f.attribute(11).toDouble();		// 探测距离(m)

			// 转换为本地坐标系(米): dx=东向, dy=北向, dz=高度差
			double dx = (planeLon - radarLon) * MM_PER_LONGITUDE_BYLAT(radarLat) / 1000.0;
			double dy = (planeLat - radarLat) * MM_PER_LATITUDE / 1000.0;
			double dz = planeAlt - radarAlt;

			// 三维距离判断
			double dist3D = sqrt(dx * dx + dy * dy + dz * dz);
			if (dist3D > range || dist3D < 0.1)
				continue;

			// 计算目标方位角(度, 0=北, 顺时针)
			double targetAz = atan2(dx, dy) * 180.0 / M_PI;
			if (targetAz < 0) targetAz += 360.0;

			// 方位角范围检查 [azStart, azStart+hBeam]，处理360°环绕
			double azS = fmod(azStart, 360.0);
			if (azS < 0) azS += 360.0;
			double azE = azS + hBeam;
			bool azOK;
			if (azE <= 360.0)
				azOK = (targetAz >= azS && targetAz <= azE);
			else
				azOK = (targetAz >= azS || targetAz <= fmod(azE, 360.0));
			if (!azOK)
				continue;

			// 计算目标俯仰角(度)
			double hDist = sqrt(dx * dx + dy * dy);
			double targetElev = atan2(dz, hDist) * 180.0 / M_PI;

			// 俯仰角范围检查 [elevC - vBeam/2, elevC + vBeam/2]
			double vHalf = (vBeam > 0) ? vBeam / 2.0 : 15.0;  // 默认±15°
			if (targetElev < elevC - vHalf || targetElev > elevC + vHalf)
				continue;

			list.append(f.attribute(0).toInt());
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
				QString tAngle = f.attribute(9).toString();	//ˮƽ��������
				QString length = f.attribute(11).toString();	//̽�����

				pt = info.tlist[m_iRaderTckID];
				QgsGeometry geometry = QgsGeometry::fromPointXY(pt);
				f.setGeometry(geometry);

				QgsAttributes attribues;
				attribues << info.id << 1 << 1 << sType.toInt() << heigth << sAngle << tAngle << length;
				f.setAttributes(attribues);
				gRadarLayerList[i]->updateFeature(f);
			}

			//��ʼ�ƶ��״���ʾͼԪ（canvas 空闲时才设位置，避免 isDetached 崩溃）
			if (!(m_mapCanvas && m_mapCanvas->isDrawing())) {
				foreach(RadarTip * r, m_radarTipList) {
					if (r->m_id == info.id.toInt())
					{
						r->setPos(pt);
						break;
					}
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
	// 一次性：将飞机矢量图层改为空渲染器（图标改由 biaopai QGraphicsItem 显示，无需 canvas 重绘）
	// 层数据和空间查询不受影响，仅不再绘制图标（消除 10fps 跳变）
	static bool s_airLayerIconHidden = false;
	if (!s_airLayerIconHidden && g_pAirLayer)
	{
		g_pAirLayer->setRenderer(new QgsNullSymbolRenderer());
		s_airLayerIconHidden = true;
	}

	processAllPlaneUpdates();

	// triggerRepaint 已无需（飞机图层用 QgsNullSymbolRenderer 渲染空，biaopai 走 QGraphicsItem 路径）
	m_bAirLayerDirty = false;

	// 轨迹记录开启时，每 2s 自动将缓冲点渲染到图层（20 × 100ms）
	static int s_trackTick = 0;
	if (m_bTrackEnabled && ++s_trackTick >= 20) {
		s_trackTick = 0;
		RefreshGj();
	}
}

// 16ms 插值定时器：平滑更新所有 PlaneIconItem 的屏幕位置
// GPS 100ms 更新一次，此处在每帧（~60fps）对图标位置做线性插值，消除跳帧卡顿
void MainWindow::onInterpTimer()
{
	// toCanvasCoordinates() 访问 canvas 内部 QVector；渲染线程持有共享副本时
	// 主线程写操作会触发 isDetached() 断言崩溃 → 仅在 canvas 空闲时执行
	if (m_planeIDvec.isEmpty()) return;
	if (m_mapCanvas && m_mapCanvas->isDrawing()) return;
	qint64 nowMs = QDateTime::currentMSecsSinceEpoch();
	for (biaopai* pPai : m_planeIDvec)
		pPai->interpolate(nowMs);
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
	for (int i = 0; i < gRadarLayerList.size(); i++)
	{
		if (gRadarLayerList[i])
			gRadarLayerList[i]->startEditing();
	}
	// 使用多图层点选工具，点击地图时自动搜索所有装备图层
	m_mapCanvas->setMapTool(mToolRadarPick);
}

// 多图层装备点选：在所有装备图层中搜索最近的要素并选中
void MainWindow::onRadarPick(const QgsPointXY &pt, Qt::MouseButton btn)
{
	if (btn != Qt::LeftButton) return;

	// 根据当前地图缩放计算容差（像素 → 地图单位）
	double tol = m_mapCanvas->mapUnitsPerPixel() * 12.0;
	QgsRectangle searchRect(pt.x() - tol, pt.y() - tol, pt.x() + tol, pt.y() + tol);

	double bestDist = 1e30;
	QgsFeatureId bestFid = -1;
	int bestLayer = -1;

	for (int i = 0; i < gRadarLayerList.size(); i++)
	{
		QgsVectorLayer *lyr = gRadarLayerList[i];
		if (!lyr) continue;

		QgsFeatureRequest req;
		req.setFilterRect(searchRect);
		req.setNoAttributes();
		QgsFeatureIterator fit = lyr->getFeatures(req);
		QgsFeature f;
		while (fit.nextFeature(f))
		{
			QgsGeometry g = f.geometry();
			if (g.isNull()) continue;
			double d = g.distance(QgsGeometry::fromPointXY(pt));
			if (d < bestDist)
			{
				bestDist = d;
				bestFid = f.id();
				bestLayer = i;
			}
		}
	}

	// 先清除所有装备图层的选中状态
	for (int i = 0; i < gRadarLayerList.size(); i++)
	{
		if (gRadarLayerList[i])
			gRadarLayerList[i]->removeSelection();
	}

	if (bestLayer >= 0)
	{
		m_mapCanvas->setCurrentLayer(gRadarLayerList[bestLayer]);
		gRadarLayerList[bestLayer]->selectByIds(QgsFeatureIds() << bestFid);
	}
}

// 查找有选中要素的装备图层索引（替代下拉框）
int MainWindow::findSelectedRadarLayerIndex()
{
	for (int i = 0; i < gRadarLayerList.size(); i++)
	{
		if (gRadarLayerList[i] && gRadarLayerList[i]->selectedFeatureCount() > 0)
			return i;
	}
	return -1;
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
	int index = findSelectedRadarLayerIndex();
	if (index < 0)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请先选择一个探测设备！"));
		return;
	}

	bool b = ui->mActionMoveRadarDev->isChecked();
	if (b)
	{
		// enter move mode: ensure layer is editable
		if (!gRadarLayerList[index]->isEditable())
			gRadarLayerList[index]->startEditing();
		m_mapCanvas->setMapTool(mMoveFeature);
	}
	else
	{
		m_mapCanvas->setMapTool(mMoveFeature);
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
		ShowRadarTip();
		syncLeafletAll();
		sync3DAll();
	}
}

void MainWindow::deleteRadarDevice()
{
	int index = findSelectedRadarLayerIndex();
	if (index < 0)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请先选择一个探测设备！"));
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
		syncLeafletAll();
		sync3DAll();

		refreshRadarCombox();
	}
}

//�޸��״����
int g_index = -1;
void MainWindow::fixRadarDevice()
{
	g_index = findSelectedRadarLayerIndex();

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
	int iType		= f.attribute(3).toInt();
	QString heigth	= f.attribute(4).toString();
	QString sAzimuth		= f.attribute(7).toString();
	QString sElevation		= f.attribute(8).toString();
	QString svWidth = f.attribute(9).toString();
	QString shWidth = f.attribute(10).toString();
	QString length	= f.attribute(11).toString();

	m_pSetRadarDlg	= new QsetRadarDlg(this, iType);

	m_pSetRadarDlg->m_fid = f.id();
	m_pSetRadarDlg->m_bInsert = false;
	m_pSetRadarDlg->m_point = f.geometry().asPoint();

	QgsPointXY p= f.geometry().asPoint();
	QString sx = QString("%1").arg(p.x(), 0, 'g',9);
	QString sy = QString("%1").arg(p.y(), 0, 'g',9);

	// 读取设备类型名称列表
	QString mPath2 = QCoreApplication::applicationDirPath();
	QSettings settings2(mPath2 + "/config.ini", QSettings::IniFormat);
	QStringList typeNames = settings2.value("DeviceType/list").toString().split("|");
	QString typeName = (iType >= 0 && iType < typeNames.size()) ? typeNames[iType] : QString::number(iType);

	m_pSetRadarDlg->ui.textEdit_1->setText(ID);
	m_pSetRadarDlg->ui.textEdit_2->setText(typeName);
	m_pSetRadarDlg->ui.textEdit_3->setText(heigth);
	m_pSetRadarDlg->ui.textEdit_4->setText(sx + "," + sy);
	m_pSetRadarDlg->ui.textEdit_6->setText(svWidth);
	m_pSetRadarDlg->ui.textEdit_7->setText(sAzimuth);
	m_pSetRadarDlg->ui.textEdit_8->setText(sElevation);
	m_pSetRadarDlg->ui.textEdit_9->setText(svWidth);
	m_pSetRadarDlg->ui.textEdit_10->setText(shWidth);
	m_pSetRadarDlg->ui.textEdit_11->setText(length);

	// 显示当前装载无人机机号（若已装载）
	{
		QString mPath2 = QCoreApplication::applicationDirPath();
		QSettings mountCfg(mPath2 + "/radar_mount.ini", QSettings::IniFormat);
		QString uavId = mountCfg.value(QString("RadarMount/radar_%1").arg(ID)).toString();
		m_pSetRadarDlg->ui.textEdit_12->setText(uavId);
	}

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

void MainWindow::refreshRadarCombox()
{
	// 下拉框已移除，此函数保留为空
}

// 实时更新装载设备的探测范围投影（删除旧扇形 → 生成新扇形），由 processAllPlaneUpdates 每 500ms 调用一次
void MainWindow::updateMountedRadarProjection(int radarId, const QgsFeature& radarFeat, QgsPointXY newPt)
{
	if (!g_pRadarTyLayer)
		return;

	if (!g_pRadarTyLayer->isEditable())
		g_pRadarTyLayer->startEditing();

	// 删除此设备的旧投影（attribute[0] 存储设备 ID）
	QgsFeature feat;
	QgsFeatureIterator fit = g_pRadarTyLayer->getFeatures();
	while (fit.nextFeature(feat))
	{
		if (feat.attribute(0).toInt() == radarId)
			g_pRadarTyLayer->deleteFeature(feat.id());
	}

	// 从雷达特征读取探测参数（这些参数不随位移变化）
	QString tAngle     = radarFeat.attribute(9).toString();   // 水平波束宽度
	QString sAzimuth   = radarFeat.attribute(7).toString();   // 方位角θ
	QString sElevation = radarFeat.attribute(8).toString();   // 俯仰角α
	QString sVBeam     = radarFeat.attribute(10).toString();  // 垂直波束宽度
	QString length     = radarFeat.attribute(11).toString();  // 探测距离(米)

	// 生成新位置的探测锥投影多边形
	QList<QgsPointXY> pts = GetTYPolygon(newPt,
		sAzimuth.toFloat(), tAngle.toInt(),
		sElevation.toFloat(), length.toInt(), sVBeam.toFloat(),
		radarFeat.attribute(4).toFloat());

	if (!pts.isEmpty())
	{
		QgsPolygonXY pxy = QgsPolygonXY() << pts.toVector();
		QgsGeometry  geom = QgsGeometry::fromPolygonXY(pxy);
		QgsFeature   nf;
		nf.setGeometry(geom);
		nf.setAttributes(QgsAttributes() << QString::number(radarId));
		g_pRadarTyLayer->addFeature(nf);
	}

	g_pRadarTyLayer->triggerRepaint();
}

// 从 radar_mount.ini 加载探测设备与无人机的装载关系到 m_radarUavMount
void MainWindow::loadRadarUavMount()
{
	m_radarUavMount.clear();
	QString mPath = QCoreApplication::applicationDirPath();
	QSettings cfg(mPath + "/radar_mount.ini", QSettings::IniFormat);
	cfg.beginGroup("RadarMount");
	for (const QString &key : cfg.childKeys())
	{
		// key 格式: "radar_ID"，值为无人机机号
		int radarId = key.mid(6).toInt();   // 去掉前缀 "radar_"
		QString uavId = cfg.value(key).toString().trimmed();
		if (!uavId.isEmpty() && uavId != "0")
			m_radarUavMount[radarId] = uavId;
	}
	cfg.endGroup();
}

// 构建探测设备完整JSON数组（含方位角、波束宽度、探测距离）
QString MainWindow::buildRadarsFullJson()
{
	QString arr = "[";
	bool first = true;
	for (int i = 0; i < gRadarLayerList.size(); i++)
	{
		if (!gRadarLayerList[i]) continue;
		QgsFeatureIterator fit = gRadarLayerList[i]->getFeatures();
		QgsFeature f;
		while (fit.nextFeature(f))
		{
			QgsGeometry geom = f.geometry();
			if (geom.isNull()) continue;
			QgsPointXY pt = geom.asPoint();
			int rid = f.attribute(0).toInt();
			bool mounted = m_radarUavMount.contains(rid);
			if (!first) arr += ",";
			arr += QString("{\"id\":%1,\"lon\":%2,\"lat\":%3,\"alt\":%4,"
			               "\"azimuth\":%5,\"hBeam\":%6,\"range\":%7,\"type\":%8,"
			               "\"elevation\":%9,\"vBeam\":%10,\"mounted\":%11}")
				.arg(rid)
				.arg(pt.x(), 0, 'f', 7)
				.arg(pt.y(), 0, 'f', 7)
				.arg(f.attribute(4).toDouble(), 0, 'f', 1)
				.arg(f.attribute(7).toDouble(), 0, 'f', 1)   // 方位角θ
				.arg(f.attribute(9).toDouble(), 0, 'f', 1)   // 水平波束宽度
				.arg(f.attribute(11).toDouble(), 0, 'f', 0)  // 探测距离(m)
				.arg(i)                                       // 装备类型(0-4)
				.arg(f.attribute(8).toDouble(), 0, 'f', 1)   // 俯仰角α
				.arg(f.attribute(10).toDouble(), 0, 'f', 1)  // 垂直波束宽度
				.arg(mounted ? "true" : "false");
			first = false;
		}
	}
	return arr + "]";
}

// 构建当前所有无人机快照JSON数组（用于切换到3D视图时批量同步）
QString MainWindow::buildPlanesJson()
{
	QString arr = "[";
	bool first = true;
	for (auto it = m_latestPlaneData.begin(); it != m_latestPlaneData.end(); ++it)
	{
		const tag_PlaneMessage &p = it.value();
		if (p.planeX.isEmpty() || p.planeY.isEmpty()) continue;
		double alt = p.hZ.toDouble();
		if (alt <= 0) alt = p.xZ.toDouble();
		if (!first) arr += ",";
		arr += QString("{\"id\":\"%1\",\"lon\":%2,\"lat\":%3,\"alt\":%4,\"yaw\":%5}")
			.arg(p.ID)
			.arg(p.planeX.toDouble(), 0, 'f', 7)
			.arg(p.planeY.toDouble(), 0, 'f', 7)
			.arg(alt, 0, 'f', 1)
			.arg(p.Yaw.toDouble(), 0, 'f', 1);
		first = false;
	}
	return arr + "]";
}

// 构建任务区域多边形JSON数组
QString MainWindow::buildTaskAreaJson()
{
	if (!g_pAirTaskPolyLayer) return "[]";
	QString arr = "[";
	bool first = true;
	QgsFeatureIterator fit = g_pAirTaskPolyLayer->getFeatures();
	QgsFeature f;
	while (fit.nextFeature(f))
	{
		QgsGeometry geom = f.geometry();
		if (geom.isNull()) continue;
		QgsPolygonXY poly = geom.asPolygon();
		if (poly.isEmpty() || poly[0].size() < 3)
		{
			// try multipolygon fallback
			QgsMultiPolygonXY mpoly = geom.asMultiPolygon();
			if (mpoly.isEmpty() || mpoly[0].isEmpty() || mpoly[0][0].size() < 3) continue;
			poly = mpoly[0];
		}
		double minH = f.attribute(5).toDouble(); // 高度下限: 距地面最小高度(米)
		double maxH = f.attribute(6).toDouble(); // 高度上限: 距地面最大高度(米)
		if (minH < 0) minH = 0;
		if (maxH <= minH) maxH = minH + 100.0;
		if (!first) arr += ",";
		arr += QString("{\"id\":%1,\"minH\":%2,\"maxH\":%3,\"coords\":[")
			.arg(f.attribute(0).toInt())
			.arg(minH, 0, 'f', 1)
			.arg(maxH, 0, 'f', 1);
		const QgsPolylineXY &ring = poly[0];
		for (int k = 0; k < ring.size(); k++)
		{
			if (k) arr += ",";
			arr += QString("[%1,%2]")
				.arg(ring[k].x(), 0, 'f', 7)
				.arg(ring[k].y(), 0, 'f', 7);
		}
		arr += "]}";
		first = false;
	}
	return arr + "]";
}