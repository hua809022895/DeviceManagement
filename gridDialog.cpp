#include "stdafx.h"
#include "gridDialog.h"
#include "ui_griddialog.h"
#include <qgsvectorlayer.h>
#include "qgsMapToolSelectUtils.h"
#include <qgsvectorlayerfeatureiterator.h>
#include <qdebug.h>
#include <QMessageBox>
#include <QTextcodec.h>

#include "comm.h"
#include "mainWindow.h"

extern QTextCodec *pCode;
GridDialog::GridDialog(QWidget *parent) :QDialog(parent), ui(new Ui::GridDialog)
{
    ui->setupUi(this);
    ui->mButtonBox->button(QDialogButtonBox::Ok )->setText(pCode->toUnicode("关闭"));
    ui->mButtonBox->button(QDialogButtonBox::Apply )->setText(QString::fromLocal8Bit("初始化"));
    
	ui->mLineEditExtent->setText(pCode->toUnicode("坐标...."));
	ui->mLineEditSideLength->setText("400"); //网格边长，单位米

    connect(ui->mButtonDrawOnCanvas, &QAbstractButton::clicked, this, &GridDialog::setOutExtentFromDrawOnCanvas);	//战场画布时候，回调函数
    connect(ui->mComboBoxRoadLayer,SIGNAL( currentIndexChanged(int) ), this, SLOT( setRoadTypeOptions(int)));		//道路组合框，回调函数
    
    connect(ui->mButtonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(createGridAbundance()));//初始化动作，回调函数
    connect(ui->mPushButtonDeleteAllGrid, SIGNAL(clicked()), this, SLOT(cleartGird()));							//清除战场区域画布，回调函数

	ui->label_5->setVisible(false);
	ui->mProgressBar->setVisible(false);
	ui->mProgressBar->setOrientation(Qt::Horizontal);
}

GridDialog::~GridDialog()
{
    delete ui;
    mMapToolExtent.release();
}

//添加图层
void GridDialog::setLayersOptions()
{
    if ( mCanvas)
    {
       int counter = mCanvas->layerCount();
       for ( int i = 0; i < counter ; ++i )
       {
           QString s=mCanvas->layers().at( i )->name();        //QMessageBox::information(this,"SUCCESS",s);
           ui->mComboBoxRoadLayer->addItem(s);
       }
       ui->mComboBoxRoadLayer->setCurrentText(QString::fromLocal8Bit("道路.shp"));
       ui->mComboBoxRoadLayer->setEnabled(false);
    }
}

