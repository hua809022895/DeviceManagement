#include "stdafx.h"
#include "informationDetailDialog.h"
#include "ui_informationdetaildialog.h"

#include <QTextCodec>

InformationDetailDialog::InformationDetailDialog(QWidget *parent) :QDialog(parent),
    ui(new Ui::InformationDetailDialog)
{
    ui->setupUi(this);

	//settings.setIniCodec(QTextCodec::codecForName("UTF-8"));
    //QString types     = settings.value("WeaponType/WeaponTypelist").toString();
    //QString tasks     = settings.value("WeaponType/tasklist").toString();
    QStringList typelist = WeaponTypeList.split("|");
    QStringList tasklist = g_taskList.split("|");

     ui->comboBoxtype->setEditable(true);
     ui->comboBoxtype->addItems( typelist );
 }

InformationDetailDialog::~InformationDetailDialog()
{
    delete ui;
}

void InformationDetailDialog::setPicture()
{
    QString pos = QString("%1,%2").arg(QString::number(mappoint->x,'f',5)).arg(QString::number(mappoint->y,'f',5));
    ui->lineeditpostion->setText(pos);
    ui->lineedittime->setText(mappoint->planeTime);
    ui->comboBoxtype->setCurrentText( mappoint->weaponname); //武器类型
}

void InformationDetailDialog::resizeEvent(QResizeEvent *)
{
    scalePiture();
}
//打开helloPlane.db数据库中的objectImage表，并显示相应的图片记录信息
void InformationDetailDialog::scalePiture()
{
    QSqlQuery query(g_sqliteDbase);
    query.exec("select jpg from objectImage where messageId=" +mappoint->messageId);

     if (query.next())
     {
		 QByteArray jpg = query.value("jpg").toByteArray();
		 QPixmap pixmap;
		 pixmap.loadFromData(jpg, "jpg");

		 QPixmap pixmapcopy = pixmap.scaled(ui->mFrameBottom->width()-50, ui->mFrameBottom->height()-50);
		 ui->picture->setPixmap(pixmapcopy);
     }
     //mDatabase.close();
}

void InformationDetailDialog::initControl()
{
    ui->comboBoxtype->setCurrentText( mappoint->weaponname );
    connect( ui->comboBoxtype, &QComboBox::currentTextChanged, this, &InformationDetailDialog::setHandledResult );
}

void InformationDetailDialog::setHandledResult(const QString &text)
{
    mappoint->ishanded = text;
}
