#pragma once

#include <memory>
#include <qgsPointxy.h>
#include <qTextDocument.h>
#include <qgsMapCanvas.h>
#include "qgsTextAnnotationA.h"
#include <QgsMapCanvasAnnotationItem.h>
#include <qgsMarkerSymbolLayer.h>
#include <QFont>
#include <QFontMetrics>
#include <QColor>

//任务区域显示标签
class TaskAreaTip
{
public:
	TaskAreaTip(QgsMapCanvas* _canvas)
	{
		m_pTextAnnotation = new QgsTextAnnotationA();
		m_pTextAnnotation->setMapPosition(QgsPointXY(116, 40));
		m_pTextAnnotation->setFrameSize(QSizeF(80, 26));
		m_pTextAnnotation->setTaskSymbol();

		m_pCanvasItem = new QgsMapCanvasAnnotationItem((QgsTextAnnotation*)m_pTextAnnotation, _canvas);
		m_pCanvasItem->setFlag(QGraphicsItem::ItemIsMovable,   false);
		m_pCanvasItem->setFlag(QGraphicsItem::ItemIsSelectable, false);
		m_pCanvasItem->setAcceptedMouseButtons(Qt::NoButton);
	}

	~TaskAreaTip()
	{
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

	int      m_id = 0;
	QgsPoint m_pt;

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

private:
	QgsTextAnnotationA*          m_pTextAnnotation = nullptr;
	QgsMapCanvasAnnotationItem*  m_pCanvasItem     = nullptr;
	QString                      m_labelText;
};
