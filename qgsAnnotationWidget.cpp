/***************************************************************************
                              qgsannotationwidget.cpp
                              ------------------------
  begin                : February 25, 2010
  copyright            : (C) 2010 by Marco Hugentobler
  email                : marco dot hugentobler at hugis dot net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "stdafx.h"
#include "qgsAnnotationWidget.h"
#include "qgsmapcanvasAnnotationItem.h"
#include "qgsannotation.h"
#include "qgsstyle.h"
#include "qgssymbollayerutils.h"
#include "qgssymbol.h"
#include "qgssymbolselectordialog.h"

#include <QColorDialog>
#include "mainWindow.h"

QgsAnnotationWidget::QgsAnnotationWidget( QgsMapCanvasAnnotationItem *item, QWidget *parent, Qt::WindowFlags f )
  : QWidget( parent, f )
  , mItem( item )
{
  setupUi( this );
  //mLayerComboBox->setAllowEmptyLayer( true );

  //mMapMarkerButton->setSymbolType( QgsSymbol::Marker );
  //mFrameStyleButton->setSymbolType( QgsSymbol::Fill );

  if ( mItem && mItem->annotation() )
  {
    QgsAnnotation *annotation = mItem->annotation();
    blockAllSignals( true );

    if ( annotation->hasFixedMapPosition() )
    {
      mMapPositionFixedCheckBox->setCheckState( Qt::Checked );
    }
    else
    {
      mMapPositionFixedCheckBox->setCheckState( Qt::Unchecked );
    }

//    whileBlocking( mSpinTopMargin )->setValue( annotation->contentsMargin().top() );
//    whileBlocking( mSpinLeftMargin )->setValue( annotation->contentsMargin().left() );
//    whileBlocking( mSpinRightMargin )->setValue( annotation->contentsMargin().right() );
//    whileBlocking( mSpinBottomMargin )->setValue( annotation->contentsMargin().bottom() );

//    mLayerComboBox->setLayer( annotation->mapLayer() );

    const QgsMarkerSymbol *symbol = annotation->markerSymbol();
    if ( symbol )
    {
     // mMapMarkerButton->setSymbol( symbol->clone() );
    }
    const QgsFillSymbol *fill = annotation->fillSymbol();
    if ( fill )
    {
    //  mFrameStyleButton->setSymbol( fill->clone() );
    }

    blockAllSignals( false );
  }
//  mMapMarkerButton->setMapCanvas( &(MainWindow::instance()->mapCanvas) );
//  mMapMarkerButton->setMessageBar( MainWindow::instance()->mInfoBar);
//  mFrameStyleButton->setMapCanvas( &(MainWindow::instance()->mapCanvas) );
//  mFrameStyleButton->setMessageBar( MainWindow::instance()->messageBar() );

//  connect( mFrameStyleButton, &QgsSymbolButton::changed, this, &QgsAnnotationWidget::frameStyleChanged );
}

QColor QgsAnnotationWidget::backgroundColor()
{
    //zjrzjr
    return QColor();//mFrameStyleButton->symbol() ? mFrameStyleButton->symbol()->color() : QColor();
}

void QgsAnnotationWidget::frameStyleChanged()
{
    emit backgroundColorChanged( backgroundColor() );
}

void QgsAnnotationWidget::apply()
{
  if ( mItem )
  {
    QgsAnnotation *annotation = mItem->annotation();
    if ( annotation )
    {

      QStringList list = mlineEditXY->text().split(QString(","));
      double x = list.at( 0 ).toDouble();
      double y = list.at( 1 ).toDouble();
      annotation->setMapPosition( QgsPointXY(x,y) );

      QgsSvgAnnotation *svg =  static_cast<QgsSvgAnnotation*>(annotation);
      if ( svg )
      {
          //zjr zjr svg->mAngle = mLineEditAngle->text().toDouble();
      }

      annotation->setHasFixedMapPosition( mMapPositionFixedCheckBox->checkState() == Qt::Checked );

//      annotation->setFillSymbol( mFrameStyleButton->clonedSymbol<QgsFillSymbol>() );
//      annotation->setMarkerSymbol( mMapMarkerButton->clonedSymbol<QgsMarkerSymbol>() );
//      annotation->setMapLayer( mLayerComboBox->currentLayer() );
//      annotation->setContentsMargin( QgsMargins( mSpinLeftMargin->value(),
//                                     mSpinTopMargin->value(),
//                                     mSpinRightMargin->value(),
//                                     mSpinBottomMargin->value() ) );
    }
    mItem->update();
  }
}

void QgsAnnotationWidget::blockAllSignals( bool block )
{
  mMapPositionFixedCheckBox->blockSignals( block );
  //mMapMarkerButton->blockSignals( block );
  //mLayerComboBox->blockSignals( block );
}
