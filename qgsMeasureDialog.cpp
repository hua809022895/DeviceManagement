/***************************************************************************
                                 qgsmeasure.h
                               ------------------
        begin                : March 2005
        copyright            : (C) 2005 by Radim Blazek
        email                : blazek@itc.it
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#include "stdafx.h"
//#include <qgisapp.h>
#include "qgsMeasureDialog.h"
#include "qgsMeasureTool.h"

#include "qgslogger.h"
//#include "qgsDistanceArea.h"
#include "qgsMapCanvas.h"
#include "qgsProject.h"
#include "qgsCoordinatereferencesystem.h"
#include "qgsUnittypes.h"
#include "qgsSettings.h"
#include "qgsgui.h"

#include <QClipboard>
#include <QCloseEvent>
#include <QLocale>
#include <QPushButton>
#include "global.h"

QgsMeasureDialog::QgsMeasureDialog( QgsMeasureTool *tool, Qt::WindowFlags f):QDialog( nullptr, f)
					,mMeasureArea( tool->measureArea()), mTool( tool), mCanvas( tool->canvas())
{
	setupUi(this);
	QgsGui::instance()->enableAutoGeometryRestore(this);
	connect(buttonBox, &QDialogButtonBox::helpRequested, this, &QgsMeasureDialog::showHelp);

	// hide 3D related options
	editHorizontalTotal->hide();
	totalHorizontalDistanceLabel->hide();

	QPushButton *nb = new QPushButton(QString::fromLocal8Bit("重新测量"));
	buttonBox->addButton(nb, QDialogButtonBox::ActionRole);
	connect(nb, &QAbstractButton::clicked, this, &QgsMeasureDialog::restart);

	//  Add a configuration button
	//QPushButton *cb = new QPushButton( tr( "&Configuration"));
	//buttonBox->addButton( cb, QDialogButtonBox::ActionRole);
	//connect( cb, &QAbstractButton::clicked, this, &QgsMeasureDialog::openConfigTab);
	/*if (!mMeasureArea)
	{
		QPushButton *cpb = new QPushButton(QString::fromLocal8Bit("复制结果"));
		buttonBox->addButton(cpb, QDialogButtonBox::ActionRole);
		connect(cpb, &QAbstractButton::clicked, this, &QgsMeasureDialog::copyMeasurements);
	}*/

	QString s = QString::fromLocal8Bit("米");
	mUnitsCombo->addItem(s);
	mUnitsCombo->setCurrentIndex(0); // mUnitsCombo->findData(QgsUnitTypes::DistanceMeters));

	updateSettings();

	//connect(mUnitsCombo, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &QgsMeasureDialog::unitsChanged);
	connect(buttonBox,	&QDialogButtonBox::rejected,			this, &QgsMeasureDialog::reject);
	connect(mCanvas,	&QgsMapCanvas::destinationCrsChanged,	this, &QgsMeasureDialog::crsChanged);
//	connect(mCartesian, &QRadioButton::toggled, this, &QgsMeasureDialog::projChanged);
	//this->setWindowTitle(QString::fromLocal8Bit("测量"));

	buttonBox->button(QDialogButtonBox::Close)->setText(QString::fromLocal8Bit("关闭"));
	buttonBox->button(QDialogButtonBox::Help)->hide();
}

void QgsMeasureDialog::projChanged()
{
	//如果按平面测量
	mDa.setEllipsoid(geoNone());
	mTable->clear();
	mTotal = 0.;
	updateUi();
}

void QgsMeasureDialog::crsChanged()
{
	if (!mCanvas->mapSettings().destinationCrs().isValid())
	{
		mUnitsCombo->setEnabled(false);
		if (mMeasureArea)
			mUnitsCombo->setCurrentIndex(mUnitsCombo->findData(QgsUnitTypes::DistanceUnknownUnit));
		else
			mUnitsCombo->setCurrentIndex(mUnitsCombo->findData(QgsUnitTypes::AreaUnknownUnit));
	}
	else
	{
		mUnitsCombo->setEnabled(true);
	}

	mTable->clear();
	mTotal = 0.;
	updateUi();
}

