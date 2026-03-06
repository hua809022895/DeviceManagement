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
#include "DlgAreaRadarList.h"

#include "Polygon.h"
#include "global.h"
#include "clipper2/include/clipper.h"
#include "DlgTaskGuihua.h"

//添加无人机任务区域
void MainWindow::AddAirTaskArea()
{
	if (m_pDlgAddTaskArea == nullptr)
	{
		m_pDlgAddTaskArea = new DlgAddTaskArea(this);
	}

	QString s = m_lblCoordinate->text();
	m_pDlgAddTaskArea->ui.lineEdit_1->setText(s);
	m_pDlgAddTaskArea->show();
}
//选择无人机任务区域
void MainWindow::SelectTaskArea()
{
	m_mapCanvas->setCurrentLayer(g_pAirTaskPolyLayer);
	m_mapCanvas->setMapTool(mToolSelect);
	g_pAirTaskPolyLayer->startEditing();
}
//修改无人机任务区域
void MainWindow::FixTaskArea()
{
	//判断是否有选中的任务区域图元
	m_mapCanvas->setCurrentLayer(g_pAirTaskPolyLayer);

	if (g_pAirTaskPolyLayer->selectedFeatures().count() == 0)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请先选中一个 无人机任务区域   ！！"));
		return;
	}

	if (m_pDlgModifyArea)
		delete m_pDlgModifyArea;

	m_pDlgModifyArea = new DlgModifyTaskArea(this);

	g_pAirTaskPolyLayer->startEditing();

	QgsFeature f = g_pAirTaskPolyLayer->selectedFeatures().at(0);

	m_pDlgModifyArea->m_fid = f.id();

	QString	airID	= f.attribute(0).toString();	//任务区域编号	
	QString	sPt		= f.attribute(2).toString();	//任务区域中心位置坐标
	QString sWidth	= f.attribute(3).toString();	//区域宽度
	QString sLen	= f.attribute(4).toString();	//区域长度
	QString	sHeight = f.attribute(5).toString();	//区域高度
	QString	sZ		= f.attribute(6).toString();	//区域相对高度
	QString sAngle	= f.attribute(7).toString();	//区域角度
	QString sTime	= f.attribute(8).toString();	//区域飞行时间
	QString sRemark = f.attribute(9).toString();//备注

	m_pDlgModifyArea->ui.lineEdit_1->setText(sPt);
	m_pDlgModifyArea->ui.lineEdit_2->setText(airID);
	m_pDlgModifyArea->ui.lineEdit_3->setText(sWidth);
	m_pDlgModifyArea->ui.lineEdit_4->setText(sLen);
	m_pDlgModifyArea->ui.lineEdit_5->setText(sHeight);
	m_pDlgModifyArea->ui.lineEdit_6->setText(sTime);
	m_pDlgModifyArea->ui.lineEdit_7->setText(sAngle);
	m_pDlgModifyArea->ui.lineEdit_8->setText(sRemark);
	m_pDlgModifyArea->ui.lineEdit_9->setText(sZ);

	m_pDlgModifyArea->m_pMapCanvas = m_mapCanvas;
	m_pDlgModifyArea->show();
}

//移动无人机任务区域
void MainWindow::MoveTaskArea()
{
	m_mapCanvas->setCurrentLayer(g_pAirTaskPolyLayer);
	if (g_pAirTaskPolyLayer->selectedFeatures().count() == 0)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请先选中一个 无人机任务区域   ！！"));
		return;
	}
	
	g_pAirTaskPolyLayer->startEditing();
	m_mapCanvas->setMapTool(mMoveFeature);

	bool b = ui->mActionMoveTaskArea->isChecked();

	qDebug() <<"b=" << b << endl;

	if (!b)//如果是选中了，就说明已经移动完成了
	{
		g_pAirTaskPolyLayer->startEditing();
		QgsFeature f;
		QgsFeatureIterator fit = g_pAirTaskPolyLayer->getSelectedFeatures();
		while (fit.nextFeature(f))
		{
			QgsPointXY p = mMoveFeature->m_stopPoint;

			if (p.isEmpty()) break;
			QString	sID = f.attribute(0).toString();	//无人机任务区域编号

			QString sx = QString("%1").arg(p.x(), 0, 'g',9);
			QString sy = QString("%1").arg(p.y(), 0, 'g',9);

			QString	sPt =sx+"," +sy; 	//任务区域中心位置坐标
			//QString sWidth = f.attribute(3).toString();	//区域宽度
			//QString slen = f.attribute(4).toString();	//区域长度
			//QString heigth = f.attribute(1).toString();	//区域高度
			//QString sAngle = f.attribute(5).toString();	//区域角度

			QgsAttributes attribues;
			attribues << sID <<"NAME" << sPt;
			f.setAttributes(attribues);
			g_pAirTaskPolyLayer->updateFeature(f);

			g_pAirTaskPolyLayer->commitChanges();

			ShowTaskAreaTip();//跟新任务区域提示标签
			break;
		}		
	}
}
//删除无人机任务区域
void MainWindow::DeleteTaskArea()
{
	//判断是否有选中的任务区域图元
	m_mapCanvas->setCurrentLayer(g_pAirTaskPolyLayer);

	if (g_pAirTaskPolyLayer->selectedFeatures().count() == 0)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("请先选中一个 无人机任务区域   ！！"));
		return;
	}

	QMessageBox::StandardButton id = QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("您确实想删除选中的区域吗？？"), QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No);

	if (id == QMessageBox::StandardButton::Yes)
	{
		g_pAirTaskPolyLayer->startEditing();
		g_pAirTaskPolyLayer->deleteSelectedFeatures();
		g_pAirTaskPolyLayer->commitChanges();
		g_pAirTaskPolyLayer->startEditing();

		ShowTaskAreaTip();
	}	
}

