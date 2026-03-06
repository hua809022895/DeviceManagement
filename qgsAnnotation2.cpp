#include "stdafx.h"
#include "qgsAnnotation2.h"
#include "qgssymbollayerutils.h"
#include <qgsMapCanvas.h>
#include "qgsmaplayer.h"
#include "qgsproject.h"
#include "qgsgeometryutils.h"
#include "qgsstyleentityvisitor.h"

#include <QPen>
#include <QPainter>

//Q_GUI_EXPORT 
extern int qt_defaultDpiX();

QgsAnnotation2::QgsAnnotation2(QObject *parent):QObject(parent), m_MarkerSymbol(new QgsMarkerSymbol())
{
	QgsStringMap props;
	props.insert(QStringLiteral("color"), QStringLiteral("white"));
	props.insert(QStringLiteral("style"), QStringLiteral("solid"));
	props.insert(QStringLiteral("style_border"), QStringLiteral("solid"));
	props.insert(QStringLiteral("color_border"), QStringLiteral("black"));
	props.insert(QStringLiteral("width_border"), QStringLiteral("0.3"));
	props.insert(QStringLiteral("joinstyle"), QStringLiteral("miter"));
	mFillSymbol.reset(QgsFillSymbol::createSimple(props));

	QColor color1(255, 0, 0, 0); //设置无人机轨迹线红色,透明
	mFillSymbol->setColor(color1);
	//mFillSymbol->setOpacity(0.5);//设置透明度

	m_MarkerSymbol->setColor(QColor(Qt::blue));
}

void QgsAnnotation2::setVisible(bool visible)
{
	if (mVisible == visible)
		return;
	mVisible = visible;
	emit appearanceChanged();
}

void QgsAnnotation2::setHasFixedMapPosition( bool fixed )
{
	if (mHasFixedMapPosition == fixed)
		return;

	mHasFixedMapPosition = fixed;
	updateBalloon();
	emit moved();
}

void QgsAnnotation2::setMapPositionCrs(const QgsCoordinateReferenceSystem &crs)
{
	mMapPositionCrs = crs;
	emit moved();
}

void QgsAnnotation2::setRelativePosition(QPointF position )
{
	mRelativePosition = position;
	emit moved();
}

void QgsAnnotation2::setFrameOffsetFromReferencePoint(QPointF offset )
{
	// convert from offset in pixels at 96 dpi to mm
	setFrameOffsetFromReferencePointMm(offset / 3.7795275);
}

QPointF QgsAnnotation2::frameOffsetFromReferencePoint() const
{
	return mOffsetFromReferencePoint / 3.7795275;
}

void QgsAnnotation2::setFrameOffsetFromReferencePointMm(QPointF offset)
{
	mOffsetFromReferencePoint = offset;
	updateBalloon();
	emit moved();
	emit appearanceChanged();
}

void QgsAnnotation2::setFrameSize(QSizeF size)
{  // convert from size in pixels at 96 dpi to mm
	setFrameSizeMm(size / 3.7795275);
}

QSizeF QgsAnnotation2::frameSize() const
{
	return mFrameSize / 3.7795275;
}

void QgsAnnotation2::setFrameSizeMm( QSizeF size )
{
	QSizeF frameSize = minimumFrameSize().expandedTo(size); //don't allow frame sizes below minimum
	mFrameSize = frameSize;
	updateBalloon();
	emit moved();
	emit appearanceChanged();
}

void QgsAnnotation2::setContentsMargin( const QgsMargins &margins )
{
	mContentsMargins = margins;
	emit appearanceChanged();
}

void QgsAnnotation2::setFillSymbol(QgsFillSymbol *symbol)
{
	mFillSymbol.reset(symbol);
	emit appearanceChanged();
}

void QgsAnnotation2::drawFrame(QgsRenderContext &context) const
{
	if (!mFillSymbol)
		return;

	QPolygonF			poly;
	QVector<QPolygonF>	rings; //empty list
	//QgsCoordinateReferenceSystem crs = mapPositionCrs();
	//QgsCoordinateTransform t(crs, m_pCanvas->mapSettings().destinationCrs(), QgsProject::instance());

	for (int i = 0; i < m_PtList.size(); ++i)
	{		
		QgsPointXY coord= m_PtList[i];
		qreal x = coord.x(), y = coord.y();
		m_pCanvas->getCoordinateTransform()->transformInPlace(x, y);
		poly << QPointF(x, y);		
	}
	
	mFillSymbol->startRender(context);
	mFillSymbol->renderPolygon(poly, &rings, nullptr, context);
	mFillSymbol->stopRender(context);
}

