#include "stdafx.h"
#include "qgsAnnotationA.h"
#include "qgssymbollayerutils.h"
#include "qgsmaplayer.h"
#include "qgsproject.h"
#include "qgsgeometryutils.h"
#include "qgsstyleentityvisitor.h"

#include <QPen>
#include <QPainter>
#include <QLinearGradient>

QgsAnnotationA::QgsAnnotationA(QObject *parent):QObject(parent), mMarkerSymbol(new QgsMarkerSymbol())
{
}

void QgsAnnotationA::setAirSymbol()
{
	// mFillSymbol fully transparent: QGIS drawFrame draws nothing; we render in renderAnnotation()
	QgsStringMap props;
	props.insert(QStringLiteral("color"), QStringLiteral("0,0,0,0"));
	props.insert(QStringLiteral("style"), QStringLiteral("solid"));
	props.insert(QStringLiteral("style_border"), QStringLiteral("no"));
	mFillSymbol.reset(QgsFillSymbol::createSimple(props));
	mMarkerSymbol.reset(nullptr);
	mOffsetFromReferencePoint = QPointF(8, -4);
	m_displayColor = QColor(255, 255, 200); // light yellow
}

void QgsAnnotationA::setTaskSymbol()
{
	// mFillSymbol fully transparent: QGIS drawFrame draws nothing; we render in renderAnnotation()
	QgsStringMap props;
	props.insert(QStringLiteral("color"), QStringLiteral("0,0,0,0"));
	props.insert(QStringLiteral("style"), QStringLiteral("solid"));
	props.insert(QStringLiteral("style_border"), QStringLiteral("no"));
	mFillSymbol.reset(QgsFillSymbol::createSimple(props));
	mMarkerSymbol.reset(nullptr);
	mOffsetFromReferencePoint = QPointF(5, -4);
	m_displayColor = QColor(255, 220, 80); // golden yellow for task areas
}

void QgsAnnotationA::setRadarSymbol()
{
	// mFillSymbol fully transparent: QGIS drawFrame draws nothing; we render in renderAnnotation()
	QgsStringMap props;
	props.insert(QStringLiteral("color"), QStringLiteral("0,0,0,0"));
	props.insert(QStringLiteral("style"), QStringLiteral("solid"));
	props.insert(QStringLiteral("style_border"), QStringLiteral("no"));
	mFillSymbol.reset(QgsFillSymbol::createSimple(props));
	mMarkerSymbol.reset(nullptr);
	mOffsetFromReferencePoint = QPointF(5, -4);
	m_displayColor = QColor(255, 255, 255); // white (overridden by setFillColor per type)
}

void QgsAnnotationA::setFillColor(const QColor &color)
{
	m_displayColor = color;
	emit appearanceChanged();
}

void QgsAnnotationA::setVisible(bool visible)
{
	if (mVisible == visible)
		return;

	mVisible = visible;
	emit appearanceChanged();
}

void QgsAnnotationA::setHasFixedMapPosition( bool fixed )
{
	if (mHasFixedMapPosition == fixed)
		return;

	mHasFixedMapPosition = fixed;
	updateBalloon();
	emit moved();
}

void QgsAnnotationA::setMapPosition( const QgsPointXY &position )
{
	mMapPosition = position;
	emit moved();
}

void QgsAnnotationA::setMapPositionCrs(const QgsCoordinateReferenceSystem &crs )
{
	mMapPositionCrs = crs;
	emit moved();
}

void QgsAnnotationA::setRelativePosition( QPointF position )
{
	mRelativePosition = position;
	emit moved();
}

void QgsAnnotationA::setFrameOffsetFromReferencePoint( QPointF offset )
{
    // convert from offset in pixels at 96 dpi to mm
    setFrameOffsetFromReferencePointMm( offset / 3.7795275);
}

QPointF QgsAnnotationA::frameOffsetFromReferencePoint() const
{
	return mOffsetFromReferencePoint / 3.7795275;
}

void QgsAnnotationA::setFrameOffsetFromReferencePointMm( QPointF offset )
{
	mOffsetFromReferencePoint = offset;

	updateBalloon();
	emit moved();
	emit appearanceChanged();
}

void QgsAnnotationA::setFrameSize(QSizeF size)
{
	// convert from size in pixels at 96 dpi to mm
	setFrameSizeMm(size / 3.7795275);
}

QSizeF QgsAnnotationA::frameSize() const
{
	return mFrameSize / 3.7795275;
}

// set frame size in mm
void QgsAnnotationA::setFrameSizeMm(QSizeF size)
{
	QSizeF frameSize = minimumFrameSize().expandedTo(size); //don't allow frame sizes below minimum
	mFrameSize = frameSize;
	updateBalloon();
	emit moved();
	emit appearanceChanged();
}

