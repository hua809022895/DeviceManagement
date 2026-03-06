/***************************************************************************
    qgsmaptoolmovefeature.h  -  map tool for translating features by mouse drag
 ***************************************************************************/

#ifndef QGSMAPTOOLMOVEFEATURE_H
#define QGSMAPTOOLMOVEFEATURE_H

#include "qgsmaptooladvanceddigitizing.h"
#include "qgspointxy.h"
#include "qgsfeatureid.h"

class QgsSnapIndicator;

//! Map tool for translating feature position by mouse drag
class  QgsMapToolMoveFeature: public QgsMapToolAdvancedDigitizing
{
    Q_OBJECT
  public:
    //! Mode for moving features
    enum MoveMode
    {
      Move, //!< Move feature
      CopyMove  //!< Copy and move feature
    };

    QgsMapToolMoveFeature( QgsMapCanvas *canvas, MoveMode mode = Move, QgsAdvancedDigitizingDockWidget *cad = nullptr);
    ~QgsMapToolMoveFeature() override;

	QgsPointXY	m_stopPoint;//鼠标移动后抬起的坐标
    void cadCanvasMoveEvent( QgsMapMouseEvent *e ) override;
    void cadCanvasReleaseEvent( QgsMapMouseEvent *e ) override;
    void deactivate() override;

    //! catch escape when active to action
    void keyReleaseEvent( QKeyEvent *e ) override;

  private:
    //! Start point of the move in map coordinates
    QgsPointXY mStartPointMapCoords;

    //! Rubberband that shows the feature being moved
    QgsRubberBand *mRubberBand = nullptr;

    //! Snapping indicators
    std::unique_ptr<QgsSnapIndicator> mSnapIndicator;

    //! Id of moved feature
    QgsFeatureIds mMovedFeatures;

    QPoint mPressPos;

    MoveMode mMode;

};

#endif
