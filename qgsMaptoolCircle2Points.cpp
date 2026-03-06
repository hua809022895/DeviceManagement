/***************************************************************************
    qgmaptoolcircle2points.cpp  -  map tool for adding circle
    from 2 points
    ---------------------
    begin                : July 2017
    copyright            : (C) 2017 by Loïc Bartoletti
    email                : lbartoletti at tuxfamily dot org
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "stdafx.h"
#include "qgsMaptoolCircle2Points.h"
#include "qgsgeometryrubberband.h"
#include "qgspoint.h"
#include "qgsmapmouseevent.h"
#include "qgssnapindicator.h"
#include "marketCircleDialog.h"
#include "qgsMapToolSelectUtils.h"

QgsMapToolCircle2Points::QgsMapToolCircle2Points( QgsMapToolCapture *parentTool,
    QgsMapCanvas *canvas, CaptureMode mode): QgsMapToolAddCircle( parentTool, canvas, mode )
{
  mToolName = tr( "Add circle from 2 points" );
}

void QgsMapToolCircle2Points::cadCanvasReleaseEvent( QgsMapMouseEvent *e )
{
  QgsPoint point = mapPoint( *e );

  if (!currentVectorLayer())
  {
    notifyNotVectorLayer();
    clean();
    stopCapturing();
    e->ignore();
    return;
  }

  if ( e->button() == Qt::LeftButton )
  {
      QgsRectangle r = QgsMapToolSelectUtils::expandSelectRectangle( point, mCanvas, g_pPolygonLayer);
      QgsFeatureIds ids = QgsMapToolSelectUtils::getMatchingFeatures( mCanvas, QgsGeometry::fromRect( r ),false,true );
      QSetIterator<QgsFeatureId> iterator(ids);
      if (iterator.hasNext()){
        QgsFeature feature = g_pPolygonLayer->getFeature(iterator.next());
        QString s_class = feature.attribute("class").value<QString>();
        QString radius = feature.attribute("radius").value<QString>();

        if ((s_class=="禁飞区") && (abs(radius.toDouble()) > 0.001) )
        {
			MarketCircleDialog marketcircleDialog(gMainWindow); // MainWindow::instance());
            marketcircleDialog.isinsert = false;
            marketcircleDialog.setupdatefeature( feature );
            marketcircleDialog.exec();
            return;
        }
      }

    if (mPoints.isEmpty())
      mPoints.append(point );

     if ( !mTempRubberBand )
    {
      mTempRubberBand = createGeometryRubberBand( mLayerType, true );
      mTempRubberBand->show();
    }
  }
  else if ( e->button() == Qt::RightButton )
  {
    if ( !mTempRubberBand )
    {
        release( e );
        return;
    }
    mPoints.append( point );
    release( e );

    //mpPolygonLayer

	MarketCircleDialog marketcircleDialog(gMainWindow);// MainWindow::instance() );
    marketcircleDialog.mCircle = mCircle;
    double r = mCircle.radius();

    double factorUnits = QgsUnitTypes::fromUnitToUnitFactor( QgsUnitTypes::DistanceDegrees, QgsUnitTypes::DistanceMeters );
    r = r * factorUnits;

    double x = mCircle.center().x();
    double y = mCircle.center().y();
    marketcircleDialog.setPositionAndR( QString::number(x,'f',6)+","+QString::number(y,'f',6), QString::number(r,'f',1) );


    marketcircleDialog.exec();
    mTempRubberBand->setActive(false);
    delete mTempRubberBand;
    mTempRubberBand = nullptr;
  }
}

void QgsMapToolCircle2Points::cadCanvasMoveEvent( QgsMapMouseEvent *e )
{
  QgsPoint point = mapPoint( *e );

  mSnapIndicator->setMatch( e->mapPointMatch() );

  if ( mTempRubberBand )
  {
    mCircle = QgsCircle().fromCenterPoint( mPoints.at( 0 ), point );
    mTempRubberBand->setGeometry( mCircle.toCircularString( true ) );
  }
}
