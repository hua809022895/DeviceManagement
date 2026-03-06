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

#include <qgslayertreemodel.h>
#include <qgsproject.h>

// ---------------------------------------------------------------------------
// 在线瓦片预设
// ---------------------------------------------------------------------------
struct TilePreset { const char* name; const char* url; const char* tip; };
static const TilePreset kOnlineMaps[] = {
    { "OpenStreetMap",
      "type=xyz&url=http://tile.openstreetmap.org/{z}/{x}/{y}.png",
      "" },
    { "\xe9\xab\x98\xe5\xbe\xb7\xe6\x99\xae\xe9\x80\x9a",   // 高德普通
      "type=xyz&url=https://webrd01.is.autonavi.com/appmaptile?lang=zh_cn&size=1&scale=1&style=7&x={x}&y={y}&z={z}",
      "" },
    { "\xe9\xab\x98\xe5\xbe\xb7\xe5\x8d\xab\xe6\x98\x9f",   // 高德卫星
      "type=xyz&url=https://webst01.is.autonavi.com/appmaptile?style=6&x={x}&y={y}&z={z}",
      "" },
    { "ArcGIS \xe5\xbd\xb1\xe5\x83\x8f",                    // ArcGIS 影像
      "type=xyz&url=https://server.arcgisonline.com/ArcGIS/rest/services/World_Imagery/MapServer/tile/{z}/{y}/{x}",
      "" },
    { "\xe5\xa4\xa9\xe5\x9c\xb0\xe5\x9b\xbe\xe7\x9f\xa2\xe9\x87\x8f",  // 天地图矢量
      "type=xyz&url=http://t0.tianditu.gov.cn/vec_w/wmts?SERVICE=WMTS&REQUEST=GetTile&VERSION=1.0.0&LAYER=vec&STYLE=default&TILEMATRIXSET=w&FORMAT=tiles&TILEMATRIX={z}&TILEROW={y}&TILECOL={x}&tk=your_key",
      "\xe9\x9c\x80\xe5\xb0\x86 your_key \xe6\x9b\xbf\xe6\x8d\xa2\xe4\xb8\xba\xe5\xa4\xa9\xe5\x9c\xb0\xe5\x9b\xbe API Key" }, // 需将 your_key 替换为天地图 API Key
};
static const int kOnlineCount = (int)(sizeof(kOnlineMaps) / sizeof(kOnlineMaps[0]));

// ---------------------------------------------------------------------------
DlgMapManager::DlgMapManager(QgsMapCanvas* canvas,
                             const QString& currentBasemap,
                             QWidget* parent)
    : QDialog(parent), m_canvas(canvas), m_currentBasemap(currentBasemap)
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

    // ---- Tab 1: 本地底图 ----
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

    tabs->addTab(tabLocal, QString::fromLocal8Bit("本地底图"));

    // ---- Tab 2: 在线地图 ----
    auto* tabOnline = new QWidget();
    auto* vOnline   = new QVBoxLayout(tabOnline);
    vOnline->setContentsMargins(8, 8, 8, 8);
    vOnline->setSpacing(6);

    vOnline->addWidget(new QLabel(QString::fromLocal8Bit("在线瓦片地图预设（需联网）："), tabOnline));

    m_onlineList = new QListWidget(tabOnline);
    m_onlineList->setSelectionMode(QAbstractItemView::SingleSelection);
    setupOnlineMaps();
    vOnline->addWidget(m_onlineList, 1);

    auto* btnOnline = new QPushButton(QString::fromLocal8Bit("设为底图"), tabOnline);
    vOnline->addWidget(btnOnline);
    connect(btnOnline, &QPushButton::clicked, this, &DlgMapManager::onLoadOnline);

    tabs->addTab(tabOnline, QString::fromLocal8Bit("在线地图"));

    // ---- Tab 3: 图层管理 ----
    auto* tabLayers = new QWidget();
    auto* vLayers   = new QVBoxLayout(tabLayers);
    vLayers->setContentsMargins(4, 4, 4, 4);

    m_layerTree = new QgsLayerTreeView(tabLayers);
    QgsLayerTreeModel* model = new QgsLayerTreeModel(
        QgsProject::instance()->layerTreeRoot(), m_layerTree);
    model->setFlag(QgsLayerTreeModel::AllowNodeChangeVisibility);
    m_layerTree->setModel(model);
    vLayers->addWidget(m_layerTree);

    tabs->addTab(tabLayers, QString::fromLocal8Bit("图层管理"));

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
        item->setData(Qt::UserRole,     QString::fromUtf8(kOnlineMaps[i].url));
        item->setData(Qt::UserRole + 1, QString::fromUtf8(kOnlineMaps[i].name));
        if (kOnlineMaps[i].tip[0])
            item->setToolTip(QString::fromUtf8(kOnlineMaps[i].tip));
    }
}

// ---------------------------------------------------------------------------
void DlgMapManager::setCurrentBasemap(const QString& path)
{
    m_currentBasemap = path;
    scanLocalMaps();   // 刷新列表标注
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

void DlgMapManager::onLoadOnline()
{
    auto* item = m_onlineList->currentItem();
    if (!item) return;
    QString url  = item->data(Qt::UserRole).toString();
    QString name = item->data(Qt::UserRole + 1).toString();
    if (url.isEmpty()) return;
    emit loadOnlineMapRequested(url, name);
}
