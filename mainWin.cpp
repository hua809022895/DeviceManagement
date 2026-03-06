#include "stdafx.h"
#include "mainWindow.h"
#include "ui_mainwindow.h"

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstring>

#include <QComboBox>
#include <QLineEdit>
#include <QGridLayout>
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

	ui->mPlaneWidget->setItem(row, 0, new QTableWidgetItem(p->ID));//ID��
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
		break;
	case 1: // Leaflet 2D
		m_mapCanvas->hide();
		if (m_p2DMapView) m_p2DMapView->show();
		m_pWebEngineView->hide();
		// 同步当前 QGIS 中心点到 Leaflet
		if (m_leafletReady && m_mapCanvas)
		{
			QgsPointXY center = m_mapCanvas->center();
			QString js = QString("setCenter(%1,%2)")
				.arg(center.y(), 0, 'f', 6)
				.arg(center.x(), 0, 'f', 6);
			m_p2DMapView->page()->runJavaScript(js);
		}
		break;
	case 2: // 3D CesiumJS
		m_mapCanvas->hide();
		if (m_p2DMapView) m_p2DMapView->hide();
		m_pWebEngineView->show();
		{
			QRect rc = m_mapCanvas->rect();
			m_pWebEngineView->setGeometry(0, 0, rc.width(), rc.height());
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
.plane-lbl {
  background: rgba(0,80,160,0.85);
  color: #fff;
  padding: 1px 6px;
  border-radius: 3px;
  font: bold 12px Arial;
  white-space: nowrap;
  border: 1px solid #0055cc;
  pointer-events: none;
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

function makeIcon(yaw) {
  var svg = '<svg xmlns="http://www.w3.org/2000/svg" width="28" height="28" viewBox="0 0 28 28">'
    + '<g transform="rotate(' + (yaw||0) + ' 14 14)">'
    + '<polygon points="14,3 19,25 14,20 9,25" fill="#00ccff" stroke="#fff" stroke-width="1.2"/>'
    + '</g></svg>';
  return L.divIcon({ html:svg, className:'', iconSize:[28,28], iconAnchor:[14,14] });
}

function updatePlane(id, lat, lng, label, yaw) {
  lat = +lat; lng = +lng;
  if (!planes[id]) {
    var m = L.marker([lat,lng], { icon:makeIcon(yaw) }).addTo(map);
    var lblEl = document.createElement('div');
    lblEl.className = 'plane-lbl';
    lblEl.innerText = label || id;
    var lbl = L.marker([lat,lng], {
      icon: L.divIcon({ html:lblEl, className:'', iconAnchor:[-16,8] })
    }).addTo(map);
    planes[id] = { m:m, lbl:lbl };
  } else {
    var ll = L.latLng(lat, lng);
    planes[id].m.setLatLng(ll);
    planes[id].m.setIcon(makeIcon(yaw));
    planes[id].lbl.setLatLng(ll);
  }
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
</script>
</body></html>
)HTML";
		f.close();
	}

	connect(m_p2DMapView, &QWebEngineView::loadFinished, this, [this](bool ok) {
		m_leafletReady = ok;
	});
	m_p2DMapView->load(QUrl::fromLocalFile(htmlPath));
}