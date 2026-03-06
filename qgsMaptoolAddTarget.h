#ifndef QGSMAPTOOLADDTARGET_H
#define QGSMAPTOOLADDTARGET_H

#include "qgsmaptooldigitizefeature.h"

class QgsMapToolAddTarget : public QgsMapToolDigitizeFeature
{
    Q_OBJECT
public:
    QgsMapToolAddTarget( QgsMapCanvas *canvas, CaptureMode mode, QgsAdvancedDigitizingDockWidget* mDock);

     void cadCanvasPressEvent( QgsMapMouseEvent *e ) override;

};

#endif // QGSMAPTOOLADDTARGET_H
