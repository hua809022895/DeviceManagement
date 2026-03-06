#ifndef INFORMATIONDETAILDIALOG_H
#define INFORMATIONDETAILDIALOG_H

#include <QDialog>
#include "comm.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

namespace Ui {
class InformationDetailDialog;
}

class InformationDetailDialog : public QDialog
{
    Q_OBJECT
public:
    explicit InformationDetailDialog(QWidget *parent = nullptr);
    ~InformationDetailDialog();
public:
    Mappoint * mappoint = nullptr;
    void setPicture();
protected:
    virtual void resizeEvent(QResizeEvent *) override ;

public slots:
    void scalePiture();
    void initControl();
    void setHandledResult( const QString &text );


private:
    Ui::InformationDetailDialog *ui;
};

#endif // INFORMATIONDETAILDIALOG_H
