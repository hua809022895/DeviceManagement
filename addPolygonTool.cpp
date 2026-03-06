#include "stdafx.h"
#include "comm.h"
#include "qgsMaptool.h"
#include "addPolygonTool.h"
#include <qgsMapMouseEvent.h>
#include <qgsCategorizedSymbolRenderer.h>
#include <qgsMapCanvas.h>

AddPolygonTool::AddPolygonTool(QgsMapCanvas *canvas):QgsMapToolEdit(canvas)
{
}

AddPolygonTool::~AddPolygonTool()
{
}

void AddPolygonTool::canvasMoveEvent(QgsMapMouseEvent *e)
{
}

void AddPolygonTool::canvasPressEvent(QgsMapMouseEvent *e)
{
    if(!mpRubberBand)
    {
        if(this->m_captureMode == this->CapturePoint)
        {
            mpRubberBand = createRubberBand(QgsWkbTypes::GeometryType::PointGeometry);            
            layer = g_pTargetLayer;
        }
        else if(this->m_captureMode == this->CaptureLine)
        {
            mpRubberBand = createRubberBand(QgsWkbTypes::GeometryType::LineGeometry);
            layer = g_pLineLayer;

        }
        else if(this->m_captureMode == this->CapturePolygon)
        {
            mpRubberBand = createRubberBand(QgsWkbTypes::GeometryType::PolygonGeometry,true);
            layer = g_pPolygonLayer;
        }
    }
    mpRubberBand->addPoint(e->mapPoint());

    if (e->button() == Qt::MouseButton::RightButton)
        saveToLayer();
}

void AddPolygonTool::saveToLayer()
{
    if (layer == nullptr || mpRubberBand == nullptr)
		return;

    std::vector<QColor> colorList;

    QColor color(255,0,0);
    colorList.push_back(color);
    QgsFeature f(layer->fields());

    mpRubberBand->setColor(color);
    f.setGeometry(mpRubberBand->asGeometry());

    layer->dataProvider()->addFeature(f);

    QgsFields currentLayerFields = layer->fields();

    QString fieldName = "class";
    int currentFieldIndexOf = layer->fields().indexOf( fieldName );
    if(currentFieldIndexOf == -1)
    {
      QList<QgsField> fieldList;
      QgsField shpField("class", QVariant::String);
      fieldList.push_back(shpField);
      layer->dataProvider()->addAttributes(fieldList);
    }
    else
    {
      QgsAttributeMap changeAttributeMap;
      QgsChangedAttributesMap changeMap;

      //注意global_poly_Index不支持分布式
      int index = global_poly_Index++;
      QString status = QString("%1").arg( index );
      changeAttributeMap.insert( currentFieldIndexOf, QVariant( status ) );
      changeMap.insert( index, changeAttributeMap );
      layer->dataProvider()->changeAttributeValues(changeMap);
    }

    QSet<QVariant> unique = layer->uniqueValues( currentFieldIndexOf );

    //得到字段值对应的所有属性
    QVariantList uniqueValues;
    uniqueValues= unique.values();

    //获取所有属性对应的类别
    QgsCategoryList cats = QgsCategorizedSymbolRenderer::createCategories( uniqueValues, QgsSymbol::defaultSymbol( layer->geometryType() ), layer, "class" );
    QColor startColor;
    //qDebug()<<"cats.size():"<<cats.size();
    //qDebug()<<"colorList:"<<colorList.size();
    for(int index = 0;index <cats.size();index++)
    {
      QRgb mRgb = qRgb(colorList[index].red(),colorList[index].green(),colorList[index].blue());
      startColor.setRgba(mRgb);
      cats[index].symbol()->setColor(startColor);
    }

    //加载进QgsCategorizedSymbolRenderer渲染器中
    std::unique_ptr< QgsCategorizedSymbolRenderer > r = qgis::make_unique< QgsCategorizedSymbolRenderer >( fieldName, cats );
    //渲染图层
    layer->setRenderer(r->clone());

    // 开始编辑
    layer->startEditing();
    // 保存
    layer->commitChanges();

    // 更新范围
    layer->updateExtents();
    mCanvas->refresh();
    finishDrawOne();
}

void AddPolygonTool::finishDrawOne()
{
    layer = nullptr;
    mpRubberBand = nullptr;//注意，自动释放
}
