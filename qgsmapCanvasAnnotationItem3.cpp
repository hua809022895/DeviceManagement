#include "stdafx.h"
#include "qgsmapcanvasannotationItem3.h"
#include "qgsAirAnnotation.h"
#include "qgsmapcanvas.h"
#include "qgsmaptool.h"
#include "qgsvectorlayer.h"
#include "qgsfeatureiterator.h"
#include "qgsexception.h"
#include "qgssymbollayerutils.h"
#include "qgsproject.h"
#include "qgsannotationmanager.h"
#include <QPainter>


QgsMapCanvasAnnotationItem3::QgsMapCanvasAnnotationItem3(QgsAirAnnotation *annotation, QgsMapCanvas *mapCanvas)
		:QgsMapCanvasItem( mapCanvas), mAnnotation( annotation)
{
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	if (mapCanvas && !mapCanvas->annotationsVisible())
		setVisible(false);

	connect(mAnnotation, &QgsAirAnnotation::appearanceChanged, this, [this] { update(); });
	connect(mAnnotation, &QgsAirAnnotation::moved, this, [this] { updatePosition(); });
	connect(mAnnotation, &QgsAirAnnotation::moved, this, &QgsMapCanvasAnnotationItem3::setFeatureForMapPosition);
	connect(mMapCanvas,	 &QgsMapCanvas::destinationCrsChanged, this, [=] { updatePosition(); });
	connect(mAnnotation, &QgsAirAnnotation::appearanceChanged, this, &QgsMapCanvasAnnotationItem3::updateBoundingRect);
	connect(mMapCanvas,  &QgsMapCanvas::layersChanged, this, &QgsMapCanvasAnnotationItem3::onCanvasLayersChanged);
	connect(mAnnotation, &QgsAirAnnotation::mapLayerChanged, this, &QgsMapCanvasAnnotationItem3::onCanvasLayersChanged);

	//lifetime is tied to annotation!
	connect(mAnnotation, &QgsAirAnnotation::destroyed, this, &QgsMapCanvasAnnotationItem3::annotationDeleted);

	updatePosition();
	setFeatureForMapPosition();
}

void QgsMapCanvasAnnotationItem3::updatePosition()
{
	if (!mAnnotation)
		return;

	if (mAnnotation->hasFixedMapPosition())
	{
		QgsCoordinateTransform t(mAnnotation->mapPositionCrs(), mMapCanvas->mapSettings().destinationCrs(), QgsProject::instance());
		QgsPointXY coord = mAnnotation->mapPosition();
		try
		{
			coord = t.transform(coord);
		}
		catch (QgsCsException &)
		{
		}
		setPos(toCanvasCoordinates(coord));
	}
	else
	{
		//relative position

		double x = mAnnotation->relativePosition().x() * mMapCanvas->width();
		double y = mAnnotation->relativePosition().y() * mMapCanvas->height();
		setPos(x, y);
	}
	updateBoundingRect();
}

QRectF QgsMapCanvasAnnotationItem3::boundingRect() const
{
  return mBoundingRect;
}