void QgsMeasureDialog::updateSettings()
{
	QgsSettings settings;

	mDecimalPlaces = settings.value(QStringLiteral("qgis/measure/decimalplaces"), "0").toInt();
	mCanvasUnits = mCanvas->mapUnits();
	// Configure QgsDistanceArea
	mDistanceUnits		= QgsProject::instance()->distanceUnits();
	mMapDistanceUnits	= QgsProject::instance()->crs().mapUnits();
	mAreaUnits = QgsProject::instance()->areaUnits();
	mDa.setSourceCrs(mCanvas->mapSettings().destinationCrs(), QgsProject::instance()->transformContext());
	projChanged();

	mDa.setEllipsoid(geoNone());
}

void QgsMeasureDialog::unitsChanged( int index )
{
	if (mMeasureArea)//如果是测量面积
	{
		mAreaUnits = static_cast<QgsUnitTypes::AreaUnit>(mUnitsCombo->itemData(index).toInt());
		if (mAreaUnits == QgsUnitTypes::AreaUnknownUnit)
		{
			mAreaUnits = QgsUnitTypes::distanceToAreaUnit(mMapDistanceUnits);
		}
	}
	else
	{
		mDistanceUnits = static_cast<QgsUnitTypes::DistanceUnit>(mUnitsCombo->itemData(index).toInt());
		if (mDistanceUnits == QgsUnitTypes::DistanceUnknownUnit)
		{
			mDistanceUnits = mMapDistanceUnits;
		}
	}

	mTable->clear();
	mTotal = 0.;
	updateUi();

	if (!mTool->done())
	{
		// re-add temporary mouse cursor position
		addPoint();
		mouseMove(mLastMousePoint);
	}
}

void QgsMeasureDialog::restart()
{
	mTool->restart();
	mTable->clear();
	mTotal = 0.;
	updateUi();
}

void QgsMeasureDialog::mouseMove(const QgsPointXY &point)
{
	mLastMousePoint = point;
	// show current distance/area while moving the point
	// by creating a temporary copy of point array
	// and adding moving point at the end
	if (mMeasureArea && mTool->points().size() >= 2)
	{
		QVector<QgsPointXY> tmpPoints = mTool->points();
		tmpPoints.append(point);
		double area = mDa.measurePolygon(tmpPoints);
		//editTotal->setText( formatArea(area));
		eTotal->setText(QLocale().toString(area, 'f',2));
	}
	else 
		if (!mMeasureArea && !mTool->points().empty())
	{
		QVector< QgsPointXY > tmpPoints = mTool->points();
		QgsPointXY p1(tmpPoints.at(tmpPoints.size() - 1)), p2(point);
		//double d = mDa.measureLine(p1, p2);//这行代码，在windows上有问题，在ubuntu20上正常

		QgsPoint pt1,pt2;
		pt1.setX(p1.x()); pt1.setY(p1.y());
		pt2.setX(p2.x()); pt2.setY(p2.y());

		double d = GetDistance(pt1, pt2);
		QString s= QString::number(d, 'f',2);
		eTotal->setText(s);
		//mTotal += d;
		//eTotal->setText(formatDistance(mTotal, mConvertToDisplayUnits));
		//d = convertLength(d, mDistanceUnits);

		// Set moving
		QTreeWidgetItem *item = mTable->topLevelItem(mTable->topLevelItemCount() - 1);
		if (item)
		{
			item->setText(0, QLocale().toString(d, 'f', mDecimalPlaces));
		}
	}
}

//添加一个点
void QgsMeasureDialog::addPoint()
{
	int numPoints = mTool->points().size();
	if (mMeasureArea && numPoints > 2)
	{
		double area = mDa.measurePolygon(mTool->points());
		eTotal->setText(formatArea(area));
	}
	else if (!mMeasureArea && numPoints >= 1)
	{
		if (!mTool->done())
		{
			QTreeWidgetItem *item = new QTreeWidgetItem(QStringList(QLocale().toString(0.0, 'f', mDecimalPlaces)));
			item->setTextAlignment(0, Qt::AlignRight);
			mTable->addTopLevelItem(item);
			mTable->scrollToItem(item);
		}
		if (numPoints > 1)
		{
			mTotal = mDa.measureLine(mTool->points());
			eTotal->setText(QLocale().toString(mTotal, 'f', 2));
		}
	}
}

