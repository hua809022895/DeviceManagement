#ifndef MARKETPOINTDIALOG_H
#define MARKETPOINTDIALOG_H

#include <QDialog>
#include <qgsvectorlayer.h>
#include <qgsmapcanvas.h>

namespace Ui {
class MarketPointDialog;
}

class MarketPointDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MarketPointDialog(QWidget *parent = nullptr);
    ~MarketPointDialog();

public:
    QgsFeature mfeature;
    void setFeature(QgsFeature& feature);
    void setPosition();
    bool isinsert = true;
    double x=0,y=0;
    QgsMapCanvas* canvas;

private:
    Ui::MarketPointDialog *ui;
public:
    QString mId,mName,mClass,mPostion;
public slots:
    void savePoint();
};

#endif // MARKETPOINTDIALOG_H
