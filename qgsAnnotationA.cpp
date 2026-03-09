#include "stdafx.h"
#include "qgsAnnotationA.h"
#include "qgssymbollayerutils.h"

QgsAnnotationA::QgsAnnotationA(QObject *parent) : QgsAnnotation(parent)
{
}

void QgsAnnotationA::setAirSymbol()
{
    QgsStringMap props;
    props.insert(QStringLiteral("color"), QStringLiteral("0,0,0,0"));
    props.insert(QStringLiteral("style"), QStringLiteral("solid"));
    props.insert(QStringLiteral("style_border"), QStringLiteral("no"));
    setFillSymbol(QgsFillSymbol::createSimple(props));
    setMarkerSymbol(nullptr);
    setFrameOffsetFromReferencePointMm(QPointF(8, -4));
    m_displayColor = QColor(255, 255, 200); // light yellow
}

void QgsAnnotationA::setTaskSymbol()
{
    QgsStringMap props;
    props.insert(QStringLiteral("color"), QStringLiteral("0,0,0,0"));
    props.insert(QStringLiteral("style"), QStringLiteral("solid"));
    props.insert(QStringLiteral("style_border"), QStringLiteral("no"));
    setFillSymbol(QgsFillSymbol::createSimple(props));
    setMarkerSymbol(nullptr);
    setFrameOffsetFromReferencePointMm(QPointF(5, -4));
    m_displayColor = QColor(255, 220, 80); // golden yellow for task areas
}

void QgsAnnotationA::setRadarSymbol()
{
    QgsStringMap props;
    props.insert(QStringLiteral("color"), QStringLiteral("0,0,0,0"));
    props.insert(QStringLiteral("style"), QStringLiteral("solid"));
    props.insert(QStringLiteral("style_border"), QStringLiteral("no"));
    setFillSymbol(QgsFillSymbol::createSimple(props));
    setMarkerSymbol(nullptr);
    setFrameOffsetFromReferencePointMm(QPointF(5, -4));
    m_displayColor = QColor(255, 255, 255); // white
}

void QgsAnnotationA::setFillColor(const QColor &color)
{
    m_displayColor = color;
    emit appearanceChanged();
}

QSizeF QgsAnnotationA::minimumFrameSize() const
{
    return QSizeF(0, 0);
}
