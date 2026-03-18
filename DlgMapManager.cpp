// DlgMapManager.cpp  (UTF-8 + BOM)
#include "stdafx.h"
#include "DlgMapManager.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QLabel>
#include <QPushButton>
#include <QFrame>
#include <QDirIterator>
#include <QFileInfo>
#include <QFont>
#include <QMessageBox>
#include <QWebEngineView>

#include <qgslayertreemodel.h>
#include <qgsproject.h>

// ---------------------------------------------------------------------------
// Leaflet 在线瓦片预设（同时设置 QGIS 底图 + Leaflet 瓦片）
// ---------------------------------------------------------------------------
struct TilePreset { const char* name; const char* url; const char* leafletUrl; const char* tip; };
static const TilePreset kOnlineMaps[] = {
    { "OpenStreetMap",
      "type=xyz&url=http://tile.openstreetmap.org/{z}/{x}/{y}.png",
      "http://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png",
      "" },
    { "\xe9\xab\x98\xe5\xbe\xb7\xe6\x99\xae\xe9\x80\x9a",   // 高德普通
      "type=xyz&url=https://webrd01.is.autonavi.com/appmaptile?lang=zh_cn&size=1&scale=1&style=7&x={x}&y={y}&z={z}",
      "https://webrd01.is.autonavi.com/appmaptile?lang=zh_cn&size=1&scale=1&style=7&x={x}&y={y}&z={z}",
      "" },
    { "\xe9\xab\x98\xe5\xbe\xb7\xe5\x8d\xab\xe6\x98\x9f",   // 高德卫星
      "type=xyz&url=https://webst01.is.autonavi.com/appmaptile?style=6&x={x}&y={y}&z={z}",
      "https://webst01.is.autonavi.com/appmaptile?style=6&x={x}&y={y}&z={z}",
      "" },
    { "ArcGIS \xe5\xbd\xb1\xe5\x83\x8f",                    // ArcGIS 影像
      "type=xyz&url=https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}",
      "https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}",
      "" },
    { "\xe5\xa4\xa9\xe5\x9c\xb0\xe5\x9b\xbe\xe7\x9f\xa2\xe9\x87\x8f",  // 天地图矢量
      "type=xyz&url=http://t0.tianditu.gov.cn/vec_w/wmts?SERVICE=WMTS&REQUEST=GetTile&VERSION=1.0.0&LAYER=vec&STYLE=default&TILEMATRIXSET=w&FORMAT=tiles&TILEMATRIX={z}&TILEROW={y}&TILECOL={x}&tk=your_key",
      "http://t0.tianditu.gov.cn/vec_w/wmts?SERVICE=WMTS&REQUEST=GetTile&VERSION=1.0.0&LAYER=vec&STYLE=default&TILEMATRIXSET=w&FORMAT=tiles&TILEMATRIX={z}&TILEROW={y}&TILECOL={x}&tk=your_key",
      "\xe9\x9c\x80\xe5\xb0\x86 your_key \xe6\x9b\xbf\xe6\x8d\xa2\xe4\xb8\xba\xe5\xa4\xa9\xe5\x9c\xb0\xe5\x9b\xbe API Key" }, // 需将 your_key 替换为天地图 API Key
};
static const int kOnlineCount = (int)(sizeof(kOnlineMaps) / sizeof(kOnlineMaps[0]));

