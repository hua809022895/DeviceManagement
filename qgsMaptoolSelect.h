#ifndef QGSMAPTOOLSELECT_H
#define QGSMAPTOOLSELECT_H

#include <QMenu>

#include <qgsmaptool.h>
#include "qgsMaptoolSelectionHandler.h"

class QgsMapCanvas;
class QMouseEvent;
class QgsMapToolSelect : public QgsMapTool
{
    Q_OBJECT
  public:

    enum Mode
    {
      GeometryIntersectsSetSelection,
      GeometryIntersectsAddToSelection,
      GeometryIntersectsSubtractFromSelection,
      GeometryIntersectsIntersectWithSelection,
      GeometryWithinSetSelection,
      GeometryWithinAddToSelection,
      GeometryWithinSubtractFromSelection,
      GeometryWithinIntersectWithSelection,
    };

    QgsMapToolSelect( QgsMapCanvas *canvas );

    void setSelectionMode( QgsMapToolSelectionHandler::SelectionMode selectionMode );

    void canvasPressEvent( QgsMapMouseEvent *e ) override;
    void canvasMoveEvent( QgsMapMouseEvent *e ) override;
    void canvasReleaseEvent( QgsMapMouseEvent *e ) override;
    void keyPressEvent( QKeyEvent *e ) override;
    void keyReleaseEvent( QKeyEvent *e ) override;
    void deactivate() override;
    Flags flags() const override;
    void populateContextMenu( QMenu *menu ) override;

  signals:

    void modeChanged( Mode mode );

  private slots:
    void selectFeatures( Qt::KeyboardModifiers modifiers );

  private:
    std::unique_ptr<QgsMapToolSelectionHandler> mSelectionHandler;
    float x,y;

    void modifiersChanged( bool ctrlModifier, bool shiftModifier, bool altModifier );

};


#endif // QGSMAPTOOLSELECT_H
