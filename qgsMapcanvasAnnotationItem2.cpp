/***************************************************************************
 ***************************************************************************/
#include "stdafx.h"
#include "qgsmapCanvasAnnotationItem2.h"
#include "qgsAnnotation.h"
#include "qgsAnnotation2.h"
#include "qgsmapcanvas.h"
#include "qgsmaptool.h"
#include "qgsvectorlayer.h"
#include "qgsfeatureiterator.h"
#include "qgsexception.h"
#include "qgssymbollayerutils.h"
#include "qgsproject.h"
#include "qgsannotationmanager.h"
#include "qgssvgannotation.h"
#include "jpgAnnotation.h"
#include <QPainter>
#include <QDebug>

//无人机轨迹线其中的点对象类
QgsMapCanvasAnnotationItem2::QgsMapCanvasAnnotationItem2(QgsAnnotation2 *annotation, QgsMapCanvas *mapCanvas)
  :mAnnotation(annotation),QgsMapCanvasItem(mapCanvas)
{
	setFlag(QGraphicsItem::ItemIsSelectable, true);
	if (mapCanvas && !mapCanvas->annotationsVisible())
		setVisible(false);

	connect(mAnnotation, &QgsAnnotation2::appearanceChanged, this, [this] { update(); });
	connect(mAnnotation, &QgsAnnotation2::moved, this, [this] { updatePosition(); });
	connect(mAnnotation, &QgsAnnotation2::moved, this, &QgsMapCanvasAnnotationItem2::setFeatureForMapPosition);
	connect(mMapCanvas, &QgsMapCanvas::destinationCrsChanged, this, [=] { updatePosition(); });
	connect(mAnnotation, &QgsAnnotation2::appearanceChanged, this, &QgsMapCanvasAnnotationItem2::updateBoundingRect);
	connect(mMapCanvas, &QgsMapCanvas::layersChanged, this, &QgsMapCanvasAnnotationItem2::onCanvasLayersChanged);
	connect(mAnnotation, &QgsAnnotation2::mapLayerChanged, this, &QgsMapCanvasAnnotationItem2::onCanvasLayersChanged);

	//lifetime is tied to annotation!
	connect(mAnnotation, &QgsAnnotation::destroyed, this, &QgsMapCanvasAnnotationItem2::annotationDeleted);

	updatePosition();
	setFeatureForMapPosition();
}

void QgsMapCanvasAnnotationItem2::updatePosition()
{
	if (!mAnnotation)
		return;

	QgsCoordinateTransform t(mAnnotation->mapPositionCrs(), mMapCanvas->mapSettings().destinationCrs(), QgsProject::instance());

	QgsPointXY coord;// = mAnnotation->m_Position;
	try
	{
		coord = t.transform(coord);
	}
	catch (QgsCsException &)
	{
	}
	QPointF qfp = toCanvasCoordinates(coord);
	//setPos(qfp);
}

QRectF QgsMapCanvasAnnotationItem2::boundingRect() const
{
	return mBoundingRect;
}

void QgsMapCanvasAnnotationItem2::updateBoundingRect()
{
	prepareGeometryChange();
}

void QgsMapCanvasAnnotationItem2::onCanvasLayersChanged()
{
	if (!mAnnotation)
		return;
}

void QgsMapCanvasAnnotationItem2::setFeatureForMapPosition()
{
	if (!mAnnotation || !mAnnotation->hasFixedMapPosition())
		return;

	QgsVectorLayer *vectorLayer = qobject_cast<QgsVectorLayer *>(mAnnotation->mapLayer());
	if (!vectorLayer)
		return;

	double halfIdentifyWidth = QgsMapTool::searchRadiusMU(mMapCanvas);
	
	QgsPointXY mapPosition;
	try
	{
		QgsCoordinateTransform ct(mAnnotation->mapPositionCrs(), mMapCanvas->mapSettings().destinationCrs(), QgsProject::instance());
		if (ct.isValid())
			mapPosition = ct.transform(mapPosition);
	}
	catch (QgsCsException &)
	{
	}
}