void QgsAnnotation2::drawMarkerSymbol(QgsRenderContext &context) const
{
	if (!context.painter())
		return;
	QgsCoordinateReferenceSystem crs = mapPositionCrs();
	QgsCoordinateTransform t(crs, m_pCanvas->mapSettings().destinationCrs(), QgsProject::instance());

	for (int i = 0; i < m_PtList.size(); ++i)
	{
		QgsPointXY coord = m_PtList[i];
		qreal x = coord.x(), y = coord.y();
		m_pCanvas->getCoordinateTransform()->transformInPlace(x, y);

		m_MarkerSymbol->startRender(context);
		m_MarkerSymbol->renderPoint(QPointF(x, y), nullptr, context);
		m_MarkerSymbol->stopRender(context);
	}
}

void QgsAnnotation2::render(QgsRenderContext &context) const
{
	QPainter *painter = context.painter();
	if (!painter)
		return;

	drawFrame(context);
	//drawMarkerSymbol(context);
	//m_MarkerSymbol->startRender(context);
	//m_MarkerSymbol->renderPoint(QPointF(0, 0), nullptr, context);
	//m_MarkerSymbol->stopRender(context);
	//painter->drawPoint(QPointF(x, y));

	//painter->drawLines(m_points);
	QSizeF size(context.convertToPainterUnits(mFrameSize.width(), QgsUnitTypes::RenderMillimeters) - context.convertToPainterUnits(mContentsMargins.left() + mContentsMargins.right(), QgsUnitTypes::RenderMillimeters),
		context.convertToPainterUnits(mFrameSize.height(), QgsUnitTypes::RenderMillimeters) - context.convertToPainterUnits(mContentsMargins.top() + mContentsMargins.bottom(), QgsUnitTypes::RenderMillimeters));

	renderAnnotation(context, size);
}

void QgsAnnotation2::setMapLayer(QgsMapLayer *layer)
{
	mMapLayer = layer;
	emit mapLayerChanged();
}

void QgsAnnotation2::setAssociatedFeature( const QgsFeature &feature)
{
	mFeature = feature;
}

bool QgsAnnotation2::accept(QgsStyleEntityVisitorInterface *visitor ) const
{
	// NOTE: if visitEnter returns false it means "don't visit the annotation", not "abort all further visitations"
	if (!visitor->visitEnter(QgsStyleEntityVisitorInterface::Node(QgsStyleEntityVisitorInterface::NodeType::Annotation, QStringLiteral("annotation"), tr("Annotation"))))
		return true;

	if (m_MarkerSymbol)
	{
		QgsStyleSymbolEntity entity(m_MarkerSymbol.get());
		if (!visitor->visit(QgsStyleEntityVisitorInterface::StyleLeaf(&entity, QStringLiteral("marker"), QObject::tr("Marker"))))
			return false;
	}

	if (mFillSymbol)
	{
		QgsStyleSymbolEntity entity(mFillSymbol.get());
		if (!visitor->visit(QgsStyleEntityVisitorInterface::StyleLeaf(&entity, QStringLiteral("fill"), QObject::tr("Fill"))))
			return false;
	}

	if (!visitor->visitExit(QgsStyleEntityVisitorInterface::Node(QgsStyleEntityVisitorInterface::NodeType::Annotation, QStringLiteral("annotation"), tr("Annotation"))))
		return false;

	return true;
}

QSizeF QgsAnnotation2::minimumFrameSize() const
{
  return QSizeF( 0, 0 );
}

