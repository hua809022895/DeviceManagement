#include "stdafx.h"
#include "DlgAreaRadarList.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <qgsProject.h>
#include "comm.h"

#include "mainWindow.h"
#include "global.h"
#include "clipper2/include/clipper.h"

#include <cmath>
#include <algorithm>

// -- 体积格式化：统一输出立方千米（km3）
static QString fmtVol(double m3)
{
	return QString::number(m3 / 1e9, 'f', 4) + " km3";
}

// ── 点是否在 PathsD 内（任意路径）───────────────────────────────────────
static bool pointInPaths(double x, double y, const cl::PathsD &paths)
{
	for (const auto &path : paths)
		if (cl::PointInPolygon(cl::PointD(x, y), path) != cl::PointInPolygonResult::IsOutside)
			return true;
	return false;
}

// ── 单装备：3D 交叉体积（m³）──────────────────────────────────────────
// interSec: 装备扇形 ∩ 任务区域的 2D 交集（地理坐标，度）
// devPt: 装备位置（经度, 纬度，度）
// devAlt: 装备高度 AGL（m）
// elevation: 俯仰角中心（度，向下为负）
// vBeam: 垂直波束宽度（度，总宽）
// taskMinH/taskMaxH: 任务区域高度范围 AGL（m）
static double computeIntersectionVolume(
	const cl::PathsD &interSec,
	QgsPointXY devPt, double devAlt,
	double elevation, double vBeam,
	double taskMinH, double taskMaxH)
{
	if (interSec.empty()) return 0.0;

	// 每度经纬度对应的米数
	double mPerDegLon = MM_PER_LONGITUDE_BYLAT(devPt.y()) / 1000.0;
	double mPerDegLat = MM_PER_LATITUDE / 1000.0;

	double elevLowRad  = (elevation - vBeam / 2.0) * M_PI / 180.0;
	double elevHighRad = (elevation + vBeam / 2.0) * M_PI / 180.0;

	// 交集多边形包围盒
	double minX=1e18, minY=1e18, maxX=-1e18, maxY=-1e18;
	for (const auto &path : interSec)
		for (const auto &pt : path)
		{ minX=qMin(minX,pt.x); maxX=qMax(maxX,pt.x); minY=qMin(minY,pt.y); maxY=qMax(maxY,pt.y); }

	const int N = 30;
	double dx = (maxX - minX) / N;
	double dy = (maxY - minY) / N;
	if (dx <= 0 || dy <= 0) return 0.0;

	double cellAreaM2 = dx * mPerDegLon * dy * mPerDegLat;
	double totalVol   = 0.0;

	for (int i = 0; i < N; i++) {
		double cx = minX + (i + 0.5) * dx;
		for (int j = 0; j < N; j++) {
			double cy = minY + (j + 0.5) * dy;
			if (!pointInPaths(cx, cy, interSec)) continue;

			// 装备到格元水平距离（m）
			double ddx = (cx - devPt.x()) * mPerDegLon;
			double ddy = (cy - devPt.y()) * mPerDegLat;
			double d   = qMax(1.0, sqrt(ddx*ddx + ddy*ddy));

			// 探测锥在距离 d 处的高度范围
			double coneBot = devAlt + d * tan(elevLowRad);
			double coneTop = devAlt + d * tan(elevHighRad);
			if (coneBot > coneTop) std::swap(coneBot, coneTop);

			// 与任务高度范围取交集
			double h = qMax(0.0, qMin(coneTop, taskMaxH) - qMax(coneBot, taskMinH));
			totalVol += cellAreaM2 * h;
		}
	}
	return totalVol;
}

// ── 总覆盖体积：网格采样 + 高度区间并集（正确处理多装备重叠）──────────
struct EquipVolInfo {
	QgsPointXY pt;
	double devAlt, elevation, vBeam;
	cl::PathsD sec2D;  // 装备扇形 2D 多边形
};

