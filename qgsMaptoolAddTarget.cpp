#include "stdafx.h"
#include "comm.h"
#include "qgsMaptoolAddTarget.h"
#include "qgsmapcanvas.h"
#include "qgsmapmouseevent.h"
#include "qgspoint.h"

#include "qgsMapToolSelectUtils.h"
#include <qgsVectorLayerFeatureiterator.h>
#include "qDialogTarget.h"

#include "mainWindow.h"
#include "marketPointDialog.h"
#include "marketCircleDialog.h"
#include <QSettings>

QgsMapToolAddTarget::QgsMapToolAddTarget( QgsMapCanvas *canvas, CaptureMode mode,QgsAdvancedDigitizingDockWidget* mDock )
    : QgsMapToolDigitizeFeature( canvas, mDock, mode )
{
     setLayer( canvas->currentLayer() );
}

void QgsMapToolAddTarget::cadCanvasPressEvent(QgsMapMouseEvent *e)
{
    QgsPoint point = mapPoint( *e );
    QgsVectorLayer* layer = currentVectorLayer();

    if ( !layer )
    {
      notifyNotVectorLayer();
      clean();
      stopCapturing();
      e->ignore();
      return;
    }

    if ( e->button() == Qt::LeftButton )
    {
       if (layer==g_pTargetLayer)
       {
           QgsRectangle r = QgsMapToolSelectUtils::expandSelectRectangle(point, mCanvas, layer );
           QgsFeatureIds ids = QgsMapToolSelectUtils::getMatchingFeatures(mCanvas, QgsGeometry::fromRect( r ),true,true );
           QSetIterator<QgsFeatureId> iterator(ids);

           if (iterator.hasNext())
           {
               MainWindow::sInstance->m_lblSelectTargetCount->setText( QString::fromLocal8Bit("选中1个") );
               QgsFeature feature = g_pTargetLayer->getFeature(iterator.next());
               QDialogTarget target(MainWindow::instance());
               target.setWindowTitle(QString::fromLocal8Bit("修改目标"));
               target.userinvoke = "modify";
               target.setFeature( feature );
               target.exec();
           }else{
               MainWindow::sInstance->m_lblSelectTargetCount->setText( QString("") );
               QDialogTarget target(MainWindow::instance());
               target.setWindowTitle(QString::fromLocal8Bit("添加目标"));
               target.userinvoke = "add";
               target.pos_x = point.x();
               target.pos_y = point.y();
               target.pos_z = 0;
               target.setPosition();
               target.exec();
           }
       }

       /*if ( layer  ==m_PointLayer)
       {
             QgsRectangle r = QgsMapToolSelectUtils::expandSelectRectangle( point, mCanvas, layer );
             QgsFeatureIds ids = QgsMapToolSelectUtils::getMatchingFeatures( mCanvas, QgsGeometry::fromRect( r ),true,true );
             QSetIterator<QgsFeatureId> iterator(ids);
             if (iterator.hasNext()){
                 QgsFeature feature = m_PointLayer->getFeature(iterator.next());
                 MarketPointDialog pointdialog(MainWindow::instance());
                 pointdialog.isinsert = false;
                 pointdialog.x = point.x();
                 pointdialog.y = point.y();
                 pointdialog.setFeature( feature );
                 pointdialog.exec();
             }else{
                 MarketPointDialog pointdialog(MainWindow::instance());
                 pointdialog.isinsert = true;
                 pointdialog.canvas = mCanvas;
                 pointdialog.x = point.x();
                 pointdialog.y = point.y();
                 pointdialog.exec();
             }
       }*/
    }
    else if ( e->button() == Qt::RightButton)
    {
        if ( layer  == g_pTargetLayer){
            QgsRectangle r = QgsMapToolSelectUtils::expandSelectRectangle( point, mCanvas, layer );
           // QgsMapToolSelectUtils::selectSingleFeature( mCanvas, QgsGeometry::fromRect( r ), Qt::NoModifier );
            QgsFeatureIds ids = QgsMapToolSelectUtils::getMatchingFeatures( mCanvas, QgsGeometry::fromRect( r ),true,true );

            QSetIterator<QgsFeatureId> iterator(ids);
            if (iterator.hasNext()){
                QgsFeature feature = g_pTargetLayer->getFeature(iterator.next());
                QDialogTarget target(MainWindow::instance());
                target.setWindowTitle(QString::fromLocal8Bit("发送任务"));
                target.setFeature( feature );
                target.exec();
            }
        }
    }
}