void QgsAnnotationA::setContentsMargin( const QgsMargins &margins )
{
	mContentsMargins = margins;
	emit appearanceChanged();
}

void QgsAnnotationA::setFillSymbol( QgsFillSymbol *symbol )
{
	mFillSymbol.reset(symbol);
	emit appearanceChanged();
}

void QgsAnnotationA::render(QgsRenderContext &context ) const
{
	QPainter* painter = context.painter();
	if (!painter)
		return;

	QgsScopedQPainterState painterState(context.painter());
	context.setPainterFlagsUsingContext();

	drawFrame(context);
	if (mHasFixedMapPosition)
	{
		drawMarkerSymbol(context);
	}

	if (mHasFixedMapPosition)
	{
		painter->translate(context.convertToPainterUnits(mOffsetFromReferencePoint.x(), QgsUnitTypes::RenderMillimeters) + context.convertToPainterUnits(mContentsMargins.left(), QgsUnitTypes::RenderMillimeters),
			context.convertToPainterUnits(mOffsetFromReferencePoint.y(), QgsUnitTypes::RenderMillimeters) + context.convertToPainterUnits(mContentsMargins.top(), QgsUnitTypes::RenderMillimeters));
	}
	else
	{
		painter->translate(context.convertToPainterUnits(mContentsMargins.left(), QgsUnitTypes::RenderMillimeters),
			context.convertToPainterUnits(mContentsMargins.top(), QgsUnitTypes::RenderMillimeters));
	}
	QSizeF size(context.convertToPainterUnits(mFrameSize.width(), QgsUnitTypes::RenderMillimeters) - context.convertToPainterUnits(mContentsMargins.left() + mContentsMargins.right(), QgsUnitTypes::RenderMillimeters),
		context.convertToPainterUnits(mFrameSize.height(), QgsUnitTypes::RenderMillimeters) - context.convertToPainterUnits(mContentsMargins.top() + mContentsMargins.bottom(), QgsUnitTypes::RenderMillimeters));

	// scale back from painter dpi to 96 dpi --
  // double dotsPerMM = context.painter()->device()->logicalDpiX() / ( 25.4 * 3.78 );
  // context.painter()->scale( dotsPerMM, dotsPerMM );

	renderAnnotation(context, size);
}

void QgsAnnotationA::setMarkerSymbol(QgsMarkerSymbol *symbol)
{
	mMarkerSymbol.reset(symbol);
	emit appearanceChanged();
}

void QgsAnnotationA::setMapLayer( QgsMapLayer *layer )
{
  mMapLayer = layer;
  emit mapLayerChanged();
}

void QgsAnnotationA::setAssociatedFeature( const QgsFeature &feature)
{
  mFeature = feature;
}

