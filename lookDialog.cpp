#include "stdafx.h"
#include "lookDialog.h"
#include "ui_lookdialog.h"
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>

#include "mainWindow.h"
#include <QSpacerItem>
#include <QMessageBox>
#include <QFileDialog>

#include "comm.h"

#include <qgsMarkersymbollayer.h>
#include <qgsExpressioncontextutils.h>
#include <qgsExpressioncontext.h>
#include <qgsFilterlineedit.h>
#include <qgsHeatmaprenderer.h>
#include <qgsLinesymbollayer.h>
#include <qgsSinglesymbolrenderer.h>
#include <qgsSymbol.h>
//#include "Xlsx-src/xlsx/xlsxdocument.h"       //
#include <qgsAnnotationLayer.h>
#include "qgsAnnotationMarkerItem2.h"

//extern QTextCodec *pCode;
LookDialog::LookDialog( QWidget *parent ) :QDialog(parent),ui(new Ui::LookDialog)
{
    ui->setupUi(this);
    ui->mProgressBar->setVisible(false);          
	ui->mProgressBar->setOrientation(Qt::Horizontal);

	QPushButton *pbtn = ui->buttonBox->button(QDialogButtonBox::Ok);
	pbtn->setText(QString::fromLocal8Bit("确定"));
	
	pbtn = ui->buttonBox->button(QDialogButtonBox::Cancel);
	pbtn->setText(QString::fromLocal8Bit("取消"));

	//设置正在执行任务的飞机，列表
	int m = 0;
    ui->mTableWidgetPlane->setRowCount(1);
    ui->mTableWidgetPlane->setColumnCount(6);    
    ui->mTableWidgetPlane->setHorizontalHeaderItem(m++, new QTableWidgetItem(""));
    ui->mTableWidgetPlane->setHorizontalHeaderItem(m++, new QTableWidgetItem(QString::fromLocal8Bit("飞机id" )));
    ui->mTableWidgetPlane->setHorizontalHeaderItem(m++, new QTableWidgetItem(QString::fromLocal8Bit("名字")));
    ui->mTableWidgetPlane->setHorizontalHeaderItem(m++, new QTableWidgetItem(QString::fromLocal8Bit("位置")));
    ui->mTableWidgetPlane->setHorizontalHeaderItem(m++, new QTableWidgetItem(QString::fromLocal8Bit("类型")));
    ui->mTableWidgetPlane->setHorizontalHeaderItem(m++, new QTableWidgetItem(QString::fromLocal8Bit("任务")));
	connect(ui->mPushButtonRefreshPlane, &QAbstractButton::clicked, this, &LookDialog::refreshPlane);

	//设置飞机库存列表
    ui->mTableWidgetPicture->setColumnCount(7);
    int i =0;	
    ui->mTableWidgetPicture->setHorizontalHeaderItem(i++, new  QTableWidgetItem(QString::fromLocal8Bit("飞机组号")));
    ui->mTableWidgetPicture->setHorizontalHeaderItem(i++, new  QTableWidgetItem(QString::fromLocal8Bit("飞机编号")));
    ui->mTableWidgetPicture->setHorizontalHeaderItem(i++, new  QTableWidgetItem(QString::fromLocal8Bit("飞机状态")));
    ui->mTableWidgetPicture->setHorizontalHeaderItem(i++, new  QTableWidgetItem(QString::fromLocal8Bit("飞机机型")));
    ui->mTableWidgetPicture->setHorizontalHeaderItem(i++, new  QTableWidgetItem(QString::fromLocal8Bit("任务")));
    ui->mTableWidgetPicture->setHorizontalHeaderItem(i++, new  QTableWidgetItem(QString::fromLocal8Bit("电量")));
    ui->mTableWidgetPicture->setHorizontalHeaderItem(i++, new  QTableWidgetItem(QString::fromLocal8Bit("位置")));
    ui->mTableWidgetPicture->verticalHeader()->setVisible(false);
    ui->mTableWidgetPicture->setSelectionBehavior (QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    ui->mTableWidgetPicture->setSelectionMode (QAbstractItemView::SingleSelection); //设置选择模式，选择单行
	connect(ui->mActionButtonAddCurrent, &QAbstractButton::clicked, this, &LookDialog::insertPlaneStock);		//添加一条记录到数据库表里
	connect(ui->mActionPushbtnDelCurrent,&QAbstractButton::clicked, this, &LookDialog::deleteUnHandlerPicture);//删除库存记录
	connect(ui->mTableWidgetPicture, &QTableWidget::itemClicked, this, &LookDialog::tableWidgetPicture);
    refreshPlaneStock(); //刷新飞机库存表

	//设置历史情报，表格对象
	i = 0;
    ui->mTableWidgetHistory->setColumnCount(7);
    ui->mTableWidgetHistory->setHorizontalHeaderItem(i++, new  QTableWidgetItem(QString::fromLocal8Bit("序号")));
    ui->mTableWidgetHistory->setHorizontalHeaderItem(i++, new  QTableWidgetItem(QString::fromLocal8Bit("图片")));
    ui->mTableWidgetHistory->setHorizontalHeaderItem(i++, new  QTableWidgetItem(QString::fromLocal8Bit("位置")));
    ui->mTableWidgetHistory->setHorizontalHeaderItem(i++, new  QTableWidgetItem(QString::fromLocal8Bit("时间")));
    ui->mTableWidgetHistory->setHorizontalHeaderItem(i++, new  QTableWidgetItem(QString::fromLocal8Bit("机器判断")));
    ui->mTableWidgetHistory->setHorizontalHeaderItem(i++, new  QTableWidgetItem(QString::fromLocal8Bit("任务")));
    ui->mTableWidgetHistory->setHorizontalHeaderItem(i++, new  QTableWidgetItem(QString::fromLocal8Bit("人工确认")));
    ui->mTableWidgetHistory->setColumnWidth(4,120);
    ui->mTableWidgetHistory->setColumnWidth(6,120);
    ui->mTableWidgetHistory->verticalHeader()->setVisible( false);
    ui->mTableWidgetHistory->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    ui->mTableWidgetHistory->setSelectionMode (QAbstractItemView::SingleSelection); //设置选择模式，选择单行

    ui->mDateTimeEditbegin->setDateTime(QDateTime::currentDateTime().addDays(-1));
    ui->mDateTimeEditEnd->setDateTime(QDateTime::currentDateTime().addDays(1));

    ui->mComboBoxHandletype->addItem(QString::fromLocal8Bit("全部"));
    ui->mComboBoxHandletype->addItem(QString::fromLocal8Bit("已放弃"));
    ui->mComboBoxHandletype->addItem(QString::fromLocal8Bit("未处理"));
    ui->mComboBoxHandletype->addItem(QString::fromLocal8Bit("已人工确认"));
    ui->mComboBoxHandletype->addItem(QString::fromLocal8Bit("已监视"));
    ui->mComboBoxHandletype->addItem(QString::fromLocal8Bit("已打击"));

    QStringList typelist = WeaponTypeList.split("|");
    ui->mComboBoxWeapon->addItems( typelist);
    ui->mComboBoxWeapon->insertItem(0,QString::fromLocal8Bit("全部"));
	
	connect(ui->mPushButtonLocate, &QAbstractButton::clicked, this, &LookDialog::locateCurrent);	//定位命令按钮
	connect(ui->mActionButtonDelete, &QAbstractButton::clicked, this, &LookDialog::deleteHistory);	//删除单个记录按钮
	connect(ui->mPushButtonDeleteAll, &QAbstractButton::clicked, this, &LookDialog::deleteAll);		//删除全部
	connect(ui->mPushButtonExport, &QAbstractButton::clicked, this, &LookDialog::exportExcel);		//导出到excel表格中，命令按钮	
	connect(ui->mPushButtonHistory, &QAbstractButton::clicked, this, &LookDialog::historyPicture);	//查询全部	
	connect(ui->mActionButtonquery, &QAbstractButton::clicked, this, &LookDialog::contionQuery);	//按条件查询
	connect(ui->mPushButtonExtend, &QAbstractButton::clicked, this, &LookDialog::setOutExtentFromDrawOnCanvas);//按经纬度范围搜索
	
}

LookDialog::~LookDialog()
{
    delete ui;
}

void LookDialog::setMapCavas(QgsMapCanvas *mapcanvas)
{
    mCanvas = mapcanvas;
}

//向飞机库存表中添加一条记录
void LookDialog::insertPlaneStock()
{
	QString delete_sql = " delete from  planeStock where planeGroup = '" + ui->mLineEditgroup->text() + "' and planenumber = '" + ui->mLineEditPlaneNumber->text() + "' ";
	QSqlQuery sql_delete_query(g_sqliteDbase);
	if (!sql_delete_query.exec(delete_sql))
	{
		qDebug() << sql_delete_query.lastError();
	}

	QString insert_sql = "insert into planestock( planegroup,planenumber,planestatus,planemodel,planetask,planepower,planeposition ) "
		" values ( :planegroup,:planenumber,:planestatus,:planemodel,:planetask,:planepower,:planeposition )";
	QSqlQuery sql_query(g_sqliteDbase);
	sql_query.prepare(insert_sql);
	sql_query.bindValue(":planegroup", ui->mLineEditgroup->text());
	sql_query.bindValue(":planenumber", ui->mLineEditPlaneNumber->text());
	sql_query.bindValue(":planestatus", ui->mComboBoxPlaneStatus->currentText());
	sql_query.bindValue(":planemodel", ui->mComboBoxPlaneModel->currentText());
	sql_query.bindValue(":planetask", ui->mComboBoxTask->currentText());
	sql_query.bindValue(":planepower", ui->mLineEditPower->text());
	sql_query.bindValue(":planeposition", ui->mLineEditPlanePosion->text());

	if (!sql_query.exec())
	{
		qDebug() << sql_query.lastError();
	}

	refreshPlaneStock();//刷新飞机库存
}
//删除库存记录
void LookDialog::deleteUnHandlerPicture()
{
	QString delete_sql = " delete from  planeStock where planeGroup = '" + ui->mLineEditgroup->text() + "' and planeNumber = '" + ui->mLineEditPlaneNumber->text() + "' ";
	QSqlQuery sql_delete_query(g_sqliteDbase);
	if (!sql_delete_query.exec(delete_sql))
	{
		qDebug() << sql_delete_query.lastError();
	}
	refreshPlaneStock();
}

//正在执行任务的飞机子窗口，刷新飞机
void LookDialog::refreshPlane()
{
    while ( ui->mTableWidgetPlane->rowCount() >0 )
    {
        ui->mTableWidgetPlane->removeRow(0);
    }

    foreach (tag_PlaneMessage plane, m_Planes) {
        ui->mTableWidgetPlane->insertRow(0);
        ui->mTableWidgetPlane->setItem(0, 0, new QTableWidgetItem( " " ));
        ui->mTableWidgetPlane->setItem(0, 1, new QTableWidgetItem("planeId"));
        ui->mTableWidgetPlane->setItem(0, 2, new QTableWidgetItem("planeName"));
        ui->mTableWidgetPlane->setItem(0, 3, new QTableWidgetItem(QString("%1,%2" ).arg( plane.planeX ).arg(plane.planeY)));
        ui->mTableWidgetPlane->setItem(0, 4, new QTableWidgetItem("planetype"));
        ui->mTableWidgetPlane->setItem(0, 5, new QTableWidgetItem("taskname"));
    }
}


void LookDialog::setOutExtentFromDrawOnCanvas()
{
    if ( mCanvas )
    {
      mMapToolPrevious = mCanvas->mapTool();
      if ( !mMapToolExtent )
      {
        mMapToolExtent.reset( new QgsMapToolExtent( mCanvas ) );
        connect( mMapToolExtent.get(), &QgsMapToolExtent::extentChanged, this, &LookDialog::extentDrawn );
        connect( mMapToolExtent.get(), &QgsMapTool::deactivated, this, [ = ]
        {
          emit toggleDialogVisibility( true );
          mMapToolPrevious = nullptr;
        } );
      }
      mMapToolExtent->setRatio( mRatio );
      mCanvas->setMapTool( mMapToolExtent.get() );

      emit toggleDialogVisibility( false );
      showMinimized();
    }
}

void LookDialog::extentDrawn(const QgsRectangle &extent)
{
    setOutputExtent( extent, mCanvas->mapSettings().destinationCrs(), DrawOnCanvas );
    mMapToolPrevious = nullptr;
    mCanvas->unsetMapTool( mMapToolExtent.get() );
    showNormal();
    raise();
    activateWindow();
    emit toggleDialogVisibility( true );
}

//刷新飞机库存
void LookDialog::refreshPlaneStock()
{
    QString select_sql = "select * from planeStock";
    QSqlQuery sql_select_query(g_sqliteDbase);
    if( !sql_select_query.exec(select_sql))
    {
        qDebug() << sql_select_query.lastError();
    }
   
	//首先删除表格内所有记录
    while ( ui->mTableWidgetPicture->rowCount()>0 ){
         ui->mTableWidgetPicture->removeRow(0);
    }

	//向飞机库存的列表框中，添加记录
    while (sql_select_query.next()) 
	{
        ui->mTableWidgetPicture->insertRow(0);
        int i=0;       
        ui->mTableWidgetPicture->setItem(0, i++,  new QTableWidgetItem( sql_select_query.value("planegroup").toString() ) );
        ui->mTableWidgetPicture->setItem(0, i++,  new QTableWidgetItem( sql_select_query.value("planenumber").toString() ) );
        ui->mTableWidgetPicture->setItem(0, i++, new QTableWidgetItem( sql_select_query.value("planestatus").toString() ));
        ui->mTableWidgetPicture->setItem(0, i++, new QTableWidgetItem( sql_select_query.value("planemodel").toString()  ));
        ui->mTableWidgetPicture->setItem(0, i++, new QTableWidgetItem( sql_select_query.value("planetask").toString()  ));
        ui->mTableWidgetPicture->setItem(0, i++, new QTableWidgetItem( sql_select_query.value("planepower").toString() ));
        ui->mTableWidgetPicture->setItem(0, i++, new QTableWidgetItem( sql_select_query.value("planeposition").toString() ));
    }
}

void LookDialog::tableWidgetPicture()
{
    int r = ui->mTableWidgetPicture->currentRow();
    ui->mLineEditgroup->setText(ui->mTableWidgetPicture->item(r,0)->text());
    ui->mLineEditPlaneNumber->setText(ui->mTableWidgetPicture->item(r,1)->text());
    ui->mComboBoxPlaneStatus->setCurrentText(ui->mTableWidgetPicture->item(r,2)->text());
    ui->mComboBoxPlaneModel->setCurrentText(ui->mTableWidgetPicture->item(r,3)->text());
    ui->mComboBoxTask->setCurrentText(ui->mTableWidgetPicture->item(r,4)->text());
    ui->mLineEditPower->setText(ui->mTableWidgetPicture->item(r,5)->text());
    ui->mLineEditPlanePosion->setText(ui->mTableWidgetPicture->item(r,6)->text());
}

void LookDialog::setOutputExtent(const QgsRectangle &r, const QgsCoordinateReferenceSystem &srcCrs, LookDialog::ExtentState state)
{
    if ( !mHasFixedOutputCrs )
    {
      mOutputCrs = srcCrs;
      extent = r;
    }
    else
    {
      if ( mOutputCrs == srcCrs )
      {
        extent = r;
      }
      else
      {
        try
        {
          QgsCoordinateTransform ct( srcCrs, mOutputCrs, QgsProject::instance() );
          extent = ct.transformBoundingBox( r );
        }
        catch ( QgsCsException & )
        {
          extent = r;
        }
      }
    }

    int decimals = 4;
    switch ( mOutputCrs.mapUnits())
    {
      case QgsUnitTypes::DistanceDegrees:
      case QgsUnitTypes::DistanceUnknownUnit:
        decimals = 9;
        break;
      case QgsUnitTypes::DistanceMeters:
      case QgsUnitTypes::DistanceKilometers:
      case QgsUnitTypes::DistanceFeet:
      case QgsUnitTypes::DistanceNauticalMiles:
      case QgsUnitTypes::DistanceYards:
      case QgsUnitTypes::DistanceMiles:
      case QgsUnitTypes::DistanceCentimeters:
      case QgsUnitTypes::DistanceMillimeters:
        decimals = 4;
        break;
    }
    QString condensed = QStringLiteral( "%1,%2,%3,%4" ).arg( QString::number( extent.xMinimum(), 'f', decimals ),
                        QString::number( extent.yMinimum(), 'f', decimals ),
                        QString::number( extent.xMaximum(), 'f', decimals ),
                        QString::number( extent.yMaximum(), 'f', decimals ) );
    condensed += QStringLiteral( " [%1]" ).arg( mOutputCrs.userFriendlyIdentifier( QgsCoordinateReferenceSystem::ShortString ) );
    ui->mLineEditExtend->setText( condensed );

    mExtentState = state;

    if ( !mIsValid )
      setValid( true );

    emit extentChanged( extent );
}

void LookDialog::setValid(bool valid)
{
    if ( valid == mIsValid )
      return;

    mIsValid = valid;
    emit validationChanged( mIsValid );
}

void LookDialog::setPlanes(QVector<tag_PlaneMessage> &planes)
{
	m_Planes = planes;
}

void LookDialog::closeEvent(QCloseEvent *e)
{
    QDialog::closeEvent(e);
}