//显示雷达已覆盖任务区域，图层
void MainWindow::ShowRadarNoArea()
{
	g_pRadarTaskLayer->startEditing();

	if (g_pRadarTaskLayer->featureCount() > 0)
	{
		QgsFeature feat;
		QgsFeatureIterator fit = g_pRadarTaskLayer->getFeatures();
		while (fit.nextFeature(feat))
			g_pRadarTaskLayer->deleteFeature(feat.id());
	}
	else
	{	//遍历任务区域图层,内全部任务区域
		QgsFeature				taskFeat;	//任务区域图元
		QgsFeatureIterator		taskFit = g_pAirTaskPolyLayer->getFeatures();//任务区域图层

		for (int i = 0; i < gRadarLayerList.size(); i++)
		{
			//gRadarLayerList[i]->startEditing();
			while (taskFit.nextFeature(taskFeat))
			{
				//QgsFeature feature = selectedFeatureList.at(i);
				//QString name = feature.attribute("name").value<QString>();
				QString sID = taskFeat.attribute(0).toString();
				QPolygonF polyxy = taskFeat.geometry().asQPolygonF();
				polyxy.erase(polyxy.begin());//删除多余的点

				// 创建矩形
				cl::PathsD			rect_poly = rectangle_to_polygon(polyxy);
				float dTaskArea = calculate_area(rect_poly);//

				if (dTaskArea < 0)
					dTaskArea = dTaskArea * -1;
				QgsFeature			feat;
				QgsFeatureIterator	fit = gRadarLayerList[i]->getFeatures();//雷达扇形区域

				cl::PathsD union_sectors;//所有合并后的扇形多边形

				while (fit.nextFeature(feat))
				{
					//根据每个雷达图元信息，创建一个新的扇形投影图元
					QgsPointXY pt = feat.geometry().asPoint();
					QString ssID = feat.attribute(0).toString();
					QString sAngle = feat.attribute(5).toString();	//起始角度
					QString eAngle = feat.attribute(6).toString();	//结束角度
					QString pitch = feat.attribute(8).toString();	//俯仰角度
					QString length = feat.attribute(11).toString();//投影长度

					//根据设备信息自动生成投影多边形全部点
					QList<QgsPointXY>	set = GetTYPolygon(pt, sAngle.toInt(), eAngle.toInt(), pitch.toFloat(), length.toInt());
					cl::PathsD			sec = sector_to_polygon(set);//将扇形转换为多边形近似
					union_sectors = Union(union_sectors, sec, cl::FillRule::NonZero, 8);
				}

				// 计算扇形与矩形的交集
				cl::PathsD interSection = Intersect(union_sectors, rect_poly, cl::FillRule::NonZero, 8);

				// 计算面积
				double area = calculate_area(interSection);
				qDebug() << "iCount area=" << area << " dTaskArea=" << dTaskArea << endl;
				QList<QgsPointXY>	set1;

				for (const auto& path : interSection) {
					for (const auto& point : path) {
						set1.append(QgsPointXY(point.x, point.y));
					}
				}

				int iCount = set1.size();
				if (iCount < 3)
					continue;
				QgsPolygonXY	pxy = QgsPolygonXY() << set1.toVector();
				QgsGeometry		geom = QgsGeometry::fromPolygonXY(pxy);		//根据多边形的点信息生成几何图形对象

				iCount = g_pRadarTaskLayer->featureCount();
				QgsFeature f = QgsFeature();
				f.setGeometry(geom);
				f.setAttributes(QgsAttributes() << iCount);
				bool b = g_pRadarTaskLayer->addFeature(f);
				qDebug() << "Plane key:" << b << endl;
			}
		}
	}
	g_pRadarTaskLayer->commitChanges();
}

/*
QgsGeometry geometry = QgsGeometry::fromPointXY(QgsPointXY(point.getX(), point.getY()));
						QgsFeature ftpt = QgsFeature();
						ftpt.setGeometry(geometry);
						ftpt.setAttributes(QgsAttributes() << i+10
							<< QString::number(point.getX(), 'f', 6)
							<< QString::number(point.getY(), 'f', 6)
							<< 1 << 111 <<  //设备所在 高度
							0 <<											//起始角度
							45 <<											//探测角度
							2200);											//探测距离
						g_pRadarLayer->addFeature(ftpt);*/
//显示任务区域所覆盖的雷达列表
void MainWindow::AreaRadarList()
{
	DlgAreaRadarList dlg;
	dlg.exec();
}

//更新显示，任务区域提示信息图元
void MainWindow::ShowTaskAreaTip()
{
	foreach(TaskAreaTip *w, m_TAreaTipList) {
		delete w;
	}
	m_TAreaTipList.clear();

	QString mPath = QCoreApplication::applicationDirPath();

	//遍历图层内全部任务区域图元
	QgsFeature			feat;
	QgsFeatureIterator	fit = g_pAirTaskPolyLayer->getFeatures();

	while (fit.nextFeature(feat))
	{
		QString		sxy= feat.attribute(2).toString();
		QStringList list=sxy.split(",");
		QgsPointXY  p(list[0].toDouble(), list[1].toDouble());
		QString		ID = feat.attribute(0).toString();

		TaskAreaTip *pTip = new TaskAreaTip((QgsMapCanvas*)m_mapCanvas);
		pTip->setString(QString::fromLocal8Bit("任务区域") + ID);
		pTip->setPos(p);
		pTip->m_id = ID.toInt();

		m_TAreaTipList.append(pTip);
	}
}

//任务规划
void MainWindow::TaskGuihua()
{
	DlgTaskGuihua dlg;
	dlg.exec();
}