// ---------------------------------------------------------------------------
// 3D 卫星影像源预设
// ---------------------------------------------------------------------------
struct Sat3DPreset { const char* name; const char* imgBase; const char* tip; };
static const Sat3DPreset k3DMaps[] = {
    { "ArcGIS \xe5\x8d\xab\xe6\x98\x9f\xe5\xbd\xb1\xe5\x83\x8f",  // ArcGIS 卫星影像
      "https://services.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/export",
      "\xe9\xbb\x98\xe8\xae\xa4\xe5\x8d\xab\xe6\x98\x9f\xe5\xbd\xb1\xe5\x83\x8f\xef\xbc\x8c\xe8\xa6\x86\xe7\x9b\x96\xe5\x85\xa8\xe7\x90\x83" }, // 默认卫星影像，覆盖全球
    { "\xe9\xab\x98\xe5\xbe\xb7\xe5\x8d\xab\xe6\x98\x9f",          // 高德卫星
      "https://webst01.is.autonavi.com/appmaptile?style=6&x={x}&y={y}&z={z}",
      "\xe4\xb8\xad\xe5\x9b\xbd\xe5\x9c\xb0\xe5\x8c\xba\xe6\xb8\x85\xe6\x99\xb0\xe5\xba\xa6\xe6\x9b\xb4\xe9\xab\x98" },  // 中国地区清晰度更高
};
static const int k3DCount = (int)(sizeof(k3DMaps) / sizeof(k3DMaps[0]));

// ---------------------------------------------------------------------------
DlgMapManager::DlgMapManager(QgsMapCanvas* canvas,
                             const QString& currentBasemap,
                             QWebEngineView* leafletView,
                             QWebEngineView* view3D,
                             QWidget* parent)
    : QDialog(parent), m_canvas(canvas), m_currentBasemap(currentBasemap),
      m_leafletView(leafletView), m_view3D(view3D)
{
    setWindowTitle(QString::fromLocal8Bit("地图管理"));
    setMinimumSize(480, 420);
    buildUi();
}

void DlgMapManager::buildUi()
{
    auto* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(6);

    auto* tabs = new QTabWidget(this);

    // ---- Tab 1: 2D 本地地图 ----
    auto* tabLocal = new QWidget();
    auto* vLocal   = new QVBoxLayout(tabLocal);
    vLocal->setContentsMargins(8, 8, 8, 8);
    vLocal->setSpacing(6);

    vLocal->addWidget(new QLabel(QString::fromLocal8Bit("已发现的 TIF 地图文件："), tabLocal));

    m_localList = new QListWidget(tabLocal);
    m_localList->setSelectionMode(QAbstractItemView::SingleSelection);
    vLocal->addWidget(m_localList, 1);

    auto* btnRowLocal = new QHBoxLayout();
    auto* btnScan     = new QPushButton(QString::fromLocal8Bit("扫描 / 刷新"), tabLocal);
    auto* btnSwitch   = new QPushButton(QString::fromLocal8Bit("切换为选中底图"), tabLocal);
    btnRowLocal->addWidget(btnScan);
    btnRowLocal->addStretch();
    btnRowLocal->addWidget(btnSwitch);
    vLocal->addLayout(btnRowLocal);

    auto* line = new QFrame(tabLocal);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    vLocal->addWidget(line);

    auto* btnShp = new QPushButton(QString::fromLocal8Bit("加载 SHP 矢量图层..."), tabLocal);
    vLocal->addWidget(btnShp);

    connect(btnScan,   &QPushButton::clicked, this, &DlgMapManager::onScanLocal);
    connect(btnSwitch, &QPushButton::clicked, this, &DlgMapManager::onSwitchLocal);
    connect(btnShp,    &QPushButton::clicked, this, &DlgMapManager::loadShpRequested);

    tabs->addTab(tabLocal, QString::fromLocal8Bit("2D 本地地图"));

    // ---- Tab 2: 在线 Leaflet ----
    auto* tabOnline = new QWidget();
    auto* vOnline   = new QVBoxLayout(tabOnline);
    vOnline->setContentsMargins(8, 8, 8, 8);
    vOnline->setSpacing(6);

    vOnline->addWidget(new QLabel(QString::fromLocal8Bit(
        "在线瓦片地图预设（仅设置 Leaflet 在线地图）："), tabOnline));

    m_onlineList = new QListWidget(tabOnline);
    m_onlineList->setSelectionMode(QAbstractItemView::SingleSelection);
    setupOnlineMaps();
    vOnline->addWidget(m_onlineList, 1);

    auto* btnOnline = new QPushButton(QString::fromLocal8Bit("设为底图"), tabOnline);
    vOnline->addWidget(btnOnline);
    connect(btnOnline, &QPushButton::clicked, this, &DlgMapManager::onSetLeafletTile);

    tabs->addTab(tabOnline, QString::fromLocal8Bit("在线 Leaflet"));

    // ---- Tab 3: 3D ----
    auto* tab3D = new QWidget();
    auto* v3D   = new QVBoxLayout(tab3D);
    v3D->setContentsMargins(8, 8, 8, 8);
    v3D->setSpacing(6);

    v3D->addWidget(new QLabel(QString::fromLocal8Bit(
        "3D 卫星影像源（下次加载 3D 地图时生效）："), tab3D));

    m_3dList = new QListWidget(tab3D);
    m_3dList->setSelectionMode(QAbstractItemView::SingleSelection);
    setup3DMaps();
    v3D->addWidget(m_3dList, 1);

    auto* btn3D = new QPushButton(QString::fromLocal8Bit("设为 3D 影像源"), tab3D);
    connect(btn3D, &QPushButton::clicked, this, &DlgMapManager::onSet3DSatellite);
    v3D->addWidget(btn3D);

    auto* line3D = new QFrame(tab3D);
    line3D->setFrameShape(QFrame::HLine);
    line3D->setFrameShadow(QFrame::Sunken);
    v3D->addWidget(line3D);

    v3D->addWidget(new QLabel(QString::fromLocal8Bit("地图缓存："), tab3D));
    m_cacheLabel = new QLabel(tab3D);
    m_cacheLabel->setWordWrap(true);
    v3D->addWidget(m_cacheLabel);
    updateCacheSize();

    v3D->addWidget(new QLabel(QString::fromLocal8Bit(
        "提示：缓存数据保存在软件目录 map_cache/ 下，\n"
        "可随软件整体拷贝到其他设备使用。"), tab3D));

    tabs->addTab(tab3D, QString::fromLocal8Bit("3D"));

    mainLayout->addWidget(tabs, 1);

    // ---- 关闭按钮 ----
    auto* btnRow  = new QHBoxLayout();
    auto* btnClose = new QPushButton(QString::fromLocal8Bit("关闭"), this);
    btnRow->addStretch();
    btnRow->addWidget(btnClose);
    mainLayout->addLayout(btnRow);
    connect(btnClose, &QPushButton::clicked, this, &QDialog::hide);

    // 初始扫描
    scanLocalMaps();
}

