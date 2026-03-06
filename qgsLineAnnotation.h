/***************************************************************************
                              QgsLineAnnotation.h
 ***************************************************************************/

#ifndef QGSlineANNOTATION_H
#define QGSlineANNOTATION_H

#include "qgsAnnotation2.h"
#include <QTextDocument>
#include <qgsMapCanvas.h>
#include "qgis_core.h"

//class CORE_EXPORT QgsTextAnnotation: public QgsAnnotation
//无人机轨迹线类
class QgsLineAnnotation : public QgsAnnotation2
{
    Q_OBJECT
public:
	QgsLineAnnotation(QObject *parent SIP_TRANSFERTHIS = nullptr);	
	QgsLineAnnotation *clone() const override SIP_FACTORY;
	void writeXml(QDomElement &elem, QDomDocument &doc, const QgsReadWriteContext &context) const override;
	void readXml(const QDomElement &itemElem, const QgsReadWriteContext &context) override;
	static QgsLineAnnotation *create() SIP_FACTORY { return new QgsLineAnnotation(); }
signals:
	void FixbInitMsg(int *);	//修改信号,刷新轨迹线
protected:
	void renderAnnotation(QgsRenderContext &context, QSizeF size) const override;
private:
};

#endif // QGSTEXTANNOTATION_H