static double computeTotalVolume(
	const cl::PathsD &taskPoly,
	const QVector<EquipVolInfo> &equips,
	double taskMinH, double taskMaxH)
{
	if (taskPoly.empty() || equips.empty()) return 0.0;

	double mPerDegLon = MM_PER_LONGITUDE_BYLAT(equips[0].pt.y()) / 1000.0;
	double mPerDegLat = MM_PER_LATITUDE / 1000.0;

	double minX=1e18,minY=1e18,maxX=-1e18,maxY=-1e18;
	for (const auto &path : taskPoly)
		for (const auto &pt : path)
		{ minX=qMin(minX,pt.x); maxX=qMax(maxX,pt.x); minY=qMin(minY,pt.y); maxY=qMax(maxY,pt.y); }

	const int N = 40;
	double dx = (maxX-minX)/N, dy = (maxY-minY)/N;
	if (dx<=0||dy<=0) return 0.0;
	double cellAreaM2 = dx*mPerDegLon * dy*mPerDegLat;
	double totalVol = 0.0;

	for (int i = 0; i < N; i++) {
		double cx = minX + (i+0.5)*dx;
		for (int j = 0; j < N; j++) {
			double cy = minY + (j+0.5)*dy;
			if (!pointInPaths(cx, cy, taskPoly)) continue;

			// 收集所有装备在此格元处覆盖的高度区间
			QVector<QPair<double,double>> intervals;
			for (const auto &e : equips) {
				if (!pointInPaths(cx, cy, e.sec2D)) continue;

				double ddx = (cx - e.pt.x()) * mPerDegLon;
				double ddy = (cy - e.pt.y()) * mPerDegLat;
				double d   = qMax(1.0, sqrt(ddx*ddx + ddy*ddy));

				double elevLow  = (e.elevation - e.vBeam/2.0) * M_PI/180.0;
				double elevHigh = (e.elevation + e.vBeam/2.0) * M_PI/180.0;
				double coneBot  = e.devAlt + d * tan(elevLow);
				double coneTop  = e.devAlt + d * tan(elevHigh);
				if (coneBot > coneTop) std::swap(coneBot, coneTop);

				double lo = qMax(coneBot, taskMinH);
				double hi = qMin(coneTop, taskMaxH);
				if (hi > lo) intervals.append({lo, hi});
			}
			if (intervals.isEmpty()) continue;

			// 高度区间并集
			std::sort(intervals.begin(), intervals.end(),
				[](const QPair<double,double>&a, const QPair<double,double>&b){ return a.first < b.first; });
			double h = 0.0, curLo = intervals[0].first, curHi = intervals[0].second;
			for (int k = 1; k < intervals.size(); k++) {
				if (intervals[k].first <= curHi)
					curHi = qMax(curHi, intervals[k].second);
				else { h += curHi - curLo; curLo = intervals[k].first; curHi = intervals[k].second; }
			}
			h += curHi - curLo;
			totalVol += cellAreaM2 * h;
		}
	}
	return totalVol;
}

// ─────────────────────────────────────────────────────────────────────────────

DlgAreaRadarList::DlgAreaRadarList(QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);

	QgsFeature			feat;
	QgsFeatureIterator	fit = g_pAirTaskPolyLayer->getFeatures();
	QStringList			data;

	while (fit.nextFeature(feat))
	{
		QString sID = feat.attribute(0).toString();

		QPolygonF polyxy = feat.geometry().asQPolygonF();
		polyxy.erase(polyxy.begin());

		cl::PathsD rect = rectangle_to_polygon(polyxy);
		double area = calculate_area(rect);
		double areaM2 = qAbs(area) * (1.0 / 0.00001) * (MM_PER_LATITUDE / 1000.0);
		QString areaStr = (areaM2 >= 1e6)
			? QString::number(areaM2 / 1e6, 'f', 2) + " km2"
			: QString::number(areaM2, 'f', 0) + " m2";

		// 读取高度范围
		double minH = qMax(0.0, feat.attribute(5).toDouble());
		double maxH = feat.attribute(6).toDouble();
		if (maxH <= minH) maxH = minH + 100.0;
		double taskH = maxH - minH;
		double volM3 = areaM2 * taskH;

		QString s = sID + " (" + areaStr + " x " + QString::number(taskH,'f',0) + "m = " + fmtVol(volM3) + ")";
		data << s;
	}
	data.sort();

	foreach(QString p, data)
		ui.listWidget1->addItem(p);

	ui.listWidget1->setViewMode(QListView::ListMode);
	this->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);

	connect(ui.listWidget1, &QListWidget::itemClicked, this, &DlgAreaRadarList::onItemClicked);

	if (ui.listWidget1->count() > 0)
	{
		ui.listWidget1->setCurrentRow(0);
		onItemClicked(ui.listWidget1->item(0));
	}
}

DlgAreaRadarList::~DlgAreaRadarList()
{}

