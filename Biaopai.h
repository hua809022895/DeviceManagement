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
#include <QgsMapCanvasItem.h>
#include <QPainter>
#include <QPolygonF>
#include <QDateTime>

// ── 飞机图标 QGraphicsItem ────────────────────────────────────────────────────
// 独立于 canvas 重绘周期，setPos 后立即在 QGraphicsScene 更新
// interpolate() 由外部 16ms 定时器驱动，实现 GPS 更新间的平滑插值
class PlaneIconItem : public QgsMapCanvasItem
{
public:
	PlaneIconItem(QgsMapCanvas* canvas) : QgsMapCanvasItem(canvas)
	{
		setZValue(1001);   // 在标注层之上
	}

	// 接收新的目标位置（每 100ms 由 GPS 更新驱动）
	void setMapPos(const QgsPointXY& pt)
	{
		if (!m_hasPos)
		{
			// 首次定位：直接跳到目标位置
			m_hasPos    = true;
			m_prevPos   = pt;
			m_nextPos   = pt;
			m_curPos    = pt;
			m_t0        = QDateTime::currentMSecsSinceEpoch();
			m_dt        = 100;
			m_interpDone = false;
			updatePosition();
			return;
		}
		// 位置未变化时不重置插值（无人机静止时跳过，避免重复 setPos 刷 BSP 树）
		if (qFuzzyCompare(pt.x(), m_nextPos.x()) && qFuzzyCompare(pt.y(), m_nextPos.y()))
			return;
		// 记录当前插值位置为新起点，目标为新终点
		m_prevPos    = m_curPos;
		m_nextPos    = pt;
		m_t0         = QDateTime::currentMSecsSinceEpoch();
		m_dt         = 100;
		m_interpDone = false;
	}

	// 由外部 16ms QTimer 调用：平滑插值到目标位置
	void interpolate(qint64 nowMs)
	{
		if (!m_hasPos || m_interpDone) return;
		double t = (m_dt > 0) ? qMin(1.0, double(nowMs - m_t0) / double(m_dt)) : 1.0;
		double x = m_prevPos.x() + (m_nextPos.x() - m_prevPos.x()) * t;
		double y = m_prevPos.y() + (m_nextPos.y() - m_prevPos.y()) * t;
		m_curPos = QgsPointXY(x, y);
		setPos(toCanvasCoordinates(m_curPos));
		if (t >= 1.0)
			m_interpDone = true;  // 到达目标，停止调用 setPos 直到下次 GPS 更新
	}

	void setYaw(double yaw)
	{
		if (qAbs(yaw - m_yaw) < 0.5) return;
		m_yaw = yaw;
		update();          // 仅触发 QGraphicsItem 局部重绘
	}

	void setVisible(bool v) { QGraphicsItem::setVisible(v); }

	QRectF boundingRect() const override { return QRectF(-36, -36, 72, 72); }

	void paint(QPainter* painter) override
	{
		painter->save();
		painter->setRenderHint(QPainter::Antialiasing);
		painter->rotate(m_yaw);   // 0=朝上（北），顺时针

		// 飞机轮廓：机头朝上的对称箭头形（约 2.5x 放大）
		static const QPolygonF kShape = QPolygonF()
			<< QPointF(  0, -32)   // 机头
			<< QPointF( 22,  17)   // 右翼尖
			<< QPointF(  0,   5)   // 机腹中
			<< QPointF(-22,  17);  // 左翼尖

		painter->setPen(QPen(QColor(255, 160, 80), 1.5));
		painter->setBrush(QBrush(QColor(255, 50, 0)));
		painter->drawPolygon(kShape);
		painter->restore();
	}

protected:
	void updatePosition() override
	{
		setPos(toCanvasCoordinates(m_curPos));
	}

private:
	QgsPointXY m_prevPos{ 116.0, 40.0 };
	QgsPointXY m_curPos { 116.0, 40.0 };
	QgsPointXY m_nextPos{ 116.0, 40.0 };
	qint64     m_t0  = 0;
	qint64     m_dt  = 100;
	double     m_yaw = 0.0;
	bool       m_hasPos    = false;
	bool       m_interpDone = true;   // 插值完成标志；GPS 更新到来前不重复调用 setPos
};

// ── 飞机标牌（文字标签） ──────────────────────────────────────────────────────
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

		// 独立的飞机图标 QGraphicsItem（不走 canvas 重绘路径）
		m_pIconItem = new PlaneIconItem(_canvas);
	}

	~biaopai()
	{
		if (m_pItem)
		{
			QObject::disconnect(m_pTextAnnotation, nullptr, m_pItem, nullptr);
			delete m_pItem;
			m_pItem = nullptr;
		}
		delete m_pTextAnnotation;
		m_pTextAnnotation = nullptr;
		// m_pIconItem 由 QGraphicsScene 管理，加入 scene 时 scene 拥有它
		// 但 QgsMapCanvasItem 的析构会从 scene 移除，需手动 delete
		delete m_pIconItem;
		m_pIconItem = nullptr;
	}

	int		 m_id = 0;
	QgsPoint m_pt;

	void setPos(QgsPointXY _point)
	{
		m_pt.setX(_point.x());
		m_pt.setY(_point.y());
		m_pTextAnnotation->setMapPosition(_point);
		if (m_pIconItem) m_pIconItem->setMapPos(_point);
	}

	void setVisible(bool v)
	{
		if (m_pItem)     m_pItem->setVisible(v);
		if (m_pIconItem) m_pIconItem->setVisible(v);
	}

	// 更新飞机朝向（yaw=0 朝北，顺时针度），变化 < 0.5° 跳过
	void setYaw(double yaw)
	{
		if (m_pIconItem) m_pIconItem->setYaw(yaw);
	}

	// 由外部 16ms 定时器调用：插值飞机图标位置
	void interpolate(qint64 nowMs)
	{
		if (m_pIconItem) m_pIconItem->interpolate(nowMs);
	}

	void setString(QString id)
	{
		m_id = id.toInt();
		m_labelText = id;
		updateSize(1.0);
	}

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
	QgsMapCanvasAnnotationItem	*m_pItem           = nullptr;
	PlaneIconItem				*m_pIconItem        = nullptr;
	QString						 m_labelText;
};
