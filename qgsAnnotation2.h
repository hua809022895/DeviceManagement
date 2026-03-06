/***************************************************************************
                             qgsAnnotation2.h
 ***************************************************************************/

#ifndef QGSANNOTATION2_H
#define QGSANNOTATION2_H

#include "qgis_core.h"
#include "qgis.h"
#include "qgspointxy.h"
#include "qgscoordinatereferencesystem.h"
#include "qgssymbol.h"
#include "qgsmargins.h"
#include "qgsmaplayer.h"
#include <qgsMapCanvasItem.h>

class QgsRenderContext;
class QgsMapCanvas;

//无人机轨迹线类
class QgsAnnotation2: public QObject//, public QgsMapCanvasItem
{
    Q_OBJECT
    //Q_PROPERTY(bool visible READ isVisible WRITE setVisible )
    //Q_PROPERTY(bool hasFixedMapPosition READ hasFixedMapPosition WRITE setHasFixedMapPosition )
    //Q_PROPERTY(QSizeF frameSize READ frameSize WRITE setFrameSize )
 public:
	 QgsAnnotation2(QObject *parent SIP_TRANSFERTHIS = nullptr);
	 virtual QgsAnnotation2 *clone() const = 0 SIP_FACTORY;
	 bool isVisible() const { return mVisible; }
	 void setVisible(bool visible);

	 bool hasFixedMapPosition() const { return mHasFixedMapPosition; }
	 void setHasFixedMapPosition(bool fixed);

	 int					m_id = -1;
	 QgsMapCanvas			*m_pCanvas = nullptr;//画布对象指针
	 QVector<QgsPointXY>	m_PtList;	//轨迹线点的集合

	
	 QgsCoordinateReferenceSystem mapPositionCrs() const { return mMapPositionCrs; }

	 /* Sets the CRS of the map position.\see mapPositionCrs()  */
	 void setMapPositionCrs(const QgsCoordinateReferenceSystem &crs);
	 QPointF relativePosition() const { return mRelativePosition; }
	 void setRelativePosition(QPointF position);

	 Q_DECL_DEPRECATED void setFrameOffsetFromReferencePoint(QPointF offset) SIP_DEPRECATED;
	 Q_DECL_DEPRECATED QPointF frameOffsetFromReferencePoint() const SIP_DEPRECATED;
	 void setFrameOffsetFromReferencePointMm(QPointF offset);
	 QPointF frameOffsetFromReferencePointMm() const { return mOffsetFromReferencePoint; }

	 Q_DECL_DEPRECATED void setFrameSize(QSizeF size) SIP_DEPRECATED;
	 Q_DECL_DEPRECATED QSizeF frameSize() const SIP_DEPRECATED;

	 void setFrameSizeMm(QSizeF size);
	 QSizeF frameSizeMm() const { return mFrameSize; }
	 void setContentsMargin(const QgsMargins &margins);
	 QgsMargins contentsMargin() const { return mContentsMargins; }
	 void setFillSymbol(QgsFillSymbol *symbol SIP_TRANSFER);
	 QgsFillSymbol *fillSymbol() const { return mFillSymbol.get(); }

	 void render(QgsRenderContext &context) const;
	 virtual void writeXml(QDomElement &elem, QDomDocument &doc, const QgsReadWriteContext &context) const = 0;
	 virtual void readXml(const QDomElement &itemElem, const QgsReadWriteContext &context) = 0;

	 QgsMarkerSymbol *markerSymbol() const { return m_MarkerSymbol.get(); }
	 QgsMapLayer *mapLayer() const { return mMapLayer.data(); }
	 void setMapLayer(QgsMapLayer *layer);

	 QgsFeature associatedFeature() const { return mFeature; }
	 virtual void setAssociatedFeature(const QgsFeature &feature);
	 virtual bool accept(QgsStyleEntityVisitorInterface *visitor) const;
signals:
	//! Emitted whenever the annotation's appearance changes
	void appearanceChanged();
	/* Emitted when the annotation's position has changed and items need to be moved to reflect this.  */
	void moved();
	/** Emitted when the map layer associated with the annotation changes. */
	void mapLayerChanged();
protected:
	virtual void renderAnnotation(QgsRenderContext &context, QSizeF size) const = 0;
	virtual QSizeF minimumFrameSize() const;

	void _writeXml(QDomElement &itemElem, QDomDocument &doc, const QgsReadWriteContext &context) const;
	void _readXml(const QDomElement &annotationElem, const QgsReadWriteContext &context);
	void copyCommonProperties(QgsAnnotation2 *target) const;
private:
	//! Check where to attach the balloon connection between frame and map point
	void updateBalloon();
	//! Gets the frame line (0 is the top line, 1 right, 2 bottom, 3 left)
	QLineF segment(int index, QgsRenderContext *context) const;

	//! Draws the annotation frame to a destination painter
	void drawFrame(QgsRenderContext &context) const;
	//! Draws the map position marker symbol to a destination painter
	void drawMarkerSymbol(QgsRenderContext &context) const;
	bool mVisible = true;
	//! True if the item stays at the same map position, FALSE if the item stays on same screen position
	bool mHasFixedMapPosition = true;

	QgsCoordinateReferenceSystem mMapPositionCrs;//! CRS of the map position	
	QPointF mRelativePosition;	//! Relative position (for non-fixed items) (0-1)
	//! Describes the shift of the item content box to the reference point in mm
	QPointF mOffsetFromReferencePoint = QPointF(13, -13);
	
	QSizeF mFrameSize;//! Size of the frame in mm (without balloon)
	//! Point symbol that is to be drawn at the map reference location
	std::unique_ptr<QgsMarkerSymbol> m_MarkerSymbol;//点标记符号
	QgsMargins mContentsMargins;
	//! Fill symbol used for drawing annotation
	std::unique_ptr<QgsFillSymbol> mFillSymbol;

	//! Segment number where the connection to the map point is attached. -1 if no balloon needed (e.g. if point is contained in frame)
	int mBalloonSegment = -1;
	//! First segment point for drawing the connection (ccw direction) (always in mm)
	QPointF mBalloonSegmentPoint1;
	//! Second segment point for drawing the balloon connection (ccw direction) (always in mm)
	QPointF mBalloonSegmentPoint2;
	//! Associated layer (or NULLPTR if not attached to a layer)
	QgsWeakMapLayerPointer mMapLayer;
	//! Associated feature, or invalid feature if no feature associated
	QgsFeature mFeature;
	//! Roughly 10 pixels at 96 dpi, to match earlier version rendering
	double mSegmentPointWidthMm = 2.64;
};

#endif // QGSANNOTATION_H