void DlgAreaRadarList::onItemClicked(QListWidgetItem *item)
{
	if (!item) return;
	ui.listWidget2->clear();

	// 找到选中的任务区域
	QgsFeature feat;
	QgsFeatureIterator fit = g_pAirTaskPolyLayer->getFeatures();
	double    dTaskArea = 0;
	QPolygonF polyxy;
	double taskMinH = 0, taskMaxH = 100;

	while (fit.nextFeature(feat))
	{
		if (feat.attribute(0).toString() != item->text().split(" ")[0]) continue;

		polyxy = feat.geometry().asQPolygonF();
		polyxy.erase(polyxy.begin());
		cl::PathsD rect = rectangle_to_polygon(polyxy);
		dTaskArea = qAbs(calculate_area(rect));

		taskMinH = qMax(0.0, feat.attribute(5).toDouble());
		taskMaxH = feat.attribute(6).toDouble();
		if (taskMaxH <= taskMinH) taskMaxH = taskMinH + 100.0;
		break;
	}
	if (dTaskArea == 0) return;

	double taskH      = taskMaxH - taskMinH;
	double taskAreaM2 = dTaskArea * (1.0 / 0.00001) * (MM_PER_LATITUDE / 1000.0);
	double taskVolM3  = taskAreaM2 * taskH;

	QString mPath = QCoreApplication::applicationDirPath();
	QSettings settings(mPath + "/config.ini", QSettings::IniFormat);
	QStringList list = settings.value("DeviceType/list").toString().split("|");

	cl::PathsD rect_poly = rectangle_to_polygon(polyxy);
	MainWindow *pDlg = (MainWindow*)parentWidget();

	cl::PathsD all_union;
	QStringList coverItems;
	QVector<EquipVolInfo> equipInfos;

	for (int i = 0; i < gRadarLayerList.size(); i++)
	{
		fit = gRadarLayerList[i]->getFeatures();
		cl::PathsD layer_union;

		while (fit.nextFeature(feat))
		{
			QgsPointXY devPt    = feat.geometry().asPoint();
			QString    sID      = feat.attribute(0).toString();
			int        type     = feat.attribute(3).toInt();
			double     devAlt   = feat.attribute(4).toDouble();
			float      sAngle   = feat.attribute(7).toFloat();   // attribute(7)=方位角起始
			float      tAngle   = feat.attribute(9).toFloat();   // attribute(9)=水平波束宽度
			double     elevation= feat.attribute(8).toDouble();
			double     vBeam    = feat.attribute(10).toDouble();
			int        length   = feat.attribute(11).toInt();

			QList<QgsPointXY> set = pDlg->GetTYPolygon(
				devPt, sAngle, (int)tAngle, (float)elevation, length, (float)vBeam, (float)devAlt);
			cl::PathsD sec = sector_to_polygon(set);
			layer_union = Union(layer_union, sec, cl::FillRule::NonZero, 8);

			cl::PathsD singleSec = Intersect(sec, rect_poly, cl::FillRule::NonZero, 8);
			double area2D = qAbs(calculate_area(singleSec));
			double pct2D  = (dTaskArea > 0) ? area2D / dTaskArea * 100.0 : 0.0;

			// 3D 体积
			double vol    = computeIntersectionVolume(singleSec, devPt, devAlt, elevation, vBeam, taskMinH, taskMaxH);
			double pct3D  = (taskVolM3 > 0) ? vol / taskVolM3 * 100.0 : 0.0;

			if (pct2D >= 0.01 || pct3D >= 0.01)
			{
				QString typeStr = (type >= 0 && type < list.size()) ? list[type] : QString::number(type);
				coverItems << typeStr + sID + ":  " + fmtVol(vol) + "  (" + QString::number(pct3D, 'f', 1) + "%)";
				equipInfos.push_back({devPt, devAlt, elevation, vBeam, sec});
			}
		}
		all_union = Union(all_union, layer_union, cl::FillRule::NonZero, 8);
	}

	if (coverItems.isEmpty())
		ui.listWidget2->addItem(QString::fromLocal8Bit("\u65e0\u88c5\u5907\u8986\u76d6\u8be5\u533a\u57df"));
	else
		foreach (const QString &s, coverItems)
			ui.listWidget2->addItem(s);

	// 总覆盖体积（正确处理多装备重叠：用高度区间并集）
	cl::PathsD totalSec2D = Intersect(all_union, rect_poly, cl::FillRule::NonZero, 8);
	double totalVol = computeTotalVolume(totalSec2D, equipInfos, taskMinH, taskMaxH);
	double totalPct = (taskVolM3 > 0) ? qMin(totalVol / taskVolM3 * 100.0, 100.0) : 0.0;

	ui.listWidget2->addItem(QString::fromLocal8Bit("\u4efb\u52a1\u4f53\u79ef: ") + fmtVol(taskVolM3));
	ui.listWidget2->addItem(QString(22, '-'));
	ui.listWidget2->addItem(QString::fromLocal8Bit("\u5df2\u63a2\u8986\u76d6: ") + fmtVol(totalVol) + "  " + QString::number(totalPct, 'f', 1) + "%");

	QListWidgetItem *uncoveredItem = new QListWidgetItem(
		QString::fromLocal8Bit("\u672a\u63a2\u8986\u76d6: ") + fmtVol(qMax(0.0, taskVolM3 - totalVol))
		+ "  " + QString::number(qMax(0.0, 100.0 - totalPct), 'f', 1) + "%");
	uncoveredItem->setForeground(Qt::red);
	ui.listWidget2->addItem(uncoveredItem);
}

void DlgAreaRadarList::on_exitBtn_click()
{
	// reserved
}
