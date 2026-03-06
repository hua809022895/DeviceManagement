#include "stdafx.h"
#include <QDialog>

#include "qgsMaptoolAnnotation.h"
#include "qgsannotation.h"
//#include "qgsformannotationdialog.h"
//#include "qgsformannotation.h"
#include "qgshtmlannotation.h"
//#include "qgshtmlannotationdialog.h"
#include "qgslogger.h"
#include "qgsmapcanvas.h"
#include "jpgAnnotation.h"

#include "qgsproject.h"
#include "qgsexception.h"
#include "qgsannotationmanager.h"
#include "qgsmapmouseevent.h"

#include "qgsMaptoolJpgAnnotation.h"

QgsMapToolJpgAnnotation::QgsMapToolJpgAnnotation(QgsMapCanvas *canvas):QgsMapTool(canvas)
{
  mCursor = QCursor( Qt::ArrowCursor );
}

void QgsMapToolJpgAnnotation::canvasReleaseEvent( QgsMapMouseEvent *e )
{
  Q_UNUSED( e )

  mCurrentMoveAction = QgsMapCanvasAnnotationItem::NoAction;
  mCanvas->setCursor( mCursor );
}

void QgsMapToolJpgAnnotation::canvasPressEvent( QgsMapMouseEvent *e )
{
  if ( !mCanvas )
  {
    return;
  }

  mLastMousePosition = e->pos();

  QgsMapCanvasAnnotationItem *existingItem = itemAtPos( e->pos() );
  if ( existingItem )
  {
    existingItem->setSelected( true );
    JpgAnnotation *jpgAnnotation =  reinterpret_cast<JpgAnnotation*>(existingItem->annotation()) ;
    emit showLeftTableMessage(mLastMousePosition,true,jpgAnnotation->mppoint);
  }else{
      mCanvas->scene()->clearSelection();
      emit showLeftTableMessage(mLastMousePosition,false,nullptr);
  }

//   QgsMapCanvasAnnotationItem *item = selectedItem();
//  if ( item )
//  {
//    mCurrentMoveAction = item->moveActionForPosition( e->pos() );
//    if ( mCurrentMoveAction != QgsMapCanvasAnnotationItem::NoAction )
//    {
//      emit showLeftTableMessage(mLastMousePosition,true);
//      return;
//    }
//  }

//  if ( !item || mCurrentMoveAction == QgsMapCanvasAnnotationItem::NoAction )
//  {
//    //select a new item if there is one at this position
//    mCanvas->scene()->clearSelection();

//    QgsMapCanvasAnnotationItem *existingItem = itemAtPos( e->pos() );
//    if ( existingItem )
//    {
//      existingItem->setSelected( true );
//    }

//  }
}
void QgsMapToolJpgAnnotation::canvasMoveEvent( QgsMapMouseEvent *e )
{
  // mLastMousePosition = e->pos();
}

void QgsMapToolJpgAnnotation::keyPressEvent( QKeyEvent *e )
{

}

void QgsMapToolJpgAnnotation::canvasDoubleClickEvent( QgsMapMouseEvent *e )
{

}


QgsMapCanvasAnnotationItem *QgsMapToolJpgAnnotation::itemAtPos( QPointF pos ) const
{
  if ( !mCanvas )
  {
    return nullptr;
  }

  QList<QGraphicsItem *> graphicItems = mCanvas->items( pos.toPoint() );
  QList<QGraphicsItem *>::iterator gIt = graphicItems.begin();
  for ( ; gIt != graphicItems.end(); ++gIt )
  {
    QgsMapCanvasAnnotationItem *annotationItem = dynamic_cast<QgsMapCanvasAnnotationItem *>( *gIt );
    if ( annotationItem )
    {
      return annotationItem;
    }
  }
  return nullptr;
}

QgsMapCanvasAnnotationItem *QgsMapToolJpgAnnotation::selectedItem() const
{
  if ( !mCanvas || !mCanvas->scene() )
  {
    return nullptr;
  }
  QList<QGraphicsItem *> gItemList = mCanvas->scene()->selectedItems();
  QList<QGraphicsItem *>::iterator it = gItemList.begin();
  for ( ; it != gItemList.end(); ++it )
  {
    QgsMapCanvasAnnotationItem *aItem = dynamic_cast<QgsMapCanvasAnnotationItem *>( *it );
    if ( aItem )
    {
      return aItem;
    }
  }
  return nullptr;
}

QList<QgsMapCanvasAnnotationItem *> QgsMapToolJpgAnnotation::annotationItems() const
{
  if ( !mCanvas )
  {
    return QList<QgsMapCanvasAnnotationItem *>();
  }
  else
  {
    return mCanvas->annotationItems();
  }
}



QgsPointXY QgsMapToolJpgAnnotation::transformCanvasToAnnotation( QgsPointXY p, QgsAnnotation *annotation ) const
{
  if ( annotation->mapPositionCrs() != mCanvas->mapSettings().destinationCrs() )
  {
    QgsCoordinateTransform transform( mCanvas->mapSettings().destinationCrs(), annotation->mapPositionCrs(), QgsProject::instance() );
    try
    {
      p = transform.transform( p );
    }
    catch ( const QgsCsException & )
    {
      // ignore
    }
  }
  return p;
}
