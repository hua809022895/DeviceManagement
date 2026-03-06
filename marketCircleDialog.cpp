#include "stdafx.h"
#include "marketCircleDialog.h"
#include "ui_marketcircledialog.h"
#include <qpushbutton.h>

#include "comm.h"
#include "qmessagebox.h"
#include <qgsvectorlayer.h>
extern QTextCodec *pCode;

MarketCircleDialog::MarketCircleDialog(QWidget *parent):QDialog(parent),
    ui(new Ui::MarketCircleDialog)
{
    ui->setupUi(this);//this->setWindowTitle("圆形标注设置");
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(pCode->toUnicode("确定"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(pCode->toUnicode("取消"));
    
	connect(ui->buttonBox->button(QDialogButtonBox::Ok) , &QAbstractButton::clicked, this, &MarketCircleDialog::saveCircle);
    ui->mDateTimeEditBegin->setDateTime(QDateTime::currentDateTime());
    ui->mDateTimeEditEnd->setDateTime(QDateTime::currentDateTime());

    ui->mComboBoxClass->setCurrentText(pCode->toUnicode("禁飞区"));
}

MarketCircleDialog::~MarketCircleDialog()
{
    delete ui;
}

void MarketCircleDialog::setPositionAndR(QString pos, QString r)
{
    ui->mLineEditR->setText(r);
    ui->mLineEditPosition->setText(pos);
}

void MarketCircleDialog::setupdatefeature(QgsFeature &feature)
{
    mFeature = feature;
    QString id = feature.attribute("id").value<QString>();;
    QString name = feature.attribute("name").value<QString>();
    QString s_class = feature.attribute("class").value<QString>();
    QString s_postion = feature.attribute("position").value<QString>();
    QString height = feature.attribute("height").value<QString>();
    QString radius = feature.attribute("radius").value<QString>();
    QDateTime begintime =  QDateTime::fromString( feature.attribute("begintime").value<QString>(),"yyyy-MM-dd hh:mm:ss");
    QDateTime endtime =  QDateTime::fromString( feature.attribute("endtime").value<QString>(),"yyyy-MM-dd hh:mm:ss");

    ui->mLineEditId->setText( id );
    ui->mLineEditName->setText(name);
    ui->mComboBoxClass->setCurrentText(s_class);
    ui->mLineEditPosition->setText(s_postion);
    ui->mLineEditHeight->setText(height);
    ui->mLineEditR->setText(radius);
    ui->mDateTimeEditBegin->setDateTime(begintime);
    ui->mDateTimeEditEnd->setDateTime(endtime);
}
//保存按钮
void MarketCircleDialog::saveCircle()
{
	if (isinsert) {
		mFeature= QgsFeature();
		mCircle = QgsCircle();
	}

	double x = ui->mLineEditPosition->text().section(',', 0, 0).toDouble();
	double y = ui->mLineEditPosition->text().section(',', 1, 1).toDouble();
	double r = ui->mLineEditR->text().toDouble();
	mCircle.setCenter(QgsPoint(x, y));
	double factorUnits = QgsUnitTypes::fromUnitToUnitFactor(QgsUnitTypes::DistanceMeters, QgsUnitTypes::DistanceDegrees);
	r = r * factorUnits;
	mCircle.setRadius(r);

	QgsPointSequence criclpoints = mCircle.points(36);
	QVector<QPointF> vecortorF;
	for (int i = 0; i < criclpoints.count(); i++)
	{
		QPointF pointF(criclpoints[i].x(), criclpoints[i].y());
		vecortorF.push_back(pointF);
	}
	vecortorF.push_back(QPointF(criclpoints[0].x(), criclpoints[0].y()));
	const QPolygonF polygon(vecortorF);
	QgsGeometry tempgeometry = QgsGeometry::fromQPolygonF(polygon);
	mFeature.setGeometry(tempgeometry);

	mFeature.setAttributes(QgsAttributes() << ui->mLineEditId->text()
		<< ui->mComboBoxClass->currentText()
		<< ui->mLineEditName->text()
		<< ui->mLineEditPosition->text()
		<< ""
		<< ""
		<< ui->mLineEditHeight->text()
		<< ui->mLineEditR->text()
		<< ui->mDateTimeEditBegin->dateTime().toString("yyyy-MM-dd hh:mm:ss")
		<< ui->mDateTimeEditEnd->dateTime().toString("yyyy-MM-dd hh:mm:ss"));

	g_pPolygonLayer->startEditing();

	if (isinsert) {
		if (g_pPolygonLayer->addFeature(mFeature))
		{
			g_pPolygonLayer->commitChanges();
			g_pPolygonLayer->triggerRepaint();
		}
	}
	else {
		if (g_pPolygonLayer->updateFeature(mFeature))
		{
			g_pPolygonLayer->commitChanges();
			g_pPolygonLayer->triggerRepaint();
		}
	}
}
