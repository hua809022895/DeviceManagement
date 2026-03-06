/***************************************************************************                          
begin                : February 9, 2010
copyright            : (C) 2010 by Marco Hugentobler
 ***************************************************************************/
#include "stdafx.h"
#include "qgstextAnnotationA.h"
#include <QDomDocument>
#include <QPainter>
#include <QLinearGradient>

QgsTextAnnotationA::QgsTextAnnotationA(QObject *parent):QgsAnnotationA(parent), mDocument(new QTextDocument(QString()))
{
    mDocument->setUseDesignMetrics(true);
}

QgsTextAnnotationA  *QgsTextAnnotationA::clone() const
{
	std::unique_ptr< QgsTextAnnotationA > c(new QgsTextAnnotationA());
	copyCommonProperties(c.get());
	c->setDocument(mDocument.get());
	return c.release();
}

const QTextDocument * QgsTextAnnotationA::document() const
{
  return mDocument.get();
}

void QgsTextAnnotationA::setDocument( const QTextDocument *doc )
{
  if ( doc )
    mDocument.reset( doc->clone() );
  else
    mDocument.reset();
  emit appearanceChanged();
}

void QgsTextAnnotationA::renderAnnotation(QgsRenderContext &context, QSizeF size ) const
{
	QPainter* painter = context.painter();
	if (!mDocument)
		return;

	QgsScopedQPainterState painterState(context.painter());
	const double scaleFactor = context.painter()->device()->logicalDpiX() / 96.0;
	context.painter()->scale(scaleFactor, scaleFactor);
	size /= scaleFactor;

	const double fw = size.width();
	const double fh = size.height();
	const double radius = fh * 0.50; // full pill: semicircular ends

	const QColor fillColor   = displayColor();
	const QColor borderColor = fillColor.darker(160);

	painter->setRenderHint(QPainter::Antialiasing, true);
	painter->setPen(Qt::NoPen);

	// Soft blurred shadow (3 layers)
	for (int i = 3; i >= 1; --i)
	{
		painter->setBrush(QColor(0, 0, 0, 22 * i));
		painter->drawRoundedRect(QRectF(i + 1.0, i + 1.0, fw, fh), radius, radius);
	}

	// Gradient fill: lighter at top for a 3-D chip look
	QLinearGradient grad(QPointF(0.0, 0.0), QPointF(0.0, fh));
	grad.setColorAt(0.0, fillColor.lighter(118));
	grad.setColorAt(1.0, fillColor);
	painter->setBrush(grad);
	painter->setPen(QPen(borderColor, 1.5));
	painter->drawRoundedRect(QRectF(0, 0, fw, fh), radius, radius);

	// Draw text content — vertically centered
	mDocument->setTextWidth(fw);
	const double docH    = mDocument->size().height();
	const double yOffset = qMax(0.0, (fh - docH) / 2.0);
	painter->translate(0, yOffset);
	QRectF clipRect(0, 0, fw, fh);
	if (painter->hasClipping())
		clipRect = clipRect.intersected(painter->clipRegion().boundingRect());
	mDocument->drawContents(painter, clipRect);
}

void QgsTextAnnotationA::writeXml( QDomElement &elem, QDomDocument &doc, const QgsReadWriteContext &context ) const
{
  QDomElement annotationElem = doc.createElement( QStringLiteral( "TextAnnotationItem" ) );
  if ( mDocument )
  {
    annotationElem.setAttribute( QStringLiteral( "document" ), mDocument->toHtml() );
  }
  _writeXml( annotationElem, doc, context );
  elem.appendChild( annotationElem );
}

void QgsTextAnnotationA::readXml( const QDomElement &itemElem, const QgsReadWriteContext &context )
{
  mDocument.reset( new QTextDocument );
  mDocument->setHtml( itemElem.attribute( QStringLiteral( "document" ), QString() ) );
  QDomElement annotationElem = itemElem.firstChildElement( QStringLiteral( "AnnotationItem" ) );
  if ( !annotationElem.isNull() )
  {
    _readXml( annotationElem, context );
  }
}