// ---------------------------------------------------------------------------
void DlgMapManager::scanLocalMaps()
{
    m_localList->clear();
    QString appDir = QCoreApplication::applicationDirPath();

    QDirIterator it(appDir, QStringList() << "*.tif" << "*.tiff" << "*.TIF" << "*.TIFF",
                    QDir::Files, QDirIterator::Subdirectories);

    while (it.hasNext()) {
        it.next();
        QFileInfo fi = it.fileInfo();

        // 限制深度：相对路径层数 ≤ 3
        QString rel = QDir(appDir).relativeFilePath(fi.absoluteFilePath());
        if (rel.count('/') > 3) continue;

        QString label = fi.completeBaseName();
        bool isCurrent = (fi.absoluteFilePath() == m_currentBasemap);
        if (isCurrent)
            label += QString::fromLocal8Bit("  ★ 当前");

        auto* item = new QListWidgetItem(label, m_localList);
        item->setData(Qt::UserRole, fi.absoluteFilePath());
        item->setToolTip(rel);

        if (isCurrent) {
            QFont f = item->font();
            f.setBold(true);
            item->setFont(f);
        }
    }

    if (m_localList->count() == 0)
        m_localList->addItem(QString::fromLocal8Bit("（未找到 TIF 文件，请将地图放在程序目录下）"));
}

