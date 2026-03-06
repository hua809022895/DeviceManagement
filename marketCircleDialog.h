#ifndef MARKETCIRCLEDIALOG_H
#define MARKETCIRCLEDIALOG_H

#include <QDialog>
#include <qgscircle.h>
#include <qgsfeature.h>

namespace Ui {
class MarketCircleDialog;
}

class MarketCircleDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MarketCircleDialog(QWidget *parent = nullptr);
    ~MarketCircleDialog();
    void setPositionAndR( QString pos, QString r );
public:
    QgsCircle mCircle;
    bool isinsert = true;
    QgsFeature mFeature;
    void setupdatefeature(QgsFeature& feature);
private:
    Ui::MarketCircleDialog *ui;
public slots:
    void saveCircle();

};

#endif // MARKETCIRCLEDIALOG_H
