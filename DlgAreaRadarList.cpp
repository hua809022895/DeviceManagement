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

DlgAreaRadarList::DlgAreaRadarList(QWidget *parent): QDialog(parent)
{
	ui.setupUi(this);

	int i = 0;
	QgsFeature			feat;
	QStringList			data;
	QgsFeatureIterator	fit = g_pAirTaskPolyLayer->getFeatures();
	while (fit.nextFeature(feat))
	{
		QString		sID = feat.attribute(0).toString();

		QPolygonF polyxy = feat.geometry().asQPolygonF();
		polyxy.erase(polyxy.begin());	//删除重复的点

		cl::PathsD rect = rectangle_to_polygon(polyxy);

		double area = calculate_area(rect);
		// EW: 1m = 0.00001 deg (hardcoded in GetPolygon), NS: 1 deg = MM_PER_LATITUDE/1000 m
		double areaM2 = qAbs(area) * (1.0 / 0.00001) * (MM_PER_LATITUDE / 1000.0);
		QString areaStr = (areaM2 >= 1e6)
			? QString::number(areaM2 / 1e6, 'f', 2) + " km²"
			: QString::number(areaM2, 'f', 0) + " m²";
		QString s = sID + " (" + areaStr + ")";
		data << s;
	}
	data.sort();

	foreach(QString p, data)
	{
		ui.listWidget1->addItem(p);
	}

	ui.listWidget1->setViewMode(QListView::ListMode);
	this->setWindowFlags(Qt::Dialog | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);

	connect(ui.listWidget1, &QListWidget::itemClicked, this, &DlgAreaRadarList::onItemClicked);

	// 自动选中第一项并显示覆盖情况
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
	if (item == nullptr)
		return;

	ui.listWidget2->clear();

	// 找到选中的任务区域多边形
	QgsFeature feat;
	QgsFeatureIterator fit = g_pAirTaskPolyLayer->getFeatures();
	double    dTaskArea = 0;
	QPolygonF polyxy;
	while (fit.nextFeature(feat))
	{
		QString sID = feat.attribute(0).toString();
		if (sID == item->text().split(" ")[0])
		{
			polyxy = feat.geometry().asQPolygonF();
			polyxy.erase(polyxy.begin());
			cl::PathsD rect = rectangle_to_polygon(polyxy);
			dTaskArea = calculate_area(rect);
			if (dTaskArea < 0) dTaskArea *= -1;
			break;
		}
	}
	if (dTaskArea == 0) return;

	QString mPath = QCoreApplication::applicationDirPath();
	QSettings settings(mPath + "/config.ini", QSettings::IniFormat);
	QStringList list = settings.value("DeviceType/list").toString().split("|");

	cl::PathsD rect_poly  = rectangle_to_polygon(polyxy);
	cl::PathsD all_union;     // 所有图层扇形的总并集
	QStringList coverItems;   // 覆盖率 >= 0.01% 的装备条目

	MainWindow* pDlg = (MainWindow*)this->parentWidget();

	for (int i = 0; i < gRadarLayerList.size(); i++)
	{
		fit = gRadarLayerList[i]->getFeatures();
		cl::PathsD layer_union;

		while (fit.nextFeature(feat))
		{
			QgsPointXY devPt  = feat.geometry().asPoint();
			QString    sID    = feat.attribute(0).toString();
			int        type   = feat.attribute(3).toInt();
			QString    sAngle = feat.attribute(5).toString();
			QString    tAngle = feat.attribute(6).toString();
			QString    pitch  = feat.attribute(8).toString();
			QString    length = feat.attribute(11).toString();

			QList<QgsPointXY> set = pDlg->GetTYPolygon(
				devPt, sAngle.toFloat(), tAngle.toInt(), pitch.toFloat(), length.toInt());
			cl::PathsD sec = sector_to_polygon(set);
			layer_union = Union(layer_union, sec, cl::FillRule::NonZero, 8);

			cl::PathsD singleSec = Intersect(sec, rect_poly, cl::FillRule::NonZero, 8);
			double area  = calculate_area(singleSec);
			float  dOver = (float)(qAbs(area) / dTaskArea * 100.0);

			if (dOver >= 0.01f)
			{
				double areaM2 = qAbs(area) * (1.0 / 0.00001) * (MM_PER_LATITUDE / 1000.0);
				QString areaStr = (areaM2 >= 1e6)
					? QString::number(areaM2 / 1e6, 'f', 2) + " km²"
					: QString::number(areaM2, 'f', 0) + " m²";
				coverItems << list[type] + sID + ":  " + areaStr
					+ "  (" + QString::number(dOver, 'f', 1) + "%)";
			}
		}
		all_union = Union(all_union, layer_union, cl::FillRule::NonZero, 8);
	}

	// 显示有覆盖的装备列表
	if (coverItems.isEmpty())
	{
		ui.listWidget2->addItem(QString::fromLocal8Bit("无装备覆盖该区域"));
	}
	else
	{
		foreach (const QString& s, coverItems)
			ui.listWidget2->addItem(s);
	}

	// 计算总覆盖（所有装备并集与任务区域的交集）
	cl::PathsD totalSec = Intersect(all_union, rect_poly, cl::FillRule::NonZero, 8);
	double totalArea = calculate_area(totalSec);
	double totalPct  = qMin(qAbs(totalArea) / dTaskArea * 100.0, 100.0);

	ui.listWidget2->addItem("─────────────────");
	ui.listWidget2->addItem(QString::fromLocal8Bit("已探覆盖: ") + QString::number(totalPct, 'f', 1) + "%");

	QListWidgetItem* uncoveredItem = new QListWidgetItem(
		QString::fromLocal8Bit("未探覆盖: ") + QString::number(100.0 - totalPct, 'f', 1) + "%");
	uncoveredItem->setForeground(Qt::red);
	ui.listWidget2->addItem(uncoveredItem);
}

void DlgAreaRadarList::on_exitBtn_click()
{
	MainWindow *pDlg = (MainWindow*)this->parentWidget();
}
