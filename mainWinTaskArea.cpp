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

//�������˻���������
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
//ѡ�����˻���������
void MainWindow::SelectTaskArea()
{
	m_mapCanvas->setCurrentLayer(g_pAirTaskPolyLayer);
	m_mapCanvas->setMapTool(mToolSelect);
	g_pAirTaskPolyLayer->startEditing();
}
//�޸����˻���������
void MainWindow::FixTaskArea()
{
	//�ж��Ƿ���ѡ�е���������ͼԪ
	m_mapCanvas->setCurrentLayer(g_pAirTaskPolyLayer);

	if (g_pAirTaskPolyLayer->selectedFeatures().count() == 0)
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u9009\u62e9\u4e00\u4e2a \u65e0\u4eba\u673a\u4efb\u52a1\u533a\u57df \uff01\uff01\uff01"));
		return;
	}

	if (m_pDlgModifyArea)
		delete m_pDlgModifyArea;

	m_pDlgModifyArea = new DlgModifyTaskArea(this);

	g_pAirTaskPolyLayer->startEditing();

	QgsFeature f = g_pAirTaskPolyLayer->selectedFeatures().at(0);

	m_pDlgModifyArea->m_fid = f.id();

	QString	airID	= f.attribute(0).toString();	//����������	
	QString	sPt		= f.attribute(2).toString();	//������������λ������
	QString sWidth	= f.attribute(3).toString();	//�������
	QString sLen	= f.attribute(4).toString();	//���򳤶�
	QString	sHeight = f.attribute(5).toString();	//����߶�
	QString	sZ		= f.attribute(6).toString();	//������Ը߶�
	QString sAngle	= f.attribute(7).toString();	//����Ƕ�
	QString sTime	= f.attribute(8).toString();	//�������ʱ��
	QString sRemark = f.attribute(9).toString();//��ע

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

//�ƶ����˻���������
void MainWindow::MoveTaskArea()
{
	m_mapCanvas->setCurrentLayer(g_pAirTaskPolyLayer);
	if (g_pAirTaskPolyLayer->selectedFeatures().count() == 0)
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u9009\u62e9\u4e00\u4e2a \u65e0\u4eba\u673a\u4efb\u52a1\u533a\u57df \uff01\uff01\uff01"));
		return;
	}
	
	g_pAirTaskPolyLayer->startEditing();
	m_mapCanvas->setMapTool(mMoveFeature);

	bool b = ui->mActionMoveTaskArea->isChecked();

	qDebug() <<"b=" << b << endl;

	if (!b)//�����ѡ���ˣ���˵���Ѿ��ƶ������
	{
		g_pAirTaskPolyLayer->startEditing();
		QgsFeature f;
		QgsFeatureIterator fit = g_pAirTaskPolyLayer->getSelectedFeatures();
		while (fit.nextFeature(f))
		{
			QgsPointXY p = mMoveFeature->m_stopPoint;

			if (p.isEmpty()) break;
			QString	sID = f.attribute(0).toString();	//���˻�����������

			QString sx = QString("%1").arg(p.x(), 0, 'g',9);
			QString sy = QString("%1").arg(p.y(), 0, 'g',9);

			QString	sPt =sx+"," +sy; 	//������������λ������
			//QString sWidth = f.attribute(3).toString();	//�������
			//QString slen = f.attribute(4).toString();	//���򳤶�
			//QString heigth = f.attribute(1).toString();	//����߶�
			//QString sAngle = f.attribute(5).toString();	//����Ƕ�

			QgsAttributes attribues;
			attribues << sID <<"NAME" << sPt;
			f.setAttributes(attribues);
			g_pAirTaskPolyLayer->updateFeature(f);

			g_pAirTaskPolyLayer->commitChanges();

			ShowTaskAreaTip();//��������������ʾ��ǩ
			break;
		}		
	}
}
//ɾ�����˻���������
void MainWindow::DeleteTaskArea()
{
	//�ж��Ƿ���ѡ�е���������ͼԪ
	m_mapCanvas->setCurrentLayer(g_pAirTaskPolyLayer);

	if (g_pAirTaskPolyLayer->selectedFeatures().count() == 0)
	{
		QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u9009\u62e9\u4e00\u4e2a \u65e0\u4eba\u673a\u4efb\u52a1\u533a\u57df \uff01\uff01\uff01"));
		return;
	}

	QMessageBox::StandardButton id = QMessageBox::information(this, QStringLiteral("\u63d0\u793a"), QStringLiteral("\u8bf7\u786e\u8ba4\u662f\u5426\u5220\u9664\u9009\u4e2d\u7684\u533a\u57df\uff1f\uff1f"), QMessageBox::StandardButton::Yes | QMessageBox::StandardButton::No);

	if (id == QMessageBox::StandardButton::Yes)
	{
		g_pAirTaskPolyLayer->startEditing();
		g_pAirTaskPolyLayer->deleteSelectedFeatures();
		g_pAirTaskPolyLayer->commitChanges();
		g_pAirTaskPolyLayer->startEditing();

		ShowTaskAreaTip();
	}	
}