void QgsAnnotation2::updateBalloon()
{
	//first test if the point is in the frame. In that case we don't need a balloon.
	if (!mHasFixedMapPosition ||
		(mOffsetFromReferencePoint.x() < 0 && (mOffsetFromReferencePoint.x() + mFrameSize.width()) > 0
			&& mOffsetFromReferencePoint.y() < 0 && (mOffsetFromReferencePoint.y() + mFrameSize.height()) > 0))
	{
		mBalloonSegment = -1;
		return;
	}

	//edge list
	QList<QLineF> segmentList;
	segmentList << segment(0, nullptr);
	segmentList << segment(1, nullptr);
	segmentList << segment(2, nullptr);
	segmentList << segment(3, nullptr);

	//find  closest edge / closest edge point
	double minEdgeDist = std::numeric_limits<double>::max();
	int minEdgeIndex = -1;
	QLineF minEdge;
	QgsPointXY minEdgePoint;
	QgsPointXY origin(0, 0);

	for (int i = 0; i < 4; ++i)
	{
		QLineF currentSegment = segmentList.at(i);
		QgsPointXY currentMinDistPoint;
		double currentMinDist = origin.sqrDistToSegment(currentSegment.x1(), currentSegment.y1(), currentSegment.x2(), currentSegment.y2(), currentMinDistPoint);
		bool isPreferredSegment = false;
		if (qgsDoubleNear(currentMinDist, minEdgeDist))
		{
			// two segments are close - work out which looks nicer
			const double angle = fmod(origin.azimuth(currentMinDistPoint) + 360.0, 360.0);
			if (angle < 45 || angle > 315)
				isPreferredSegment = i == 0;
			else if (angle < 135)
				isPreferredSegment = i == 3;
			else if (angle < 225)
				isPreferredSegment = i == 2;
			else
				isPreferredSegment = i == 1;
		}
		else if (currentMinDist < minEdgeDist)
			isPreferredSegment = true;

		if (isPreferredSegment)
		{
			minEdgeIndex = i;
			minEdgePoint = currentMinDistPoint;
			minEdgeDist = currentMinDist;
			minEdge = currentSegment;
		}
	}

	if (minEdgeIndex < 0)
		return;

	mBalloonSegment = minEdgeIndex;
	QPointF minEdgeEnd = minEdge.p2();
	mBalloonSegmentPoint1 = QPointF(minEdgePoint.x(), minEdgePoint.y());
	if (std::sqrt(minEdgePoint.sqrDist(minEdgeEnd.x(), minEdgeEnd.y())) < mSegmentPointWidthMm)
	{
		double x = 0;
		double y = 0;
		QgsGeometryUtils::pointOnLineWithDistance(minEdge.p2().x(), minEdge.p2().y(), minEdge.p1().x(), minEdge.p1().y(), mSegmentPointWidthMm, x, y);
		mBalloonSegmentPoint1 = QPointF(x, y);
	}

	{
		double x = 0;
		double y = 0;
		QgsGeometryUtils::pointOnLineWithDistance(mBalloonSegmentPoint1.x(), mBalloonSegmentPoint1.y(), minEdge.p2().x(), minEdge.p2().y(), mSegmentPointWidthMm, x, y);
		mBalloonSegmentPoint2 = QPointF(x, y);
	}
}

QLineF QgsAnnotation2::segment( int index, QgsRenderContext *context ) const
{
	auto scaleSize = [context](double size)->double
	{
		return context ? context->convertToPainterUnits(size, QgsUnitTypes::RenderMillimeters) : size;
	};

	switch (index)
	{
	case 0:
		return QLineF(scaleSize(mOffsetFromReferencePoint.x()),
			scaleSize(mOffsetFromReferencePoint.y()),
			scaleSize(mOffsetFromReferencePoint.x()) + scaleSize(mFrameSize.width()),
			scaleSize(mOffsetFromReferencePoint.y()));
	case 1:
		return QLineF(scaleSize(mOffsetFromReferencePoint.x()) + scaleSize(mFrameSize.width()),
			scaleSize(mOffsetFromReferencePoint.y()),
			scaleSize(mOffsetFromReferencePoint.x()) + scaleSize(mFrameSize.width()),
			scaleSize(mOffsetFromReferencePoint.y()) + scaleSize(mFrameSize.height()));
	case 2:
		return QLineF(scaleSize(mOffsetFromReferencePoint.x()) + scaleSize(mFrameSize.width()),
			scaleSize(mOffsetFromReferencePoint.y()) + scaleSize(mFrameSize.height()),
			scaleSize(mOffsetFromReferencePoint.x()),
			scaleSize(mOffsetFromReferencePoint.y()) + scaleSize(mFrameSize.height()));
	case 3:
		return QLineF(scaleSize(mOffsetFromReferencePoint.x()),
			scaleSize(mOffsetFromReferencePoint.y()) + scaleSize(mFrameSize.height()),
			scaleSize(mOffsetFromReferencePoint.x()),
			scaleSize(mOffsetFromReferencePoint.y()));
	default:
		return QLineF();
	}
}

