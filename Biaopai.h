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

//���˻�������
class biaopai
{
public:
	biaopai(QgsMapCanvas* _canvas)
	{
		m_pTextAnnotation = new QgsTextAnnotationA();
		m_pTextAnnotation->setMapPosition(QgsPointXY(116, 40));
		m_pTextAnnotation->setFrameSize(QSize(35, 15));
		m_pTextAnnotation->setAirSymbol();
		m_pItem = new QgsMapCanvasAnnotationItem(m_pTextAnnotation, _canvas);
	}

	~biaopai()
	{
		if (m_pItem)
		{
			// 先断开 annotation→item 的 destroyed() 信号，防止后续 delete m_pTextAnnotation 再次触发 deleteLater
			QObject::disconnect(m_pTextAnnotation, nullptr, m_pItem, nullptr);
			delete m_pItem;   // 立即从画布场景同步移除
			m_pItem = nullptr;
		}
		delete m_pTextAnnotation;
		m_pTextAnnotation = nullptr;
	}

	int			m_id=0;		//���˻�id
	QgsPoint	m_pt;		//�����
	void setPos(QgsPointXY _point)
	{
		m_pt.setX(_point.x());
		m_pt.setY(_point.y());
		m_pTextAnnotation->setMapPosition(_point);
	}

	void setString(QString id)
	{
		m_id = id.toInt();
		m_labelText = id;   // just the aircraft number, no prefix
		updateSize(1.0);
	}

	// sf = scale factor (1.0 = zoom 14 base; computed in scaleChanged)
	void updateSize(double sf)
	{
		QFont font;
		font.setPointSizeF(8.5 * sf);
		font.setBold(true);
		QFontMetricsF fm(font);
		qreal textW = fm.horizontalAdvance(m_labelText);
		qreal textH = fm.ascent() + fm.descent();
		qreal w = textW + 10.0;
		qreal h = qMax(textH + 4.0, 12.0);
		m_pTextAnnotation->setFrameSize(QSizeF(w, h));
		// Center badge horizontally on icon; 20px below icon center
		m_pTextAnnotation->setFrameOffsetFromReferencePoint(QPointF(-w / 2.0, 20.0));
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
	QgsTextAnnotationA			*m_pTextAnnotation = nullptr;
	QgsMapCanvasAnnotationItem	*m_pItem = nullptr;
	QString						 m_labelText;
};