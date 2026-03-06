#ifndef QGSMAPTOOLJPGANNOTATION_H
#define QGSMAPTOOLJPGANNOTATION_H

#include "qgsmaptool.h"
#include "comm.h"
#include "qgsmapcanvasannotationitem.h"

class QgsMapToolJpgAnnotation : public QgsMapTool
{
    Q_OBJECT
public:
  QgsMapToolJpgAnnotation( QgsMapCanvas *canvas );

  void canvasPressEvent( QgsMapMouseEvent *e ) override;
  void canvasReleaseEvent( QgsMapMouseEvent *e ) override;
  void canvasMoveEvent( QgsMapMouseEvent *e ) override;
  void canvasDoubleClickEvent( QgsMapMouseEvent *e ) override;
  void keyPressEvent( QKeyEvent *e ) override;

signals:
  void showLeftTableMessage(QPoint& pos,bool show, Mappoint* point);
protected:

  /** Creates a new item. To be implemented by subclasses. */
  virtual QgsAnnotation *createItem() const { return nullptr; }

  //! Creates an editor widget (caller takes ownership)
  //QDialog *createItemEditor( QgsMapCanvasAnnotationItem *item );
private:
  //! Returns the topmost annotation item at the position (or 0 if none)
  QgsMapCanvasAnnotationItem *itemAtPos( QPointF pos ) const;
  QgsMapCanvasAnnotationItem *selectedItem() const;
  //! Returns a list of all annotationitems in the canvas
  QList<QgsMapCanvasAnnotationItem *> annotationItems() const;
  QgsPointXY transformCanvasToAnnotation( QgsPointXY p, QgsAnnotation *annotation ) const;
  QgsMapCanvasAnnotationItem::MouseMoveAction mCurrentMoveAction = QgsMapCanvasAnnotationItem::NoAction;
  QPoint mLastMousePosition = QPoint( 0, 0 );
};

#endif // QGSMAPTOOLJPGANNOTATION_H
