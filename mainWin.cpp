#include "stdafx.h"
#include "mainWindow.h"
#include "ui_mainwindow.h"
#include "Biaopai.h"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstring>

#include <QComboBox>
#include <QLineEdit>
#include <QGridLayout>

// 数值排序的表格项（ID列按整数大小而非字符串排序）
class NumericTableItem : public QTableWidgetItem {
public:
    explicit NumericTableItem(const QString &text) : QTableWidgetItem(text) {}
    bool operator<(const QTableWidgetItem &other) const override {
        return text().toInt() < other.text().toInt();
    }
};
#include <QFileDialog>
#include <QMessageBox>
#include <QTextcodec.h>

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
//#include <qgsWeakRelation.h>
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

#include "qgsMapCanvasAnnotationitem2.h"
#include "jpgAnnotation.h"
#include "qgsSvgAnnotation.h"
#include "qdialogTarget.h"
#include "dialogSendGrid.h"
#include "layerTreeViewMenuProvider.h"
#include "qSetLayerDialog.h"

#include "dataBackDlg.h"
#include "mavlink5Thread.h"
#include "FixPlaneThread.h"
#include "qgsAnnotation2.h"
#include "qgsLineAnnotation.h"
#include "polygon2.h"
#include "recMavlinkThread.h"
//#include <boost/geometry.hpp>
//#include <boost/geometry/geometries/point_xy.hpp>
//#include <boost/geometry/geometries/polygon.hpp>
//#include <boost/foreach.hpp>
//#include <boost\assign.hpp>

#include <gdal.h>
#include <gdal_priv.h>

#include "Dlg3Dtest.h"
//Polygon poly;
void MainWindow::ReadIniFile()
{
	//boost::geometry::read_wkt()
	QString mPath = QCoreApplication::applicationDirPath();
	QSettings settings(mPath +"/helloPlane.ini", QSettings::IniFormat);
	localAlorithmPort = settings.value("Local/localAlorithmPort").toString();
	localBundancePort = settings.value("Local/localbundanceport").toString();
	localAttackPort = settings.value("Local/localAttackPort").toString();
	localIP = settings.value("Local/localIP").toString();
	localPlanePort = settings.value("Local/localPlanePort").toString();
	localTaskPort = settings.value("Local/localTaskPort").toString();
	localPicture = settings.value("Local/localPicture").toString();
	localEletricPort = settings.value("Local/localEletricPort").toString();

	algorithmsIp   = settings.value("Other/algorithmsIp").toString();     //
	algorithmsPort = settings.value("Other/algorithmsPort").toString();     //

	bundancePort= settings.value("Other/bundancePort").toString();     //
	bundanceIP	= settings.value("Other/bundanceIP").toString();

	getPlaneIP = settings.value("Other/getPlaneIP").toString();
	getPlanePort = settings.value("Other/getPlanePort").toString();
	sendTaskIP = settings.value("Other/sendTaskIP").toString();
	sendTaskPort = settings.value("Other/sendTaskPort").toString();
	attackerIP = settings.value("Other/attackerIP").toString();
	attackerPort = settings.value("Other/attackerPort").toString();

	WeaponTypeList = settings.value("WeaponType/WeaponTypeList").toString();
	g_taskList = settings.value("WeaponType/taskList").toString();
	g_targetType = settings.value("WeaponType/targetType").toString();
	g_targetTask = settings.value("WeaponType/targetTask").toString();
}

//ˢ�¹켣��,�����еĵ㵽ͼ����
void MainWindow::RefreshGj()
{
	// 渲染期间不写数据，避免写锁竞争；下次计时器到来时自动补刷
	if (m_mapCanvas && m_mapCanvas->isDrawing())
		return;

	// 没有新轨迹点则直接返回，避免无意义的 startEditing/commitChanges 触发多余重绘
	g_planeMutex.lock();
	bool hasRadar = !g_RadarTrackList.isEmpty();
	bool hasPlane = !g_PlaneList.isEmpty();
	g_planeMutex.unlock();
	if (!hasRadar && !hasPlane)
		return;

	bool b;
	if (hasRadar) g_pRadarPtLayer->startEditing();
	if (hasPlane)  g_pPointLayer->startEditing();
	g_planeMutex.lock();

	if (hasRadar)
	{
		foreach(QgsPointXY p, g_RadarTrackList)
		{
			QgsGeometry geometry = QgsGeometry::fromPointXY(p);
			if (!geometry.isGeosValid()) {
				qDebug() << "[RefreshGj] RadarTrack: invalid geometry, skipped";
				continue;
			}
			QgsFeature ftpt = QgsFeature();
			ftpt.setGeometry(geometry);
			ftpt.setAttributes(QgsAttributes() << (int)g_pRadarPtLayer->featureCount() + 1
				<< QString::number(p.y(), 'f', 7) << QString::number(p.x(), 'f', 7) << QString::number(0) << "other" << "factor");
			b = g_pRadarPtLayer->addFeature(ftpt);
		}
	}

	if (hasPlane)
	{
		foreach(QgsPointXY p, g_PlaneList)
		{
			QgsGeometry geometry = QgsGeometry::fromPointXY(p);
			if (!geometry.isGeosValid()) {
				qDebug() << "[RefreshGj] PlaneList: invalid geometry, skipped";
				continue;
			}
			QgsFeature ftpt = QgsFeature();
			ftpt.setGeometry(geometry);
			ftpt.setAttributes(QgsAttributes() << (int)g_pPointLayer->featureCount() + 1
				<< QString::number(p.y(), 'f', 7) << QString::number(p.x(), 'f', 7) << QString::number(0) << "other" << "factor");
			b = g_pPointLayer->addFeature(ftpt);
		}
	}

	g_RadarTrackList.clear();
	g_PlaneList.clear();

	g_planeMutex.unlock();
	// commitChanges 内部已调用 triggerRepaint，无需再次显式调用，避免每次提交触发两次重绘
	if (hasRadar) g_pRadarPtLayer->commitChanges();
	if (hasPlane)  g_pPointLayer->commitChanges();
}

// Toggle real-time trajectory recording on/off
void MainWindow::toggleTrack()
{
	m_bTrackEnabled = !m_bTrackEnabled;
	if (m_pActTrack)
		m_pActTrack->setChecked(m_bTrackEnabled);
}

// Clear all trajectory features from map layers without stopping future recording
void MainWindow::clearTrajectory()
{
	// Drop any pending (not yet rendered) points
	g_planeMutex.lock();
	g_PlaneList.clear();
	g_RadarTrackList.clear();
	g_planeMutex.unlock();

	// Delete all features from each trajectory layer
	QgsVectorLayer *layers[] = { g_pPointLayer, g_pRadarPtLayer };
	for (QgsVectorLayer *layer : layers) {
		if (!layer) continue;
		QgsFeatureIds fids = layer->allFeatureIds();
		if (fids.isEmpty()) continue;
		layer->startEditing();
		layer->deleteFeatures(fids);
		layer->commitChanges();  // commitChanges 内部已触发 repaintRequested
	}

	// Clear 3D trail lines
	if (m_3dReady && m_pWebEngineView)
		m_pWebEngineView->page()->runJavaScript(
			"if(typeof clearTrails3D==='function') clearTrails3D();");
}

DWORD iLastTick = GetTickCount(); //���һ���������˻�ʱ�䣬
int		count = 0;
//����mavlinkЭ�飬�յ��ķɻ���Ϣ�������˻�ͼ��������һ���ɻ�
bool MainWindow::addPlaneLayer(tag_PlaneMessage plane)
{
	qDebug() << "[addPlaneLayer] ID=" << plane.ID
	         << "diff=" << (int)(GetTickCount()-iLastTick) << "pass=" << (GetTickCount()>iLastTick+500);
	if (GetTickCount() > iLastTick +500)
	{
		//�����˻�ͼ��������һ�����˻�ͼԪ
		g_pAirLayer->startEditing();
		QgsGeometry geometry = QgsGeometry::fromPointXY(QgsPointXY(plane.planeX.toDouble(), plane.planeY.toDouble()));
		//   qDebug()<<pointTarget.asPoint().x()<<","<<pointTarget.asPoint().y()<<"/n";
		QgsFeature ftpt = QgsFeature();
		ftpt.setGeometry(geometry);

		if (!geometry.isGeosValid()) {
			qDebug() << "[addPlaneLayer] invalid geometry for plane ID=" << plane.ID;
			return false;
		}

		ftpt.setAttributes(QgsAttributes() <<(int)plane.ID.toInt()
			<<plane.planeY << plane.planeX<< QString::number(0) <<plane.Yaw);
		g_pAirLayer->addFeature(ftpt);
		g_pAirLayer->commitChanges();
		g_pAirLayer->triggerRepaint();		// 立即显示新飞机图标，不等100ms定时器

		//����һ���޸����˻�λ���߳�
		FixPlaneThread *pPlaneThread = new FixPlaneThread();
		connect(this, &MainWindow::FixPlaneMsg, pPlaneThread, &FixPlaneThread::FixPlane);
		pPlaneThread->m_id		= plane.ID.toInt();
		pPlaneThread->start();
		m_planeThreadVec.append(pPlaneThread);	// 跟踪线程以便断开时清理

		qDebug() << "Plane key:" << plane.ID << endl;//����/ֹͣ����	
		iLastTick = GetTickCount();
		return true;
	}
	return false;
}

void MainWindow::addFeatureToMap(Mappoint *pUser)
{
	QLabel		*lable = (QLabel*)pUser->picture;
	QByteArray	array;
	QBuffer		buffer(&array);
	buffer.open(QIODevice::WriteOnly);
	lable->pixmap()->save(&buffer, "JPG");

	insertPlaneMessage(pUser, array);		//�������˻���Ϣ��sqllite���ݿ���...

	//emit AddJpgAnnotaionMsg(pUser, 128, 128,0); //����һ���Զ�����Ϣ
	delete lable;
	pUser->picture = nullptr;
}

//�������˻���Ϣ��sqllite���ݿ���...
bool MainWindow::insertPlaneMessage(Mappoint* message, QByteArray& bytearray)
{
	QString insert_sql = "insert into objectImage(messageId,planeId,taskname,typename,planePositionX,planePostionY,planetime,"
		"minJpgX,minJpgY,maxJpgX,maxJpgY,angleJpgX,angleJpgY,ishanded,jpg) "
		"values ( :messageId,:planeId,:taskname,:typename,:planePositionX,:planePostionY,:planetime,"
		":minJpgX,:minJpgY,:maxJpgX,:maxJpgY,:angleJpgX,:angleJpgY,:ishanded,:jpg)";

	QSqlQuery sql_query(g_sqliteDbase);
	sql_query.prepare(insert_sql);
	sql_query.bindValue(":messageId", message->messageId);
	sql_query.bindValue(":planeId", message->planeId);
	sql_query.bindValue(":taskname", message->taskname);
	sql_query.bindValue(":typename", message->weaponname);
	sql_query.bindValue(":planePositionX", message->x);
	sql_query.bindValue(":planePostionY", message->y);
	sql_query.bindValue(":planetime", message->planeTime);
	sql_query.bindValue(":minJpgX", message->xmin);
	sql_query.bindValue(":minJpgY", message->ymin);
	sql_query.bindValue(":maxJpgX", message->xmax);
	sql_query.bindValue(":maxJpgY", message->ymax);

	sql_query.bindValue(":angleJpgX", message->angleX);
	sql_query.bindValue(":angleJpgY", message->angleY);
	sql_query.bindValue(":ishanded", message->ishanded);

	sql_query.bindValue(":jpg", bytearray);

	if (!sql_query.exec())
	{
		qDebug() << sql_query.lastError();
	}
	return true;
}

//����ѯ�����е��б�������һ�����˻�����
void MainWindow::appendImage(QString currentID, const QByteArray& array)
{
	const char* ch = array.data();
	uint8_t objectId = (uint8_t)*(ch + 1);	// 用无符号类型，避免负数作为数组下标越界
	double planePositionX = *((double*)(ch + 2));	//����;
	double planePositionY = *((double*)(ch + 10));	//γ��

	QByteArray imagearray;
	imagearray.append(array.mid(26));

	QPixmap pixmap;
	pixmap.loadFromData(imagearray, "jpg");

	QLabel *label = new QLabel("");
	label->setPixmap(pixmap);

	QStringList typelist = WeaponTypeList.split("|");
	QDateTime dt = QDateTime::currentDateTime();
	Mappoint *mpoint = new Mappoint;

	mpoint->x			= planePositionX;
	mpoint->y			= planePositionY;
	mpoint->messageId	= currentID;
	mpoint->rowid		= QString::number(m_row++);
	mpoint->planeTime	= dt.toString("hh:mm:ss yyyy-MM-dd");
	mpoint->picture		= label;

	if (objectId < typelist.count()) {
		mpoint->weaponname = typelist.at(objectId);
	}

	addFeatureToMap(mpoint);
}

//���ݻط�
void MainWindow::dataBack()               
{
	QString mPath = QCoreApplication::applicationDirPath();

	if (m_pDBBackDlg == nullptr)
	{
		m_pDBBackDlg = new dataBackDlg(this);		
	}
	m_pDBBackDlg->show();

	//QString sql = "select * from mavlink where(1 = 1) and (time >= '2025-05-22 10:08:46' and time <= '2025-05-27 10:08:46') order by Id";
	//m_pMavThread->m_sql = sql;
	//m_pMavThread->start();
}

