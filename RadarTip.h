#pragma once

#include <memory>
#include <qgsPointxy.h>
#include <qTextDocument.h>
#include <qgsMapCanvas.h>
#include <qgsTextAnnotation.h>
#include <QgsMapCanvasAnnotationItem.h>
#include <qgsMarkerSymbolLayer.h>
#include "qgsTextAnnotationA.h"
#include <QFont>
#include <QFontMetrics>
#include <QColor>

//雷达设备显示标签
class RadarTip
{
public:
	RadarTip(QgsMapCanvas* _canvas)
	{
		m_pTextAnnotation = new QgsTextAnnotationA();
		m_pTextAnnotation->setMapPosition(QgsPointXY(116, 40));
		m_pTextAnnotation->setFrameSize(QSizeF(80, 26));			//宽度,高度（初始值，setString中会自动调整）
		m_pTextAnnotation->setRadarSymbol();

		m_pCanvasItem = new QgsMapCanvasAnnotationItem((QgsTextAnnotation*)m_pTextAnnotation, _canvas);
		// 禁止鼠标交互：QgsAnnotationA 与 QgsAnnotation 类型不兼容，
		// 点击时 QGIS 会通过错误的 vtable 调用虚函数导致崩溃
		m_pCanvasItem->setFlag(QGraphicsItem::ItemIsMovable,   false);
		m_pCanvasItem->setFlag(QGraphicsItem::ItemIsSelectable, false);
		m_pCanvasItem->setAcceptedMouseButtons(Qt::NoButton);
	}

	~RadarTip()
	{
		// 先删除 canvas item（从画布移除），再删除 annotation，避免悬空指针
		if (m_pCanvasItem)
		{
			delete m_pCanvasItem;
			m_pCanvasItem = nullptr;
		}
		if (m_pTextAnnotation)
		{
			delete m_pTextAnnotation;
			m_pTextAnnotation = nullptr;
		}
	}

	int			m_id=0;		//雷达id
	QgsPoint	m_pt;		//坐标点
	void setPos(QgsPointXY _point)
	{
		m_pt.setX(_point.x());
		m_pt.setY(_point.y());
		m_pTextAnnotation->setMapPosition(_point);
	}

	void setString(QString _string)
	{
		m_labelText = _string;
		updateSize(1.0);
	}

	// sf = scale factor (1.0 = zoom 14 base; computed in scaleChanged)
	void updateSize(double sf)
	{
		QFont font;
		font.setPointSizeF(9.0 * sf);
		font.setBold(true);
		QFontMetricsF fm(font);
		qreal textW = fm.horizontalAdvance(m_labelText);
		qreal textH = fm.ascent() + fm.descent();
		// frame snugly wraps text with 5px horizontal and 4px vertical padding
		qreal w = textW + 10.0;
		qreal h = qMax(textH + 4.0, 12.0);
		m_pTextAnnotation->setFrameSize(QSizeF(w, h));
		QTextDocument doc;
		doc.setDefaultFont(font);
		doc.setDocumentMargin(0);
		doc.setPlainText(m_labelText);
		QTextOption opt;
		opt.setAlignment(Qt::AlignHCenter);
		doc.setDefaultTextOption(opt);
		m_pTextAnnotation->setDocument(&doc);
	}

	// Set label background color by device type index
	void setType(int type)
	{
		static const QColor palette[] = {
			QColor(180, 215, 255),  // 0: light blue   (e.g. radar)
			QColor(180, 235, 195),  // 1: light green  (e.g. optical)
			QColor(255, 220, 170),  // 2: light orange (e.g. infrared)
			QColor(220, 190, 255),  // 3: light purple (e.g. acoustic)
			QColor(255, 245, 170),  // 4: light yellow (e.g. other)
		};
		const int n = int(sizeof(palette) / sizeof(palette[0]));
		QColor c = (type >= 0 && type < n) ? palette[type] : QColor(255, 255, 255);
		m_pTextAnnotation->setFillColor(c);
	}

private:
	QgsTextAnnotationA*          m_pTextAnnotation = nullptr;
	QgsMapCanvasAnnotationItem*  m_pCanvasItem     = nullptr;
	QString                      m_labelText;
};
