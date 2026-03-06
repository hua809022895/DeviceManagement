#ifndef QGSANNOTATIONMARKERITEM2_H
#define QGSANNOTATIONMARKERITEM2_H

#include "qgis_sip.h"
#include <qgsannotationitem.h>
#include <qgsmapcanvas.h>

class  QgsAnnotationMarkerItem2 : public QgsAnnotationItem
{
  public:
    /** Constructor for QgsAnnotationMarkerItem, at the specified \a point. */
    QgsAnnotationMarkerItem2(const QgsPoint &point);
    ~QgsAnnotationMarkerItem2() override;

    QString type() const override;
	void render(QgsRenderContext &context);
    void render(QgsRenderContext &context, QgsFeedback *feedback) override;
    void renderDirectly (QPainter* painter);
    bool writeXml(QDomElement &element, QDomDocument &document, const QgsReadWriteContext &context ) const override;

    /** Creates a new marker annotation item.     */
    static QgsAnnotationMarkerItem2 *create() SIP_FACTORY;

    bool readXml( const QDomElement &element, const QgsReadWriteContext &context ) override;
    QgsAnnotationMarkerItem2 *clone() override SIP_FACTORY;
    QgsRectangle boundingBox() const override;
    QgsPointXY geometry() const { return mPoint; }

    void setGeometry( const QgsPoint &geometry ) { mPoint = geometry; }

    /** Returns the symbol used to render the marker item.see setSymbol()     */
    const QgsMarkerSymbol *symbol() const;
    void setSymbol( QgsMarkerSymbol *symbol SIP_TRANSFER );
    void setFilePath(const QString &file);

  public:
	QgsPointXY		mLeftup,	mRightUp;  //左上，右上
    QgsPointXY		mLeftdown,	mRightdown;//左下，右下
    QgsMapCanvas	*mMapCanvas;
    bool			*mIsfirst;
    bool			*mIsmove;
  private:
    QgsPoint							mPoint;
    std::unique_ptr< QgsMarkerSymbol>	mSymbol;
    int			testFourLineType();
    QString		mFilePath;
};
#endif