//�������˻�������ؼ���
void MainWindow::insertToTable(tag_PlaneMessage *p)
{
	int c = 2;
	int row = ui->mPlaneWidget->rowCount();
	ui->mPlaneWidget->insertRow(row);

	ui->mPlaneWidget->setItem(row, 0, new NumericTableItem(p->ID)); // ID，数值排序
	ui->mPlaneWidget->item(row, 0)->setTextAlignment(Qt::AlignTop);

	//���ȣ�γ�ȣ���Ը߶ȣ����θ߶�
	QString s = QString("%1").arg(QString::number(p->planeX.toDouble(), 'f', 5));
	ui->mPlaneWidget->setItem(row, 1, new QTableWidgetItem(s));

	s = QString("%1").arg(QString::number(p->planeY.toDouble(), 'f', 5));
	ui->mPlaneWidget->setItem(row, c++, new QTableWidgetItem(s));

	s = QString("%1").arg(QString::number(p->xZ.toDouble(), 'f', 2));
	ui->mPlaneWidget->setItem(row, c++, new QTableWidgetItem(s));

	s = QString("%1").arg(QString::number(p->hZ.toDouble(), 'f', 2));
	ui->mPlaneWidget->setItem(row, c++, new QTableWidgetItem(s));

	s = QString("%1").arg(QString::number(p->Pitch.toDouble(), 'f', 2));
	ui->mPlaneWidget->setItem(row, c++, new QTableWidgetItem(s));

	s = QString("%1").arg(QString::number(p->Roll.toDouble(), 'f', 2));
	ui->mPlaneWidget->setItem(row, c++, new QTableWidgetItem(s));

	s = QString("%1").arg(QString::number(p->Yaw.toDouble(), 'f', 2));
	ui->mPlaneWidget->setItem(row, c++, new QTableWidgetItem(s));

	s = QString("%1").arg(QString::number(p->airSpeed.toDouble(), 'f', 2));
	ui->mPlaneWidget->setItem(row, c++, new QTableWidgetItem(s));

	s = QString("%1").arg(QString::number(p->groundSpd.toDouble(), 'f', 2));
	ui->mPlaneWidget->setItem(row, c++, new QTableWidgetItem(s));
	
	ui->mPlaneWidget->setItem(row, c++, new QTableWidgetItem(p->gpsStatus));

	s = QString("%1").arg(QString::number(p->value.toDouble(), 'f', 2));//��ѹ
	ui->mPlaneWidget->setItem(row, c++, new QTableWidgetItem(s));

	ui->mPlaneWidget->setItem(row, c++, new QTableWidgetItem(p->gpsNum));	//gps����
	ui->mPlaneWidget->setItem(row, c++, new QTableWidgetItem(p->taskType));	//������������
	ui->mPlaneWidget->setItem(row, c++, new QTableWidgetItem(p->gjMode));	//�켣ģʽ
	ui->mPlaneWidget->setItem(row, c++, new QTableWidgetItem(p->qkRunMode));//Ⱥ������ģʽ
	ui->mPlaneWidget->setItem(row, c++, new QTableWidgetItem(p->qkCmdMode));//Ⱥ��ָ��ģʽ
	ui->mPlaneWidget->setItem(row, c++, new QTableWidgetItem(p->fkRunMode));//�ɿ�����ģʽ
	ui->mPlaneWidget->setItem(row, c++, new QTableWidgetItem(p->jqtbTime));	//ͬ��ʱ��

	// 按无人机ID数值升序排列
	ui->mPlaneWidget->sortItems(0, Qt::AscendingOrder);
}

//ʵʱ�޸ı������˻�����
void MainWindow::ModifyTable(tag_PlaneMessage *p)
{
	int row = ui->mPlaneWidget->rowCount();
	
	for (int i = 0; i < row; i++)
	{
		if (p->ID == ui->mPlaneWidget->item(i, 0)->text())
		{
			//���ȣ�γ�ȣ���Ը߶ȣ����θ߶�
			QString s = QString("%1").arg(QString::number(p->planeX.toDouble(), 'f', 5));
			ui->mPlaneWidget->setItem(i, 1, new QTableWidgetItem(s));
			
			s = QString("%1").arg(QString::number(p->planeY.toDouble(), 'f', 5));
			ui->mPlaneWidget->setItem(i, 2, new QTableWidgetItem(s));
			
			s = QString("%1").arg(QString::number(p->xZ.toDouble(), 'f', 2));
			ui->mPlaneWidget->setItem(i, 3, new QTableWidgetItem(s));
			
			s = QString("%1").arg(QString::number(p->hZ.toDouble(), 'f', 2));
			ui->mPlaneWidget->setItem(i, 4, new QTableWidgetItem(s));			

			s = QString("%1").arg(QString::number(p->Pitch.toDouble(), 'f', 2));
			ui->mPlaneWidget->setItem(i, 5, new QTableWidgetItem(s));
			
			s = QString("%1").arg(QString::number(p->Roll.toDouble(), 'f', 2));
			ui->mPlaneWidget->setItem(i, 6, new QTableWidgetItem(s));
			
			s = QString("%1").arg(QString::number(p->Yaw.toDouble(), 'f', 2));
			ui->mPlaneWidget->setItem(i, 7, new QTableWidgetItem(s));			

			s = QString("%1").arg(QString::number(p->airSpeed.toDouble(), 'f', 2));
			ui->mPlaneWidget->setItem(i, 8, new QTableWidgetItem(s));

			s = QString("%1").arg(QString::number(p->groundSpd.toDouble(), 'f', 2));
			ui->mPlaneWidget->setItem(i, 9, new QTableWidgetItem(s));

			ui->mPlaneWidget->setItem(i, 10, new QTableWidgetItem(p->gpsStatus));

			s = QString("%1").arg(QString::number(p->value.toDouble(), 'f', 2));//��ѹ
			ui->mPlaneWidget->setItem(i, 11, new QTableWidgetItem(s));

			ui->mPlaneWidget->setItem(i, 12, new QTableWidgetItem(p->gpsNum));		//gps����
			ui->mPlaneWidget->setItem(i, 13, new QTableWidgetItem(p->taskType));	//������������
			ui->mPlaneWidget->setItem(i, 14, new QTableWidgetItem(p->gjMode));		//�켣ģʽ
			ui->mPlaneWidget->setItem(i, 15, new QTableWidgetItem(p->qkRunMode));	//
			ui->mPlaneWidget->setItem(i, 16, new QTableWidgetItem(p->qkCmdMode));	//
			ui->mPlaneWidget->setItem(i, 17, new QTableWidgetItem(p->fkRunMode));	//
			ui->mPlaneWidget->setItem(i, 18, new QTableWidgetItem(p->jqtbTime));	//
			break;
		}		
	}
}

void MainWindow::hideTable()    //������ʾʵʱ����
{
	bool b = ui->mPlaneWidget->isHidden();
	if (b)
		ui->mPlaneWidget->show();
	else
		ui->mPlaneWidget->hide();

	m_mapCanvas->refresh();
}

//ѡ�����˻���������ͼ��
void MainWindow::SelectTaskLayer()
{
	m_mapCanvas->setCurrentLayer(g_pAirTaskPolyLayer);
	g_pAirTaskPolyLayer->startEditing();
	m_mapCanvas->unsetMapTool(mToolPan);
}

#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QTextStream>

void MainWindow::show2Dor3Dwindow()  // 循环切换: QGIS(0) → Leaflet2D(1) → 3D(2)
{
	switchMapViewMode((m_mapViewMode + 1) % 3);
}

// 切换到指定地图显示模式: 0=QGIS 1=Leaflet 2=3D CesiumJS
void MainWindow::switchMapViewMode(int mode)
{
	m_mapViewMode = mode;
	switch (mode)
	{
	case 0: // QGIS 2D
		m_mapCanvas->show();
		if (m_p2DMapView) m_p2DMapView->hide();
		m_pWebEngineView->hide();
		// 切回 QGIS 时恢复 biaopai 显示
		for (biaopai *bp : m_planeIDvec) bp->setVisible(true);
		// 从 Leaflet 切回 QGIS 时，将 Leaflet 当前中心点同步回 QGIS 画布
		if (m_leafletReady && m_p2DMapView && m_mapCanvas)
		{
			m_p2DMapView->page()->runJavaScript(
				"JSON.stringify({lat:map.getCenter().lat,lng:map.getCenter().lng})",
				[this](const QVariant &v) {
					QString s = v.toString();
					auto getVal = [&](const QString &key) -> double {
						int i = s.indexOf('"' + key + '"');
						if (i < 0) return 0;
						i = s.indexOf(':', i);
						if (i < 0) return 0;
						int j = s.indexOf(',', i + 1);
						if (j < 0) j = s.indexOf('}', i + 1);
						return s.mid(i + 1, j - i - 1).trimmed().toDouble();
					};
					double lat = getVal("lat"), lng = getVal("lng");
					if (lat != 0 && lng != 0 && m_mapCanvas)
						m_mapCanvas->setCenter(QgsPointXY(lng, lat));
				});
		}
		break;
	case 1: // Leaflet 2D
		m_mapCanvas->hide();
		if (m_p2DMapView) m_p2DMapView->show();
		m_pWebEngineView->hide();
		// 将 QGIS 当前范围（extent）同步到 Leaflet，实现视角统一
		if (m_leafletReady && m_mapCanvas)
		{
			QgsRectangle ext = m_mapCanvas->extent();
			QString js = QString("fitBounds(%1,%2,%3,%4)")
				.arg(ext.yMinimum(), 0, 'f', 6)
				.arg(ext.xMinimum(), 0, 'f', 6)
				.arg(ext.yMaximum(), 0, 'f', 6)
				.arg(ext.xMaximum(), 0, 'f', 6);
			m_p2DMapView->page()->runJavaScript(js);
			syncLeafletAll();
		}
		break;
	case 2: // 3D Three.js
		m_mapCanvas->hide();
		if (m_p2DMapView) m_p2DMapView->hide();
		m_pWebEngineView->show();
		m_pWebEngineView->update();
		if (!m_3dInitDone)
		{
			// 首次切换时才加载HTML，确保 WebEngineView 已可见（WebGL canvas 尺寸正确）
			m_3dInitDone = true;
			init3DView();
		}
		else if (m_3dReady)
		{
			sync3DAll();
			// 飞行到当前 QGIS 视角中心，实现三图视角统一
			if (m_mapCanvas)
			{
				QgsRectangle ext = m_mapCanvas->extent();
				QgsPointXY c = ext.center();
				double span = qMax(ext.width(), ext.height()) / 2.0;
				QString js = QString("if(typeof flyToGeoPos==='function')flyToGeoPos(%1,%2,%3);")
					.arg(c.x(), 0, 'f', 6)
					.arg(c.y(), 0, 'f', 6)
					.arg(span, 0, 'f', 4);
				m_pWebEngineView->page()->runJavaScript(js);
			}
		}
		break;
	}
}

