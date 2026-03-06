/***************************************************************************
    qgsmaptoolrotatefeature.h  -  map tool for rotating features by mouse drag
 ***************************************************************************/
#ifndef QGSMAPTOOLROTATEFEATURE_H
#define QGSMAPTOOLROTATEFEATURE_H

#include "qgsmapTooledit.h"
#include "qgsVectorLayer.h"

class QgsVertexMarker;

/**Map tool for translating feature position by mouse drag*/
class QgsMapToolRotateFeature: public QgsMapToolEdit   //APP_EXPORT
{
    Q_OBJECT
  public:
    QgsMapToolRotateFeature( QgsMapCanvas* canvas );
    virtual ~QgsMapToolRotateFeature();

    virtual void canvasMoveEvent(QMouseEvent *e);
    virtual void canvasPressEvent(QMouseEvent *e);
    virtual void canvasReleaseEvent(QMouseEvent *e);

    void keyPressEvent( QKeyEvent* e);
    void keyReleaseEvent(QKeyEvent* e);

    //! to reset the rotation anchor to selectionbound center
    //void resetAnchor();
    //! called when map tool is being deactivated
    void	deactivate();
    void	activate();
	void	autoRotate(QPoint e);	//根据屏幕坐标旋转图元

  private:
    QgsGeometry		rotateGeometry(QgsGeometry geom, QgsPoint point, double angle);
    QgsPoint		rotatePoint(QgsPoint point, double angle);

    /**Start point of the move in map coordinates*/
	QgsPointXY		m_StartCoords;	//旋转时图元中心的起始的经纬度坐标值

    /**Rubberband that shows the feature being moved*/
    QgsRubberBand	*mRubberBand=NULL;	//旋转时总共的区域

    /**Id of moved feature*/
    QgsFeatureIds	m_RotatedFeatures;	//要旋转的图元集合
    double			m_Rotation;			//旋转角度值，按顺时针开始算共计360度，初始值0
    QPoint			mStPoint;
    QgsVertexMarker	*mAnchorPoint;
    bool			m_Ctrl;	/** flag if crtl is pressed */
};

#endif
