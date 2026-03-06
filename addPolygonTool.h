#ifndef ADDPOLYGONTOOL_H
#define ADDPOLYGONTOOL_H

#include <qgsmaptooledit.h>
#include <qgsrubberband.h>

class AddPolygonTool : public QgsMapToolEdit
{
     Q_OBJECT
public:
     AddPolygonTool(QgsMapCanvas *canvas);
      ~AddPolygonTool();

     enum CapMode // 矢量化类型
     {
         CaptureNone,       // 无
         CapturePoint,      // 点
         CaptureLine,       // 线
         CapturePolygon     // 面
     };

     CapMode        m_captureMode;
     QgsRubberBand  *mpRubberBand = nullptr;

     int global_poly_Index=0;

     void finishDrawOne();
     void saveToLayer();
private:

     bool mIsDrawing        = false;
     QgsVectorLayer         * layer = nullptr; // 当前编辑层
     QList<QgsMapLayer *>   layers;
protected:
    void canvasMoveEvent(QgsMapMouseEvent * e);
    void canvasPressEvent(QgsMapMouseEvent * e);


};

#endif // ADDPOLYGONTOOL_H