void QgsMeasureDialog::removeLastPoint()
{
	int numPoints = mTool->points().size();
	if (mMeasureArea)//如果是测量面积
	{
		if (numPoints > 1)
		{
			QVector<QgsPointXY> tmpPoints = mTool->points();
			if (!mTool->done())
				tmpPoints.append(mLastMousePoint);
			double area = mDa.measurePolygon(tmpPoints);
			//QString s = formatArea(area);
			eTotal->setText(QLocale().toString(area, 'f', mDecimalPlaces));
		}
		else
		{
			eTotal->setText(formatArea(0));
		}
	}
	else if (!mMeasureArea && numPoints >= 1)
	{
		//remove final row
		delete mTable->takeTopLevelItem(mTable->topLevelItemCount() - 1);

		mTotal = mDa.measureLine(mTool->points());

		eTotal->setText("0");

		if (!mTool->done())
		{
			// need to add the distance for the temporary mouse cursor point
			QVector< QgsPointXY > tmpPoints = mTool->points();
			QgsPointXY p1(tmpPoints.at(tmpPoints.size() - 1));
			double d = 0;// mDa.measureLine(p1, mLastMousePoint);

			QTreeWidgetItem *item = mTable->topLevelItem(mTable->topLevelItemCount() - 1);
			//item->setText(0, QLocale().toString(d, 'f', mDecimalPlaces));
			eTotal->setText("0");
		}
	}
}

void QgsMeasureDialog::closeEvent(QCloseEvent *e)
{
	reject();
	e->accept();
}

QString QgsMeasureDialog::formatArea( double area, bool convertUnits ) const
{
	//if ( convertUnits )area = convertArea( area, mAreaUnits );
	return QgsDistanceArea::formatArea(area, mDecimalPlaces, mAreaUnits, true );
}

void QgsMeasureDialog::updateUi()
{
	// Set tooltip to indicate how we calculate measurements
	//QString toolTip = ("计算基于:");

	mDa.setEllipsoid(QgsProject::instance()->ellipsoid());
	mConvertToDisplayUnits = true;

	//toolTip += "<br> * 按平面测量.  ";
	mConvertToDisplayUnits = true;

	mDa.setEllipsoid(geoNone());

	//eTotal->setToolTip(toolTip);
	//mTable->setToolTip(toolTip);
	//mNotesLabel->setText( toolTip );

	mUnitsCombo->setCurrentIndex(0);// mUnitsCombo->findData(QgsUnitTypes::DistanceMeters));
	mTable->setHeaderLabels(QStringList(QString::fromLocal8Bit("线段 [%1]").arg(QgsUnitTypes::toString(QgsUnitTypes::DistanceMeters))));

    QVector<QgsPointXY>::const_iterator it;
    bool b = true; // first point

    QgsPointXY p1, p2;
    mTotal = 0;
    QVector< QgsPointXY > tmpPoints = mTool->points();
    for ( it = tmpPoints.constBegin(); it != tmpPoints.constEnd(); ++it )
    {
		p2 = *it;
		if (!b)
		{
			double d = -1;  //d = mDa.measureLine( p1, p2 );        

			QTreeWidgetItem *item = new QTreeWidgetItem(QStringList(QLocale().toString(d, 'f', mDecimalPlaces)));
			item->setTextAlignment(0, Qt::AlignRight);
			mTable->addTopLevelItem(item);
			mTable->scrollToItem(item);
		}
		p1 = p2;
		b = false;
    }

    mTotal = mDa.measureLine( mTool->points() );
    mTable->show(); // Show the table with items
    mSpacer->changeSize( 40, 5, QSizePolicy::Fixed, QSizePolicy::Maximum );
    eTotal->setText("");
}

void QgsMeasureDialog::reject()
{
	restart();
	QDialog::close();
}

void QgsMeasureDialog::showHelp()
{
 // QgsHelp::openHelp( QStringLiteral( "introduction/general_tools.html#measuring" ) );
}