// 初始化Leaflet 2D地图：生成HTML到appDir/map2d.html 并加载
void MainWindow::initLeafletMap()
{
	if (!m_p2DMapView)
		return;

	QString appDir = QCoreApplication::applicationDirPath();
	QString htmlPath = appDir + "/map2d.html";

	QFile f(htmlPath);
	if (f.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&f);
		out.setCodec("UTF-8");
		out << R"HTML(<!DOCTYPE html>
<html><head><meta charset="utf-8">
<style>
* { margin:0; padding:0; }
html, body, #map { width:100%; height:100%; background:#1a2030; }
.plane-icon {}
.plane-lbl.leaflet-tooltip {
  background: rgba(140,35,0,0.93) !important;
  color: #ffe0c0 !important;
  padding: 2px 8px !important;
  border-radius: 3px !important;
  font: bold 13px Arial,sans-serif !important;
  border: 1px solid #ff8800 !important;
  box-shadow: 0 0 5px rgba(255,136,0,0.5) !important;
  white-space: nowrap !important;
}
.plane-lbl.leaflet-tooltip-bottom::before {
  border-bottom-color: rgba(140,35,0,0.93) !important;
}
</style>
<link rel="stylesheet" href="https://unpkg.com/leaflet@1.9.4/dist/leaflet.css"/>
<script src="https://unpkg.com/leaflet@1.9.4/dist/leaflet.js"></script>
</head><body><div id="map"></div>
<script>
var map = L.map('map', { zoomControl:true, attributionControl:false, preferCanvas:true });
L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', { maxZoom:20 }).addTo(map);
map.setView([34.0, 108.0], 10);

var planes = {};

// ── 图标路径 & 尺寸 ───────────────────────────────────────────────────────────
var _plSz = 80;
var _planePath = 'm516.749 400.364v-52.717l-199.765-120.063v-142.692c.034-34.958-51.215-34.958-51.274-.958v143.65l-196.91 120.064v52.717l198.827-65.43v106.646l-45.529 35.944v41.694l70.928-22.047 70.927 22.047v-41.694l-46.009-35.944v-106.646z';
function _makePlaneIcon(yaw) {
  var sz = _plSz, h = sz/2;
  var svg = '<svg xmlns="http://www.w3.org/2000/svg" width="'+sz+'" height="'+sz+'" viewBox="0 0 32 32">'
    + '<g class="pr" transform="rotate('+(yaw||0)+' 16 16) translate(-0.8,-0.3) scale(0.055)">'
    + '<path d="'+_planePath+'" fill="#ff3300" stroke="#ff9966" stroke-width="10" stroke-linejoin="round"/>'
    + '</g></svg>';
  return L.divIcon({ html:svg, className:'plane-icon', iconSize:[sz,sz], iconAnchor:[h,h] });
}

// ── RAF 插值：C++ 每 100ms 发目标点，JS 60fps 线性插值驱动 setLatLng ──────────
// 根本解决"图标跳跃卡顿"：IPC 延迟/定时器抖动不再影响视觉帧率
var _plState  = {};   // id → {pLat,pLng,nLat,nLng,t0,dt}  prev/next 插值端点
var _rafLive  = false;
function _rafTick() {
  var now = performance.now();
  var any = false;
  for (var id in _plState) {
    if (!planes[id]) continue;
    var s = _plState[id];
    var t = s.dt > 0 ? Math.min(1, (now - s.t0) / s.dt) : 1;
    planes[id].m.setLatLng([s.pLat + (s.nLat - s.pLat)*t,
                             s.pLng + (s.nLng - s.pLng)*t]);
    any = true;
  }
  if (any) requestAnimationFrame(_rafTick);
  else _rafLive = false;
}
function _rafKick() {
  if (!_rafLive) { _rafLive = true; requestAnimationFrame(_rafTick); }
}

// ── 航迹 ─────────────────────────────────────────────────────────────────────
var _plTrails = {};
var _maxPlTrail = 800;

// ── 主入口：C++ 每 100ms 调用一次 ─────────────────────────────────────────────
function updatePlane(id, lat, lng, label, yaw, alt) {
  lat = +lat; lng = +lng;
  var now = performance.now();
  if (!planes[id]) {
    // 首次：建 marker + 永久 tooltip
    var m = L.marker([lat,lng], { icon:_makePlaneIcon(yaw) }).addTo(map);
    m.bindTooltip(
      '<span class="pt-id">'+label+'</span><br>'
      +'<span class="pt-alt" style="font-size:11px;opacity:0.9;">\u9ad8\u5ea6:'+Math.round(alt||0)+'m</span>',
      { permanent:true, direction:'bottom', className:'plane-lbl', offset:[0,4] }
    );
    _plTrails[id] = L.polyline([], {color:'#ff6600',weight:2.5,opacity:0.7,dashArray:'6,4'}).addTo(map);
    planes[id] = { m:m, cnt:0 };
    _plState[id]  = { pLat:lat, pLng:lng, nLat:lat, nLng:lng, t0:now, dt:100 };
  } else {
    // 后续：计算当前插值位置作为新起点，更新目标点，启动/延续 RAF
    var s = _plState[id];
    var tp = s.dt > 0 ? Math.min(1, (now - s.t0) / s.dt) : 1;
    var cLat = s.pLat + (s.nLat - s.pLat)*tp;
    var cLng = s.pLng + (s.nLng - s.pLng)*tp;
    _plState[id] = { pLat:cLat, pLng:cLng, nLat:lat, nLng:lng,
                     t0:now, dt: Math.max(50, now - s.t0) };

    // 朝向：直接修改 SVG DOM（不重建 icon）
    var el = planes[id].m.getElement();
    if (el) {
      var g = el.querySelector('.pr');
      if (g) g.setAttribute('transform','rotate('+(yaw||0)+' 16 16) translate(-0.8,-0.3) scale(0.055)');
    }
    // 高度：仅更新 tooltip 内文本节点
    var tip = planes[id].m.getTooltip();
    if (tip) {
      var te = tip.getElement();
      if (te) { var ae = te.querySelector('.pt-alt'); if (ae) ae.textContent = '\u9ad8\u5ea6:'+Math.round(alt||0)+'m'; }
    }
  }
  // 启动 60fps 插值循环
  _rafKick();
  // 航迹：追加 GPS 真实点（O(1)），超限时截断
  if (_plTrails[id]) {
    _plTrails[id].addLatLng([lat,lng]);
    planes[id].cnt = (planes[id].cnt||0) + 1;
    if (planes[id].cnt > _maxPlTrail) {
      var pts = _plTrails[id].getLatLngs();
      _plTrails[id].setLatLngs(pts.slice(pts.length - _maxPlTrail/2));
      planes[id].cnt = _maxPlTrail/2;
    }
  }
}

function setPlaneDetected(id) {
  if (_plTrails[id]) _plTrails[id].setStyle({color:'#ff0000',weight:3,dashArray:null,opacity:0.9});
}
function setPlaneNormal(id) {
  if (_plTrails[id]) _plTrails[id].setStyle({color:'#ff6600',weight:2.5,dashArray:'6,4',opacity:0.7});
}

function setTileUrl(url) {
  map.eachLayer(function(l){ if (l instanceof L.TileLayer) map.removeLayer(l); });
  L.tileLayer(url, { maxZoom:20 }).addTo(map);
}

function setCenter(lat, lng, zoom) {
  map.setView([+lat, +lng], zoom !== undefined ? +zoom : map.getZoom());
}

function fitBounds(s, w, n, e) {
  map.fitBounds([[+s,+w],[+n,+e]]);
}

// Type-based colors for Leaflet radar icons
var _lColors=[
  {stroke:'#1a6aff',arc1:'#00ccff',arc2:'#1a6aff',bg:'#001a55',badge:'#00aaff',text:'#7edfff',lbg:'rgba(0,26,85,0.88)',ltc:'#7edfff'},
  {stroke:'#00cc55',arc1:'#00ff88',arc2:'#00cc55',bg:'#003322',badge:'#00ff88',text:'#88ffcc',lbg:'rgba(0,51,34,0.88)',ltc:'#88ffcc'},
  {stroke:'#ff8800',arc1:'#ffcc00',arc2:'#ff8800',bg:'#330f00',badge:'#ffaa00',text:'#ffcc88',lbg:'rgba(51,15,0,0.88)',ltc:'#ffcc88'},
  {stroke:'#cc0000',arc1:'#ff4444',arc2:'#cc0000',bg:'#330000',badge:'#ff4444',text:'#ffaaaa',lbg:'rgba(51,0,0,0.88)',ltc:'#ffaaaa'},
  {stroke:'#9900cc',arc1:'#cc44ff',arc2:'#9900cc',bg:'#220033',badge:'#cc44ff',text:'#ee99ff',lbg:'rgba(34,0,51,0.88)',ltc:'#ee99ff'}
];
var _lTypeNames=['\u5149\u5b66','\u96f7\u8fbe','\u9065\u6d4b','\u5fae\u6ce2','\u7535\u6218'];
// Radar marker SVG: antenna mast with signal arcs, type-based colors, ID badge
function _makeRadarSvg(id, type) {
  var c = _lColors[(type||0)%_lColors.length];
  return '<svg xmlns="http://www.w3.org/2000/svg" width="36" height="36" viewBox="0 0 36 36">'
    + '<line x1="18" y1="32" x2="18" y2="17" stroke="'+c.stroke+'" stroke-width="2" stroke-linecap="round"/>'
    + '<line x1="10" y1="32" x2="26" y2="32" stroke="'+c.stroke+'" stroke-width="2.5" stroke-linecap="round"/>'
    + '<path d="M11,17 Q18,11 25,17" fill="none" stroke="'+c.arc1+'" stroke-width="2.2" stroke-linecap="round"/>'
    + '<path d="M8,14 Q18,5 28,14" fill="none" stroke="'+c.arc2+'" stroke-width="1.4" stroke-dasharray="2,2" opacity="0.85"/>'
    + '<path d="M5,11 Q18,-1 31,11" fill="none" stroke="'+c.arc2+'" stroke-width="1.0" stroke-dasharray="2,2" opacity="0.5"/>'
    + '<circle cx="18" cy="17" r="5.5" fill="'+c.bg+'" stroke="'+c.badge+'" stroke-width="1.2"/>'
    + '<text x="18" y="20" font-size="6.5" text-anchor="middle" fill="'+c.text+'" font-weight="bold">'+id+'</text>'
    + '</svg>';
}

// Radar markers with coverage fan
var _radars = {};
function syncRadars(arr) {
  for (var k in _radars) {
    if (_radars[k].m) map.removeLayer(_radars[k].m);
    if (_radars[k].f) map.removeLayer(_radars[k].f);
  }
  _radars = {};
  if (!arr || !arr.length) return;
  arr.forEach(function(r) {
    var ti = (r.type||0) % _lColors.length;
    var c = _lColors[ti];
    var tn = _lTypeNames[ti];
    // Icon + label combined in one divIcon: label is absolutely positioned below the SVG
    var combinedHtml =
      '<div style="position:relative;display:inline-block;text-align:center;">'
      + _makeRadarSvg(r.id, r.type)
      + '<div style="position:absolute;left:50%;transform:translateX(-50%);top:38px;'
      + 'background:'+c.lbg+';color:'+c.ltc+';font:bold 14px Arial,sans-serif;'
      + 'line-height:1;padding:4px 10px;border-radius:4px;'
      + 'border:2px solid '+c.badge+';white-space:nowrap;'
      + 'box-shadow:0 0 7px '+c.badge+',0 2px 4px rgba(0,0,0,0.8);'
      + 'pointer-events:none;">'
      + tn+'#'+r.id+'</div></div>';
    var mk = L.marker([r.lat, r.lon], {
      icon: L.divIcon({ html:combinedHtml, className:'', iconSize:null, iconAnchor:[18,32] })
    }).addTo(map);
    var fan = null;
    if (r.range > 0 && r.hBeam > 0) {
      var az = r.azimuth, hb = r.hBeam, rng = Math.min(r.range, 80000);
      var elev = r.elevation || 0, vb = r.vBeam || 0;
      var cosLat = Math.cos(r.lat * Math.PI / 180);
      var crossZenith = (vb > 0) && (elev + vb / 2 > 90);
      var devH = r.alt || 0;
      var pts;
      if (elev < 0 && devH > 0 && vb > 0) {
        var DR = Math.PI / 180;
        var vbh2 = vb / 2, elevR = elev * DR;
        var azC = (az + hb / 2) * DR;
        var kx = -Math.cos(azC), kz = Math.sin(azC);
        var cT = Math.cos(elevR), sT = Math.sin(elevR);
        var hM = devH;
        var proj = function(aDeg, eLD) {
          var a = aDeg * DR, el = eLD * DR;
          var ce = Math.cos(el), se = Math.sin(el);
          var px = ce*Math.sin(a), py = se, pz = ce*Math.cos(a);
          var kdp = kx*px + kz*pz;
          var rx = px*cT + (-kz*py)*sT + kx*kdp*(1-cT);
          var ry = py*cT + (kz*px - kx*pz)*sT;
          var rz = pz*cT + (kx*py)*sT + kz*kdp*(1-cT);
          var t = (ry < -0.001) ? Math.min(hM/Math.abs(ry), rng) : rng;
          return [r.lat + t*rz/111320, r.lon + t*rx/111320/cosLat];
        };
        pts = [];
        var N = 36, M = Math.max(Math.round(vb/3), 2);
        for (var i = 0; i <= N; i++) pts.push(proj(az + hb*i/N, -vbh2));
        for (var j = 1; j < M; j++) pts.push(proj(az + hb, -vbh2 + vb*j/M));
        for (var i = N; i >= 0; i--) pts.push(proj(az + hb*i/N, vbh2));
        for (var j = M-1; j > 0; j--) pts.push(proj(az, -vbh2 + vb*j/M));
      } else if (!crossZenith) {
        var effPitch = (vb > 0) ? Math.max(0, elev - vb / 2) : elev;
        var d = rng * Math.cos(effPitch * Math.PI / 180) / 111320;
        pts = [[r.lat, r.lon]];
        for (var i = 0; i <= 36; i++) {
          var ag = (az + hb * i / 36) * Math.PI / 180;
          pts.push([r.lat + d * Math.cos(ag), r.lon + d * Math.sin(ag) / cosLat]);
        }
      } else {
        var DR = Math.PI / 180;
        var vbh2 = vb / 2 * DR, elevR = elev * DR;
        var azC = (az + hb / 2) * DR;
        var kx = -Math.cos(azC), kz = Math.sin(azC);
        var cT = Math.cos(elevR), sT = Math.sin(elevR);
        var proj = function(aDeg, eL) {
          var a = aDeg * DR, ce = Math.cos(eL), se = Math.sin(eL);
          var px = rng*ce*Math.sin(a), py = rng*se, pz = rng*ce*Math.cos(a);
          var kcx = -kz*py, kcz = kx*py, kdp = kx*px + kz*pz;
          var rx = px*cT + kcx*sT + kx*kdp*(1-cT);
          var rz = pz*cT + kcz*sT + kz*kdp*(1-cT);
          return [r.lat + rz/111320, r.lon + rx/111320/cosLat];
        };
        pts = [];
        var N = 36, M = Math.max(Math.round(vb/3), 2);
        for (var i = 0; i <= N; i++) pts.push(proj(az + hb*i/N, -vbh2));
        for (var j = 1; j < M; j++) pts.push(proj(az + hb, -vbh2 + vb*DR*j/M));
        for (var i = N; i >= 0; i--) pts.push(proj(az + hb*i/N, vbh2));
        for (var j = M-1; j > 0; j--) pts.push(proj(az, -vbh2 + vb*DR*j/M));
      }
      fan = L.polygon(pts, {
        color:c.arc1, weight:1.5, dashArray:'6,4',
        fillColor:c.arc1, fillOpacity:0.10
      }).addTo(map);
    }
    _radars[r.id] = {m: mk, f: fan};
  });
}
// Move a single radar marker to new position (for mounted equipment following UAV)
function moveRadar(rid, lat, lng){
  if(!_radars[rid]) return;
  var ll=L.latLng(lat, lng);
  _radars[rid].m.setLatLng(ll);
  // Detection fan also needs to move — shift all vertices by delta
  if(_radars[rid].f){
    var old=_radars[rid].f.getLatLngs()[0];
    if(old && old.length>0){
      var oCenter=_radars[rid].f.getBounds().getCenter();
      var dlat=lat-oCenter.lat, dlng=lng-oCenter.lng;
      var nll=old.map(function(p){return L.latLng(p.lat+dlat,p.lng+dlng);});
      _radars[rid].f.setLatLngs(nll);
    }
  }
}

// Task area polygons with centroid label
var _tasks = {};
function syncTaskAreas(arr) {
  for (var k in _tasks) {
    if (_tasks[k].poly) map.removeLayer(_tasks[k].poly);
    if (_tasks[k].lbl)  map.removeLayer(_tasks[k].lbl);
  }
  _tasks = {};
  if (!arr || !arr.length) return;
  arr.forEach(function(a) {
    if (!a.coords || a.coords.length < 3) return;
    var pts = a.coords.map(function(c) { return [c[1], c[0]]; });
    var poly = L.polygon(pts, {
      color:'#00ff88', weight:2, dashArray:'7,5',
      fillColor:'#00cc55', fillOpacity:0.20
    }).addTo(map);
    var sumLat=0, sumLon=0;
    pts.forEach(function(p){sumLat+=p[0]; sumLon+=p[1];});
    var cLat=sumLat/pts.length, cLon=sumLon/pts.length;
    var lblHtml='<div style="background:rgba(0,60,25,0.92);color:#88ffbb;font:bold 15px Arial,sans-serif;line-height:1.4;'
      +'padding:4px 10px;border-radius:4px;border:2px dashed #00ff88;white-space:nowrap;'
      +'box-shadow:0 0 8px #00ff88,0 2px 4px rgba(0,0,0,0.8);">\u4efb\u52a1\u533a#'+a.id+'</div>';
    var lbl=L.marker([cLat,cLon],{
      icon:L.divIcon({html:lblHtml,className:'',iconSize:null,iconAnchor:[50,14]})
    }).addTo(map);
    _tasks[a.id]={poly:poly, lbl:lbl};
  });
}
// Status bar: report mouse position + zoom via document.title
var _sbZoom = map.getZoom();
map.on('mousemove', function(e) {
  document.title = 'SB:' + e.latlng.lng.toFixed(6) + ',' + e.latlng.lat.toFixed(6) + '|z=' + _sbZoom;
});
map.on('zoomend', function() {
  _sbZoom = map.getZoom();
  var c = map.getCenter();
  document.title = 'SB:' + c.lng.toFixed(6) + ',' + c.lat.toFixed(6) + '|z=' + _sbZoom;
});
</script>
</body></html>
)HTML";
		f.close();
	}

	connect(m_p2DMapView, &QWebEngineView::loadFinished, this, [this](bool ok) {
		m_leafletReady = ok;
		// Leaflet 激活时隐藏 QGIS biaopai（避免 QGIS/Leaflet 两套系统渲染速度不同导致图标与标签错位）
		for (biaopai *bp : m_planeIDvec) bp->setVisible(!ok);
		// 页面加载完成后，若当前正处于 Leaflet 模式，立即同步视角 + 装备/任务区域图标
		// （类似 3D 的 loadFinished → sync3DAll()，避免用户在页面加载完成前切到 Leaflet 时图标丢失）
		if (ok && m_mapViewMode == 1 && m_mapCanvas) {
			QgsRectangle ext = m_mapCanvas->extent();
			if (!ext.isEmpty()) {
				QString js = QString("fitBounds(%1,%2,%3,%4)")
					.arg(ext.yMinimum(), 0, 'f', 6)
					.arg(ext.xMinimum(), 0, 'f', 6)
					.arg(ext.yMaximum(), 0, 'f', 6)
					.arg(ext.xMaximum(), 0, 'f', 6);
				m_p2DMapView->page()->runJavaScript(js);
			}
			syncLeafletAll();
		}
	});
	// Leaflet status bar: parse title changes like "SB:lng,lat|z=13"
	connect(m_p2DMapView, &QWebEngineView::titleChanged, this, [this](const QString &title) {
		if (m_mapViewMode != 1 || !title.startsWith("SB:")) return;
		// "SB:lng,lat|z=13"
		int bar = title.indexOf('|');
		if (bar < 0) return;
		QString coord = title.mid(3, bar - 3);  // "lng,lat"
		QString zPart = title.mid(bar + 3);      // "13"
		m_lblCoordinate->setText(coord);
		char buf[128];
		sprintf(buf, "\xe7\xbc\xa9\xe6\x94\xbe\xe7\xba\xa7\xe5\x88\xab=%s", zPart.toUtf8().constData());
		m_lblSelectTargetCount->setText(QString::fromUtf8(buf));
	});
	m_p2DMapView->load(QUrl::fromLocalFile(htmlPath));
}