// ---------------------------------------------------------------------------
void DlgMapManager::setupOnlineMaps()
{
    for (int i = 0; i < kOnlineCount; ++i) {
        auto* item = new QListWidgetItem(QString::fromUtf8(kOnlineMaps[i].name), m_onlineList);
        item->setData(Qt::UserRole,     QString::fromUtf8(kOnlineMaps[i].url));       // QGIS wms URL
        item->setData(Qt::UserRole + 1, QString::fromUtf8(kOnlineMaps[i].name));
        item->setData(Qt::UserRole + 2, QString::fromUtf8(kOnlineMaps[i].leafletUrl)); // Leaflet URL
        if (kOnlineMaps[i].tip[0])
            item->setToolTip(QString::fromUtf8(kOnlineMaps[i].tip));
    }
}

// ---------------------------------------------------------------------------
void DlgMapManager::setup3DMaps()
{
    for (int i = 0; i < k3DCount; ++i) {
        auto* item = new QListWidgetItem(QString::fromUtf8(k3DMaps[i].name), m_3dList);
        item->setData(Qt::UserRole,     QString::fromUtf8(k3DMaps[i].imgBase));
        item->setData(Qt::UserRole + 1, QString::fromUtf8(k3DMaps[i].name));
        item->setToolTip(QString::fromUtf8(k3DMaps[i].tip));
    }
    // 默认选中第一项
    if (m_3dList->count() > 0)
        m_3dList->setCurrentRow(0);
}

// ---------------------------------------------------------------------------
void DlgMapManager::updateCacheSize()
{
    QString cacheDir = QCoreApplication::applicationDirPath() + "/map_cache";
    qint64 totalSize = 0;
    int fileCount = 0;
    QDirIterator it(cacheDir, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        it.next();
        totalSize += it.fileInfo().size();
        fileCount++;
    }
    double mb = totalSize / (1024.0 * 1024.0);
    m_cacheLabel->setText(QString::fromLocal8Bit("缓存路径：map_cache/\n"
        "缓存大小：%1 MB（%2 个文件）")
        .arg(mb, 0, 'f', 1).arg(fileCount));
}

// ---------------------------------------------------------------------------
void DlgMapManager::setCurrentBasemap(const QString& path)
{
    m_currentBasemap = path;
    scanLocalMaps();   // 刷新列表标注
    updateCacheSize();
}

// ---------------------------------------------------------------------------
void DlgMapManager::onScanLocal()
{
    scanLocalMaps();
}

void DlgMapManager::onSwitchLocal()
{
    auto* item = m_localList->currentItem();
    if (!item) return;
    QString path = item->data(Qt::UserRole).toString();
    if (path.isEmpty()) return;

    QFileInfo fi(path);
    emit switchBaseMapRequested(path, fi.completeBaseName());
}

// ---------------------------------------------------------------------------
void DlgMapManager::onSetLeafletTile()
{
    auto* item = m_onlineList->currentItem();
    if (!item) return;
    QString leafletUrl = item->data(Qt::UserRole + 2).toString();
    if (leafletUrl.isEmpty()) return;

    // 设置 Leaflet 瓦片 URL 并切换到 Leaflet 模式
    if (m_leafletView) {
        QString js = QString("if(typeof setTileUrl==='function')setTileUrl('%1');")
            .arg(leafletUrl);
        m_leafletView->page()->runJavaScript(js);
    }
    emit switchMapModeRequested(1); // 切换到 Leaflet 模式
}

// ---------------------------------------------------------------------------
void DlgMapManager::onSet3DSatellite()
{
    auto* item = m_3dList->currentItem();
    if (!item) return;
    QString imgBase = item->data(Qt::UserRole).toString();
    QString name    = item->data(Qt::UserRole + 1).toString();
    if (imgBase.isEmpty()) return;

    // 更新 3D 视图中的影像源（需要 3D 页面重新加载才能生效）
    if (m_view3D) {
        QString js = QString("if(typeof _IMG_BASE!=='undefined')_IMG_BASE='%1';").arg(imgBase);
        m_view3D->page()->runJavaScript(js);
    }

    QMessageBox::information(this, QString::fromLocal8Bit("3D 影像源"),
        QString::fromLocal8Bit("已设置 3D 影像源为：%1\n下次切换到 3D 视图时生效。").arg(name));
}
