#ifndef LAYERTREEVIEWMENUPROVIDER_H
#define LAYERTREEVIEWMENUPROVIDER_H

#include <QtCore>
#include <QObject>

#include <qgslayertreeview.h>
#include <qgsmaplayer.h>
#include "qgsvectorlayer.h"
#include "qgslayertreenode.h"
#include "qgslayertreegroup.h"
class QAction;
class QgsMapCanvas;

#define POINT_LAYER     "临时点层"
#define LINE_LAYER      "临时线层"
#define POLY_LAYER      "临时面层"

class layerTreeViewMenuProvider : public QObject, public QgsLayerTreeViewMenuProvider
{
    Q_OBJECT

public:
    layerTreeViewMenuProvider( QgsLayerTreeView *view, QgsMapCanvas* canvas );
    ~layerTreeViewMenuProvider() override;
    QMenu* createContextMenu() override;
public slots:
    void click_remove();
signals:
    void removelayer();
protected:

    void addCustomLayerActions( QMenu* menu, QgsMapLayer* layer );
    QgsLayerTreeView* mView;
    QgsMapCanvas* mCanvas;
};

#endif // LAYERTREEVIEWMENUPROVIDER_H