//添加道路字段
void GridDialog::setRoadTypeOptions(int index)
{
    ui->mComboBoxRoadTypeField->clear();
	m_pRoadLayer = g_pRoadLayer;
    mCanvas->setCurrentLayer(m_pRoadLayer);
    for (int i = 0; i < m_pRoadLayer->fields().count(); ++i)
    {
        ui->mComboBoxRoadTypeField->addItem(m_pRoadLayer->fields()[i].name() );
    }
}
//初始化作战矩形区域，按钮动作，回调函数
void GridDialog::createGridAbundance()
{
    cleartGird();
    ui->mButtonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
	
    ModifyBoundance::mutexModify.lock();    
    double proportion  = 111319.5;	//米和纬度转换
    double sidelength  = ui->mLineEditSideLength->text().toDouble();//网格边长，单位米
    if (sidelength<49.0)
    {
         QMessageBox msgBox;
         msgBox.setText(QString::fromLocal8Bit("网格边长不能小于49米 ！！！"));
         msgBox.exec();
         return;
    }

	ui->label_5->setVisible(true);
	ui->mProgressBar->setVisible(true);
	this->update();
	Sleep(10);
	this->update();
	Sleep(10);
    double x = m_extent.xMinimum();//初始的作战矩形网格区域经度最小值
    double y = m_extent.yMinimum();//初始的作战矩形网格区域纬度最小值

    double sidelengthL= sidelength/ proportion;							//网格边长所占用的纬度值
    double sidelengthD= sidelength/ (proportion * cos(y*3.14/180.0));	//x

    double halfSidelengthL = 0.5 * sidelengthL;		//网络格高度
    double halfSidelengthD = 0.5 * sidelengthD ;	//网络格宽度
    long long rows = static_cast<long long>(std::ceil(m_extent.height()/ sidelengthL));
    long long cols = static_cast<long long>(std::ceil(m_extent.width() / sidelengthD));

	//计算需要多少个网格对象
    for (long long row = 0; row < rows - 1; row++)
    {
         y  = m_extent.yMinimum()  +  row * sidelengthL;//网格所在纬度
        for ( long long col = 0; col < cols - 1; col++)
        {
            x = m_extent.xMinimum() + col * sidelengthD;//网格所在经度
            GridInfo gridinfo;
            gridinfo.col = col;
            gridinfo.row = row;
            gridinfo.plan_x = x;
            gridinfo.plan_y = y;
			m_nodes.append( gridinfo);
        }
    }

	g_pGirdLayer->startEditing();//设置全局网格图层为可编辑模式

    for (int i=0; i<m_nodes.size(); i++)
    {
         QgsPoint centerPoint(m_nodes[i].plan_x +halfSidelengthD,  m_nodes[i].plan_y +halfSidelengthL);
         QgsRectangle centerSizeRect = QgsRectangle::fromCenterAndSize(centerPoint, sidelengthD, sidelengthL);
         QgsGeometry cellRect = QgsGeometry::fromRect( centerSizeRect);
         
		 mCanvas->setCurrentLayer(m_pRoadLayer);
         QgsFeatureIds	featureIds =  QgsMapToolSelectUtils::getMatchingFeatures(mCanvas, cellRect, false, false);
         QgsGeometry	pt= QgsGeometry::fromPointXY(centerPoint);
		 QgsFeature		f = QgsFeature();
         f.setGeometry(pt);
         
         double r = 0.01;
         double k = 0.01;
		 double q = conculateBundance(featureIds, &r, &k);
         f.setAttributes(QgsAttributes()<<i+1<<centerPoint.rx()<<centerPoint.ry()<< sidelength<<sidelength <<" "<<q<<q<<r<<k<<m_nodes[i].row <<m_nodes[i].col);
         mCanvas->setCurrentLayer(g_pGirdLayer);
		 g_pGirdLayer->addFeature(f);		 
		 ui->mProgressBar->setValue(i %100);
		 this->update();
    }

	g_pGirdLayer->commitChanges();
	g_pGirdLayer->triggerRepaint();
    ModifyBoundance::mutexModify.unlock();

	//在主画布上添加一个红色作战矩形区域
    QgsPointSequence points;
    points.push_back(QgsPoint(m_extent.xMinimum(), m_extent.yMinimum()));
    points.push_back(QgsPoint(m_extent.xMaximum(), m_extent.yMinimum()));
    points.push_back(QgsPoint(m_extent.xMaximum(), m_extent.yMaximum()));
    points.push_back(QgsPoint(m_extent.xMinimum(), m_extent.yMaximum()));
    points.push_back(QgsPoint(m_extent.xMinimum(), m_extent.yMinimum()));

    //QgsPolyline polyline(points);
    QgsGeometry rectGeometry = QgsGeometry::fromPolyline(QgsPolyline(points));
    QgsFeature f1 = QgsFeature();
    f1.setGeometry( rectGeometry);
    f1.setAttributes(QgsAttributes() << 10000000 <<10000000 << 10000000   <<1 << 1 << 1);

	g_pLineLayer->startEditing();
	g_pLineLayer->addFeature(f1);	//在主画布的，线图层上添加一个作战矩形区域
	g_pLineLayer->commitChanges();
	g_pLineLayer->triggerRepaint();
    ui->mButtonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
	ui->label_5->setVisible(false);
	ui->mProgressBar->setVisible(false);
}

void GridDialog::cleartGird()
{
	m_nodes.clear();

    ui->mButtonBox->button(QDialogButtonBox::Apply)->setEnabled(false);
    ui->mPushButtonDeleteAllGrid->setEnabled(false);
    ModifyBoundance::mutexModify.lock();
    
	//清除全局-网格光栅图层内，所有图元
	g_pGirdLayer->startEditing();
	QgsFeatureIds ids = g_pGirdLayer->allFeatureIds();
	g_pGirdLayer->deleteFeatures(ids);
	g_pGirdLayer->commitChanges();
	g_pGirdLayer->triggerRepaint();
	ModifyBoundance::mutexModify.unlock();

    //清除全局-线图层内，所有图元
	g_pLineLayer->startEditing();
    g_pLineLayer->deleteFeatures(g_pLineLayer->allFeatureIds());
	g_pLineLayer->commitChanges();
	g_pLineLayer->triggerRepaint();
    
    ui->mButtonBox->button(QDialogButtonBox::Apply)->setEnabled(true);
    ui->mPushButtonDeleteAllGrid->setEnabled(true);
}

void GridDialog::setOutputExtent(const QgsRectangle &r, const QgsCoordinateReferenceSystem &srcCrs, GridDialog::ExtentState state)
{
	m_extent = r;
	int decimals =5;//经纬度后面小数点保留位数
	
	QString condensed = QStringLiteral("%1,%2,%3,%4").arg(QString::number(m_extent.xMinimum(), 'f', decimals),
		QString::number(m_extent.yMinimum(), 'f', decimals),
		QString::number(m_extent.xMaximum(), 'f', decimals),
		QString::number(m_extent.yMaximum(), 'f', decimals));

	//condensed += QStringLiteral(" [%1]").arg(mOutputCrs.userFriendlyIdentifier(QgsCoordinateReferenceSystem::ShortString));
	ui->mLineEditExtent->setText(condensed);	//设置矩形区域的坐标，左上，右下

	mExtentState = state;
	if (!mIsValid)
		setValid(true);

	emit extentChanged(m_extent);
}

