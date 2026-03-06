#ifndef QSETLAYERDIALOG_H
#define QSETLAYERDIALOG_H

#include <QDialog>
#include <qgsmapcanvas.h>

namespace Ui {
class QSetLayerDialog;
}

class QSetLayerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QSetLayerDialog(QWidget *parent = nullptr);
    explicit QSetLayerDialog(QWidget *parent ,QgsMapCanvas* mapCavas);
    ~QSetLayerDialog();

    QgsMapCanvas *mQgsMapcavas;
    void showLayers();
private:
    Ui::QSetLayerDialog *ui;
};

#endif // QSETLAYERDIALOG_H