void QgsMapCanvasAnnotationItem3::updateBoundingRect()
{
  prepareGeometryChange();

  QgsRenderContext rc = QgsRenderContext::fromQPainter( nullptr );
  double fillSymbolBleed = mAnnotation && mAnnotation->fillSymbol() ?
                           QgsSymbolLayerUtils::estimateMaxSymbolBleed( mAnnotation->fillSymbol(), rc ) : 0;

  const double mmToPixelScale = mMapCanvas->logicalDpiX() / 25.4;

  if ( mAnnotation && !mAnnotation->hasFixedMapPosition() )
  {
    mBoundingRect = QRectF( - fillSymbolBleed, -fillSymbolBleed,
                            mmToPixelScale * mAnnotation->frameSizeMm().width() + fillSymbolBleed * 2,
                            mmToPixelScale * mAnnotation->frameSizeMm().height() + fillSymbolBleed * 2 );
  }
  else
  {
    double halfSymbolSize = 0.0;
    if ( mAnnotation && mAnnotation->markerSymbol() )
    {
      halfSymbolSize = scaledSymbolSize() / 2.0;
    }

    QPointF offset = mAnnotation ? QPointF( mAnnotation->frameOffsetFromReferencePointMm().x() * mmToPixelScale,
                                            mAnnotation->frameOffsetFromReferencePointMm().y() * mmToPixelScale ) : QPointF( 0, 0 );

    QSizeF frameSize = mAnnotation ? QSizeF( mAnnotation->frameSizeMm().width() * mmToPixelScale,
                       mAnnotation->frameSizeMm().height() * mmToPixelScale ) : QSizeF( 0.0, 0.0 );

    double xMinPos = std::min( -halfSymbolSize, offset.x() - fillSymbolBleed );
    double xMaxPos = std::max( halfSymbolSize, offset.x() + frameSize.width() + fillSymbolBleed );
    double yMinPos = std::min( -halfSymbolSize, offset.y() - fillSymbolBleed );
    double yMaxPos = std::max( halfSymbolSize, offset.y() + frameSize.height() + fillSymbolBleed );
    mBoundingRect = QRectF( xMinPos, yMinPos, xMaxPos - xMinPos, yMaxPos - yMinPos );
  }
}

void QgsMapCanvasAnnotationItem3::onCanvasLayersChanged()
{
  if ( !mAnnotation )
    return;
  if ( !mMapCanvas->annotationsVisible() )
  {
    setVisible( false );
  }
  else if ( !mAnnotation->mapLayer() )
  {
    setVisible( true );
  }
  else
  {
    setVisible( mMapCanvas->mapSettings().layers().contains( mAnnotation->mapLayer() ) );
  }
}

void QgsMapCanvasAnnotationItem3::setFeatureForMapPosition()
{
  if ( !mAnnotation || !mAnnotation->hasFixedMapPosition() )
    return;

  QgsVectorLayer *vectorLayer = qobject_cast< QgsVectorLayer * >( mAnnotation->mapLayer() );
  if ( !vectorLayer )
    return;

  double halfIdentifyWidth = QgsMapTool::searchRadiusMU( mMapCanvas );
  QgsPointXY mapPosition = mAnnotation->mapPosition();

  try
  {
    QgsCoordinateTransform ct( mAnnotation->mapPositionCrs(), mMapCanvas->mapSettings().destinationCrs(), QgsProject::instance() );
    if ( ct.isValid() )
      mapPosition = ct.transform( mapPosition );
  }
  catch ( QgsCsException & )
  {
  }

  QgsRectangle searchRect( mapPosition.x() - halfIdentifyWidth, mapPosition.y() - halfIdentifyWidth,
                           mapPosition.x() + halfIdentifyWidth, mapPosition.y() + halfIdentifyWidth );

  searchRect = mMapCanvas->mapSettings().mapToLayerCoordinates( vectorLayer, searchRect );

  QgsFeatureIterator fit = vectorLayer->getFeatures( QgsFeatureRequest().setFilterRect( searchRect ).setFlags( QgsFeatureRequest::ExactIntersect ).setLimit( 1 ) );

  QgsFeature currentFeature;
  ( void )fit.nextFeature( currentFeature );
  mAnnotation->setAssociatedFeature( currentFeature );
}

void QgsMapCanvasAnnotationItem3::annotationDeleted()
{
  mAnnotation = nullptr;
  deleteLater();
}

void QgsMapCanvasAnnotationItem3::drawSelectionBoxes( QPainter *p ) const
{
  if ( !p )
  {
    return;
  }

  double handlerSize = 10;
  p->setPen( Qt::NoPen );
  p->setBrush( QColor( 200, 200, 210, 120 ) );
  p->drawRect( QRectF( mBoundingRect.left(), mBoundingRect.top(), handlerSize, handlerSize ) );
  p->drawRect( QRectF( mBoundingRect.right() - handlerSize, mBoundingRect.top(), handlerSize, handlerSize ) );
  p->drawRect( QRectF( mBoundingRect.right() - handlerSize, mBoundingRect.bottom() - handlerSize, handlerSize, handlerSize ) );
  p->drawRect( QRectF( mBoundingRect.left(), mBoundingRect.bottom() - handlerSize, handlerSize, handlerSize ) );
}