void QgsMapCanvasAnnotationItem2::annotationDeleted()
{
	mAnnotation = nullptr;
	deleteLater();
}

void QgsMapCanvasAnnotationItem2::drawSelectionBoxes(QPainter *p ) const
{
	if (!p)
		return;

	double handlerSize = 10;
	p->setPen(Qt::NoPen);
	p->setBrush(QColor(200, 200, 210, 120));
	p->drawRect(QRectF(mBoundingRect.left(), mBoundingRect.top(), handlerSize, handlerSize));
	p->drawRect(QRectF(mBoundingRect.right() - handlerSize, mBoundingRect.top(), handlerSize, handlerSize));
	p->drawRect(QRectF(mBoundingRect.right() - handlerSize, mBoundingRect.bottom() - handlerSize, handlerSize, handlerSize));
	p->drawRect(QRectF(mBoundingRect.left(), mBoundingRect.bottom() - handlerSize, handlerSize, handlerSize));
}

QgsMapCanvasAnnotationItem2::MouseMoveAction QgsMapCanvasAnnotationItem2::moveActionForPosition(QPointF pos ) const
{
	QPointF itemPos = mapFromScene(pos);

	int cursorSensitivity = 7;

	if (mAnnotation && mAnnotation->hasFixedMapPosition() &&
		std::fabs(itemPos.x()) < cursorSensitivity && std::fabs(itemPos.y()) < cursorSensitivity) //move map point if position is close to the origin
	{
		return MoveMapPosition;
	}

	const double mmToPixelScale = mMapCanvas->logicalDpiX() / 25.4;

	QPointF offset = mAnnotation && mAnnotation->hasFixedMapPosition() ? mAnnotation->frameOffsetFromReferencePointMm() * mmToPixelScale : QPointF(0, 0);
	QSizeF frameSize = mAnnotation ? mAnnotation->frameSizeMm() * mmToPixelScale : QSizeF(0, 0);

	bool left, right, up, down;
	left = std::fabs(itemPos.x() - offset.x()) < cursorSensitivity;
	right = std::fabs(itemPos.x() - (offset.x() + frameSize.width())) < cursorSensitivity;
	up = std::fabs(itemPos.y() - offset.y()) < cursorSensitivity;
	down = std::fabs(itemPos.y() - (offset.y() + frameSize.height())) < cursorSensitivity;

	if (left && up)
	{
		return ResizeFrameLeftUp;
	}
	else if (right && up)
	{
		return ResizeFrameRightUp;
	}
	else if (left && down)
	{
		return ResizeFrameLeftDown;
	}
	else if (right && down)
	{
		return ResizeFrameRightDown;
	}
	if (left)
	{
		return ResizeFrameLeft;
	}
	if (right)
	{
		return ResizeFrameRight;
	}
	if (up)
	{
		return ResizeFrameUp;
	}
	if (down)
	{
		return ResizeFrameDown;
	}

	//finally test if pos is in the frame area
	if (itemPos.x() >= offset.x() && itemPos.x() <= (offset.x() + frameSize.width())
		&& itemPos.y() >= offset.y() && itemPos.y() <= (offset.y() + frameSize.height()))
	{
		return MoveFramePosition;
	}
	return NoAction;
}

Qt::CursorShape QgsMapCanvasAnnotationItem2::cursorShapeForAction( MouseMoveAction moveAction ) const
{
	switch (moveAction)
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

double QgsMapCanvasAnnotationItem2::scaledSymbolSize() const
{
	if (!mAnnotation || !mAnnotation->markerSymbol())
		return 0.0;

	if (!mMapCanvas)
		return mAnnotation->markerSymbol()->size();

	double dpmm = mMapCanvas->logicalDpiX() / 25.4;
	return dpmm * mAnnotation->markerSymbol()->size();
}

void QgsMapCanvasAnnotationItem2::paint(QPainter *painter)
{
	if (!mAnnotation || !mAnnotation->isVisible())
		return;

	m_painter = painter;
	QgsRenderContext rc = QgsRenderContext::fromQPainter(painter);
	if (mAnnotation)
		mAnnotation->render(rc);
}