// 初始化三维地图：Three.js 本地渲染 + ArcGIS 区域卫星影像
void MainWindow::init3DView()
{
	if (!m_pWebEngineView) return;

	QString appDir = QCoreApplication::applicationDirPath();

	double cx = 116.28965, cy = 38.15358; // 泊头（河北）默认中心
	double span = 2.0; // fallback
	if (m_mapCanvas)
	{
		QgsPointXY c = m_mapCanvas->center();
		if (c.x() > 70 && c.x() < 140 && c.y() > 10 && c.y() < 55)
		{ cx = c.x(); cy = c.y(); }
		// Derive span from QGIS canvas extent so 3D matches local 2D map
		QgsRectangle ext = m_mapCanvas->extent();
		double extSpan = qMax(ext.width(), ext.height()) / 2.0;
		if (extSpan > 0.05 && extSpan < 5.0)
			span = qMin(extSpan * 1.3, 3.0); // 30% margin, cap at 3deg
	}

	// ===== URL 构建（避免 %n 占位符冲突，全部在 C++ 侧构建）=====
	QString bboxStr = QString("%1,%2,%3,%4")
		.arg(cx - span, 0, 'f', 4).arg(cy - span, 0, 'f', 4)
		.arg(cx + span, 0, 'f', 4).arg(cy + span, 0, 'f', 4);
	// 卫星影像贴图（2048x2048，覆盖中心 ±span°，比15°分辨率高7.5x+）
	QString regionUrl = "https://services.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/export"
		"?bbox=" + bboxStr + "&bboxSR=4326&size=2048,2048&imageSR=4326&format=jpg&f=image";
	// 地形高程位移贴图（1024x1024 hillshade，亮处=高处）
	QString elevUrl = "https://services.arcgisonline.com/ArcGIS/rest/services/World_Shaded_Relief/MapServer/export"
		"?bbox=" + bboxStr + "&bboxSR=4326&size=1024,1024&imageSR=4326&format=png&f=image";

	QString cxStr   = QString::number(cx,   'f', 6);
	QString cyStr   = QString::number(cy,   'f', 6);
	QString spanStr = QString::number(span, 'f', 1);
	QString elevUrlQ = elevUrl;

	QString html =
		QString("<!DOCTYPE html>\n<html><head><meta charset=\"UTF-8\">\n")
		+ R"RAW(<style>
*{margin:0;padding:0}
html,body{width:100%;height:100%;overflow:hidden;background:linear-gradient(180deg,#5ba8d4 0%,#a8d0e8 55%,#c8e4f0 100%);font-family:Arial,sans-serif}
#hud{position:absolute;top:8px;left:50%;transform:translateX(-50%);
  color:#7ec8ff;font-size:12px;background:rgba(0,15,40,.85);
  padding:4px 16px;border-radius:4px;pointer-events:none;z-index:10}
#compass{position:absolute;top:50px;right:16px;width:44px;height:44px;border-radius:50%;
  background:rgba(0,15,40,.88);border:1px solid #224466;z-index:10;pointer-events:none;
  display:flex;flex-direction:column;align-items:center;justify-content:center;
  color:#7ec8ff;font-size:11px;font-weight:bold;line-height:1.1}
#compass .na{color:#ff4444;font-size:15px;line-height:1}
#legend{position:absolute;bottom:12px;right:12px;color:#9ad4ff;font-size:11px;
  background:rgba(0,15,40,.88);padding:8px 12px;border-radius:5px;line-height:1.85;z-index:10}
.li{display:flex;align-items:center;gap:6px}
.dot{width:10px;height:10px;border-radius:50%;flex-shrink:0}
.sq{width:10px;height:10px;border-radius:2px;flex-shrink:0}
#status{position:absolute;top:50%;left:50%;transform:translate(-50%,-50%);
  color:#7ec8ff;font-size:13px;z-index:20;pointer-events:none;text-align:center;
  background:rgba(0,15,40,.92);padding:18px 32px;border-radius:8px;white-space:pre-line;
  min-width:260px;box-shadow:0 4px 24px rgba(0,0,0,.5)}
#status .title{font-size:15px;font-weight:bold;margin-bottom:10px}
#status .item{display:flex;align-items:center;gap:8px;margin:6px 0;font-size:11px}
#status .item-name{width:72px;text-align:right;flex-shrink:0;opacity:.85}
#status .bar-bg{flex:1;height:8px;background:rgba(255,255,255,.1);border-radius:4px;overflow:hidden;min-width:120px}
#status .bar-fill{height:100%;border-radius:4px;transition:width .15s;width:0}
#status .item-pct{width:36px;text-align:right;font-size:10px;opacity:.7;flex-shrink:0}
#status .done{color:#44dd88}
#status .fail{color:#ff6644}
</style>
</head><body>
<div id="hud">&#x4e09;&#x7ef4;&#x5730;&#x56fe; | &#x5de6;&#x952e;&#x65cb;&#x8f6c;  &#x53f3;&#x952e;&#x5e73;&#x79fb;  &#x6eda;&#x8f6e;&#x7f29;&#x653e;  &#x53cc;&#x51fb;&#x91cd;&#x7f6e;</div>
<div id="compass"><span class="na">&#x25b2;</span>&#x5317;</div>
<div id="legend">
 <div class="li"><div class="dot" style="background:#ff5500"></div>&#x65e0;&#x4eba;&#x673a;</div>
 <div class="li"><div class="dot" style="background:#00aaff"></div>&#x88c5;&#x5907;</div>
 <div class="li"><div class="sq" style="background:#00cc66;opacity:.7"></div>&#x4efb;&#x52a1;&#x533a;</div>
</div>
<div id="status">
 <div class="title">&#x6b63;&#x5728;&#x52a0;&#x8f7d;&#x4e09;&#x7ef4;&#x5730;&#x7403;</div>
 <div class="item" id="pi-webgl"><span class="item-name">WebGL</span><div class="bar-bg"><div class="bar-fill" style="background:#7ec8ff"></div></div><span class="item-pct"></span></div>
 <div class="item" id="pi-region"><span class="item-name">&#x536b;&#x661f;&#x5f71;&#x50cf;</span><div class="bar-bg"><div class="bar-fill" style="background:#44aaff"></div></div><span class="item-pct"></span></div>
 <div class="item" id="pi-elev"><span class="item-name">&#x5730;&#x5f62;&#x9ad8;&#x7a0b;</span><div class="bar-bg"><div class="bar-fill" style="background:#44ddaa"></div></div><span class="item-pct"></span></div>
</div>
<div id="dbg" style="position:fixed;bottom:0;left:0;right:0;max-height:220px;overflow-y:auto;background:rgba(0,0,0,.88);color:#0f0;font-size:11px;font-family:monospace;padding:4px 8px;z-index:9999;pointer-events:none;"></div>
<script>
var _dbgEl=document.getElementById('dbg');
function dbg(m){if(_dbgEl){var d=document.createElement('div');d.textContent=m;_dbgEl.appendChild(d);_dbgEl.scrollTop=9999;}}
window.onerror=function(m,s,l){dbg('[ERR]'+m+' L:'+l);return false;};
dbg('Step1: early script OK, div found='+(!!_dbgEl));
</script>
<script src="3D/threejs/three.min.js"
  onload="dbg('Step2: three.min.js loaded, THREE='+(typeof THREE))"
  onerror="dbg('Step2: three.min.js FAILED')"></script>
<script src="3D/threejs/OrbitControls.js"
  onload="dbg('Step3: OrbitControls loaded')"
  onerror="dbg('Step3: OrbitControls FAILED')"></script>
<script>
)RAW"
		+ "var _CX=" + cxStr + ", _CY=" + cyStr + ", _SPAN=" + spanStr + ";\n"
		+ "var _REGION_URL='" + regionUrl + "';\n"
		+ "var _ELEV_URL='" + elevUrlQ + "';\n"
		+ "var _IMG_BASE='https://services.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/export';\n"
		+ "var _ELV_BASE='https://services.arcgisonline.com/ArcGIS/rest/services/World_Shaded_Relief/MapServer/export';\n"
		+ R"RAW(
