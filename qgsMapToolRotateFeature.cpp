/***************************************************************************
    qgsmaptoolrotatefeature.cpp  -  map tool for rotating features by mouse drag
    ---------------------
    begin                : January 2012
    copyright            : (C) 2012 by Vinayan Parameswaran
    email                : vinayan123 at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "stdafx.h"
#include "qgsMaptoolRotateFeature.h"
#include "qgsgeometry.h"
#include "qgslogger.h"
#include "qgsmapcanvas.h"
#include "qgsrubberband.h"
#include "qgsvectordataprovider.h"
#include "qgsvectorlayer.h"
#include "qgstolerance.h"
#include <QMessageBox>
#include <QMouseEvent>
#include <QSettings>
#include <limits>
#include <math.h>
#include <qgsVertexMarker.h>

#define PI 3.14159265

QgsMapToolRotateFeature::QgsMapToolRotateFeature( QgsMapCanvas* canvas):QgsMapToolEdit(canvas), mRubberBand(0)
{
	m_Rotation	= 0;
	mAnchorPoint= 0;
	m_Ctrl		= false;
}

QgsMapToolRotateFeature::~QgsMapToolRotateFeature()
{
	delete mAnchorPoint;
	delete mRubberBand;
}

void QgsMapToolRotateFeature::canvasMoveEvent( QMouseEvent * e )
{
	if (m_Ctrl)
	{
		if (!mAnchorPoint)
			return;
		
		mAnchorPoint->setCenter(toMapCoordinates( e->pos()));
		m_StartCoords = toMapCoordinates( e->pos());
		mStPoint = e->pos();
		return;
	}

	if (mRubberBand)
	{
		double XDistance = mStPoint.x() - e->pos().x();
		double YDistance = mStPoint.y() - e->pos().y();
		m_Rotation = atan2( YDistance, XDistance ) * (180/PI);//计算旋转角度值

		//转换地图坐标到屏幕坐标
		mStPoint = toCanvasCoordinates(m_StartCoords);
		double offsetX = mStPoint.x() - mRubberBand->x();
		double offsetY = mStPoint.y() - mRubberBand->y();

		mRubberBand->setTransform(QTransform().translate(offsetX, offsetY).rotate(m_Rotation).translate( -1 * offsetX, -1 * offsetY));
		mRubberBand->update();
	}
}

void QgsMapToolRotateFeature::canvasPressEvent(QMouseEvent *e)
{
	m_Rotation = 0;
	if (m_Ctrl)
		return;

	delete mRubberBand;
	mRubberBand = 0;  //mInitialPos = e->pos();

	QgsVectorLayer* vlayer = currentVectorLayer();
	if ( !vlayer )
	{
		notifyNotVectorLayer();
		return;
	}

	if ( !vlayer->isEditable() )
	{
		notifyNotEditableLayer();
		return;
	}

	QPoint pt=e->pos();
	QgsPointXY layerCoords = toLayerCoordinates( vlayer, e->pos());
	double searchRadius = QgsTolerance::vertexSearchRadius( mCanvas->currentLayer(), mCanvas->mapSettings() );
	QgsRectangle selectRect( layerCoords.x() - searchRadius, layerCoords.y() - searchRadius,
		layerCoords.x() + searchRadius, layerCoords.y() + searchRadius );

	if (vlayer->selectedFeatureCount()==0)
	{
		QgsFeatureIterator fit = vlayer->getFeatures( QgsFeatureRequest().setFilterRect( selectRect ).setSubsetOfAttributes( QgsAttributeList() ) );

		//find the closest feature
		//QgsPointXY pxy=
		QgsGeometry pointGeometry = QgsGeometry::fromPointXY(layerCoords );
		if ( pointGeometry.isEmpty())
			return;

		double minDistance = std::numeric_limits<double>::max();

		QgsFeature cf;
		QgsFeature f;
		while ( fit.nextFeature(f))
		{
			if ( f.isValid())
			{
				double currentDistance = pointGeometry.distance( f.geometry() );
				if ( currentDistance < minDistance)
				{
					minDistance = currentDistance;
					cf = f;
				}
			}
		}
		//delete pointGeometry;

		if ( minDistance == std::numeric_limits<double>::max())
			return;

		QgsRectangle bound = cf.geometry().boundingBox();
		m_StartCoords = toMapCoordinates(vlayer, bound.center() );

		if (!mAnchorPoint)
			mAnchorPoint = new QgsVertexMarker(mCanvas);
		
		mAnchorPoint->setIconType( QgsVertexMarker::ICON_CROSS );
		mAnchorPoint->setCenter(m_StartCoords);
		mStPoint = toCanvasCoordinates(m_StartCoords);

		m_RotatedFeatures.clear();
		m_RotatedFeatures << cf.id(); //todo: take the closest feature, not the first one...

		mRubberBand = createRubberBand( vlayer->geometryType());
		mRubberBand->setToGeometry( cf.geometry(), vlayer);
	}
	else
	{
		m_RotatedFeatures = vlayer->selectedFeatureIds(); // > selectedFeaturesIds();
		mRubberBand = createRubberBand(vlayer->geometryType());
		for ( int i = 0; i < vlayer->selectedFeatureCount(); i++)
		{
			mRubberBand->addGeometry(vlayer->selectedFeatures()[i].geometry(), vlayer);
		}
	}

	mRubberBand->setColor(QColor(255, 0, 0, 65));
	mRubberBand->setWidth(2);
	mRubberBand->show();
}

void QgsMapToolRotateFeature::canvasReleaseEvent(QMouseEvent *e)
{
	Q_UNUSED( e );
	if ( !mRubberBand)
		return;

	QgsVectorLayer* vlayer = currentVectorLayer();
	if ( !vlayer)
		return;

	m_Rotation = 20;
	//calculations for affine transformation
	double angle = -1 *m_Rotation * ( PI/180);
	QgsPointXY anchorPoint = toLayerCoordinates(vlayer, m_StartCoords);
	double a = cos( angle );
	double b = -1 * sin( angle );
	double c = anchorPoint.x() - cos( angle ) * anchorPoint.x() + sin(angle) * anchorPoint.y();
	double d = sin( angle );
	double ee = cos( angle );
	double f = anchorPoint.y() - sin( angle ) * anchorPoint.x() - cos(angle) * anchorPoint.y();

	vlayer->beginEditCommand(tr("Features Rotated"));

	int start;
	if (vlayer->geometryType()==2)
		start = 1;
	else
		start = 0;

	//循环遍历要旋转的图元，根据mRotatedFeatures
	int i = 0;
	foreach (QgsFeatureId id, m_RotatedFeatures)
	{
		QgsFeature feat;
		vlayer->getFeatures(QgsFeatureRequest().setFilterFid(id)).nextFeature(feat);
		QgsGeometry geom = feat.geometry();

		if (!geom.isEmpty())
		{
			i = start;
			QgsPoint vertex = geom.vertexAt(i);
			while (vertex != QgsPoint(0, 0))
			{
				double newX = a * vertex.x() + b * vertex.y() + c;
				double newY = d * vertex.x() + ee * vertex.y() + f;

				vlayer->moveVertex( newX, newY, id, i );
				i = i + 1;
				vertex = geom.vertexAt( i );
			}
		}		
	}

	double anchorX = a * anchorPoint.x() + b * anchorPoint.y() + c;
	double anchorY = d * anchorPoint.x() + ee * anchorPoint.y() + f;

	mAnchorPoint->setCenter(QgsPoint( anchorX, anchorY));

	delete mRubberBand;
	mRubberBand = 0;

	mCanvas->refresh();
	vlayer->endEditCommand();
}

void QgsMapToolRotateFeature::keyPressEvent(QKeyEvent* e)
{
  if ( e->key() == Qt::Key_Control )
  {
    m_Ctrl= true;
    mCanvas->viewport()->setMouseTracking( true );
    return;
  }
}

void QgsMapToolRotateFeature::keyReleaseEvent( QKeyEvent* e )
{
  if ( e->key() == Qt::Key_Control)
  {
    m_Ctrl= false;
    mCanvas->viewport()->setMouseTracking( false);
    return;
  }
}

void QgsMapToolRotateFeature::deactivate()
{
	delete mRubberBand;
	delete mAnchorPoint;
	mRubberBand		= 0;
	mAnchorPoint	= 0;
	QgsMapTool::deactivate();
}

//选中此地图工具的时候执行此函数
void QgsMapToolRotateFeature::activate()
{
	QgsVectorLayer	*vlayer=currentVectorLayer();
	if ( !vlayer)
		return;

	if ( !vlayer->isEditable())
		return;

	if (vlayer->selectedFeatureCount()>0)
	{
		QgsRectangle bound = vlayer->boundingBoxOfSelected();//得到选中图元的经纬度面积值坐标
		m_StartCoords = toMapCoordinates(vlayer, bound.center());

		mAnchorPoint = new QgsVertexMarker(mCanvas);
		mAnchorPoint->setIconType(QgsVertexMarker::ICON_CROSS);
		mAnchorPoint->setCenter(m_StartCoords);
		mStPoint = toCanvasCoordinates(m_StartCoords);

		QgsMapTool::activate();
	}
}
//测试函数，根据屏幕坐标e，自动旋转图元
void QgsMapToolRotateFeature::autoRotate(QPoint ePt)
{
	QgsVectorLayer	*vlayer = currentVectorLayer();

	QPoint e;
	//遍历图层内图元

	vlayer->selectAll(); 
	
	if (vlayer->selectedFeatureCount()==0)
	{
		return;
	}

	if (QgsPoint(0, 0)==m_StartCoords)
	{
		QgsRectangle bound = vlayer->boundingBoxOfSelected();//得到选中图元的经纬度面积值坐标
		m_StartCoords = toMapCoordinates(vlayer, bound.center());

		mAnchorPoint = new QgsVertexMarker(mCanvas);
		mAnchorPoint->setIconType(QgsVertexMarker::ICON_CROSS);
		mAnchorPoint->setCenter(m_StartCoords);
		mStPoint = toCanvasCoordinates(m_StartCoords);
	}

	//鼠标按下
	m_Rotation = 2;//新的旋转角度值

	delete mRubberBand;
	mRubberBand = 0;

	if (!vlayer)
	{
		notifyNotVectorLayer();
		return;
	}

	if (!vlayer->isEditable())
	{
		notifyNotEditableLayer();
		return;
	}
	
	QgsPointXY layerCoords = toLayerCoordinates(vlayer, e);
	double searchRadius = QgsTolerance::vertexSearchRadius( mCanvas->currentLayer(), mCanvas->mapSettings());
	QgsRectangle selectRect(layerCoords.x() - searchRadius, layerCoords.y() - searchRadius,
					layerCoords.x() + searchRadius, layerCoords.y() + searchRadius);
	
	m_RotatedFeatures = vlayer->selectedFeatureIds();
	mRubberBand = createRubberBand( vlayer->geometryType());
	for (int i = 0; i < vlayer->selectedFeatureCount(); i++)
	{
		mRubberBand->addGeometry(vlayer->selectedFeatures()[i].geometry(), vlayer);
	}

	//鼠标旋转,移动
	if (mRubberBand)
	{
		//转换地图坐标到屏幕坐标
		mStPoint = toCanvasCoordinates(m_StartCoords);
		double offsetX = mStPoint.x() - mRubberBand->x();
		double offsetY = mStPoint.y() - mRubberBand->y();

		mRubberBand->setTransform(QTransform().translate(offsetX, offsetY).rotate(m_Rotation).translate( -1 * offsetX, -1 * offsetY));
		mRubberBand->update();
	}

	//鼠标抬起
	//calculations for affine transformation
	double angle = -1 *m_Rotation * ( PI/180);
	QgsPointXY anchorPoint = toLayerCoordinates(vlayer, m_StartCoords);
	double a = cos( angle );
	double b = -1 * sin( angle );
	double c = anchorPoint.x() - cos( angle ) * anchorPoint.x() + sin(angle) * anchorPoint.y();
	double d = sin( angle );
	double ee = cos( angle );
	double f = anchorPoint.y() - sin( angle ) * anchorPoint.x() - cos(angle) * anchorPoint.y();

	vlayer->beginEditCommand(tr("Features Rotated"));

	int start;
	if (vlayer->geometryType()==2)
		start = 1;
	else
		start = 0;

	//循环遍历要旋转的图元，根据mRotatedFeatures
	int i = 0;
	foreach (QgsFeatureId id, m_RotatedFeatures)
	{
		QgsFeature feat;
		vlayer->getFeatures(QgsFeatureRequest().setFilterFid(id)).nextFeature(feat);
		QgsGeometry geom = feat.geometry();

		if (!geom.isEmpty())
		{	//遍历某个图元的所有顶点
			i = start;	//顶点id值
			QgsPoint vertex = geom.vertexAt(i);
			while (vertex != QgsPoint(0, 0))
			{
				double newX = a * vertex.x() + b * vertex.y() + c;
				double newY = d * vertex.x() + ee * vertex.y() + f;

				vlayer->moveVertex(newX, newY, id, i);
				i = i + 1;
				vertex = geom.vertexAt(i);
			}
		}
	}

	double anchorX = a * anchorPoint.x() + b * anchorPoint.y() + c;
	double anchorY = d * anchorPoint.x() + ee * anchorPoint.y() + f;
	mAnchorPoint->setCenter(QgsPoint(anchorX, anchorY));

	delete mRubberBand;
	mRubberBand = 0;
	
	vlayer->endEditCommand();
	vlayer->deselect(vlayer->selectedFeatureIds());
	mCanvas->refresh();
}
