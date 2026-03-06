#include "stdafx.h"
#include "QgsLineAnnotation.h"
#include <QDomDocument>
#include <QPainter>

QgsLineAnnotation::QgsLineAnnotation(QObject *parent):QgsAnnotation2(parent)
{
}

QgsLineAnnotation *QgsLineAnnotation::clone() const
{
	std::unique_ptr<QgsLineAnnotation> c(new QgsLineAnnotation());
	copyCommonProperties(c.get());
	//c->setDocument(mDocument.get());
	return c.release();
}

void QgsLineAnnotation::writeXml(QDomElement &elem, QDomDocument &doc, const QgsReadWriteContext &context) const
{
	QDomElement annotationElem = doc.createElement(QStringLiteral("TextAnnotationItem"));
}

void QgsLineAnnotation::readXml(const QDomElement &itemElem, const QgsReadWriteContext &context)
{
	QDomElement annotationElem = itemElem.firstChildElement(QStringLiteral("AnnotationItem"));
	if (!annotationElem.isNull())
		_readXml(annotationElem, context);
}

void QgsLineAnnotation::renderAnnotation(QgsRenderContext &context, QSizeF size) const
{
	QPainter *painter = context.painter();
	QRectF rc(-64, 40, 30, 30);	
	//qDebug() << "painter->drawImage: m_iAngle=" << m_iAngle <<" m_id="<< m_id<< endl;
	//painter->drawLines(m_points);
	//painter->drawLine(p1, p2);
	//qDebug() << "painter->drawLine:is null..... " << endl;
	//painter->drawLine(m_pt1, m_pt2);
}