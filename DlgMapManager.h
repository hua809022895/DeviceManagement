#pragma once

#include <QDialog>
#include <QListWidget>
#include <QLabel>
#include <QgsMapCanvas.h>
#include <qgslayertreeview.h>

class QWebEngineView;

class DlgMapManager : public QDialog
{
    Q_OBJECT
public:
    explicit DlgMapManager(QgsMapCanvas* canvas,
                           const QString& currentBasemap,
                           QWebEngineView* leafletView,
                           QWebEngineView* view3D,
                           QWidget* parent = nullptr);

    void setCurrentBasemap(const QString& path);

signals:
    void switchBaseMapRequested(const QString& filePath, const QString& layerName);
    void loadOnlineMapRequested(const QString& url,      const QString& layerName);
    void loadShpRequested();
    void switchMapModeRequested(int mode); // 0=QGIS 1=Leaflet 2=3D

private slots:
    void onScanLocal();
    void onSwitchLocal();
    void onSetLeafletTile();
    void onSet3DSatellite();

private:
    void buildUi();
    void scanLocalMaps();
    void setupOnlineMaps();
    void setup3DMaps();
    void updateCacheSize();

    QgsMapCanvas*      m_canvas;
    QString            m_currentBasemap;
    QWebEngineView*    m_leafletView = nullptr;
    QWebEngineView*    m_view3D      = nullptr;

    QListWidget*       m_localList   = nullptr;
    QListWidget*       m_onlineList  = nullptr;
    QListWidget*       m_3dList      = nullptr;
    QLabel*            m_cacheLabel  = nullptr;
};
