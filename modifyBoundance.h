#ifndef MODIFYBOUNDANCE_H
#define MODIFYBOUNDANCE_H
#include <QThread>
#include "comm.h"
#include <qgsvectorlayer.h>
#include <qgsmapcanvas.h>

//auto increase boudance
class ModifyBoundance : public QThread
{
    Q_OBJECT
public:
    ModifyBoundance(QgsVectorLayer* gridLayer, QgsMapCanvas* mapCavas);

    static  QVector< QgsPointXY> messages;
    static  QMutex mutexMessages;
    static  QMutex mutexModify;

public slots:
    void handlePlaneMessage(const QgsPointXY & qgsPoint);
private:
    QgsMapCanvas	*mQgsMapCanvas;
    QgsVectorLayer	*mGridLayer;

    void handlePlaneZeroMessage(const QgsPointXY &qgsPoint);
protected:
    void run() override;

};

#endif // MODIFYBOUNDANCE_H