//��ʾ�״��Ѹ�����������ͼ��
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
	{	//������������ͼ��,��ȫ����������
		QgsFeature				taskFeat;	//��������ͼԪ
		QgsFeatureIterator		taskFit = g_pAirTaskPolyLayer->getFeatures();//��������ͼ��

		for (int i = 0; i < gRadarLayerList.size(); i++)
		{
			//gRadarLayerList[i]->startEditing();
			while (taskFit.nextFeature(taskFeat))
			{
				//QgsFeature feature = selectedFeatureList.at(i);
				//QString name = feature.attribute("name").value<QString>();
				QString sID = taskFeat.attribute(0).toString();
				QPolygonF polyxy = taskFeat.geometry().asQPolygonF();
				polyxy.erase(polyxy.begin());//ɾ������ĵ�

				// ��������
				cl::PathsD			rect_poly = rectangle_to_polygon(polyxy);
				float dTaskArea = calculate_area(rect_poly);//

				if (dTaskArea < 0)
					dTaskArea = dTaskArea * -1;
				QgsFeature			feat;
				QgsFeatureIterator	fit = gRadarLayerList[i]->getFeatures();//�״���������

				cl::PathsD union_sectors;//���кϲ�������ζ����

				while (fit.nextFeature(feat))
				{
					//����ÿ���״�ͼԪ��Ϣ������һ���µ�����ͶӰͼԪ
					QgsPointXY pt = feat.geometry().asPoint();
					QString ssID = feat.attribute(0).toString();
					QString sAngle = feat.attribute(5).toString();	//��ʼ�Ƕ�
					QString eAngle = feat.attribute(9).toString();	//ˮƽ��������
					QString pitch = feat.attribute(8).toString();	//�����Ƕ�
					QString vBeam = feat.attribute(10).toString();	//��ֱ��������
					QString length = feat.attribute(11).toString();//ͶӰ����

					//�����豸��Ϣ�Զ�����ͶӰ�����ȫ����
					QList<QgsPointXY>	set = GetTYPolygon(pt, sAngle.toInt(), eAngle.toInt(), pitch.toFloat(), length.toInt(), vBeam.toFloat(), feat.attribute(4).toFloat());
					cl::PathsD			sec = sector_to_polygon(set);//������ת��Ϊ����ν���
					union_sectors = Union(union_sectors, sec, cl::FillRule::NonZero, 8);
				}

				// ������������εĽ���
				cl::PathsD interSection = Intersect(union_sectors, rect_poly, cl::FillRule::NonZero, 8);

				// �������
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
				QgsGeometry		geom = QgsGeometry::fromPolygonXY(pxy);		//���ݶ���εĵ���Ϣ���ɼ���ͼ�ζ���

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
							<< 1 << 111 <<  //�豸���� �߶�
							0 <<											//��ʼ�Ƕ�
							45 <<											//̽��Ƕ�
							2200);											//̽�����
						g_pRadarLayer->addFeature(ftpt);*/
//��ʾ�������������ǵ��״��б�
void MainWindow::AreaRadarList()
{
	DlgAreaRadarList dlg;
	dlg.exec();
}

//������ʾ������������ʾ��ϢͼԪ
void MainWindow::ShowTaskAreaTip()
{
	foreach(TaskAreaTip *w, m_TAreaTipList) {
		delete w;
	}
	m_TAreaTipList.clear();

	QString mPath = QCoreApplication::applicationDirPath();

	//����ͼ����ȫ����������ͼԪ
	QgsFeature			feat;
	QgsFeatureIterator	fit = g_pAirTaskPolyLayer->getFeatures();

	while (fit.nextFeature(feat))
	{
		QString		sxy= feat.attribute(2).toString();
		QStringList list=sxy.split(",");
		QgsPointXY  p(list[0].toDouble(), list[1].toDouble());
		QString		ID = feat.attribute(0).toString();

		TaskAreaTip *pTip = new TaskAreaTip((QgsMapCanvas*)m_mapCanvas);
		pTip->setString(QStringLiteral("\u4efb\u52a1\u533a\u57df") + ID);
		pTip->setPos(p);
		pTip->m_id = ID.toInt();

		m_TAreaTipList.append(pTip);
	}
}

//����滮
void MainWindow::TaskGuihua()
{
	DlgTaskGuihua dlg;
	dlg.exec();
}