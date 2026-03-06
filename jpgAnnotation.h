#ifndef JPGANNOTATION_H
#define JPGANNOTATION_H

#include <qgsannotation.h>
#include <qgis_core.h>
#include <qgsmapcanvas.h>
#include "comm.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

class JpgAnnotation : public QgsAnnotation
{
    Q_OBJECT
public:
    /** Constructor for JpgAnnotation.   */
    JpgAnnotation( QObject *parent SIP_TRANSFERTHIS = nullptr );

    JpgAnnotation *clone() const override SIP_FACTORY;

    void writeXml( QDomElement &elem, QDomDocument &doc, const QgsReadWriteContext &context ) const override;
    void readXml( const QDomElement &itemElem, const QgsReadWriteContext &context ) override;

    /** Sets the file path for the source SVG file.  * \see filePath()  */
    void setFilePath( const QString &file );
    /**  * Returns the file path for the source SVG file. \see setFilePath()  */
    QString filePath() const { return mFilePath; }

    static JpgAnnotation *create() SIP_FACTORY { return new JpgAnnotation(); }

    QgsPointXY minMapXy,maxMapXy;
    uint32_t width,height;

    //QgsMapCanvas	*m_pMapCanvas;
    Mappoint		*mppoint;
    bool isrender = true;

    void setImageTransparent(QImage &sourceImage, QImage &destintImage, int trans);
  protected:
    void renderAnnotation( QgsRenderContext &context, QSizeF size ) const override;
  private:
    QString mFilePath;
};

class JpgAnnotation2 : public QgsAnnotation
{
    Q_OBJECT
public:
    /** Constructor for JpgAnnotation.  */
    JpgAnnotation2( QObject *parent SIP_TRANSFERTHIS = nullptr );

    JpgAnnotation2 *clone() const override SIP_FACTORY;

    void writeXml( QDomElement &elem, QDomDocument &doc, const QgsReadWriteContext &context ) const override;
    void readXml( const QDomElement &itemElem, const QgsReadWriteContext &context ) override;

    /**     * Sets the file path for the source SVG file.see filePath()    */
    void setFilePath( const QString &file );
    QString filePath() const { return mFilePath; }

    static JpgAnnotation2 *create() SIP_FACTORY { return new JpgAnnotation2(); }

    QgsPointXY minMapXy,maxMapXy;
    uint32_t width,height;

   // QImage  mJpg;
    QgsPoint	mPoint;
    QgsMapCanvas *mMapCanvas;

    bool isrender = true;
    QgsPointXY  mLeftup,mRightUp,mLeftdown;
    QgsPointXY  mRightdown;

    bool *mIsfirst;
    bool *mIsmove;
    std::unique_ptr< QgsMarkerSymbol > mSymbol;
    int testFourLineType();
    QString mFilePath;
    QRectF getBound();
    void setImageTransparent(QImage &sourceImage, QImage &destintImage, int trans);
  protected:
    void renderAnnotation( QgsRenderContext &context, QSizeF size ) const override;
};

#endif // JPGANNOTATION_H