QgsMapCanvasAnnotationItem3::MouseMoveAction QgsMapCanvasAnnotationItem3::moveActionForPosition( QPointF pos ) const
{
  QPointF itemPos = mapFromScene( pos );

  int cursorSensitivity = 7;

  if ( mAnnotation && mAnnotation->hasFixedMapPosition() &&
       std::fabs( itemPos.x() ) < cursorSensitivity && std::fabs( itemPos.y() ) < cursorSensitivity ) //move map point if position is close to the origin
  {
    return MoveMapPosition;
  }

  const double mmToPixelScale = mMapCanvas->logicalDpiX() / 25.4;

  QPointF offset = mAnnotation && mAnnotation->hasFixedMapPosition() ? mAnnotation->frameOffsetFromReferencePointMm() * mmToPixelScale : QPointF( 0, 0 );
  QSizeF frameSize = mAnnotation ? mAnnotation->frameSizeMm() * mmToPixelScale : QSizeF( 0, 0 );

  bool left, right, up, down;
  left = std::fabs( itemPos.x() - offset.x() ) < cursorSensitivity;
  right = std::fabs( itemPos.x() - ( offset.x() + frameSize.width() ) ) < cursorSensitivity;
  up = std::fabs( itemPos.y() - offset.y() ) < cursorSensitivity;
  down = std::fabs( itemPos.y() - ( offset.y() + frameSize.height() ) ) < cursorSensitivity;

  if ( left && up )
  {
    return ResizeFrameLeftUp;
  }
  else if ( right && up )
  {
    return ResizeFrameRightUp;
  }
  else if ( left && down )
  {
    return ResizeFrameLeftDown;
  }
  else if ( right && down )
  {
    return ResizeFrameRightDown;
  }
  if ( left )
  {
    return ResizeFrameLeft;
  }
  if ( right )
  {
    return ResizeFrameRight;
  }
  if ( up )
  {
    return ResizeFrameUp;
  }
  if ( down )
  {
    return ResizeFrameDown;
  }

  //finally test if pos is in the frame area
  if ( itemPos.x() >= offset.x() && itemPos.x() <= ( offset.x() + frameSize.width() )
       && itemPos.y() >= offset.y() && itemPos.y() <= ( offset.y() + frameSize.height() ) )
  {
    return MoveFramePosition;
  }
  return NoAction;
}

Qt::CursorShape QgsMapCanvasAnnotationItem3::cursorShapeForAction( MouseMoveAction moveAction ) const
{
  switch ( moveAction )
  {
    case NoAction:
      return Qt::ArrowCursor;
    case MoveMapPosition:
    case MoveFramePosition:
      return Qt::SizeAllCursor;
    case ResizeFrameUp:
    case ResizeFrameDown:
      return Qt::SizeVerCursor;
    case ResizeFrameLeft:
    case ResizeFrameRight:
      return Qt::SizeHorCursor;
    case ResizeFrameLeftUp:
    case ResizeFrameRightDown:
      return Qt::SizeFDiagCursor;
    case ResizeFrameRightUp:
    case ResizeFrameLeftDown:
      return Qt::SizeBDiagCursor;
    default:
      return Qt::ArrowCursor;
  }
}

double QgsMapCanvasAnnotationItem3::scaledSymbolSize() const
{
  if ( !mAnnotation || !mAnnotation->markerSymbol() )
  {
    return 0.0;
  }

  if ( !mMapCanvas )
  {
    return mAnnotation->markerSymbol()->size();
  }

  double dpmm = mMapCanvas->logicalDpiX() / 25.4;
  return dpmm * mAnnotation->markerSymbol()->size();
}

void QgsMapCanvasAnnotationItem3::paint( QPainter *painter )
{
  if ( !mAnnotation || !mAnnotation->isVisible() )
    return;

  QgsRenderContext rc = QgsRenderContext::fromQPainter( painter );
  rc.setFlag( QgsRenderContext::Antialiasing, true );

  if ( mAnnotation )
    mAnnotation->render( rc );

  if ( isSelected() )
  {
    drawSelectionBoxes( painter );
  }
}
