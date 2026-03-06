#pragma once

#ifndef SVGANNOTATIONITEM_H
#define SVGANNOTATIONITEM_H

#include <qgsannotationitem.h>
#include <qgsapplication.h>
#include <qgsproviderregistry.h>
#include <qgssinglesymbolrendererv2.h>
#include <qgsmaplayerregistry.h>
#include <qgsvectorlayer.h>
#include <qgsmapcanvas.h>
#include <qgsproject.h>
#include <qgslayertreenode.h>
#include <qgslayertreegroup.h>
#include <qgslayertreeregistrybridge.h>
#include <qgsattributetablemodel.h>
#include <qgsattributetableview.h>
#include <qgsattributetablefiltermodel.h>
#include <qgsvectorlayercache.h>
#include <qgseditorwidgetregistry.h>
#include <qgspallabeling.h>
#include <qgscustomlayerorderwidget.h>
#include <QgsRasterLayer.h>
#include <qgssymbollayerv2.h>
#include <qgssimplifymethod.h>
#include <qgssymbollayerv2.h>
#include <qgssymbolv2.h>
#include <qgsmarkersymbollayerv2.h>
#include <qgsvectorlayerrenderer.h>
#include <qgsrendercontext.h>
#include <qgssinglesymbolrendererv2.h>
#include <qgssymbollayerv2.h>
#include "SCVectorLayer.h"
#include "SCMapCanvas.h"
#include "qgstextannotationitem.h"
#include <qgsformannotationitem.h>
#include "MeasureTool.h"
#include <qgssvgannotationitem.h>

class  QSvgRenderer;
class SvgAnnotationImageItem : public QgsAnnotationItem
{
public:
	SvgAnnotationImageItem(QgsMapCanvas* canvas);
	~SvgAnnotationImageItem();
	void writeXML(QDomDocument& doc) const override;
	void readXML(const QDomDocument& doc, const QDomElement& itemElem) override;
	void paint(QPainter* painter) override;
	void setFilePath(const QString& file);
	QString filePath() const { return mFilePath; }
private:
	QSvgRenderer *mSvgRenderer;
	QString mFilePath;
};

#endif // SVGANNOTATIONITEM_H