dbg('Step4: main script block, THREE='+(typeof THREE));
window.addEventListener('load', function() {
  dbg('Step5: load event, THREE='+(typeof THREE));
  var st = document.getElementById('status');
  // 进度条更新工具
  function _pi(id, pct, txt) {
    var el = document.getElementById(id); if(!el) return;
    var fill = el.querySelector('.bar-fill'), lbl = el.querySelector('.item-pct');
    if(fill) fill.style.width = Math.min(pct,100)+'%';
    if(lbl) lbl.textContent = txt || (Math.round(pct)+'%');
  }
  function _piDone(id, ok) {
    var el = document.getElementById(id); if(!el) return;
    var fill = el.querySelector('.bar-fill'), lbl = el.querySelector('.item-pct');
    if(ok) { if(fill) fill.style.width='100%'; if(lbl){lbl.textContent='\u2714';lbl.className='item-pct done';} }
    else   { if(fill){fill.style.width='100%';fill.style.background='#ff6644';} if(lbl){lbl.textContent='\u2718';lbl.className='item-pct fail';} }
  }
  if (typeof THREE === 'undefined') {
    _piDone('pi-webgl',false); st.querySelector('.title').textContent='Three.js \u52a0\u8f7d\u5931\u8d25'; return;
  }
  _pi('pi-webgl', 50, '\u521d\u59cb\u5316...');

  var CX=_CX, CY=_CY, SPAN=_SPAN;
  var M_LON = 111320 * Math.cos(CY * Math.PI/180);
  var M_LAT = 110540;
  var GW = SPAN*2*M_LON, GH = SPAN*2*M_LAT;
  var R = 6371000;

  function geo2s(lon, lat, alt) {
    return new THREE.Vector3((lon-CX)*M_LON, alt||0, -(lat-CY)*M_LAT);
  }

  // Fly camera to geographic position matching QGIS canvas extent
  // spanDeg = half-width of extent in degrees; camera height proportional to span
  window.flyToGeoPos = function flyToGeoPos(lon, lat, spanDeg) {
    var pt = geo2s(lon, lat, 0);
    var h = Math.max(spanDeg * 22000, 15000); // ~22km per degree, min 15km
    oc.target.set(pt.x, 0, pt.z);
    camera.position.set(pt.x, h, pt.z + h * 0.9);
    oc.update();
  }

  var W = window.innerWidth||1280, H = window.innerHeight||800;

  var testCanvas = document.createElement('canvas');
  var gl = testCanvas.getContext('webgl') || testCanvas.getContext('experimental-webgl');
  if (!gl) { _piDone('pi-webgl',false); st.querySelector('.title').textContent='WebGL \u4e0d\u53ef\u7528'; return; }

  var renderer;
  try { renderer = new THREE.WebGLRenderer({antialias:true}); }
  catch(e) { _piDone('pi-webgl',false); st.querySelector('.title').textContent='Renderer \u5931\u8d25'; return; }
  renderer.setSize(W, H);
  renderer.setPixelRatio(window.devicePixelRatio||1);
  dbg('renderer OK size='+W+'x'+H);
  _piDone('pi-webgl', true);
  renderer.domElement.style.cssText = 'position:fixed;top:0;left:0;z-index:0;';
  document.body.appendChild(renderer.domElement);

  var scene = new THREE.Scene();
  scene.background = new THREE.Color(0x5ba8d4); // sky blue
  // Atmospheric haze: blends distant terrain into sky
  var _GD = Math.max(GW, GH);
  scene.fog = new THREE.Fog(0xa8d0e8, _GD * 1.5, _GD * 5.0);

  var _camH = GH * 0.30, _camZ = GH * 0.25;
  var camera = new THREE.PerspectiveCamera(45, W/H, 50, _GD * 10);
  camera.position.set(0, _camH, _camZ);
  camera.lookAt(0, 0, 0);

  // Hemisphere sky/ground light for natural outdoor ambient
  scene.add(new THREE.HemisphereLight(0x87ceeb, 0x4a7a3a, 0.7));
  // Main sun from south-southeast (natural for Northern Hemisphere daytime)
  var sun = new THREE.DirectionalLight(0xfff5d0, 1.4);
  sun.position.set(-GW * 0.3, GH * 0.5, -GH * 0.6); scene.add(sun);
  // Soft fill light from opposite side
  var fill = new THREE.DirectionalLight(0xd0e8ff, 0.3);
  fill.position.set(GW * 0.3, GH * 0.2, GH * 0.4); scene.add(fill);

  var oc = new THREE.OrbitControls(camera, renderer.domElement);
  oc.enableDamping = true; oc.dampingFactor = 0.10;
  oc.rotateSpeed = 0.35;
  oc.panSpeed = 0.4;
  oc.zoomSpeed = 0.6;
  oc.screenSpacePanning = false;
  oc.minDistance = 500;
  oc.maxDistance = _GD * 4;
  oc.target.set(0, 0, 0);
  oc.update();

  renderer.domElement.addEventListener('dblclick', function() {
    camera.position.set(0, _camH, _camZ); camera.lookAt(0,0,0);
    oc.target.set(0,0,0); oc.update();
  });

  var loader = new THREE.TextureLoader();
  loader.crossOrigin = 'anonymous';

  // Sea-level base plane (covers area beyond terrain to hide hard edges)
  var seaMesh = new THREE.Mesh(
    new THREE.PlaneGeometry(GW * 5, GH * 5),
    new THREE.MeshStandardMaterial({color:0x1a5c8a, roughness:0.3, metalness:0.1})
  );
  seaMesh.rotation.x = -Math.PI / 2; seaMesh.position.y = -200;
  scene.add(seaMesh);

  // Geo coordinate reference grid (lon/lat lines at scene y=0)
  (function(){
    var gm=new THREE.LineBasicMaterial({color:0x2266aa,transparent:true,opacity:0.22});
    var step=SPAN>1?0.5:0.2;
    var x0=-SPAN*M_LON,x1=SPAN*M_LON,z0=-SPAN*M_LAT,z1=SPAN*M_LAT;
    for(var lo=Math.ceil((CX-SPAN)/step)*step;lo<=CX+SPAN+0.001;lo+=step){
      var x=(lo-CX)*M_LON;
      scene.add(new THREE.Line(new THREE.BufferGeometry().setFromPoints([new THREE.Vector3(x,0,z0),new THREE.Vector3(x,0,z1)]),gm));
    }
    for(var la=Math.ceil((CY-SPAN)/step)*step;la<=CY+SPAN+0.001;la+=step){
      var z=-(la-CY)*M_LAT;
      scene.add(new THREE.Line(new THREE.BufferGeometry().setFromPoints([new THREE.Vector3(x0,0,z),new THREE.Vector3(x1,0,z)]),gm));
    }
    // North direction indicator line (red, points +Z negative = North)
    var nm=new THREE.LineBasicMaterial({color:0xff3333,transparent:true,opacity:0.7});
    var nl=GH*0.08;
    scene.add(new THREE.Line(new THREE.BufferGeometry().setFromPoints([new THREE.Vector3(0,100,0),new THREE.Vector3(0,100,-nl)]),nm));
  })();

  // Terrain: real DEM elevation via Terrarium tiles (AWS S3, free)
  // Terrarium encodes elevation in RGB: height = R*256 + G + B/256 - 32768
  var _elevRange = 1500; // default, updated after DEM decode
  // Stored DEM data for elevation queries
  var _demData = null; // {heights:Float32Array, w:int, h:int, bboxW,bboxS,bboxE,bboxN, minH}
  // Query absolute elevation at geographic coordinates (sea-level meters, for display)
  window.getElevation = function getElevation(lon, lat) {
    if (!_demData) return 0;
    var d = _demData;
    if (lon < d.bboxW || lon > d.bboxE || lat < d.bboxS || lat > d.bboxN) return 0;
    var px = Math.round((lon - d.bboxW) / (d.bboxE - d.bboxW) * (d.w - 1));
    var py = Math.round((d.bboxN - lat) / (d.bboxN - d.bboxS) * (d.h - 1));
    px = Math.max(0, Math.min(d.w - 1, px));
    py = Math.max(0, Math.min(d.h - 1, py));
    return d.heights[py * d.w + px];
  };
  // Three.js world Y for terrain surface at (lon,lat): relative height (0..range), matches displacementBias=0
  // Use this for ALL 3D object placement; use getElevation() only for display labels
  function _twY(lon, lat) {
    if (!_demData) return 0;
    var elev = getElevation(lon, lat);
    // 若在DEM范围外 getElevation返回0，避免出现负值（设备陷入地下），回退到0
    return elev > 0 ? (elev - _demData.minH) : 0;
  }
  var terrainMat = new THREE.MeshStandardMaterial({
    color: 0x4a7a3a, roughness: 0.82, metalness: 0.0,
    displacementScale: _elevRange, displacementBias: 0
  });
  var terrainGeo = new THREE.PlaneGeometry(GW, GH, 256, 256);
  var terrainMesh = new THREE.Mesh(terrainGeo, terrainMat);
  terrainMesh.rotation.x = -Math.PI / 2;
  scene.add(terrainMesh);

  // ===== IndexedDB 纹理缓存（断网时使用上次下载的卫星/高程图） =====
  var _cacheDB = null;
  function _openCacheDB(cb) {
    if (_cacheDB) { cb(_cacheDB); return; }
    var req = indexedDB.open('tile_cache', 1);
    req.onupgradeneeded = function(e) { e.target.result.createObjectStore('tiles'); };
    req.onsuccess = function(e) { _cacheDB = e.target.result; cb(_cacheDB); };
    req.onerror = function() { cb(null); };
  }
  function _cacheGet(key, cb) {
    _openCacheDB(function(db) {
      if (!db) { cb(null); return; }
      var tx = db.transaction('tiles','readonly');
      var r = tx.objectStore('tiles').get(key);
      r.onsuccess = function() { cb(r.result || null); };
      r.onerror = function() { cb(null); };
    });
  }
  function _cachePut(key, blob) {
    _openCacheDB(function(db) {
      if (!db) return;
      var tx = db.transaction('tiles','readwrite');
      tx.objectStore('tiles').put(blob, key);
    });
  }
  // 带缓存的纹理加载：先尝试网络下载（成功则缓存），失败则读缓存
  // piId: 进度条元素id，用于显示下载进度
  function _cachedLoadTex(url, cacheKey, onTex, onFail, piId) {
    _pi(piId, 0, '\u8fde\u63a5\u4e2d...');
    var xhr = new XMLHttpRequest();
    xhr.open('GET', url, true);
    xhr.responseType = 'blob';
    xhr.timeout = 12000;
    xhr.onprogress = function(e) {
      if (e.lengthComputable) {
        var pct = Math.round(e.loaded / e.total * 90);
        _pi(piId, pct, Math.round(e.loaded/1024)+'/' +Math.round(e.total/1024)+'KB');
      } else {
        _pi(piId, Math.min(50, Math.round(e.loaded/10240)), Math.round(e.loaded/1024)+'KB');
      }
    };
    xhr.onload = function() {
      if (xhr.status === 200 && xhr.response) {
        _pi(piId, 95, '\u89e3\u7801\u4e2d...');
        dbg('net OK: ' + cacheKey + ' (' + Math.round(xhr.response.size/1024) + 'KB)');
        _cachePut(cacheKey, xhr.response);
        _blobToTex(xhr.response, onTex, onFail, piId);
      } else { _tryCache(cacheKey, onTex, onFail, piId); }
    };
    xhr.onerror = xhr.ontimeout = function() {
      dbg('net fail: ' + cacheKey + ', try cache');
      _pi(piId, 30, '\u8bfb\u53d6\u7f13\u5b58...');
      _tryCache(cacheKey, onTex, onFail, piId);
    };
    xhr.send();
  }
  function _tryCache(key, onTex, onFail, piId) {
    _cacheGet(key, function(blob) {
      if (blob) { dbg('cache hit: ' + key); _pi(piId, 90, '\u89e3\u7801\u4e2d...'); _blobToTex(blob, onTex, onFail, piId); }
      else { dbg('cache miss: ' + key); _piDone(piId, false); onFail(); }
    });
  }
  function _blobToTex(blob, onTex, onFail, piId) {
    var url = URL.createObjectURL(blob);
    loader.load(url, function(tex) { URL.revokeObjectURL(url); _piDone(piId,true); onTex(tex); },
      undefined, function() { URL.revokeObjectURL(url); _piDone(piId,false); onFail(); });
  }

  // 加载卫星影像 + 高程位移贴图（带 IndexedDB 缓存）
  var imgLoaded = false, elevLoaded = false;
  function onBothLoaded() {
    if (!imgLoaded || !elevLoaded) return;
    dbg('terrain ready');
    st.querySelector('.title').textContent='\u52a0\u8f7d\u5b8c\u6210';
    st.style.transition='opacity .6s'; st.style.opacity='0';
    setTimeout(function(){ st.style.display='none'; }, 700);
    // Re-sync equipment and task areas now that DEM elevation data is available
    if (window._lastRadarsData) { dbg('resync radars after DEM'); syncRadars3D(window._lastRadarsData); }
    if (window._lastTaskData)   { dbg('resync tasks after DEM');  syncTaskAreas3D(window._lastTaskData); }
  }
  _cachedLoadTex(_REGION_URL, 'region', function(tex) {
    tex.minFilter = THREE.LinearMipmapLinearFilter; tex.magFilter = THREE.LinearFilter;
    tex.anisotropy = 4;
    terrainMat.map = tex; terrainMat.color.set(0xffffff); terrainMat.needsUpdate = true;
    imgLoaded = true; onBothLoaded();
  }, function() {
    terrainMat.color.set(0x4a7a3a); terrainMat.needsUpdate = true;
    imgLoaded = true; onBothLoaded();
  }, 'pi-region');
)RAW"
		+ R"RAW(
  // ===== Terrarium DEM tiles -> real heightmap =====
  function _lon2tile(lon,z){return Math.floor((lon+180)/360*Math.pow(2,z));}
  function _lat2tile(lat,z){var r=lat*Math.PI/180;return Math.floor((1-Math.log(Math.tan(r)+1/Math.cos(r))/Math.PI)/2*Math.pow(2,z));}
  function _tile2lon(x,z){return x/Math.pow(2,z)*360-180;}
  function _tile2lat(y,z){var n=Math.PI-2*Math.PI*y/Math.pow(2,z);return 180/Math.PI*Math.atan(0.5*(Math.exp(n)-Math.exp(-n)));}

  // Mercator helper: lat -> mercator Y (for proper tile pixel mapping)
  function _lat2mercY(lat){var r=lat*Math.PI/180;return Math.log(Math.tan(Math.PI/4+r/2));}

  // Fetch Terrarium tiles covering a bbox, decode to grayscale heightmap canvas
  // Uses XHR+blob URL to avoid canvas tainted (cross-origin) SecurityError on getImageData
  function _fetchTerrainDEM(bboxW,bboxS,bboxE,bboxN, outSize, piId, onDone, onFail) {
    var Z = 9;
    var tx0=_lon2tile(bboxW,Z), tx1=_lon2tile(bboxE,Z);
    var ty0=_lat2tile(bboxN,Z), ty1=_lat2tile(bboxS,Z);
    var nC=tx1-tx0+1, nR=ty1-ty0+1, total=nC*nR;
    dbg('DEM: z'+Z+' tiles='+nC+'x'+nR+'='+total);
    _pi(piId, 5, total+' tiles...');
    var TS=256;
    var cvs=document.createElement('canvas'); cvs.width=nC*TS; cvs.height=nR*TS;
    var ctx2d=cvs.getContext('2d');
    var loaded=0, failed=0, _done=false;
    // Timeout: if tiles don't complete in 20s, fall back
    var _timeout=setTimeout(function(){
      if(!_done){_done=true;dbg('DEM: timeout');onFail();}
    }, 20000);
    function _onTileReady(){
      loaded++;
      _pi(piId, 5+Math.round(loaded/total*70), loaded+'/'+total);
      if(loaded<total||_done) return;
      _done=true; clearTimeout(_timeout);
      if(failed>=total){dbg('DEM: all tiles failed');onFail();return;}
      _pi(piId, 80, 'decode...');
      try{
        var cw=nC*TS, ch=nR*TS;
        var px=ctx2d.getImageData(0,0,cw,ch).data;
        var heights=new Float32Array(cw*ch);
        var minH=1e9, maxH=-1e9;
        for(var i=0;i<cw*ch;i++){
          var h=(px[i*4]*256+px[i*4+1]+px[i*4+2]/256)-32768;
          if(h<0)h=0;
          heights[i]=h;
          if(h<minH)minH=h; if(h>maxH)maxH=h;
        }
        var range=maxH-minH; if(range<1)range=1;
        dbg('DEM: h='+Math.round(minH)+'~'+Math.round(maxH)+'m range='+Math.round(range)+'m');
        // Resample: Mercator tile pixels -> geographic (EPSG:4326) output
        var tW=_tile2lon(tx0,Z), tE=_tile2lon(tx1+1,Z);
        var tN=_tile2lat(ty0,Z), tS=_tile2lat(ty1+1,Z);
        var mercTN=_lat2mercY(tN), mercTS=_lat2mercY(tS);
        var out=document.createElement('canvas'); out.width=outSize; out.height=outSize;
        var octx=out.getContext('2d');
        var img=octx.createImageData(outSize,outSize);
        for(var oy=0;oy<outSize;oy++){
          var lat=bboxN-(bboxN-bboxS)*oy/outSize;
          var mercLat=_lat2mercY(lat);
          var tpy=Math.round((mercTN-mercLat)/(mercTN-mercTS)*ch);
          tpy=Math.max(0,Math.min(ch-1,tpy));
          for(var ox=0;ox<outSize;ox++){
            var lon=bboxW+(bboxE-bboxW)*ox/outSize;
            var tpx=Math.round((lon-tW)/(tE-tW)*cw);
            tpx=Math.max(0,Math.min(cw-1,tpx));
            var v=Math.round((heights[tpy*cw+tpx]-minH)/range*255);
            var idx=(oy*outSize+ox)*4;
            img.data[idx]=v; img.data[idx+1]=v; img.data[idx+2]=v; img.data[idx+3]=255;
          }
        }
        octx.putImageData(img,0,0);
        // Build resampled height array (same grid as output texture) for elevation queries
        var resH=new Float32Array(outSize*outSize);
        for(var ri=0;ri<outSize*outSize;ri++){
          resH[ri] = minH + img.data[ri*4]/255.0 * range;
        }
        _pi(piId, 95, 'texture...');
        var tex=new THREE.CanvasTexture(out);
        tex.minFilter=THREE.LinearFilter; tex.magFilter=THREE.LinearFilter;
        onDone(tex, range, resH, outSize, bboxW, bboxS, bboxE, bboxN, minH);
      }catch(e){
        dbg('DEM decode error: '+e.message);
        onFail();
      }
    }
    // Download tiles via XHR as blobs (same-origin blob URL avoids canvas taint)
    // Each tile is cached in IndexedDB for offline use & portability
    function _drawTileBlob(blob, tx, ty) {
      var burl=URL.createObjectURL(blob);
      var im=new Image();
      im.onload=function(){
        ctx2d.drawImage(im,(tx-tx0)*TS,(ty-ty0)*TS);
        URL.revokeObjectURL(burl);
        _onTileReady();
      };
      im.onerror=function(){URL.revokeObjectURL(burl);failed++;_onTileReady();};
      im.src=burl;
    }
    for(var ty=ty0;ty<=ty1;ty++){
      for(var tx=tx0;tx<=tx1;tx++){
        (function(tx,ty){
          var tileKey='dem/'+Z+'/'+tx+'/'+ty;
          // Try IndexedDB cache first
          _cacheGet(tileKey, function(cached){
            if(cached){
              _drawTileBlob(cached, tx, ty);
              return;
            }
            // Not cached -> download
            var url='https://s3.amazonaws.com/elevation-tiles-prod/terrarium/'+Z+'/'+tx+'/'+ty+'.png';
            var xhr=new XMLHttpRequest();
            xhr.open('GET',url,true);
            xhr.responseType='blob';
            xhr.timeout=15000;
            xhr.onload=function(){
              if(xhr.status===200&&xhr.response){
                _cachePut(tileKey, xhr.response); // cache for offline
                _drawTileBlob(xhr.response, tx, ty);
              } else {failed++;_onTileReady();}
            };
            xhr.onerror=xhr.ontimeout=function(){failed++;_onTileReady();};
            xhr.send();
          });
        })(tx,ty);
      }
    }
  }
  // Load real DEM for base terrain
  _fetchTerrainDEM(CX-SPAN,CY-SPAN,CX+SPAN,CY+SPAN, 1024, 'pi-elev',
    function(tex, range, resH, resSize, bW, bS, bE, bN, minH) {
      _elevRange = range;
      // Store DEM for elevation queries (minH = absolute minimum elevation in meters)
      _demData = {heights:resH, w:resSize, h:resSize, bboxW:bW, bboxS:bS, bboxE:bE, bboxN:bN, minH:minH};
      terrainMat.displacementScale = range;
      terrainMat.displacementBias = 0;  // terrain Y = 0..range (relative), _twY() matches this
      terrainMat.displacementMap = tex;
      terrainMat.bumpMap = tex; terrainMat.bumpScale = 0.8;
      terrainMat.needsUpdate = true;
      _dtlMat.displacementScale = range;
      _dtlMat.displacementBias = 0;
      _piDone('pi-elev', true);
      elevLoaded = true; dbg('DEM OK range='+Math.round(range)+'m'); onBothLoaded();
    },
    function() {
      // Fallback: use hillshade as bump only (no displacement)
      _cachedLoadTex(_ELEV_URL, 'elev', function(tex2) {
        tex2.minFilter = THREE.LinearFilter;
        terrainMat.displacementScale = 0;
        terrainMat.bumpMap = tex2; terrainMat.bumpScale = 1.5;
        terrainMat.needsUpdate = true;
        elevLoaded = true; dbg('DEM failed, hillshade bump fallback'); onBothLoaded();
      }, function() {
        elevLoaded = true; dbg('elev failed, flat terrain'); onBothLoaded();
      }, 'pi-elev');
    }
  );
)RAW"
		+ R"RAW(
  // ===== LOD: 相机缩放时在注视区域叠加更高精度卫星影像 =====
  var _lodTimer = 0, _lodSpan = SPAN, _lodLoading = false;
  var _lodMinSpan = 0.05; // 最小 ~5km
  // Detail overlay mesh (same displacement as base, updated when DEM loads)
  var _dtlMat = new THREE.MeshStandardMaterial({
    color: 0xffffff, roughness: 0.82, metalness: 0.0, transparent: true,
    displacementScale: 0, displacementBias: 0,
    polygonOffset: true, polygonOffsetFactor: -1
  });
  var _dtlMesh = null;
  function _buildUrl(base, bbox, sz, fmt) {
    return base + '?bbox=' + bbox[0].toFixed(4)+','+bbox[1].toFixed(4)+','+bbox[2].toFixed(4)+','+bbox[3].toFixed(4)
      + '&bboxSR=4326&size='+sz+','+sz+'&imageSR=4326&format='+fmt+'&f=image';
  }
  function _lodCheck() {
    if (_lodLoading) return;
    var camH = camera.position.y;
    var viewSpan = Math.max(_lodMinSpan, Math.min(SPAN * 0.8, camH / 55000));
    // 只在明显放大时触发（viewSpan < 基准的60%）
    if (viewSpan > SPAN * 0.6) {
      // 缩放到全局级别时移除 detail
      if (_dtlMesh) { scene.remove(_dtlMesh); _dtlMesh.geometry.dispose(); _dtlMesh = null; }
      _lodSpan = SPAN; return;
    }
    if (Math.abs(viewSpan - _lodSpan) / _lodSpan < 0.3) return;
    _lodSpan = viewSpan;
    var tLon = CX + oc.target.x / M_LON;
    var tLat = CY - oc.target.z / M_LAT;
    tLon = Math.max(CX - SPAN + viewSpan, Math.min(CX + SPAN - viewSpan, tLon));
    tLat = Math.max(CY - SPAN + viewSpan, Math.min(CY + SPAN - viewSpan, tLat));
    var bbox = [tLon - viewSpan, tLat - viewSpan, tLon + viewSpan, tLat + viewSpan];
    dbg('LOD: span=' + viewSpan.toFixed(3) + ' @' + tLon.toFixed(3) + ',' + tLat.toFixed(3));
    _lodLoading = true;
    // 创建/更新 detail mesh 的几何体位置和大小
    var dW = viewSpan * 2 * M_LON, dH = viewSpan * 2 * M_LAT;
    var dCx = (tLon - CX) * M_LON, dCz = -(tLat - CY) * M_LAT;
    if (_dtlMesh) { scene.remove(_dtlMesh); _dtlMesh.geometry.dispose(); }
    var dGeo = new THREE.PlaneGeometry(dW, dH, 128, 128);
    _dtlMesh = new THREE.Mesh(dGeo, _dtlMat);
    _dtlMesh.rotation.x = -Math.PI / 2;
    _dtlMesh.position.set(dCx, 0, dCz); // same level as base, polygonOffset handles z-fighting
    scene.add(_dtlMesh);
    // 同时请求影像和高程（DEM）
    var cntDone = 0;
    function onDone() { cntDone++; if (cntDone >= 2) _lodLoading = false; }
    // 卫星影像
    var xhr1 = new XMLHttpRequest();
    xhr1.open('GET', _buildUrl(_IMG_BASE, bbox, 2048, 'jpg'), true);
    xhr1.responseType = 'blob'; xhr1.timeout = 15000;
    xhr1.onload = function() {
      if (xhr1.status === 200 && xhr1.response) {
        var u = URL.createObjectURL(xhr1.response);
        loader.load(u, function(tex) {
          URL.revokeObjectURL(u);
          tex.minFilter = THREE.LinearMipmapLinearFilter; tex.magFilter = THREE.LinearFilter; tex.anisotropy = 4;
          _dtlMat.map = tex; _dtlMat.needsUpdate = true;
          dbg('LOD img OK ' + Math.round(xhr1.response.size/1024) + 'KB');
          onDone();
        }, undefined, function(){ URL.revokeObjectURL(u); onDone(); });
      } else onDone();
    };
    xhr1.onerror = xhr1.ontimeout = function(){ onDone(); };
    xhr1.send();
    // 高程 DEM (Terrarium tiles)
    _fetchTerrainDEM(bbox[0],bbox[1],bbox[2],bbox[3], 512, null,
      function(tex, range) {
        _dtlMat.displacementScale = _elevRange; // use base terrain range for consistency
        _dtlMat.displacementBias = 0;
        _dtlMat.displacementMap = tex;
        _dtlMat.bumpMap = tex; _dtlMat.bumpScale = 0.8;
        _dtlMat.needsUpdate = true;
        dbg('LOD DEM OK range='+Math.round(range)+'m');
        onDone();
      },
      function() { dbg('LOD DEM fail'); onDone(); }
    );
  }
  oc.addEventListener('change', function() {
    clearTimeout(_lodTimer);
    _lodTimer = setTimeout(_lodCheck, 800);
  });

  // ===== Task areas (flat ground polygons) =====
  var _tm=[];
  window.syncTaskAreas3D = function syncTaskAreas3D(arr){
    window._lastTaskData = arr;
    _tm.forEach(function(o){scene.remove(o);}); _tm=[];
    for(var _k in _ovLabels){if(_k.indexOf('t_')===0)_removeOvLabel(_k);}
    dbg('syncTaskAreas3D: '+(arr?arr.length:0)+' areas');
    if(!arr||!arr.length) return;
    arr.forEach(function(a){
      if(!a.coords||a.coords.length<3) return;
      var cenLon=0,cenLat=0;
      for(var ci=0;ci<a.coords.length;ci++){cenLon+=a.coords[ci][0];cenLat+=a.coords[ci][1];}
      cenLon/=a.coords.length; cenLat/=a.coords.length;
      var baseY=_twY(cenLon,cenLat);
      var minH=Math.max(a.minH||0,0);
      var maxH=Math.max(a.maxH||100,minH+50);
      var botY=baseY+minH, topY=baseY+maxH;
      // Collect 3D ground coords
      var c3=[];
      for(var ci=0;ci<a.coords.length;ci++){var cp=geo2s(a.coords[ci][0],a.coords[ci][1],0);c3.push(cp);}
      // Bottom flat polygon (filled)
      var sh=new THREE.Shape();
      sh.moveTo(c3[0].x,-c3[0].z);
      for(var i=1;i<c3.length;i++) sh.lineTo(c3[i].x,-c3[i].z);
      sh.closePath();
      var fm=new THREE.Mesh(new THREE.ShapeGeometry(sh),new THREE.MeshBasicMaterial({color:0x00cc55,transparent:true,opacity:.18,side:THREE.DoubleSide,depthWrite:false}));
      fm.rotation.x=-Math.PI/2; fm.position.y=botY; scene.add(fm); _tm.push(fm);
      // Top polygon (filled, slightly different tint)
      var tm=new THREE.Mesh(new THREE.ShapeGeometry(sh),new THREE.MeshBasicMaterial({color:0x44ff88,transparent:true,opacity:.12,side:THREE.DoubleSide,depthWrite:false}));
      tm.rotation.x=-Math.PI/2; tm.position.y=topY; scene.add(tm); _tm.push(tm);
      // Bottom outline
      var botPts=c3.map(function(p){return new THREE.Vector3(p.x,botY,p.z);}); botPts.push(botPts[0].clone());
      var bl=new THREE.Line(new THREE.BufferGeometry().setFromPoints(botPts),new THREE.LineBasicMaterial({color:0x00ff88,transparent:true,opacity:.70}));
      scene.add(bl); _tm.push(bl);
      // Top outline
      var topPts=c3.map(function(p){return new THREE.Vector3(p.x,topY,p.z);}); topPts.push(topPts[0].clone());
      var tl=new THREE.Line(new THREE.BufferGeometry().setFromPoints(topPts),new THREE.LineBasicMaterial({color:0x00ff88,transparent:true,opacity:.90}));
      scene.add(tl); _tm.push(tl);
      // Vertical edge lines at each corner
      for(var vi=0;vi<c3.length;vi++){
        var vp=[new THREE.Vector3(c3[vi].x,botY,c3[vi].z),new THREE.Vector3(c3[vi].x,topY,c3[vi].z)];
        var vl=new THREE.Line(new THREE.BufferGeometry().setFromPoints(vp),new THREE.LineBasicMaterial({color:0x00ff88,transparent:true,opacity:.55}));
        scene.add(vl); _tm.push(vl);
      }
      // Label at centroid above top face
      var cx3=0,cz3=0;
      for(var ii=0;ii<c3.length;ii++){cx3+=c3[ii].x;cz3+=c3[ii].z;}
      cx3/=c3.length; cz3/=c3.length;
      var lk2=_lblKey('t',a.id); _removeOvLabel(lk2);
      var lblTxt=Math.round(minH)+'-'+Math.round(maxH)+'m';
      var tlo=_makeOvLabel(lk2,'\u4efb\u52a1\u533a#'+a.id,lblTxt,'rgba(0,50,20,0.90)','#88ffcc');
      tlo.wpos.set(cx3,topY+300,cz3);
    });
  }

  // ===== HTML overlay label system (avoids canvas fillText issues in Qt WebEngine) =====
  var _lblOv = document.createElement('div');
  _lblOv.style.cssText = 'position:fixed;top:0;left:0;width:100%;height:100%;pointer-events:none;z-index:50;overflow:hidden;';
  document.body.appendChild(_lblOv);

  // Per-type color themes: [bodyHex, mastColor, dishColor, fanColor, fanBorder]
  var _typeColors = [
    {body:0x1a6aff,mast:0x224488,dish:0x00aaff,fan:0x00ccff,fbdr:0x00eeff,bg:'rgba(0,20,60,0.90)',bc:'#00aaff',tc:'#7edfff',name:'\u5149\u5b66'},
    {body:0x00aa44,mast:0x1a5533,dish:0x00ff88,fan:0x00ff88,fbdr:0x88ffbb,bg:'rgba(0,50,20,0.90)',bc:'#00ff88',tc:'#88ffcc',name:'\u96f7\u8fbe'},
    {body:0xff8800,mast:0x663300,dish:0xffaa00,fan:0xffaa00,fbdr:0xffcc44,bg:'rgba(60,25,0,0.90)',bc:'#ffaa00',tc:'#ffcc88',name:'\u9065\u6d4b'},
    {body:0xcc0000,mast:0x550000,dish:0xff4444,fan:0xff4444,fbdr:0xff8888,bg:'rgba(60,0,0,0.90)',bc:'#ff4444',tc:'#ffaaaa',name:'\u5fae\u6ce2'},
    {body:0x9900cc,mast:0x440055,dish:0xcc44ff,fan:0xcc44ff,fbdr:0xee99ff,bg:'rgba(40,0,60,0.90)',bc:'#cc44ff',tc:'#ee99ff',name:'\u7535\u6218'}
  ];

  var _ovLabels = {}; // key -> {el, wpos: THREE.Vector3}
  function _lblKey(prefix, id){ return prefix+'_'+id; }

  function _makeOvLabel(key, line1, line2, bc, tc) {
    var el = document.createElement('div');
    el.style.cssText = 'position:absolute;padding:2px 7px;border-radius:3px;'
      +'font:bold 12px Arial,sans-serif;white-space:nowrap;line-height:1.5;'
      +'background:'+bc+';color:'+tc+';border:1px solid '+bc+';'
      +'transform:translateX(-50%) translateY(-100%);margin-top:-5px;box-shadow:0 0 6px '+bc+';';
    el.innerHTML = line1
      + (line2 ? '<br><span style="font-weight:normal;font-size:10px;opacity:0.85;">'+line2+'</span>' : '');
    _lblOv.appendChild(el);
    _ovLabels[key] = {el:el, wpos:new THREE.Vector3()};
    return _ovLabels[key];
  }

  function _removeOvLabel(key) {
    if(_ovLabels[key]){ var e=_ovLabels[key].el; if(e&&e.parentNode)e.parentNode.removeChild(e); delete _ovLabels[key]; }
  }

  var _prjV = new THREE.Vector3();
  function _updateOvLabels() {
    for(var k in _ovLabels){
      var lo=_ovLabels[k]; if(!lo.el||!lo.wpos) continue;
      _prjV.copy(lo.wpos).project(camera);
      if(_prjV.z>=1){lo.el.style.display='none';continue;}
      var sx=(_prjV.x+1)/2*W, sy=(1-_prjV.y)/2*H;
      if(sx<-200||sx>W+200||sy<-200||sy>H+200){lo.el.style.display='none';continue;}
      lo.el.style.display='block';
      lo.el.style.left=Math.round(sx)+'px';
      lo.el.style.top=Math.round(sy)+'px';
    }
  }

)RAW"
	+ R"RAW(
  // ===== Radar equipment (type-based models + 3D range cones) =====
  var _rd={}, _rdById={};
  var _rr = GW * 0.000085;
  // 俯仰角默认值（当属性值为0时使用）
  var _elevDefault=15;

  // Build 3D detection volume wedge: centered at (az, elevCenter), spanning ±bh2 horizontal, ±vbh2 vertical
  function _buildRangeCone(az,bh2,elevCenter,vbh2,rng){
    var evLo=elevCenter-vbh2, evHi=elevCenter+vbh2;
    var N=24,M=6,pos=[];
    // Outer arc face (azimuth x elevation grid quads)
    for(var i=0;i<N;i++){
      var a0=az-bh2+2*bh2*i/N,a1=az-bh2+2*bh2*(i+1)/N;
      for(var j=0;j<M;j++){
        var e0=evLo+(evHi-evLo)*j/M, e1=evLo+(evHi-evLo)*(j+1)/M;
        var s0=Math.sin(e0),s1=Math.sin(e1),ce0=Math.cos(e0),ce1=Math.cos(e1);
        var x00=ce0*Math.sin(a0)*rng,y00=s0*rng,z00=-ce0*Math.cos(a0)*rng;
        var x01=ce0*Math.sin(a1)*rng,y01=s0*rng,z01=-ce0*Math.cos(a1)*rng;
        var x10=ce1*Math.sin(a0)*rng,y10=s1*rng,z10=-ce1*Math.cos(a0)*rng;
        var x11=ce1*Math.sin(a1)*rng,y11=s1*rng,z11=-ce1*Math.cos(a1)*rng;
        pos.push(x00,y00,z00, x01,y01,z01, x11,y11,z11);
        pos.push(x00,y00,z00, x11,y11,z11, x10,y10,z10);
      }
    }
    // Left and right side panels (from origin to arc edge)
    for(var j=0;j<M;j++){
      var e0=evLo+(evHi-evLo)*j/M, e1=evLo+(evHi-evLo)*(j+1)/M;
      var s0=Math.sin(e0),s1=Math.sin(e1),ce0=Math.cos(e0),ce1=Math.cos(e1);
      var aL=az-bh2,aR=az+bh2;
      pos.push(0,0,0, ce0*Math.sin(aL)*rng,s0*rng,-ce0*Math.cos(aL)*rng,
                      ce1*Math.sin(aL)*rng,s1*rng,-ce1*Math.cos(aL)*rng);
      pos.push(0,0,0, ce1*Math.sin(aR)*rng,s1*rng,-ce1*Math.cos(aR)*rng,
                      ce0*Math.sin(aR)*rng,s0*rng,-ce0*Math.cos(aR)*rng);
    }
    // Top cap fan (at evHi)
    var sH=Math.sin(evHi),cH=Math.cos(evHi);
    for(var i=0;i<N;i++){
      var a0=az-bh2+2*bh2*i/N,a1=az-bh2+2*bh2*(i+1)/N;
      pos.push(0,0,0, cH*Math.sin(a1)*rng,sH*rng,-cH*Math.cos(a1)*rng, cH*Math.sin(a0)*rng,sH*rng,-cH*Math.cos(a0)*rng);
    }
    // Bottom cap fan (at evLo)
    var sL=Math.sin(evLo),cL=Math.cos(evLo);
    for(var i=0;i<N;i++){
      var a0=az-bh2+2*bh2*i/N,a1=az-bh2+2*bh2*(i+1)/N;
      pos.push(0,0,0, cL*Math.sin(a0)*rng,sL*rng,-cL*Math.cos(a0)*rng, cL*Math.sin(a1)*rng,sL*rng,-cL*Math.cos(a1)*rng);
    }
    var geo=new THREE.BufferGeometry();
    geo.addAttribute('position',new THREE.Float32BufferAttribute(pos,3));
    geo.computeVertexNormals();
    return geo;
  }

  // Build per-type antenna head at top of mast (y=mastH in grp local coords)
  function _buildAntennaHead(type,tc,grp,mastH,azRad){
    var r=_rr;
    if(type===0){
      // GS: phased array flat panel
      var panel=new THREE.Mesh(new THREE.BoxGeometry(r*3.5,r*2.2,r*0.3),new THREE.MeshLambertMaterial({color:tc.dish}));
      panel.position.y=mastH+r*1.1; panel.rotation.y=azRad; grp.add(panel);
      var frame=new THREE.Mesh(new THREE.BoxGeometry(r*3.6,r*2.3,r*0.1),new THREE.MeshLambertMaterial({color:tc.body,wireframe:true}));
      frame.position.y=mastH+r*1.1; frame.rotation.y=azRad; grp.add(frame);
    } else if(type===1){
      // AD: large rotating dish
      var arm=new THREE.Mesh(new THREE.CylinderGeometry(r*0.1,r*0.1,r*2.4,5),new THREE.MeshLambertMaterial({color:tc.body}));
      arm.rotation.z=Math.PI/2; arm.position.y=mastH; grp.add(arm);
      var dish=new THREE.Mesh(new THREE.TorusGeometry(r*1.8,r*0.22,6,20,Math.PI),new THREE.MeshLambertMaterial({color:tc.dish,side:THREE.DoubleSide}));
      dish.rotation.y=azRad; dish.rotation.x=-Math.PI*0.35; dish.position.y=mastH+r*0.2; grp.add(dish);
    } else if(type===2){
      // FC/EO: ball turret + horizontal lens barrel
      var ball=new THREE.Mesh(new THREE.SphereGeometry(r*1.3,12,8),new THREE.MeshLambertMaterial({color:tc.body}));
      ball.position.y=mastH; grp.add(ball);
      var lens=new THREE.Mesh(new THREE.CylinderGeometry(r*0.35,r*0.5,r*1.2,8),new THREE.MeshLambertMaterial({color:tc.dish}));
      // rotX(PI/2)+rotY(-az) makes cylinder axis point in azimuth direction
      lens.rotation.set(Math.PI/2,-azRad,0);
      lens.position.set(Math.sin(azRad)*r*0.9,mastH,-Math.cos(azRad)*r*0.9); grp.add(lens);
      var ring=new THREE.Mesh(new THREE.TorusGeometry(r*0.35,r*0.07,5,10),new THREE.MeshLambertMaterial({color:tc.fan}));
      ring.rotation.set(Math.PI/2,-azRad,0);
      ring.position.set(Math.sin(azRad)*r*1.55,mastH,-Math.cos(azRad)*r*1.55); grp.add(ring);
    } else if(type===3){
      // TR/microwave: open horn antenna facing azimuth
      var horn=new THREE.Mesh(new THREE.ConeGeometry(r*1.2,r*2.5,8,1,true),new THREE.MeshLambertMaterial({color:tc.dish,side:THREE.DoubleSide}));
      // rotX(PI/2)+rotY(-az): cone base (opening) faces azimuth direction
      horn.rotation.set(Math.PI/2,-azRad,0); horn.position.y=mastH; grp.add(horn);
    } else {
      // EW: 6 omni whip antennas in a ring
      for(var w=0;w<6;w++){
        var wa=w*Math.PI/3;
        var whip=new THREE.Mesh(new THREE.CylinderGeometry(r*0.08,r*0.12,r*5,5),new THREE.MeshLambertMaterial({color:tc.dish}));
        whip.position.set(Math.cos(wa)*r*1.0,mastH+r*2.5,Math.sin(wa)*r*1.0); grp.add(whip);
        var tip=new THREE.Mesh(new THREE.SphereGeometry(r*0.18,5,4),new THREE.MeshLambertMaterial({color:tc.fan}));
        tip.position.set(Math.cos(wa)*r*1.0,mastH+r*5.0,Math.sin(wa)*r*1.0); grp.add(tip);
      }
    }
  }

)RAW"
	+ R"RAW(
  window.syncRadars3D = function syncRadars3D(arr){
    window._lastRadarsData = arr;
    for(var k in _rd){scene.remove(_rd[k]); _removeOvLabel(_lblKey('r',k));} _rd={}; _rdById={};
    if(!arr||!arr.length) return;
    arr.forEach(function(r){
      var t=(r.type||0)%_typeColors.length, tc=_typeColors[t];
      var pos=geo2s(r.lon,r.lat,0), grp=new THREE.Group();
      // 地面世界Y（相对高度0..range，displacementBias=0时与地形着色器完全对齐）
      var _surfY = _twY(r.lon, r.lat);
      // 设备世界Y = 地面相对高度 + 设备高度AGL
      var _gY = _surfY + (r.alt||0);
      // 海拔(绝对)用于标牌显示
      var _absElev = _demData ? getElevation(r.lon, r.lat) : 0;
      grp.position.set(pos.x,_gY,pos.z);
      var mastH=_rr*6;
      // Vertical peg from equipment base down to terrain surface (length = AGL altitude)
      var _altAGL = r.alt||0;
      var pegPts=[new THREE.Vector3(0,0,0),new THREE.Vector3(0,-_altAGL,0)];
      grp.add(new THREE.Line(new THREE.BufferGeometry().setFromPoints(pegPts),new THREE.LineBasicMaterial({color:tc.mast,transparent:true,opacity:r.mounted?0.08:0.25})));
      // Base platform + mast (all types share)
      grp.add(new THREE.Mesh(new THREE.CylinderGeometry(_rr*1.4,_rr*1.8,_rr*0.4,8),new THREE.MeshLambertMaterial({color:tc.mast})));
      var mast=new THREE.Mesh(new THREE.CylinderGeometry(_rr*0.2,_rr*0.5,mastH,8),new THREE.MeshLambertMaterial({color:tc.mast}));
      mast.position.y=mastH/2; grp.add(mast);
      // Type-specific antenna head
      _buildAntennaHead(t,tc,grp,mastH,(r.azimuth||0)*Math.PI/180);
      // Detection range — 水平hBeam，俯仰角elevation为指向，垂直vBeam为波束宽度
      var sweep=r.hBeam||0;
      if(r.range>0&&sweep>0){
        var azStart=(r.azimuth||0)*Math.PI/180;
        var bh2=sweep*Math.PI/180/2;
        var azCenter=azStart+bh2;
        var rng=Math.min(r.range,80000);
        var elevC=(r.elevation||0)*Math.PI/180;
        var vb=((r.vBeam>0)?r.vBeam:_elevDefault)*Math.PI/180;
        var vbh2=vb/2;
        // 先以水平方向(elevation=0)构建锥体，再旋转到俯仰角方向
        var coneGrp=new THREE.Group();
        var cGeo=_buildRangeCone(azCenter,bh2,0,vbh2,rng);
        var _cm=new THREE.Mesh(cGeo,new THREE.MeshBasicMaterial({color:tc.fan,transparent:true,opacity:.28,side:THREE.DoubleSide,depthWrite:false,depthTest:false}));
        _cm.renderOrder=2; coneGrp.add(_cm);
        // Bright outline: top arc + bottom arc + left/right side edges
        var evHi=vbh2, evLo=-vbh2;
        var arcHi=[], arcLo=[], lePts=[new THREE.Vector3(0,0,0)], rePts=[new THREE.Vector3(0,0,0)];
        for(var ii=0;ii<=32;ii++){var ag=azCenter-bh2+2*bh2*ii/32; arcHi.push(new THREE.Vector3(Math.cos(evHi)*Math.sin(ag)*rng,Math.sin(evHi)*rng,-Math.cos(evHi)*Math.cos(ag)*rng)); arcLo.push(new THREE.Vector3(Math.cos(evLo)*Math.sin(ag)*rng,Math.sin(evLo)*rng,-Math.cos(evLo)*Math.cos(ag)*rng));}
        for(var jj=0;jj<=16;jj++){var ef=evLo+(evHi-evLo)*jj/16; lePts.push(new THREE.Vector3(Math.cos(ef)*Math.sin(azCenter-bh2)*rng,Math.sin(ef)*rng,-Math.cos(ef)*Math.cos(azCenter-bh2)*rng)); rePts.push(new THREE.Vector3(Math.cos(ef)*Math.sin(azCenter+bh2)*rng,Math.sin(ef)*rng,-Math.cos(ef)*Math.cos(azCenter+bh2)*rng));}
        var elmat=new THREE.LineBasicMaterial({color:tc.fbdr,transparent:true,opacity:.9,depthTest:false});
        [arcHi,arcLo,lePts,rePts].forEach(function(pts){var _l=new THREE.Line(new THREE.BufferGeometry().setFromPoints(pts),elmat); _l.renderOrder=3; coneGrp.add(_l);});
        // 绕方位角中心轴旋转俯仰角（轴垂直于方位角方向且水平）
        if(elevC!==0){
          var rotAxis=new THREE.Vector3(Math.cos(azCenter),0,Math.sin(azCenter)).normalize();
          coneGrp.rotateOnAxis(rotAxis,elevC);
        }
        coneGrp.position.y=mastH;
        grp.add(coneGrp);
      }
      var rkey=r.type+'_'+r.id;
      scene.add(grp); _rd[rkey]=grp; _rdById[r.id]=rkey;
      var lo=_makeOvLabel(_lblKey('r',rkey),tc.name+'#'+r.id,'\u9ad8\u5ea6:'+Math.round(r.alt||0)+'m \u6d77\u62d4:'+Math.round(_absElev+(r.alt||0))+'m',tc.bc,tc.tc);
      lo.wpos.set(pos.x,_gY+mastH+_rr*3,pos.z);
    });
  }

  // Move a single radar to new position (for mounted equipment following UAV)
  window.moveRadar3D = function moveRadar3D(rid, lon, lat, alt){
    var k=_rdById[rid]; if(!k||!_rd[k]) return;
    var pos=geo2s(lon,lat,0);
    var gY=_twY(lon,lat)+(alt||0), mH=_rr*6;
    var absElev=_demData?getElevation(lon,lat):0;
    _rd[k].position.set(pos.x,gY,pos.z);
    var lk=_lblKey('r',k);
    if(_ovLabels[lk]){
      _ovLabels[lk].wpos.set(pos.x,gY+mH+_rr*3,pos.z);
      var sub=_ovLabels[lk].el?_ovLabels[lk].el.querySelector('.ov-sub'):null;
      if(sub) sub.textContent='\u9ad8\u5ea6:'+Math.round(alt||0)+'m \u6d77\u62d4:'+Math.round(absElev+(alt||0))+'m';
    }
  }

  // ===== UAV (aircraft body: fuselage + wings + tail, HTML overlay label) =====
  var _pl={};
  var _uavR = GW * 0.0005;
  var _trails = {};
  var _maxTrailPts = 600;
  window.clearTrails3D = function clearTrails3D(){
    for(var k in _trails){
      if(_trails[k]&&_trails[k].line){scene.remove(_trails[k].line);_trails[k].line.geometry.dispose();}
      if(_trails[k]&&_trails[k].dots) scene.remove(_trails[k].dots);
    }
    _trails={};
  }
  function _makeUavGroup(id,altM){
    var g=new THREE.Group();
    var umat=new THREE.MeshLambertMaterial({color:0xff3300});
    var wmat=new THREE.MeshLambertMaterial({color:0xcc2200});
    var fus=new THREE.Mesh(new THREE.SphereGeometry(_uavR,10,6),umat);
    fus.scale.set(0.55,0.38,3.2); g.add(fus);
    var nose=new THREE.Mesh(new THREE.ConeGeometry(_uavR*0.32,_uavR*1.1,6),umat);
    nose.rotation.x=Math.PI/2; nose.position.z=-_uavR*3.0; g.add(nose);
    var lwing=new THREE.Mesh(new THREE.BoxGeometry(_uavR*3.0,_uavR*0.15,_uavR*1.0),wmat);
    lwing.position.set(-_uavR*1.6,0,_uavR*0.2); g.add(lwing);
    var rwing=new THREE.Mesh(new THREE.BoxGeometry(_uavR*3.0,_uavR*0.15,_uavR*1.0),wmat);
    rwing.position.set(_uavR*1.6,0,_uavR*0.2); g.add(rwing);
    var fin=new THREE.Mesh(new THREE.BoxGeometry(_uavR*0.14,_uavR*0.85,_uavR*0.75),wmat);
    fin.position.set(0,_uavR*0.45,_uavR*2.2); g.add(fin);
    var htail=new THREE.Mesh(new THREE.BoxGeometry(_uavR*1.5,_uavR*0.12,_uavR*0.55),wmat);
    htail.position.set(0,0,_uavR*2.3); g.add(htail);
    var lp=[new THREE.Vector3(0,0,0),new THREE.Vector3(0,-(altM||0),0)];  // updated by updatePlane3D
    var lo=new THREE.Line(new THREE.BufferGeometry().setFromPoints(lp),new THREE.LineBasicMaterial({color:0xff6600,transparent:true,opacity:.35}));
    g.add(lo);
    // HTML overlay label (no canvas)
    _makeOvLabel(_lblKey('u',id), '\u65e0\u4eba\u673a#'+id, '\u9ad8\u5ea6:'+Math.round(altM)+'m', 'rgba(50,12,0,0.90)', '#ffcc88');
    return {g:g, lo:lo};
  }
  window.updatePlane3D = function updatePlane3D(id,lon,lat,altM,yaw){
    var pos=geo2s(lon,lat,0);
    pos.y = _twY(lon,lat) + (altM||0);
    if(!_pl[id]){ var obj=_makeUavGroup(id,altM); scene.add(obj.g); _pl[id]=obj; }
    var p=_pl[id];
    p.g.position.copy(pos); p.g.rotation.y=-(yaw||0)*Math.PI/180;
    var a=p.lo.geometry.attributes.position; a.setY(1,-(altM||0)); a.needsUpdate=true;
    // Trail update (pre-allocated buffer, update in place)
    var tk='tr_'+id;
    if(!_trails[tk]){
      var buf=new THREE.BufferAttribute(new Float32Array(_maxTrailPts*3),3);
      buf.setUsage(THREE.DynamicDrawUsage);
      var geo=new THREE.BufferGeometry(); geo.setAttribute('position',buf);
      geo.setDrawRange(0,0);
      var line=new THREE.Line(geo,new THREE.LineBasicMaterial({color:0xff8800}));
      scene.add(line);
      // WebGL不支持linewidth>1，额外叠加Points层（固定像素大小）增强航线可视性
      var dots=new THREE.Points(geo,new THREE.PointsMaterial({color:0xffaa00,size:4,sizeAttenuation:false}));
      scene.add(dots);
      _trails[tk]={cnt:0,line:line,dots:dots};
    }
    var tr=_trails[tk], attr=tr.line.geometry.attributes.position;
    if(tr.cnt<_maxTrailPts){
      attr.setXYZ(tr.cnt,pos.x,pos.y,pos.z);
      tr.cnt++;
    } else {
      // shift buffer left by 1
      var a=attr.array; a.copyWithin(0,3); var i=(_maxTrailPts-1)*3;
      a[i]=pos.x; a[i+1]=pos.y; a[i+2]=pos.z;
    }
    tr.line.geometry.setDrawRange(0,tr.cnt);
    attr.needsUpdate=true;
    // Update HTML overlay label position + altitude text
    var lk=_lblKey('u',id);
    if(!_ovLabels[lk]) _makeOvLabel(lk,'\u65e0\u4eba\u673a#'+id,'\u9ad8\u5ea6:'+Math.round(altM)+'m','rgba(50,12,0,0.90)','#ffcc88');
    var lo=_ovLabels[lk]; lo.wpos.copy(pos); lo.wpos.y+=_uavR*2;
    var sp=lo.el.querySelector('span');
    if(sp) sp.textContent='\u9ad8\u5ea6:'+Math.round(altM)+'m';
  }
  window.removePlane3D = function removePlane3D(id){
    if(_pl[id]){scene.remove(_pl[id].g);delete _pl[id];}
    _removeOvLabel(_lblKey('u',id));
    var tk='tr_'+id; if(_trails[tk]){if(_trails[tk].line)scene.remove(_trails[tk].line);if(_trails[tk].dots)scene.remove(_trails[tk].dots);delete _trails[tk];}
  }
  // Bulk sync all UAVs (called on 3D view activate or initial load)
  window.syncPlanes3D = function syncPlanes3D(arr){
    if(!arr) return;
    arr.forEach(function(u){updatePlane3D(u.id,u.lon,u.lat,u.alt,u.yaw);});
    // Remove planes no longer in list
    var ids={};
    arr.forEach(function(u){ids[u.id]=1;});
    for(var k in _pl){if(!ids[k])removePlane3D(k);}
  }

  // 首帧立即渲染（不等 rAF）
  renderer.render(scene, camera);
  dbg('first render done');
  // Status bar: mouse position on terrain via raycast
  var _raycaster = new THREE.Raycaster();
  var _mouse = new THREE.Vector2();
  var _mouseLon = CX, _mouseLat = CY, _mouseElev = 0, _mouseValid = false;
  renderer.domElement.addEventListener('mousemove', function(e) {
    _mouse.x = (e.clientX / W) * 2 - 1;
    _mouse.y = -(e.clientY / H) * 2 + 1;
    _raycaster.setFromCamera(_mouse, camera);
    var hits = _raycaster.intersectObject(terrainMesh);
    if (hits.length > 0) {
      var p = hits[0].point;
      _mouseLon = CX + p.x / M_LON;
      _mouseLat = CY - p.z / M_LAT;
      _mouseElev = Math.round(getElevation(_mouseLon, _mouseLat));
      _mouseValid = true;
    }
  });
  var _sbCnt = 0;
  function _updateStatusBar() {
    if (++_sbCnt % 10 !== 0) return;
    var camH = Math.round(camera.position.y);
    var ez = Math.max(1, Math.min(20, Math.round(Math.log2(295829344 / Math.max(camH, 100)))));
    var lon = _mouseValid ? _mouseLon : CX + oc.target.x / M_LON;
    var lat = _mouseValid ? _mouseLat : CY - oc.target.z / M_LAT;
    var elev = _mouseValid ? _mouseElev : Math.round(getElevation(lon, lat));  // absolute sea-level elevation
    document.title = 'SB:' + lon.toFixed(6) + ',' + lat.toFixed(6) + '|z=' + ez + '|h=' + camH + '|e=' + elev;
  }
  // setInterval 替代 rAF，确保在 Qt WebEngineView 不聚焦时也能渲染 (~30fps)
  setInterval(function(){oc.update();renderer.render(scene,camera);_updateOvLabels();_updateStatusBar();}, 33);
  window.addEventListener('resize',function(){
    W=window.innerWidth; H=window.innerHeight;
    camera.aspect=W/H; camera.updateProjectionMatrix(); renderer.setSize(W,H);
  });
});
</script></body></html>
)RAW";

	// 写到本地文件再 load，确保 <script src="3D/..."> 能从 file:// 解析
	QString htmlPath = appDir + "/3d_view.html";
	{
		QFile f(htmlPath);
		if (f.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			QTextStream ts(&f);
			ts.setCodec("UTF-8");
			ts << html;
		}
	}
	m_pWebEngineView->load(QUrl::fromLocalFile(htmlPath));
}