void QgsAnnotation2::_writeXml( QDomElement &itemElem, QDomDocument &doc, const QgsReadWriteContext &context ) const
{
	if (itemElem.isNull())
	{
		return;
	}

	QDomElement annotationElem = doc.createElement(QStringLiteral("AnnotationItem"));
	annotationElem.setAttribute(QStringLiteral("mapPositionFixed"), mHasFixedMapPosition);
	//annotationElem.setAttribute( QStringLiteral( "mapPosX" ), qgsDoubleToString(m_Position.x() ) );
	//annotationElem.setAttribute( QStringLiteral( "mapPosY" ), qgsDoubleToString(m_Position.y() ) );
	if (mMapPositionCrs.isValid())
		mMapPositionCrs.writeXml(annotationElem, doc);
	annotationElem.setAttribute(QStringLiteral("offsetXMM"), qgsDoubleToString(mOffsetFromReferencePoint.x()));
	annotationElem.setAttribute(QStringLiteral("offsetYMM"), qgsDoubleToString(mOffsetFromReferencePoint.y()));
	annotationElem.setAttribute(QStringLiteral("frameWidthMM"), qgsDoubleToString(mFrameSize.width()));
	annotationElem.setAttribute(QStringLiteral("frameHeightMM"), qgsDoubleToString(mFrameSize.height()));
	annotationElem.setAttribute(QStringLiteral("canvasPosX"), qgsDoubleToString(mRelativePosition.x()));
	annotationElem.setAttribute(QStringLiteral("canvasPosY"), qgsDoubleToString(mRelativePosition.y()));
	annotationElem.setAttribute(QStringLiteral("contentsMargin"), mContentsMargins.toString());
	annotationElem.setAttribute(QStringLiteral("visible"), isVisible());
	if (mMapLayer)
	{
		annotationElem.setAttribute(QStringLiteral("mapLayer"), mMapLayer->id());
	}

	if (m_MarkerSymbol)
	{
		QDomElement symbolElem = QgsSymbolLayerUtils::saveSymbol(QStringLiteral("marker symbol"), m_MarkerSymbol.get(), doc, context);
		if (!symbolElem.isNull())
		{
			annotationElem.appendChild(symbolElem);
		}
	}
	if (mFillSymbol)
	{
		QDomElement fillElem = doc.createElement(QStringLiteral("fillSymbol"));
		QDomElement symbolElem = QgsSymbolLayerUtils::saveSymbol(QStringLiteral("fill symbol"), mFillSymbol.get(), doc, context);
		if (!symbolElem.isNull())
		{
			fillElem.appendChild(symbolElem);
			annotationElem.appendChild(fillElem);
		}
	}
	itemElem.appendChild(annotationElem);
}