bool QgsAnnotationA::accept(QgsStyleEntityVisitorInterface *visitor) const
{
	// NOTE: if visitEnter returns false it means "don't visit the annotation", not "abort all further visitations"
	if (!visitor->visitEnter(QgsStyleEntityVisitorInterface::Node(QgsStyleEntityVisitorInterface::NodeType::Annotation, QStringLiteral("annotation"), tr("Annotation"))))
		return true;

	if (mMarkerSymbol)
	{
		QgsStyleSymbolEntity entity(mMarkerSymbol.get());
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

QSizeF QgsAnnotationA::minimumFrameSize() const
{
	return QSizeF( 0, 0);
}

void QgsAnnotationA::updateBalloon()
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

	for (int i = 0; i <4; ++i)
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
	{
		return;
	}

	mBalloonSegment = minEdgeIndex;
	QPointF minEdgeEnd = minEdge.p2();
	mBalloonSegmentPoint1 = QPointF(minEdgePoint.x() , minEdgePoint.y());
	if (std::sqrt(minEdgePoint.sqrDist(minEdgeEnd.x(), minEdgeEnd.y())) < mSegmentPointWidthMm)
	{
		double x = 0;
		double y = 0;
		QgsGeometryUtils::pointOnLineWithDistance(minEdge.p2().x(), minEdge.p2().y(), minEdge.p1().x(), minEdge.p1().y(), mSegmentPointWidthMm, x, y);
		mBalloonSegmentPoint1 = QPointF(x, y);
	}
	else
	{
		double x = 0;
		double y = 0;
		QgsGeometryUtils::pointOnLineWithDistance(mBalloonSegmentPoint1.x(), mBalloonSegmentPoint1.y(), minEdge.p2().x(), minEdge.p2().y(), mSegmentPointWidthMm, x, y);
		mBalloonSegmentPoint2 = QPointF(x, y);
	}
}

QLineF QgsAnnotationA::segment(int index, QgsRenderContext *context ) const
{
	auto scaleSize = [context](double size)->double
	{
		return context ? context->convertToPainterUnits(size, QgsUnitTypes::RenderMillimeters) : size;
	};
	if (mHasFixedMapPosition)
	{
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
	else
	{
		switch (index)
		{
		case 0:
			return QLineF(0, 0, scaleSize(mFrameSize.width()), 0);
		case 1:
			return QLineF(scaleSize(mFrameSize.width()), 0,
				scaleSize(mFrameSize.width()), scaleSize(mFrameSize.height()));
		case 2:
			return QLineF(scaleSize(mFrameSize.width()), scaleSize(mFrameSize.height()),
				0, scaleSize(mFrameSize.height()));
		case 3:
			return QLineF(0, scaleSize(mFrameSize.height()),
				0, 0);
		default:
			return QLineF();
		}
	}
}

void QgsAnnotationA::drawFrame(QgsRenderContext &context ) const
{
	QPainter *painter = context.painter();
	if (!painter || !mFillSymbol)
		return;

	auto toPixels = [&](double mm) -> double {
		return context.convertToPainterUnits(mm, QgsUnitTypes::RenderMillimeters);
	};

	const double ox = toPixels(mOffsetFromReferencePoint.x());
	const double oy = toPixels(mOffsetFromReferencePoint.y());
	const double fw = toPixels(mFrameSize.width());
	const double fh = toPixels(mFrameSize.height());
	const double radius = fh * 0.50; // full pill: semicircular ends

	const QColor fillColor   = mFillSymbol->color();
	const QColor borderColor = fillColor.darker(160);

	QgsScopedQPainterState painterState(painter);
	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(Qt::NoPen);

	// Soft blurred shadow (3 layers)
	for (int i = 3; i >= 1; --i)
	{
		painter->setBrush(QColor(0, 0, 0, 22 * i));
		painter->drawRoundedRect(QRectF(ox + i + 1.0, oy + i + 1.0, fw, fh), radius, radius);
	}

	// Gradient fill: slightly lighter at top for a 3-D chip look
	QLinearGradient grad(QPointF(ox, oy), QPointF(ox, oy + fh));
	grad.setColorAt(0.0, fillColor.lighter(118));
	grad.setColorAt(1.0, fillColor);
	painter->setBrush(grad);
	painter->setPen(QPen(borderColor, 1.5));
	painter->drawRoundedRect(QRectF(ox, oy, fw, fh), radius, radius);
}

void QgsAnnotationA::drawMarkerSymbol(QgsRenderContext &context ) const
{
	if (!context.painter())
		return;

	if (mMarkerSymbol)
	{
		mMarkerSymbol->startRender(context);
		mMarkerSymbol->renderPoint(QPointF(0, 0), nullptr, context);
		mMarkerSymbol->stopRender(context);
	}
}

void QgsAnnotationA::_writeXml(QDomElement &itemElem, QDomDocument &doc, const QgsReadWriteContext &context ) const
{
	if (itemElem.isNull())
	{
		return;
	}
	QDomElement annotationElem = doc.createElement(QStringLiteral("AnnotationItem"));
	annotationElem.setAttribute(QStringLiteral("mapPositionFixed"), mHasFixedMapPosition);
	annotationElem.setAttribute(QStringLiteral("mapPosX"), qgsDoubleToString(mMapPosition.x()));
	annotationElem.setAttribute(QStringLiteral("mapPosY"), qgsDoubleToString(mMapPosition.y()));
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
	if (mMarkerSymbol)
	{
		QDomElement symbolElem = QgsSymbolLayerUtils::saveSymbol(QStringLiteral("marker symbol"), mMarkerSymbol.get(), doc, context);
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

void QgsAnnotationA::_readXml(const QDomElement &annotationElem, const QgsReadWriteContext &context)
{
}

void QgsAnnotationA::copyCommonProperties(QgsAnnotationA *target ) const
{
	target->mVisible = mVisible;
	target->mHasFixedMapPosition = mHasFixedMapPosition;
	target->mMapPosition = mMapPosition;
	target->mMapPositionCrs = mMapPositionCrs;
	target->mRelativePosition = mRelativePosition;
	target->mOffsetFromReferencePoint = mOffsetFromReferencePoint;
	target->mFrameSize = mFrameSize;
	target->mMarkerSymbol.reset(mMarkerSymbol ? mMarkerSymbol->clone() : nullptr);
	target->mContentsMargins = mContentsMargins;
	target->mFillSymbol.reset(mFillSymbol ? mFillSymbol->clone() : nullptr);
	target->mBalloonSegment = mBalloonSegment;
	target->mBalloonSegmentPoint1 = mBalloonSegmentPoint1;
	target->mBalloonSegmentPoint2 = mBalloonSegmentPoint2;
	target->mSegmentPointWidthMm = mSegmentPointWidthMm;
	target->mMapLayer = mMapLayer;
	target->mFeature = mFeature;
}

