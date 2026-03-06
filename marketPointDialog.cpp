#include "stdafx.h"
#include "marketPointDialog.h"
#include "ui_marketpointdialog.h"
#include <qpushbutton.h>
#include "comm.h"
#include <qmessageBox.h>

MarketPointDialog::MarketPointDialog(QWidget *parent) :QDialog(parent),
    ui(new Ui::MarketPointDialog)
{
    ui->setupUi(this);
    setWindowTitle(QString::fromLocal8Bit("点标注编辑"));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(QString::fromLocal8Bit("确定"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(QString::fromLocal8Bit("取消"));
    connect(  ui->buttonBox->button(QDialogButtonBox::Ok) , &QAbstractButton::clicked, this, &MarketPointDialog::savePoint );
}

MarketPointDialog::~MarketPointDialog()
{
    delete ui;
}

void MarketPointDialog::setFeature(QgsFeature &feature)
{
    if (isinsert)
    {
         ui->mLineEditPosition->setText( QString::number(x,'f',6)+QString(",")+QString::number(y,'f',6) );
    }else{
        mfeature = feature;
        mId = feature.attribute("id").value<QString>();;
        mName = feature.attribute("name").value<QString>();
        mClass = feature.attribute("class").value<QString>();
        mPostion = feature.attribute("position").value<QString>();

        ui->mLineEditId->setText( mId );
        ui->mLineEditName->setText( mName );
        ui->mComboBoxClass->setCurrentText( mClass );
        ui->mLineEditPosition->setText( QString::number(x,'f',6)+QString(",")+QString::number(y,'f',6) );
    }
}

void MarketPointDialog::setPosition()
{
    ui->mLineEditPosition->setText( QString::number(x,'f',6)+QString(",")+QString::number(y,'f',6) );
}

void MarketPointDialog::savePoint()
{
    if ( isinsert ){
        mfeature = QgsFeature();
    }

    x =  ui->mLineEditPosition->text().section(',',0,0).toFloat();
    y =  ui->mLineEditPosition->text().section(',',1,1).toFloat();
    QgsGeometry geometry = QgsGeometry::fromPointXY( QgsPointXY(x,y) );
    if ( ! geometry.isGeosValid() ) {
        QMessageBox::information(this,QString::fromLocal8Bit("提醒"), QString::fromLocal8Bit("无效坐标修改！"));
    }

    mfeature.setGeometry( geometry);

    /*zjrzjr
    if (isinsert){
        mfeature.setAttributes( QgsAttributes()<<ui->mLineEditId->text().toLongLong()
                                <<ui->mLineEditName->text()
                                <<ui->mComboBoxClass->currentText()
                                <<ui->mLineEditPosition->text()
                                <<""<<"");
        m_PointLayer->startEditing();
        m_PointLayer->addFeature( mfeature );
    }else{
        mfeature.setAttribute( "id",ui->mLineEditId->text().toLongLong() );
        mfeature.setAttribute( "name",ui->mLineEditName->text() );
        mfeature.setAttribute( "class",ui->mComboBoxClass->currentText() );
        mfeature.setAttribute( "position",ui->mLineEditPosition->text() );
        m_PointLayer->startEditing();
        m_PointLayer->updateFeature( mfeature );
    }

    m_PointLayer->commitChanges();//11008210322400
    m_PointLayer->triggerRepaint(); */
}
