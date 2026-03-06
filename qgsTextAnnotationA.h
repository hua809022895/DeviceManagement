#ifndef QGSTEXTANNOTATIONA_H
#define QGSTEXTANNOTATIONA_H

#include "qgsAnnotationA.h"
#include <QTextDocument>
#include "qgis_core.h"

class QgsTextAnnotationA: public QgsAnnotationA   //CORE_EXPORT
{
    Q_OBJECT
  public:
    QgsTextAnnotationA(QObject *parent SIP_TRANSFERTHIS = nullptr );
    QgsTextAnnotationA*clone() const override SIP_FACTORY;

    const QTextDocument *document() const;
    void setDocument( const QTextDocument *doc );
    void writeXml( QDomElement &elem, QDomDocument &doc, const QgsReadWriteContext &context ) const override;
    void readXml( const QDomElement &itemElem, const QgsReadWriteContext &context ) override;
    static QgsTextAnnotationA *create() SIP_FACTORY { return new QgsTextAnnotationA(); }
  protected:
    void renderAnnotation( QgsRenderContext &context, QSizeF size ) const override;
  private:
    std::unique_ptr< QTextDocument > mDocument;
};

#endif // QGSTEXTANNOTATION_H