// 推送当前探测设备和任务区域到 Leaflet 2D 视图
void MainWindow::syncLeafletAll()
{
	if (!m_leafletReady || !m_p2DMapView) return;
	QString radarJs = QString("if(typeof syncRadars==='function')syncRadars(%1);")
		.arg(buildRadarsFullJson());
	m_p2DMapView->page()->runJavaScript(radarJs);
	QString taskJs = QString("if(typeof syncTaskAreas==='function')syncTaskAreas(%1);")
		.arg(buildTaskAreaJson());
	m_p2DMapView->page()->runJavaScript(taskJs);
}

// 推送当前所有探测设备、任务区域和无人机到3D视图
void MainWindow::sync3DAll()
{
	if (!m_3dReady || !m_pWebEngineView) return;
	QString radarJs = QString("if(typeof syncRadars3D==='function')syncRadars3D(%1);")
		.arg(buildRadarsFullJson());
	m_pWebEngineView->page()->runJavaScript(radarJs);
	QString taskJs  = QString("if(typeof syncTaskAreas3D==='function')syncTaskAreas3D(%1);")
		.arg(buildTaskAreaJson());
	m_pWebEngineView->page()->runJavaScript(taskJs);
	QString planeJs = QString("if(typeof syncPlanes3D==='function')syncPlanes3D(%1);")
		.arg(buildPlanesJson());
	m_pWebEngineView->page()->runJavaScript(planeJs);
}