void QgsAnnotation2::_readXml(const QDomElement &annotationElem, const QgsReadWriteContext &context )
{
	if (annotationElem.isNull())
	{
		return;
	}
	QPointF pos;
	pos.setX(annotationElem.attribute(QStringLiteral("canvasPosX"), QStringLiteral("0")).toDouble());
	pos.setY(annotationElem.attribute(QStringLiteral("canvasPosY"), QStringLiteral("0")).toDouble());
	if (pos.x() >= 1 || pos.x() < 0 || pos.y() < 0 || pos.y() >= 1)
		mRelativePosition = QPointF();
	else
		mRelativePosition = pos;
	QgsPointXY mapPos;
	mapPos.setX(annotationElem.attribute(QStringLiteral("mapPosX"), QStringLiteral("0")).toDouble());
	mapPos.setY(annotationElem.attribute(QStringLiteral("mapPosY"), QStringLiteral("0")).toDouble());
	//m_Position = mapPos;

	if (!mMapPositionCrs.readXml(annotationElem))
	{
		mMapPositionCrs = QgsCoordinateReferenceSystem();
	}

	mContentsMargins = QgsMargins::fromString(annotationElem.attribute(QStringLiteral("contentsMargin")));
	const double dpiScale = 25.4 / qt_defaultDpiX();
	if (annotationElem.hasAttribute(QStringLiteral("frameWidthMM")))
		mFrameSize.setWidth(annotationElem.attribute(QStringLiteral("frameWidthMM"), QStringLiteral("5")).toDouble());
	else
		mFrameSize.setWidth(dpiScale * annotationElem.attribute(QStringLiteral("frameWidth"), QStringLiteral("50")).toDouble());
	if (annotationElem.hasAttribute(QStringLiteral("frameHeightMM")))
		mFrameSize.setHeight(annotationElem.attribute(QStringLiteral("frameHeightMM"), QStringLiteral("3")).toDouble());
	else
		mFrameSize.setHeight(dpiScale * annotationElem.attribute(QStringLiteral("frameHeight"), QStringLiteral("50")).toDouble());

	if (annotationElem.hasAttribute(QStringLiteral("offsetXMM")))
		mOffsetFromReferencePoint.setX(annotationElem.attribute(QStringLiteral("offsetXMM"), QStringLiteral("0")).toDouble());
	else
		mOffsetFromReferencePoint.setX(dpiScale * annotationElem.attribute(QStringLiteral("offsetX"), QStringLiteral("0")).toDouble());
	if (annotationElem.hasAttribute(QStringLiteral("offsetYMM")))
		mOffsetFromReferencePoint.setY(annotationElem.attribute(QStringLiteral("offsetYMM"), QStringLiteral("0")).toDouble());
	else
		mOffsetFromReferencePoint.setY(dpiScale * annotationElem.attribute(QStringLiteral("offsetY"), QStringLiteral("0")).toDouble());

	mHasFixedMapPosition = annotationElem.attribute(QStringLiteral("mapPositionFixed"), QStringLiteral("1")).toInt();
	mVisible = annotationElem.attribute(QStringLiteral("visible"), QStringLiteral("1")).toInt();
	if (annotationElem.hasAttribute(QStringLiteral("mapLayer")))
	{
		mMapLayer = QgsProject::instance()->mapLayer(annotationElem.attribute(QStringLiteral("mapLayer")));
	}

	//marker symbol
	QDomElement symbolElem = annotationElem.firstChildElement(QStringLiteral("symbol"));
	if (!symbolElem.isNull())
	{
		QgsMarkerSymbol *symbol = QgsSymbolLayerUtils::loadSymbol<QgsMarkerSymbol>(symbolElem, context);
		if (symbol)
		{
			m_MarkerSymbol.reset(symbol);
		}
	}
	mFillSymbol.reset(nullptr);
	QDomElement fillElem = annotationElem.firstChildElement(QStringLiteral("fillSymbol"));
	if (!fillElem.isNull())
	{
		QDomElement symbolElem = fillElem.firstChildElement(QStringLiteral("symbol"));
		if (!symbolElem.isNull())
		{
			QgsFillSymbol *symbol = QgsSymbolLayerUtils::loadSymbol<QgsFillSymbol>(symbolElem, context);
			if (symbol)
			{
				mFillSymbol.reset(symbol);
			}
		}
	}

	if (!mFillSymbol)
	{
		QColor frameColor;
		frameColor.setNamedColor(annotationElem.attribute(QStringLiteral("frameColor"), QStringLiteral("#000000")));
		frameColor.setAlpha(annotationElem.attribute(QStringLiteral("frameColorAlpha"), QStringLiteral("255")).toInt());
		
		QColor frameBackgroundColor;
		frameBackgroundColor.setNamedColor(annotationElem.attribute(QStringLiteral("frameBackgroundColor")));
		frameBackgroundColor.setAlpha(annotationElem.attribute(QStringLiteral("frameBackgroundColorAlpha"), QStringLiteral("255")).toInt());
		double frameBorderWidth = annotationElem.attribute(QStringLiteral("frameBorderWidth"), QStringLiteral("0.5")).toDouble();
		// need to roughly convert border width from pixels to mm - just assume 96 dpi
		frameBorderWidth = frameBorderWidth * 25.4 / 96.0;

		QgsStringMap props;
		props.insert(QStringLiteral("color"), frameBackgroundColor.name());
		props.insert(QStringLiteral("style"), QStringLiteral("solid"));
		props.insert(QStringLiteral("style_border"), QStringLiteral("solid"));
		props.insert(QStringLiteral("color_border"), frameColor.name());
		props.insert(QStringLiteral("width_border"), QString::number(frameBorderWidth));
		props.insert(QStringLiteral("joinstyle"), QStringLiteral("miter"));
		mFillSymbol.reset(QgsFillSymbol::createSimple(props));
	}

	updateBalloon();
	emit mapLayerChanged();
}

void QgsAnnotation2::copyCommonProperties(QgsAnnotation2 *target) const
{
	target->mVisible = mVisible;
	target->mHasFixedMapPosition = mHasFixedMapPosition;
	//target->m_Position			= m_Position;
	target->mMapPositionCrs		= mMapPositionCrs;
	target->mRelativePosition	= mRelativePosition;
	target->mOffsetFromReferencePoint = mOffsetFromReferencePoint;
	target->mFrameSize			= mFrameSize;
	target->m_MarkerSymbol.reset(m_MarkerSymbol ? m_MarkerSymbol->clone() : nullptr);
	target->mContentsMargins	= mContentsMargins;
	target->mFillSymbol.reset(mFillSymbol ? mFillSymbol->clone() : nullptr);
	target->mBalloonSegment = mBalloonSegment;
	target->mBalloonSegmentPoint1 = mBalloonSegmentPoint1;
	target->mBalloonSegmentPoint2 = mBalloonSegmentPoint2;
	target->mSegmentPointWidthMm = mSegmentPointWidthMm;
	target->mMapLayer = mMapLayer;
	target->mFeature = mFeature;
}

