/***************************************************************************
                             qgsAnnotation.h
 ***************************************************************************/

#ifndef QGSANNOTATIONA_H
#define QGSANNOTATIONA_H

#include "qgis_core.h"
#include "qgis.h"
#include "qgspointxy.h"
#include "qgscoordinatereferencesystem.h"
#include "qgssymbol.h"
#include "qgsmargins.h"
#include "qgsmaplayer.h"

class QgsRenderContext;
class QgsAnnotationA : public QObject  //ͼԪ��ʾ�࣬CORE_EXPORT
{
    Q_OBJECT
    Q_PROPERTY( bool visible READ isVisible WRITE setVisible )
    Q_PROPERTY( bool hasFixedMapPosition READ hasFixedMapPosition WRITE setHasFixedMapPosition )
    Q_PROPERTY( QgsPointXY mapPosition READ mapPosition WRITE setMapPosition )
    Q_PROPERTY( QSizeF frameSize READ frameSize WRITE setFrameSize )
  public:
    QgsAnnotationA(QObject *parent SIP_TRANSFERTHIS =nullptr);
    
    virtual QgsAnnotationA *clone() const = 0 SIP_FACTORY;    
    bool isVisible() const { return mVisible; }
    void setVisible( bool visible );
    bool hasFixedMapPosition() const { return mHasFixedMapPosition; }    
    void setHasFixedMapPosition( bool fixed );
    QgsPointXY mapPosition() const { return mMapPosition; }

    void setMapPosition( const QgsPointXY &position );
    QgsCoordinateReferenceSystem mapPositionCrs() const { return mMapPositionCrs; }
    void setMapPositionCrs( const QgsCoordinateReferenceSystem &crs );
    QPointF relativePosition() const { return mRelativePosition; }
    void setRelativePosition( QPointF position );
    
    Q_DECL_DEPRECATED void setFrameOffsetFromReferencePoint( QPointF offset ) SIP_DEPRECATED;
    Q_DECL_DEPRECATED QPointF frameOffsetFromReferencePoint() const SIP_DEPRECATED;

    void setFrameOffsetFromReferencePointMm( QPointF offset );
    QPointF frameOffsetFromReferencePointMm() const { return mOffsetFromReferencePoint; }

    Q_DECL_DEPRECATED void setFrameSize( QSizeF size ) SIP_DEPRECATED;
    Q_DECL_DEPRECATED QSizeF frameSize() const SIP_DEPRECATED;

    void setFrameSizeMm( QSizeF size );
    QSizeF frameSizeMm() const { return mFrameSize; }
    void setContentsMargin( const QgsMargins &margins );
    QgsMargins contentsMargin() const { return mContentsMargins; }

    void setFillSymbol( QgsFillSymbol *symbol SIP_TRANSFER );
    QgsFillSymbol *fillSymbol() const { return mFillSymbol.get(); }
    void render( QgsRenderContext &context ) const;
    virtual void writeXml( QDomElement &elem, QDomDocument &doc, const QgsReadWriteContext &context ) const = 0;
    virtual void readXml( const QDomElement &itemElem, const QgsReadWriteContext &context ) = 0;

    void setMarkerSymbol(QgsMarkerSymbol *symbol SIP_TRANSFER);

    /*Returns the symbol that is drawn at the annotation's map position.\see setMarkerSymbol()  */
    QgsMarkerSymbol *markerSymbol() const { return mMarkerSymbol.get(); }
    QgsMapLayer *mapLayer() const { return mMapLayer.data(); }

    void setMapLayer( QgsMapLayer *layer );
    QgsFeature associatedFeature() const { return mFeature; }
    virtual void setAssociatedFeature( const QgsFeature &feature );
    virtual bool accept( QgsStyleEntityVisitorInterface *visitor ) const;

    void setTaskSymbol();
    void setRadarSymbol();
    void setAirSymbol();
    void setFillColor(const QColor &color); // change label background color
    QColor displayColor() const { return m_displayColor; }
  signals:
    //! Emitted whenever the annotation's appearance changes
    void appearanceChanged();    
    void moved();/* Emitted when the annotation's position has changed and items need to be moved to reflect this.  */
    void mapLayerChanged();
  protected:
    virtual void renderAnnotation( QgsRenderContext &context, QSizeF size ) const = 0;
    virtual QSizeF minimumFrameSize() const;
    void _writeXml(QDomElement &itemElem, QDomDocument &doc, const QgsReadWriteContext &context ) const;
    void _readXml(const QDomElement &annotationElem, const QgsReadWriteContext &context );
    void copyCommonProperties(QgsAnnotationA *target) const;
  private:
    //! Check where to attach the balloon connection between frame and map point
    void updateBalloon();

    //! Gets the frame line (0 is the top line, 1 right, 2 bottom, 3 left)
    QLineF segment(int index, QgsRenderContext *context ) const;

    //! Draws the annotation frame to a destination painter
    void drawFrame(QgsRenderContext &context ) const;

    //! Draws the map position marker symbol to a destination painter
    void drawMarkerSymbol( QgsRenderContext &context ) const;

    bool mVisible = true;

    //! True if the item stays at the same map position, FALSE if the item stays on same screen position
    bool mHasFixedMapPosition = true;

    //! Map position (for fixed position items)
    QgsPointXY mMapPosition;

    //! CRS of the map position
    QgsCoordinateReferenceSystem mMapPositionCrs;    
    QPointF mRelativePosition;//! Relative position (for non-fixed items) (0-1)
    //! Describes the shift of the item content box to the reference point in mm
    QPointF mOffsetFromReferencePoint = QPointF(3, -3); //�ı���ƫ��    
    QSizeF mFrameSize;//! Size of the frame in mm (without balloon)

    //! Point symbol that is to be drawn at the map reference location
    std::unique_ptr<QgsMarkerSymbol> mMarkerSymbol;
    QgsMargins mContentsMargins;    
    std::unique_ptr<QgsFillSymbol> mFillSymbol;//! Fill symbol used for drawing annotation

    //! Segment number where the connection to the map point is attached. -1 if no balloon needed (e.g. if point is contained in frame)
    int mBalloonSegment = -1;

    //! First segment point for drawing the connection (ccw direction) (always in mm)
    QPointF mBalloonSegmentPoint1;

    //! Second segment point for drawing the balloon connection (ccw direction) (always in mm)
    QPointF mBalloonSegmentPoint2;    
    QgsWeakMapLayerPointer mMapLayer;//! Associated layer (or NULLPTR if not attached to a layer)

    //! Associated feature, or invalid feature if no feature associated
    QgsFeature mFeature;
    //! Roughly 10 pixels at 96 dpi, to match earlier version rendering
    double mSegmentPointWidthMm = 2.64;
    //! Display color for custom rounded-rect rendering in renderAnnotation()
    QColor m_displayColor = QColor(255, 255, 255);
};

#endif // QGSANNOTATION_H

