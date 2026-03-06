#include "stdafx.h"
#include "dialogSendGrid.h"
#include "ui_dialogsendgrid.h"
#include <QPushButton>

DialogSendGrid::DialogSendGrid(int* sendtype,QWidget *parent) :QDialog(parent),
    ui(new Ui::DialogSendGrid)
{
    ui->setupUi(this);

    setWindowTitle(QString::fromLocal8Bit("情报丰度发送"));
    ui->mComboBoxMapSendOption->addItem(QString::fromLocal8Bit("1给飞机重置"));
    ui->mComboBoxMapSendOption->addItem(QString::fromLocal8Bit("2仅发送给地面站"));

    mSendtype = sendtype;
    QPushButton* button = ui->buttonBox->button(QDialogButtonBox::Ok);
    button->setText(QString::fromLocal8Bit("确定"));
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(QString::fromLocal8Bit("取消"));
    connect(button,SIGNAL( clicked() ),this, SLOT( setSendType()));
}

DialogSendGrid::~DialogSendGrid()
{
    delete ui;
}

void DialogSendGrid::setSendType()
{
    *mSendtype = ui->mComboBoxMapSendOption->currentIndex();
}
