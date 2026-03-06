#pragma once

#include <QDialog>
#include <QListWidget>
#include <QgsMapCanvas.h>
#include <qgslayertreeview.h>

class DlgMapManager : public QDialog
{
    Q_OBJECT
public:
    explicit DlgMapManager(QgsMapCanvas* canvas,
                           const QString& currentBasemap,
                           QWidget* parent = nullptr);

    void setCurrentBasemap(const QString& path);

signals:
    void switchBaseMapRequested(const QString& filePath, const QString& layerName);
    void loadOnlineMapRequested(const QString& url,      const QString& layerName);
    void loadShpRequested();

private slots:
    void onScanLocal();
    void onSwitchLocal();
    void onLoadOnline();

private:
    void buildUi();
    void scanLocalMaps();
    void setupOnlineMaps();

    QgsMapCanvas*      m_canvas;
    QString            m_currentBasemap;

    QListWidget*       m_localList   = nullptr;
    QListWidget*       m_onlineList  = nullptr;
    QgsLayerTreeView*  m_layerTree   = nullptr;
};
