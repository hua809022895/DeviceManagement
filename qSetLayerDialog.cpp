#include "stdafx.h"
#include "qsetlayerdialog.h"
#include "ui_qsetlayerdialog.h"
#include <qgslayertreeview.h>
#include <qgsproject.h>
#include <qgslayertreemodel.h>
#include <qgslayertreemapcanvasbridge.h>

QSetLayerDialog::QSetLayerDialog(QWidget *parent) :QDialog(parent),
    ui(new Ui::QSetLayerDialog)
{
    ui->setupUi(this);
}

QSetLayerDialog::QSetLayerDialog( QWidget *parent, QgsMapCanvas *mapCavas ):QDialog( parent ),
    mQgsMapcavas( mapCavas ),
    ui( new Ui::QSetLayerDialog)
{
    ui->setupUi( this );
    showLayers();
}

QSetLayerDialog::~QSetLayerDialog()
{
    delete ui;
}

void QSetLayerDialog::showLayers()
{
    QgsLayerTreeView* m_layerTreeView = new QgsLayerTreeView( this );
    m_layerTreeView->setObjectName( QStringLiteral( "图层" ) );

    QgsLayerTreeModel* model = new QgsLayerTreeModel( QgsProject::instance()->layerTreeRoot(), this );
    model->setFlag( QgsLayerTreeModel::AllowNodeReorder );
    model->setFlag( QgsLayerTreeModel::AllowNodeChangeVisibility );

    m_layerTreeView->setModel( model );
    ui->mGridLayout->addWidget(m_layerTreeView );

     //连接地图画布和图层管理器
    QgsRectangle qgsRect =  mQgsMapcavas->extent();

    //QgsLayerTreeMapCanvasBridge* m_pCanvasBridge = new QgsLayerTreeMapCanvasBridge(QgsProject::instance()->layerTreeRoot(), mQgsMapcavas, this);
    mQgsMapcavas->setExtent(qgsRect);
}

