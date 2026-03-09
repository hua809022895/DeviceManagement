/***************************************************************************
                             qgsAnnotationA.h
 ***************************************************************************/

#ifndef QGSANNOTATIONA_H
#define QGSANNOTATIONA_H

#include "qgis_core.h"
#include "qgis.h"
#include "qgsannotation.h"

class QgsRenderContext;

// Custom annotation base that extends QgsAnnotation with per-type coloring.
// Inheriting from QgsAnnotation makes QgsTextAnnotationA a valid QgsAnnotation*,
// so QgsMapCanvasAnnotationItem can accept it without any cast.
class QgsAnnotationA : public QgsAnnotation
{
    Q_OBJECT
  public:
    QgsAnnotationA(QObject *parent SIP_TRANSFERTHIS = nullptr);

    // Pure virtual clone — implemented by QgsTextAnnotationA
    virtual QgsAnnotation *clone() const = 0 SIP_FACTORY;

    // Per-type symbol / color helpers
    void setAirSymbol();
    void setTaskSymbol();
    void setRadarSymbol();
    void setFillColor(const QColor &color);
    QColor displayColor() const { return m_displayColor; }

  protected:
    virtual void renderAnnotation(QgsRenderContext &context, QSizeF size) const = 0;
    QSizeF minimumFrameSize() const override;

  private:
    QColor m_displayColor = QColor(255, 255, 255);
};

#endif // QGSANNOTATIONA_H
