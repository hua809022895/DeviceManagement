/***************************************************************************
                              qgsannotationwidget.h
 ***************************************************************************/

#ifndef QGSANNOTATIONWIDGET_H
#define QGSANNOTATIONWIDGET_H

#include <memory>

class QgsMapCanvasAnnotationItem;
class QgsMarkerSymbol;
class QgsFillSymbol;
#include "ui_qgsAnnotationWidgetBase.h"
/* A configuration widget to configure the annotation item properties. Usually embedded by QgsAnnotation Subclass configuration dialogs*/

class  QgsAnnotationWidget: public QWidget, public Ui::QgsAnnotationWidgetBase
{
    Q_OBJECT
  public:

    QgsAnnotationWidget( QgsMapCanvasAnnotationItem *item, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags() );

    //! Returns the annotation frame symbol fill color
    QColor backgroundColor();

    void apply();

  private:

    void frameStyleChanged();

  signals:

    //! Emitted when the background color of the annotation is changed
    void backgroundColorChanged( const QColor &color );

  private:

    QgsMapCanvasAnnotationItem *mItem = nullptr;

    void blockAllSignals( bool block );
    void updateCenterIcon();
    void updateFillIcon();
};

#endif // QGSANNOTATIONWIDGET_H
