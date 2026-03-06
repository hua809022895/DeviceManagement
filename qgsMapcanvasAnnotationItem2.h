#ifndef QGSMAPCANVASANNOTATIONITEM2_H
#define QGSMAPCANVASANNOTATIONITEM2_H

#include "qgsmapcanvasitem.h"

class QgsAnnotation2;
class  QgsMapCanvasAnnotationItem2: public QObject, public QgsMapCanvasItem
{
    Q_OBJECT
  public:
    //! Mouse actions for interacting with item
    enum MouseMoveAction
    {
      NoAction, //!< No action
      MoveMapPosition, //!< Moving annotation map position
      MoveFramePosition, //!< Moving position of frame relative to annotation
      ResizeFrameUp, //!< Resize frame up
      ResizeFrameDown, //!< Resize frame down
      ResizeFrameLeft, //!< Resize frame left
      ResizeFrameRight, //!< Resize frame right
      ResizeFrameLeftUp, //!< Resize frame left up
      ResizeFrameRightUp, //!< Resize frame right up
      ResizeFrameLeftDown, //!< Resize frame left down
      ResizeFrameRightDown //!< Resize frame right down
    };

	QPainter					*m_painter = nullptr;
    /**  * Constructor for QgsMapCanvasAnnotationItem.    */
    QgsMapCanvasAnnotationItem2(QgsAnnotation2 *annotation SIP_TRANSFER, QgsMapCanvas *mapCanvas SIP_TRANSFERTHIS );

    /* Returns the item's annotation. \note not available in Python bindings */
    const QgsAnnotation2 *annotation() const { return mAnnotation; } SIP_SKIP

    /** Returns the item's annotation.  */
    QgsAnnotation2 *annotation() { return mAnnotation; }
    void updatePosition() override;
    QRectF boundingRect() const override;
    void paint( QPainter *painter ) override;

    /** Returns the mouse move behavior for a given position in scene coordinates   */
    MouseMoveAction moveActionForPosition( QPointF pos ) const;

    /** Returns matching cursor shape for a mouse move action.   */
    Qt::CursorShape cursorShapeForAction( MouseMoveAction moveAction ) const;
  private slots:
    void updateBoundingRect();
    void onCanvasLayersChanged();
    //! Sets a feature for the current map position
    void setFeatureForMapPosition();
    void annotationDeleted();

  private:
    //! Draws selection handles around the item
    void drawSelectionBoxes( QPainter *p ) const;

    //! Returns the symbol size scaled in (mapcanvas) pixels. Used for the counting rect calculation
    double scaledSymbolSize() const;
    QgsAnnotation2 *mAnnotation = nullptr;
    //! Bounding rect (including item frame and balloon)
    QRectF mBoundingRect;
};

#endif // QGSMAPCANVASANNOTATIONITEM_H