void GridDialog::setValid(bool valid)
{
    if ( valid == mIsValid )
      return;

    mIsValid = valid;
    emit validationChanged( mIsValid );
}

double GridDialog::conculateBundance(QgsFeatureIds ids ,double* r, double* k)
{
     double result = 0.0;
     double resultR = 1.0;
     double resultK = 1.0;
     QgsFields fields = QgsFields(m_pRoadLayer->fields());

     foreach (const QgsFeatureId &value, ids)
     {
         double rowResult = 0.0;
         double rowR = 1.0;
         double rowK = 1.0;

       ///  QString roadtypefieldname = ui->mComboBoxRoadTypeField->currentText();
 #ifndef TAIWAN
         QString roadtypefieldname = "class";
#else
		QString roadtypefieldname = "code";
#endif
         //highway: trunk, secondary, tertiary, residential, service, path, unclassified
         int fieldindex =   fields.lookupField( roadtypefieldname );
         QgsFeatureIterator it = m_pRoadLayer->getFeatures( QgsFeatureRequest( value) );
         QgsFeatureMap featureMap;
         QgsFeature feature;
         while ( it.nextFeature( feature ) )
         {
          // QgsVectorLayerUtils::matchAttributesToFields( feature, fields );
           featureMap[feature.id()] = feature;
         }

         QString highway = feature.attribute(fieldindex).toString();

#ifdef TAIWAN
         if ( "5153" == highway )
         {
            rowResult = 0.2;
            rowR      = 7.0;
            rowK      = 7.0;
         }
         if ( "5121" == highway  )
         {
            rowResult = 0.2;
            rowR      = 7.0;
            rowK      = 7.0;
         }
         if ( "5154" == highway )
         {
            rowResult = 0.1;
            rowR      = 5.0;
            rowK      = 6.0;
         }
         if ( "5141" == highway )
         {
            rowResult =  0.05;
            rowR      = 3.0;
            rowK      = 5.0;
         }
         if ( "5152" == highway )
         {
             rowResult = 0.2;
             rowR      = 5.0;
             rowK      = 8.0;
          }
#else
         if (QString::fromLocal8Bit("高速路")== highway)
         {
            rowResult = 0.3;
            rowR      = 9.0;
            rowK      = 9.0;

         }
         if (QString::fromLocal8Bit("县道")== highway )
         {
            rowResult = 0.2;
            rowR      = 7.0;
            rowK      = 7.0;
         }
         if (QString::fromLocal8Bit("省道") == highway)
         {
            rowResult = 0.2;
            rowR      = 7.0;
            rowK      = 7.0;
         }
         if (QString::fromLocal8Bit("国道") == highway)
         {
            rowResult = 0.2;
            rowR      = 7.0;
            rowK      = 7.0;
         }
         if (QString::fromLocal8Bit("乡道") == highway )
         {
            rowResult = 0.1;
            rowR      = 5.0;
            rowK      = 6.0;
         }

		 if (QString::fromLocal8Bit("乡道") == highway)
		 {
			 rowResult = 0.1;
			 rowR = 5.0;
			 rowK = 6.0;
		 }
		 
         if (QString::fromLocal8Bit("其他路")== highway )
         {
            rowResult =  0.05;
            rowR      = 3.0;
            rowK      = 5.0;
         }
         if (QString::fromLocal8Bit("停车场")== highway )
         {
             rowResult = 0.2;
             rowR      = 5.0;
             rowK      = 8.0;
          }
#endif
         result  = fmax( result, rowResult );
         resultR = fmax( resultR,rowR );
         resultK = fmax( resultK,rowK );
     }
     *r = resultR;
     *k = resultK;
     return  result;
}
//战场画布时候，回调函数
void GridDialog::setOutExtentFromDrawOnCanvas()
{
    if (mCanvas)
    {
		mMapToolPrevious = mCanvas->mapTool();
		if (!mMapToolExtent)
		{
			mMapToolExtent.reset(new QgsMapToolExtent(mCanvas));
			connect(mMapToolExtent.get(), &QgsMapToolExtent::extentChanged, this, &GridDialog::extentDrawn); //设置初始化作战区域完成后，鼠标抬起，信号回调函数
			connect(mMapToolExtent.get(), &QgsMapTool::deactivated, this, [=]
			{
				emit toggleDialogVisibility(true);
				mMapToolPrevious = nullptr;
			});
		}
		mMapToolExtent->setRatio(mRatio);
		mCanvas->setMapTool(mMapToolExtent.get());

		emit toggleDialogVisibility(false);
		showMinimized();					//把当前窗口最小化
	}
}

//设置作战区域，设置完成后，鼠标抬起时候，信号回调函数
void GridDialog::extentDrawn(const QgsRectangle &extent)
{
    setOutputExtent(extent, mCanvas->mapSettings().destinationCrs(), DrawOnCanvas );
    mMapToolPrevious = nullptr;
    mCanvas->unsetMapTool( mMapToolExtent.get() );
    showNormal();
    raise();
    activateWindow();
    emit toggleDialogVisibility( true );
}
