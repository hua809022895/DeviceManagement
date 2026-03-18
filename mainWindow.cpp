#include "stdafx.h"

#include "mainWindow.h"
#include "ui_mainwindow.h"

#include <imagehlp.h>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cstring>

#include <QComboBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QSplitter>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QTextcodec.h>
#include <QtWebView/QtWebView>
#include <QWebEngineProfile>

#include <qgsTransaction.h>
#include <qgsTransactionGroup.h>
#include <qgsMessageViewer.h>
#include <qgsLayerTreemodel.h>
#include <qgsLayerTreenode.h>
#include <qgsLayerTreegroup.h>
#include <qgsLayerTreemapCanvasBridge.h>
#include <qgsDataitem.h>
#include <qgsVectorLayerutils.h>
#include <qgsProviderRegistry.h>
#include <qgsSymbolSelectorDialog.h>
#include <qgsSingleSymbolRenderer.h>
#include <qgsSymbol.h>
#include <qgsStyle.h>
#include <qgsFileutils.h>
#include <qgsZiputils.h>
#include <qgsEditorWidgetsetup.h>
#include <qgsFieldFormatter.h>
#include <qgsEditorWidgetRegistry.h>
#include <qgsWeakRelation.h>
#include <qgsProviderMetadata.h>
#include <qgsFieldFormatterRegistry.h>
#include <qgsVectorLayerlabeling.h>
#include <qgsMarkerSymbollayer.h>
//#include <qgsExpressionContextutils.h>
#include <qgsExpressionContext.h>
#include <qgsFilterLineedit.h>
#include <qgsHeatMapRenderer.h>
#include <qgsLineSymbollayer.h>
#include <qgsLayerTree.h>
#include <qgsAnnotationManager.h>
#include <qgsAnnotationMarkerItem.h>
//#include <Python.h>
//#include "QgsMapLayerFactory.h"

#include <chrono>
#include <thread>

#include "qgsMapCanvasAnnotationitem2.h"
#include "jpgAnnotation.h"
#include "qgsSvgAnnotation.h"
#include "qdialogTarget.h"
#include "dialogSendGrid.h"
#include "layerTreeViewMenuProvider.h"
#include "qSetLayerDialog.h"
#include "ddsThread.h"
#include "dataBackDlg.h"
#include "mavlink5Thread.h"
#include "recMavlinkThread.h"
#include "TransitThread.h"
#include "qUdpThread.h"
#include "Biaopai.h"
#include "FixPlaneThread.h"
#include "qgsmapCanvasZjr.h"
#include "moc_MainWindow.cpp"
#include "QsetRadarDlg.h"
#include "dlgAddTaskArea.h"
#include "DlgEquipment.h"
#include "clipper2/include/clipper.h"

#include "Dlg3Dtest.h"
#include "DlgTuili.h"
#include "TuiliThread.h"
#include "DlgMapManager.h"

#pragma comment(lib, "imagehlp.lib")
#pragma comment(lib, "Clipper2_64.lib")

std::vector<GlobalTask> stlGlobalTask;
using namespace std::chrono;
using namespace Clipper2Lib;

MainWindow* MainWindow::sInstance = nullptr;
//QMessageBox::information(this,"SUCCESS","情报丰度读取成功!");
QTextCodec *pCode = QTextCodec::codecForName("GBK");
//PyObject	*pModule = NULL;
//PyObject	*pFunc = NULL;
//PyObject	*pArg = NULL;


//#pragma execution_character_set("utf-8")
/* QTimer *timer = new QTimer(this);
connect(timer, &QTimer::timeout, this, &MyClass::processPendingDatagrams);
timer->start(10); // 每10毫秒检查一次*/
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
	//C++
	Paths64 subject, clip, solution;
	//subject.push_back(MakePath({ 100, 50, 10, 79, 65, 2, 65, 98, 10, 21 }));
	//clip.push_back(MakePath(   { 98,  63, 4,  68, 77, 8, 52, 100,19, 12 }));
	//solution = Intersect(subject, clip, FillRule::NonZero);

    ui->setupUi(this);
    sInstance	= this;
	gMainWindow = this;
	m_row = 1;
	char buf[128] = {0};
    
	QString mPath = QCoreApplication::applicationDirPath();
    //QgsApplication::init();
	char szExePath[MAX_PATH];
	//DWORD dwSize = GetModuleFileName(NULL, g_mainDir, MAX_PATH);
	
	strncpy(g_mainDir, mPath.toUtf8().data(), mPath.size());

	sprintf(szExePath, "%s\\rec\\", g_mainDir);
	bool result = MakeSureDirectoryPathExists(szExePath);

	sprintf(szExePath, "%s\\recjpg\\", g_mainDir);
	result = MakeSureDirectoryPathExists(szExePath);

	QgsApplication::init();
	QgsApplication::setPrefixPath(mPath, true);		//设置程序启动路径
	QgsApplication::setPkgDataPath(mPath +"/data");
	QgsApplication::setPluginPath(mPath +"/plugins");//设置qgis插件路径

	//QgsApplication::setThemeName("default");	
	//QgsProviderRegistry::instance(mPath +"/plugins");

	QtWebView::initialize();

	ReadIniFile();//读取.ini配置文件

	//在主窗口中，创建一个状态栏
	m_pStatusBar = new QgsStatusBar();
	m_pStatusBar->setParentStatusBar(QMainWindow::statusBar());
	statusBar()->addPermanentWidget(m_pStatusBar, 10);
	statusBar()->addPermanentWidget(m_lblCoordinate,10);
	statusBar()->addPermanentWidget(m_lblSelectTargetCount, 20);

	m_pInfoBar = new QgsMessageBar(ui->centralwidget);

	setActionSvg();	//设置工具栏上命令按钮图标
	setMapCanvas();	//创建并设置地图画布，所有图层，比如点，线，面，无人机等

	//创建一个边界消息处理线程
	mMoidyfyBundance = new ModifyBoundance(g_pGirdLayer, (QgsMapCanvas*)&m_mapCanvas);
	//mMoidyfyBundance->start();
	//connect(this, &MainWindow::receviePlaneMessage, mMoidyfyBundance, &ModifyBoundance::handlePlaneMessage);//mavlink协议设置无人机接收，回调函数

	m_planeVector.clear();//飞机图层清理

	setEvents();		//设置命令按钮回调函数
	
	//创建地图工具
	QgsMapCanvas* pcanvas = (QgsMapCanvas*)m_mapCanvas;
	mToolPan = new QgsMapToolPan(pcanvas);
	mToolPan->setAction(ui->mActionMoveView);

	m_mapCanvas->setParallelRenderingEnabled(true);// 注意：一定要设置为并行渲染模式，否则鼠标移动地图或在地图上按下导致地图刷新时很久才显示出来
	m_mapCanvas->setCachingEnabled(true);
	
	
	//m_mapCanvas->setCurrentLayer(g_pLineLayer);
	mToolSelect			= new QgsMapToolSelect(pcanvas);
	mToolRadarPick		= new QgsMapToolEmitPoint(pcanvas);
	connect(mToolRadarPick, &QgsMapToolEmitPoint::canvasClicked, this, &MainWindow::onRadarPick);

	mDock = new QgsAdvancedDigitizingDockWidget(pcanvas);
	mDock->setMaximumWidth(50);
	mMapToolVertext		= new QgsVertexTool(pcanvas, mDock);
	
	mVectorLayerTools	= new QgsGuiVectorLayerTools();//面图层工具
	//m_mapCanvas->setCurrentLayer(g_pLineLayer);
	mMapToolAddline		= new QgsMapToolAddFeature(pcanvas, QgsMapToolCapture::CaptureLine, mDock);//创建线图元工具

	//m_mapCanvas->setCurrentLayer(g_pPolygonLayer);
	mMapToolAddPolygon=new QgsMapToolAddFeature(pcanvas, QgsMapToolCapture::CapturePolygon, mDock);//创建多边形图元工具
	mMapToolAddCircle= new QgsMapToolAddFeature(pcanvas, QgsMapToolCapture::CapturePolygon, mDock);//创建圆型图元工具
	mMapToolCircle2d = new QgsMapToolCircle2Points(mMapToolAddCircle, pcanvas);					//创建圆型图元工具
	

	mMapToolAddTarget	 = new QgsMapToolAddTarget(pcanvas, QgsMapToolCapture::CapturePoint, mDock);//添加目标点工具
	mMoveFeature		 = new QgsMapToolMoveFeature(pcanvas, QgsMapToolMoveFeature::Move, mDock);//移动图元工具
	mMaptoolSvgAnnotation= new QgsMapToolSvgAnnotation(pcanvas);//svg工具
	mMaptoolJpgAnnotation= new QgsMapToolJpgAnnotation(pcanvas);//jpg工具
		
	//mpRotateFeatureTool = new QgsMapToolRotateFeature(pcanvas);//旋转任务区域
	//mpRotateFeatureTool->setAction(ui->mActionTaskAreaAngle);

	//设置jpg大图工具，回调函数
	connect(mMaptoolJpgAnnotation, &QgsMapToolJpgAnnotation::showLeftTableMessage, this, &MainWindow::showJpgAnnotaionWindow);


	maptoolMeaserLenght= new QgsMeasureTool(pcanvas, false);	//测量长度工具
	maptoolMeaserArea  = new QgsMeasureTool(pcanvas, true);		//测量面积工具

	QgsGui::editorWidgetRegistry()->initEditors(pcanvas, m_pInfoBar);
	m_mapCanvas->enableMapTileRendering(true);

	//从注册表中得到配置数据
	/*QSettings settings("HelloPlane", "suzs");
	QByteArray ary =settings.value("Widget/geometry").toByteArray();
	restoreGeometry(settings.value("Widget/geometry").toByteArray());
	restoreState(settings.value("Widget/windowState").toByteArray());*/

	//设置图层，可以动态隐藏，显示
	m_layerTreeCanvasBridge = new QgsLayerTreeMapCanvasBridge(QgsProject::instance()->layerTreeRoot(), pcanvas, this);
	// 禁止 bridge 在第一个"有效CRS图层"加入时自动重置 canvas CRS 和 extent。
	// 切换在线瓦片图层（WMS/XYZ）时，bridge 会将 canvas CRS 改为 EPSG:3857、范围扩展为全球，
	// 导致 loadOnlineTileMap 的范围恢复失效。关闭此选项后 bridge 仅更新图层列表，不动 CRS/extent。
	m_layerTreeCanvasBridge->setAutoSetupOnFirstLayer(false);
	
	//初始化时，设置某些图层是否显示
	QgsLayerTree *qgsroot = QgsProject::instance()->layerTreeRoot();
	QList<QgsLayerTreeNode *> nodelist = qgsroot->children();
	foreach(QgsLayerTreeNode * node, nodelist) {
		if (node->name() == QString("bundanceGrid.shp"))
		{
			//node->setItemVisibilityChecked(false);
			break;
		}
	}
	//121.49486,25.02724 taiwan
#ifdef HUAYIN
	QgsPointXY center(109.9872, 34.6056); //华阴县
#endif

#ifdef JINGBIAN
	QgsPointXY center(108.7959, 37.9229);	//靖边
#endif

#ifdef BOTOU
	QgsPointXY center(116.28965, 38.15358); //泊头
#endif

#ifdef TAIWAN
	//QgsPointXY center(121.94059, 25.01320);	//TAIWAN
	QgsPointXY center(121.360697, 24.877156);	//TAIWAN
	//QgsPointXY p1(13508450.70608, 2845493.9049);	//TAIWAN，epsg:3857坐标系坐标
#endif	
	//m_mapCanvas->zoomScale(10000);	//设置缩放级别
	//m_mapCanvas->setCenter(center);  

	//初始化标题工具栏; 
	//initWidgetAttribute();		//初始化编号，时间类型窗口
	initJpgAnnoWidget();			//初始化打击侦察详情窗口
	initRightList();				//初始化右侧表格窗口

	udpRecieveEleticMagnetismInit();//初始化接收电磁信息socket

	//批量发送处理线程对象
	//m_pBatchThread = new BatchSendThread;
	//m_pBatchThread->start();

	sprintf(buf,"收到飞机数量: %d", m_planeVector.count());
	m_PlaneNumEditer->setText(QString::fromLocal8Bit(buf));
	//addJpgAnnotaion(38.1371383667,116.2953262329,38.1353416443,116.2950057983,38.1372909546,116.2935943604,38.1355018616,116.2933807373,
	//"/home/szs/test2/distorted_resn237.jpg");
	
	//g_pGirdLayer->startEditing();    //启动网格图层编辑功能

	//QgsCoordinateReferenceSystem mCrs = QgsProject::instance()->crs();// m_mapCanvas.mapSettings().destinationCrs();
	//QString s=mCrs.userFriendlyIdentifier(QgsCoordinateReferenceSystem::FullString);
	bool b = createSqliteDatabase();//创建sqlite3数据库
	if(!b)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("打开 sqlite3 数据库 失败!"));
		::PostMessage((HWND)this->winId(), WM_CLOSE, 0, 0);
	}

	//创建一个接收dds控制台程序发送过来的数据包socket
	m_pUdpsocket = new QUdpSocket(this);
	//新版本5.0	
	if (m_pUdpsocket->bind(QHostAddress("127.0.0.1"), 54000)) 
		qDebug() << pCode->toUnicode("**********绑定成功********** 绑定端口:54000");
	else
	{
		qDebug() << pCode->toUnicode("**********绑定失败**********");
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("绑定失败 54000端口!"));
		return;
	}
	QObject::connect(m_pUdpsocket, &QUdpSocket::readyRead, this, &MainWindow::UdpSockerRecvData);

		
	//创建一个无人机数据回放查询窗口
	m_pDBBackDlg = new dataBackDlg(this);
	m_pDBBackDlg->setWindowFlags(Qt::Window);  // 这将允许最小化按钮出现

	//创建一个无人机数据回放线程
	m_pMavThread = new mavlink5Thread();
	m_pMavThread->m_pdtBackDlg	= m_pDBBackDlg;
	m_pMavThread->m_pMainDlg	= this;

	m_pDBBackDlg->m_pMavThread = m_pMavThread;
	QObject::connect(m_pMavThread, SIGNAL(signal_setTitle()), m_pDBBackDlg, SLOT(on_setTitle()));	
	connect(m_pMavThread, &mavlink5Thread::signal_modifyPlane, this, &MainWindow::registerPlane);

	//return;
	// 启动时清空上次运行残留的无人机图元（startEditing 之前必须调用，否则 deleteFeature 静默失败）
	if (!g_pAirLayer->isEditable()) g_pAirLayer->startEditing();
	//删除无人机图层内所有无人机图元
	QgsFeature feat;
	QgsFeatureIterator fit = g_pAirLayer->getFeatures();
	while (fit.nextFeature(feat))
		g_pAirLayer->deleteFeature(feat.id());
	g_pAirLayer->commitChanges();

	//删除无人机的轨迹线图层内所有点图元g_pPointLayer
	g_pPointLayer->startEditing();
	fit = g_pPointLayer->getFeatures();
	while (fit.nextFeature(feat))
		g_pPointLayer->deleteFeature(feat.id());
	g_pPointLayer->commitChanges();

	g_pRadarPtLayer->startEditing();
	fit = g_pRadarPtLayer->getFeatures();
	while (fit.nextFeature(feat))
		g_pRadarPtLayer->deleteFeature(feat.id());
	g_pRadarPtLayer->commitChanges();

	loadRadarUavMount();    // 加载探测设备与无人机的装载关系
	ShowRadarTip();		    //刷新雷达提示
	ShowTaskAreaTip();	    //刷新任务区域提示

	// 启动无人机图层定时刷新（100ms=10fps），替代 per-message triggerRepaint
	if (!m_pAirLayerTimer)
	{
		m_pAirLayerTimer = new QTimer(this);
		m_pAirLayerTimer->setInterval(100);
		connect(m_pAirLayerTimer, &QTimer::timeout, this, &MainWindow::onAirLayerRefreshTimer);
	}
	m_pAirLayerTimer->start();

	// 启动飞机图标插值定时器（16ms≈60fps），使 QGIS canvas 飞机图标平滑移动
	if (!m_pInterpTimer)
	{
		m_pInterpTimer = new QTimer(this);
		m_pInterpTimer->setInterval(16);
		connect(m_pInterpTimer, &QTimer::timeout, this, &MainWindow::onInterpTimer);
	}
	m_pInterpTimer->start();

	// 初始化Leaflet 2D地图（后台加载，不阻塞启动）
	initLeafletMap();
	// 三维地图懒加载：首次切换到三维模式时再初始化（避免hidden状态下WebGL canvas 0×0问题）

	//设置画布的extent(范围)
	QgsRectangle rect;
	if(g_pRasterLayer)
		rect = g_pRasterLayer->extent();
	else
		rect = g_pAirLayer->extent();

	m_mapCanvas->setExtent(rect);
	m_mapCanvas->zoomToFullExtent();
	
	m_mapCanvas->setCenter(center);//设置地图居中
	m_mapCanvas->refresh();

	//启动陶思伟转发程序
	ZeroMemory(&m_ShExecTransit, sizeof(SHELLEXECUTEINFO));
	m_ShExecTransit.cbSize = sizeof(SHELLEXECUTEINFO);
	m_ShExecTransit.fMask = SEE_MASK_NOCLOSEPROCESS;
	m_ShExecTransit.lpVerb = ("open");

	//输入要调用的exe文件路径  
	m_ShExecTransit.lpFile = "jing.exe";// "ffplay.exe";  // python3
	//char buf[128] = { 0 };
	mPath += "/jing";
	strcpy(buf, mPath.toStdString().c_str());
	m_ShExecTransit.lpDirectory = buf;
	m_ShExecTransit.lpParameters = "1"; //若没有命令行参数，可为NULL 
	m_ShExecTransit.nShow = SW_HIDE;// SW_SHOW;
	ShellExecuteEx(&m_ShExecTransit);

	m_pDlgTuili = new DlgTuili(this);	//推理模块
	m_pDlgTuili->on_queryBtn_click();
}

//接收本机dds控制台程序发送过来的数据包
void MainWindow::UdpSockerRecvData() 
{
	while (m_pUdpsocket->hasPendingDatagrams())
	{
		QByteArray datagrams;
		datagrams.resize(m_pUdpsocket->pendingDatagramSize());

		QHostAddress	paddress;
		quint16			pport;

		// 通过readDatagram()此函数读取数据报，
		m_pUdpsocket->readDatagram(datagrams.data(), datagrams.size(), &paddress, &pport);

		int size = sizeof(BigImg);
		size = datagrams.size();
		if (sizeof(BigImg) != datagrams.size())
		{
			qDebug() << "UdpSockerRecvData 接收结构体大小错误 。。。。。.....";
			continue;		
		}
		//QString strs = datagrams.data();
		//QString peer = "[From:" + paddress.toString() + ":" + QString::number(pport) + "]:";
		BigImg img = { 0 };
		memcpy(&img, datagrams.data(), datagrams.size());
		g_ddsMutex.lock();
		g_RecievedDDS.push_back(img);
		g_ddsMutex.unlock();
		//ui->plainTextEdit->appendPlainText(peer + strs);
		qDebug() << "UdpSockerRecvData size=" << datagrams.size();
	}
}

//创建sqlite3数据库
bool MainWindow::createSqliteDatabase()
{
	QString mPath = QCoreApplication::applicationDirPath();
	g_sqliteDbase = QSqlDatabase::addDatabase("QSQLITE", "sqlite3");	//数据库类型及数据库句柄名
	g_sqliteDbase.setDatabaseName(mPath + "/helloPlane.db");			//数据库路径
	g_sqliteDbase.open();

	QSqlQuery initQuery(g_sqliteDbase);
	initQuery.exec(
		"CREATE TABLE IF NOT EXISTS mavlink ("
		"Id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"AirID TEXT, X TEXT, Y TEXT, Z TEXT, xZ TEXT,"
		"yaw TEXT, roll TEXT, pitch TEXT,"
		"airSpeed TEXT, groundSpd TEXT, vN TEXT, vE TEXT, vU TEXT,"
		"gpsStatus TEXT, voltage TEXT, gpsNum TEXT,"
		"status TEXT, taskType TEXT, gjMode TEXT,"
		"qkRunMode TEXT, qkCmdMode TEXT, fkRunMode TEXT, fkSysStatus TEXT, jqtbTime TEXT,"
		"time TEXT)"
	);
	// Migrate existing databases that have fewer columns
	{
		const char* newCols[] = {
			"xZ TEXT", "airSpeed TEXT", "groundSpd TEXT",
			"vN TEXT", "vE TEXT", "vU TEXT",
			"gpsStatus TEXT", "voltage TEXT", "gpsNum TEXT",
			"status TEXT", "taskType TEXT", "gjMode TEXT",
			"qkRunMode TEXT", "qkCmdMode TEXT",
			"fkRunMode TEXT", "fkSysStatus TEXT", "jqtbTime TEXT"
		};
		for (const char* col : newCols)
		{
			QSqlQuery alt(g_sqliteDbase);
			alt.exec(QString("ALTER TABLE mavlink ADD COLUMN %1").arg(col));
			// ignore error — column already exists
		}
	}

	QSqlQuery query(g_sqliteDbase);
	bool b = query.exec("SELECT 1 FROM mavlink LIMIT 1");
	return g_sqliteDbase.isOpen();
}

//创建一个mysql数据库
bool MainWindow::createDatabase()
{
	qDebug() << QSqlDatabase::drivers();
	//
	/*g_mysqlDbase = QSqlDatabase::addDatabase("QMYSQL");
	g_mysqlDbase.setHostName("127.0.0.1");
	g_mysqlDbase.setPort(3306);
	g_mysqlDbase.setUserName("root");
	g_mysqlDbase.setPassword("root");
	g_mysqlDbase.setDatabaseName("db001");

	if (g_mysqlDbase.open()) {
		//QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("连接MySQL成功: "));
		//当只有一个数据库时，我们需要把 sqlQuery类与数据库进行绑定，不然会执行语句失败和导入驱动失败
		//m_pSqlQuery = new QSqlQuery("db001");
	}
	else {
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("连接MySQL失败: "));
		return false;
	}*/
	return true;
}

MainWindow::~MainWindow()
{
    if (mDock ) delete mDock;
    if (mToolSelect ) delete mToolSelect;
    if (mToolRadarPick) delete mToolRadarPick;
    if (mMapToolVertext) delete mMapToolVertext;
    if (mMapToolAddline ) delete mMapToolAddline;
    if (mMapToolAddPolygon ) delete mMapToolAddPolygon;
    if (mMoveFeature )   delete mMoveFeature;
    if (mMapToolCircle2d) delete mMapToolCircle2d;
    if (maptoolMeaserLenght) delete  maptoolMeaserLenght;
    if (maptoolMeaserArea) delete maptoolMeaserArea;
    if (mMaptoolSvgAnnotation) delete mMaptoolSvgAnnotation;
    if (mMaptoolJpgAnnotation) delete mMaptoolJpgAnnotation;
    if (m_pLookDlg)    delete m_pLookDlg;
    if (m_gridDialog)     delete m_gridDialog;

	//m_pBatchThread->quit();
	//delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	// 先停接收线程
	if (m_pRecThread)
	{
		m_pRecThread->m_stopped = true;
		if (!m_pRecThread->wait(2000))
		{
			m_pRecThread->terminate();
			m_pRecThread->wait();
		}
		delete m_pRecThread;
		m_pRecThread = nullptr;
	}
	// 再停中转线程（TransitThread 以 m_stopped 干净退出）
	if (m_pTransitThread)
	{
		m_pTransitThread->m_stopped = true;
		if (!m_pTransitThread->wait(1000))
		{
			m_pTransitThread->terminate();
			m_pTransitThread->wait();
		}
		delete m_pTransitThread;
		m_pTransitThread = nullptr;
	}

    //保存到注册表中...
    QSettings settings("HelloPlane", "suzs");
    settings.setValue("Widget/geometry", saveGeometry());
    settings.setValue("Widget/windowState", saveState());
    settings.sync();

	if (m_ShExecInfo.hProcess != NULL)
	{
		TerminateProcess(m_ShExecInfo.hProcess, 0);
		//等待程序运行完毕  
		WaitForSingleObject(m_ShExecInfo.hProcess, INFINITE);
	}

	if (m_ShExecTransit.hProcess != NULL)
	{
		TerminateProcess(m_ShExecTransit.hProcess, 0);
		//等待程序运行完毕  
		WaitForSingleObject(m_ShExecTransit.hProcess, INFINITE);
	}

    QMainWindow::closeEvent(event);
}

QMenu *cmenu = NULL;
//鼠标左键按下
void MainWindow::mousePressEvent(QMouseEvent * e)
{
	int i = 0;	i = 3 + 4;
	qDebug() << QString::fromLocal8Bit("鼠标右键被点击")<<endl;
	if (cmenu)//保证同时只存在一个menu，及时释放内存
	{
		delete cmenu;
		cmenu = NULL;
	}

	if (e->button() == Qt::RightButton) 
	{
		ShowShortMenu(e);
	}
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
	// 右键菜单已在 mousePressEvent 中显示，此处仅释放旧菜单，不重复弹出
	if (cmenu)
	{
		delete cmenu;
		cmenu = NULL;
	}
}

void MainWindow::ShowShortMenu(QMouseEvent * e)
{
	if (e->button() == Qt::RightButton) {
		QString mPath = QCoreApplication::applicationDirPath();
		QSettings settings(mPath + "/config.ini", QSettings::IniFormat);
		QString slist = settings.value("DeviceType/list").toString();

		cmenu = new QMenu(m_mapCanvas);// this->parentWidget());

		QStringList list = slist.split("|");
		//foreach(QString s, list) 
		for (int i = 0; i < list.size(); i++)
		{
			QAction *pAct = cmenu->addAction(list.at(i));
			if (i == 0)
				connect(pAct, &QAction::triggered, this, &MainWindow::AddMenu1);
			if (i == 1)
				connect(pAct, &QAction::triggered, this, &MainWindow::AddMenu2);
			if (i == 2)
				connect(pAct, &QAction::triggered, this, &MainWindow::AddMenu3);
			if (i == 3)
				connect(pAct, &QAction::triggered, this, &MainWindow::AddMenu4);
			if (i == 4)
				connect(pAct, &QAction::triggered, this, &MainWindow::AddMenu5);
		}

		// 向菜单中添加分隔符
		cmenu->addSeparator();
		QAction *pAct = cmenu->addAction(QString::fromLocal8Bit("任务区域"));
		connect(pAct, &QAction::triggered, this, &MainWindow::AddAirTaskArea);

		cmenu->exec(QCursor::pos());	//在当前鼠标位置显示
	}
}

QgsVectorLayer* pLayer = nullptr;

//创建并设置地图画布图层
void MainWindow::setMapCanvas()
{
	QString		mPath = QCoreApplication::applicationDirPath();

    //创建显示飞机数量文本框对象
    m_PlaneNumEditer =new QLineEdit();
    m_PlaneNumEditer->setMaximumWidth(170);
    ui->mToolBarSchedule->addWidget(m_PlaneNumEditer);

	//创建一个2D地图窗口
	m_mapCanvas=new QgsMapCanvasZjr(ui->centralwidget);
    m_mapCanvas->setCanvasColor(QColor(155, 155, 155));
    m_mapCanvas->setVisible(true);
    m_mapCanvas->enableAntiAliasing(true);

	// ===== 磁盘缓存：所有 WebEngineView 共用 defaultProfile =====
	// HTTP 缓存 + IndexedDB 持久化存储均放在软件目录下，便于整体拷贝至其他设备
	{
		QString cacheDir = QCoreApplication::applicationDirPath() + "/map_cache";
		QDir().mkpath(cacheDir);
		QWebEngineProfile* prof = QWebEngineProfile::defaultProfile();
		prof->setHttpCacheType(QWebEngineProfile::DiskHttpCache);
		prof->setCachePath(cacheDir + "/http");                      // HTTP 瓦片缓存
		prof->setPersistentStoragePath(cacheDir + "/storage");       // IndexedDB 持久化（3D卫星+DEM）
		prof->setHttpCacheMaximumSize(512 * 1024 * 1024); // 512 MB
		prof->setHttpUserAgent(prof->httpUserAgent() + " MapCache/1.0");
	}

	//创建三维地图窗口（Three.js 3D卫星影像）
	m_pWebEngineView = new QWebEngineView(ui->centralwidget);
	m_pWebEngineView->setAutoFillBackground(true);
	m_pWebEngineView->hide();
	// 允许本地HTML加载CDN上的CesiumJS库
	m_pWebEngineView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessRemoteUrls, true);
	connect(m_pWebEngineView, &QWebEngineView::loadFinished, this, [this](bool ok) {
		m_3dReady = ok;
		if (ok) sync3DAll(); // 页面加载完成后推送初始数据
	});
	// 3D status bar: parse title "SB:lng,lat|z=10|h=50000|e=320"
	connect(m_pWebEngineView, &QWebEngineView::titleChanged, this, [this](const QString &title) {
		if (m_mapViewMode != 2 || !title.startsWith("SB:")) return;
		// Split by '|': coord, z=zoom, h=camHeight, e=elevation
		QStringList parts = title.mid(3).split('|');
		if (parts.size() < 4) return;
		QString coord = parts[0];
		QString zPart = parts[1].mid(2);   // skip "z="
		QString hPart = parts[2].mid(2);   // skip "h="
		QString ePart = parts[3].mid(2);   // skip "e="
		m_lblCoordinate->setText(coord);
		char buf[160];
		sprintf(buf, "\xe7\xbc\xa9\xe6\x94\xbe\xe7\xba\xa7\xe5\x88\xab=%s  \xe6\xb5\xb7\xe6\x8b\x94=%sm  \xe7\x9b\xb8\xe6\x9c\xba=%sm",
			zPart.toUtf8().constData(), ePart.toUtf8().constData(), hPart.toUtf8().constData());
		m_lblSelectTargetCount->setText(QString::fromUtf8(buf));
	});

	//创建Leaflet 2D地图窗口（GPU加速，缩放流畅，替代QGIS canvas用于2D显示）
	m_p2DMapView = new QWebEngineView(ui->centralwidget);
	m_p2DMapView->setAutoFillBackground(true);
	m_p2DMapView->hide();

	// 地图区容器（infobar + 三种地图视图叠放）
	QWidget* mapArea = new QWidget(ui->centralwidget);
	QGridLayout* mapGrid = new QGridLayout(mapArea);
	mapGrid->setContentsMargins(0, 0, 0, 0);
	mapGrid->setSpacing(0);
	mapGrid->addWidget(m_pInfoBar,                 0, 0);
	mapGrid->addWidget((QWidget*)m_mapCanvas,      1, 0);
	mapGrid->addWidget(m_p2DMapView,               1, 0);  // 与QGIS canvas同格，切换时show/hide
	mapGrid->addWidget((QWidget*)m_pWebEngineView, 1, 0);  // 3D视图，与QGIS canvas同格，切换时show/hide
	mapGrid->setRowStretch(1, 1);

	// 可调节分割器：上部地图区 / 下部无人机数据表
	QSplitter* mapTableSplitter = new QSplitter(Qt::Vertical, ui->centralwidget);
	mapTableSplitter->setHandleWidth(5);
	mapTableSplitter->setChildrenCollapsible(false);
	mapTableSplitter->addWidget(mapArea);
	mapTableSplitter->addWidget(ui->mPlaneWidget);
	mapTableSplitter->setStretchFactor(0, 1);
	mapTableSplitter->setStretchFactor(1, 0);
	mapTableSplitter->setSizes({700, 100});

	QVBoxLayout* central = new QVBoxLayout(ui->centralwidget);
	central->setContentsMargins(0, 0, 0, 0);
	central->setSpacing(0);
	central->addWidget(mapTableSplitter);

	//设置无人机表格控件
	int i = 0;
	ui->mPlaneWidget->setColumnCount(19);
	ui->mPlaneWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("飞机ID")));
	ui->mPlaneWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("经度")));
	ui->mPlaneWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("纬度")));
	ui->mPlaneWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("相对高度")));
	ui->mPlaneWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("海拔高度")));	
	ui->mPlaneWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("Pitch")));
	ui->mPlaneWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("Roll")));
	ui->mPlaneWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("Yaw")));
	ui->mPlaneWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("空速")));
	ui->mPlaneWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("地速")));
	ui->mPlaneWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("gps状态")));
	ui->mPlaneWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("电压")));
	ui->mPlaneWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("星数")));
	ui->mPlaneWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("任务类型")));
	ui->mPlaneWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("轨迹模式")));
	ui->mPlaneWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("群控运行模式")));
	ui->mPlaneWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("群控指令模式")));
	ui->mPlaneWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("飞控运行模式")));
	ui->mPlaneWidget->setHorizontalHeaderItem(i++, new QTableWidgetItem(QString::fromLocal8Bit("集群同步时间")));

	ui->mPlaneWidget->setColumnWidth(0, 50);
	ui->mPlaneWidget->setColumnWidth(1, 90);
	ui->mPlaneWidget->setColumnWidth(2, 90);
	ui->mPlaneWidget->setColumnWidth(3, 90);
	ui->mPlaneWidget->setColumnWidth(4, 90);
	ui->mPlaneWidget->setColumnWidth(5, 70);
	ui->mPlaneWidget->setColumnWidth(6, 70);
	ui->mPlaneWidget->setColumnWidth(7, 70);
	ui->mPlaneWidget->verticalHeader()->setVisible(false);
	ui->mPlaneWidget->setSelectionBehavior(QAbstractItemView::SelectRows); //设置选择行为，以行为单位
	ui->mPlaneWidget->setSelectionMode(QAbstractItemView::SingleSelection); //设置选择模式，选择单行

	// mPlaneWidget already added to mapTableSplitter above

	//QString mapdir = QCoreApplication::applicationDirPath();

	
	QSettings	settings(mPath + "/config.ini", QSettings::IniFormat);
	QString		slist = settings.value("DeviceType/list").toString();
	QStringList list = slist.split("|");

#ifdef TAIWAN
	mPath += "/TW-map";
#endif

#ifdef BOTOU
	mPath += "/botou-map";
#endif

#ifdef HUAYIN
	mPath += "/huayin";
#endif

	addRasterlayers();//加载栅格tif文件地图

	//OSM在线地图-投影坐标系,需要打开翻墙软件clash才会显示地图
	//addOpenSourceRasterLayer("type=xyz&url=http://c.tile.openstreetmap.org/{z}/{x}/{y}.png", "OSM", "wms");

	LoadVectorlayers(mPath + "/shpAndTif/areaDesigned.shp",	&g_pPolygonLayer);		//加载多边形面图层
	LoadVectorlayers(mPath + "/shpAndTif/AirTaskArea.shp",		&g_pAirTaskPolyLayer);	//加载无人机任务区域，多边形面图层
	// 任务区域变更后实时同步到在线地图和三维视图
	if (g_pAirTaskPolyLayer) {
		connect(g_pAirTaskPolyLayer, &QgsVectorLayer::committedFeaturesAdded,
			this, [this](const QString &, const QgsFeatureList &){ syncLeafletAll(); sync3DAll(); });
		connect(g_pAirTaskPolyLayer, &QgsVectorLayer::committedFeaturesRemoved,
			this, [this](const QString &, const QgsFeatureIds &){ syncLeafletAll(); sync3DAll(); });
		connect(g_pAirTaskPolyLayer, &QgsVectorLayer::committedGeometriesChanges,
			this, [this](const QString &, const QgsGeometryMap &){ syncLeafletAll(); sync3DAll(); });
		connect(g_pAirTaskPolyLayer, &QgsVectorLayer::committedAttributeValuesChanges,
			this, [this](const QString &, const QgsChangedAttributesMap &){ syncLeafletAll(); sync3DAll(); });
	}
	LoadVectorlayers(mPath + "/shpAndTif/lineDesigned.shp",	&g_pLineLayer);			//加载线图层
	LoadVectorlayers(mPath + "/shpAndTif/electromagnetism.shp",&m_electroMagnetism);
	LoadVectorlayers(mPath + "/shpAndTif/eletroPoint.shp",		&g_pSmallJpgLayer);		//加载小图片图层
	LoadVectorlayers(mPath + "/shpAndTif/bundanceGrid.shp",	&g_pGirdLayer);			//加载战场表格图层	
	LoadVectorlayers(mPath + "/shpAndTif/targetLayer.shp",		&g_pTargetLayer);		//加载目标点图层
	LoadVectorlayers(mPath + "/air.shp",	&g_pAirLayer);								//无人机图层
	LoadVectorlayers(mPath + "/Point.shp",	&g_pPointLayer);							//无人机轨迹线图层
	
	for (int i = 0; i < list.size(); i++)
	{		
		gRadarLayerList.push_back(pLayer);
	}

	for (int i = 0; i < list.size(); i++)
	{
		if (i == 0) LoadVectorlayers(mPath + "/radar1.shp", &gRadarLayerList[0]);
		if (i == 1) LoadVectorlayers(mPath + "/radar2.shp", &gRadarLayerList[1]);
		if (i == 2) LoadVectorlayers(mPath + "/radar3.shp", &gRadarLayerList[2]);
		if (i == 3) LoadVectorlayers(mPath + "/radar4.shp", &gRadarLayerList[3]);
		// 装备变更后实时同步到在线地图和三维视图
		QgsVectorLayer *rl = gRadarLayerList.size() > i ? gRadarLayerList[i] : nullptr;
		if (rl) {
			connect(rl, &QgsVectorLayer::committedFeaturesAdded,
				this, [this](const QString &, const QgsFeatureList &){ syncLeafletAll(); sync3DAll(); });
			connect(rl, &QgsVectorLayer::committedFeaturesRemoved,
				this, [this](const QString &, const QgsFeatureIds &){ syncLeafletAll(); sync3DAll(); });
			connect(rl, &QgsVectorLayer::committedAttributeValuesChanges,
				this, [this](const QString &, const QgsChangedAttributesMap &){ syncLeafletAll(); sync3DAll(); });
		}
	}
		
	LoadVectorlayers(mPath + "/radarTrack.shp", &g_pRadarPtLayer);						//雷达覆盖无人机轨迹图层

	g_pRadarTyLayer = CreateDeviceLayer(QColor(255, 180, 0));	//雷达设备扇形投影图层
	g_pRadarTaskLayer=CreateDeviceLayer(QColor(255, 50, 30));	//雷达投影图层和任务区域重叠图层
	
#ifdef BOTOU
	LoadVectorlayers(mPath + QString::fromLocal8Bit("/15x15km/道路.shp"), &g_pRoadLayer);
#endif

#ifdef TAIWAN
	LoadVectorlayers(mPath + QString::fromLocal8Bit("/道路1.shp"), &g_pRoadLayer);
#endif

	// 下拉框已移除，点击"装备图层"按钮即可选择各类装备

    //addJpgAnnotationToLayer();														//创建一个jpg图片大图图层

    //渲染各个图层
    SingleColor(g_pPolygonLayer);
    SingleColor(m_electroMagnetism);

	targetLayerMarker();            //渲染目标点图层风格
    gridLayerMarker();				//设置网格光栅图层风格
    
	planeLayerMarker(g_pAirLayer);	//设置飞机图层风格
    lineLayerMarker();				//设置线图层风格
    electicMagnetismMarker();		//设置电子磁性图层风格
	guijiLinePointMarker();			//设置轨迹线点图层风格
	radarDevTrackMarker();			//设置雷达覆盖轨迹图层风格
	radarDeviceMarker();			//设置雷达设备图层风格
	airTaskAreaMarker();			//设置无人机任务区域图层风格 
}

//加载在线地图
void MainWindow::addOpenSourceRasterLayer(const QString& url, const QString& basename, const QString& providerKey)
{
	QgsRasterLayer* pLayer = new QgsRasterLayer(url, basename, providerKey);
	if (!pLayer->isValid())
	{
		QMessageBox::critical(this, "error", "layer is invalid!");
		return;
	}

	g_pRasterLayer = pLayer;

	QgsProject::instance()->addMapLayer(pLayer);
	m_mapCanvas->setExtent(pLayer->extent());
	m_layers.append(pLayer);
	m_mapCanvas->setLayers(m_layers);
	m_mapCanvas->setVisible(true);
	m_mapCanvas->freeze(false);
	m_mapCanvas->refresh();
}

//创建临时内存图层
QgsVectorLayer * MainWindow::CreateDeviceLayer(QColor color)
{	
	//QString s;	s = "annoLayer" + QString("%1").arg(id);
	QgsVectorLayer *pLayer = new QgsVectorLayer("Polygon", "MemoryMap", "memory");

	if (pLayer->isValid() == false)
		return NULL;

	pLayer->startEditing();

	//向“数据提供者对象”添加字段
	QList<QgsField> fields;
	fields.append(QgsField("id", QVariant::String));		//ID属性字段

	pLayer->addAttribute(QgsField("id",		QVariant::String));
	pLayer->dataProvider()->addAttributes(fields);

	//向画布图层数据集对象中添加此图层对象
	m_layers.append(pLayer);
	pLayer->commitChanges();


	m_mapCanvas->setLayers(m_layers);
	QgsProject::instance()->addMapLayer(pLayer);

	//渲染图层
	QgsSymbol *symbol = QgsSymbol::defaultSymbol(pLayer->geometryType());

	//QColor color(100, 100,50);//).value<QColor>();
	qreal opacity = 0.5;		// .value<qreal>();
	symbol->setColor(color);
	symbol->setOpacity(opacity);

	pLayer->setRenderer(new QgsSingleSymbolRenderer(symbol));
	return pLayer;
}

//创建无人机轨迹线内存图层
void MainWindow::CreateLineLayer()
{
	QgsVectorLayer *m_pPlaneGjLayer = new QgsVectorLayer("LineString", "MemoryTestMap", "memory");

	if (m_pPlaneGjLayer->isValid() == false)
		return;

	m_pPlaneGjLayer->startEditing();

	//向“数据提供者对象”添加字段
	QList<QgsField> fields;
	fields.append(QgsField("id", QVariant::String));		//图元的ID属性字段
	m_pPlaneGjLayer->addAttribute(QgsField("id", QVariant::String));

	m_pPlaneGjLayer->dataProvider()->addAttributes(fields);

	//向画布图层数据集对象中添加此图层对象
	m_layers.append(m_pPlaneGjLayer);

	m_pPlaneGjLayer->commitChanges();
	m_pPlaneGjLayer->startEditing();

	m_mapCanvas->setLayers(m_layers);
	QgsProject::instance()->addMapLayer(m_pPlaneGjLayer);
}
//鼠标光标回调函数
void MainWindow::showMousePoint(const QgsPointXY& p)
{
	QString s= QgsCoordinateUtils::formatCoordinateForProject(QgsProject::instance(), p, m_mapCanvas->mapSettings().destinationCrs(), 6);
	QString s1 = s.left(s.indexOf(","));
	QString s2 = s.mid(s.indexOf(",") + 1);
	double x = s1.toDouble() / 20037508.34 * 180;
	double y = s2.toDouble() / 20037508.34 * 180;
	y = 180 / M_PI * (2 * atan(exp(y * M_PI / 180)) - M_PI / 2);

	s1 = QString::number(x, 'f', 6);
	s2 = QString::number(y, 'f', 6);
	//tr("%1,%2").arg(x).arg(y);
    m_lblCoordinate->setText(s);
    //  QgsPointXY pMap = m_mapCanvas.getCoordinateTransform()->toMapCoordinates(p.x(),p.y());
    // QString x1 = QString::number(pMap.x(), 'f', 5);
    // QString y2 = QString::number(pMap.y(), 'f', 5);
    // m_lblCoordinate->setText( QString("x=%1,y=%2").arg(x1).arg(y2) );
	//s1 = QString("缩放级别=%1").arg(m_zoomLevel);
	char buf[128];
	sprintf(buf,"缩放级别=%d", (int)m_zoomLevel);
	m_lblSelectTargetCount->setText(pCode->toUnicode(buf));
}
//地图缩放回调函数
void MainWindow::scaleChanged(double p)
{
	double z1scale  = 591658688.0;
	double mapScale = m_mapCanvas->scale();
	if (mapScale <= 0.0) return;         // 防止除以零导致崩溃
	float zoom = (float)log2(z1scale / mapScale);
	QString msg = QString::number(zoom, 'f',2);// '{:.2f}'.format(zoom)
	//self.dockwidget.zoomValue.setValue(zoom)

	//# estimate which XYZ zoom level would get requested
	msg = QString::number(floor(zoom + 0.586), 'f', 2);

	m_zoomLevel = floor(zoom + 0.586);
	qDebug() <<"zoom level="<< msg << "\n";
	char buf[128];
	sprintf(buf, "缩放级别=%d", (int)m_zoomLevel);
	m_lblSelectTargetCount->setText(pCode->toUnicode(buf));

	// Only shrink at low zoom; cap at 1.0 so labels never outgrow icons at high zoom
	// zoom<=10: sf=0.5, zoom=12: sf=0.75, zoom>=14: sf=1.0
	double sf = qBound(0.5, 0.5 + ((double)m_zoomLevel - 10.0) * 0.125, 1.0);
	for (biaopai *bp : m_planeIDvec)
		bp->updateSize(sf);
	for (RadarTip *rt : m_radarTipList)
		rt->updateSize(sf);
	for (TaskAreaTip *ta : m_TAreaTipList)
		ta->updateSize(sf);
}
//测量长度工具
void MainWindow::measureLength()
{
    m_mapCanvas->setMapTool(maptoolMeaserLenght);
}
//测量面积工具
void MainWindow::measureArea()
{
    m_mapCanvas->setMapTool( maptoolMeaserArea);
}

void MainWindow::annotationCreated(QgsAnnotation *annotation)
{
    QgsMapCanvasAnnotationItem *canvasItem = new QgsMapCanvasAnnotationItem(annotation, (QgsMapCanvas*)m_mapCanvas);
    Q_UNUSED(canvasItem) //item is already added automatically to canvas scene
}

/*void MainWindow::showLeftTable(QPoint &pos, bool show, Mappoint* point)
{
	int w = m_pJpgAnnoWidget->geometry().width();
	int h= m_pJpgAnnoWidget->geometry().height();
	m_pJpgAnnoWidget->setGeometry(pos.x(), pos.y(),w,h);
	m_pJpgAnnoWidget->setVisible(show);

	if (point) {
		this->m_mapPt = point;
		ui->mTableWidgetMarkAttribute->item(0, 0)->setText(point->rowid);
		ui->mTableWidgetMarkAttribute->item(0, 1)->setText(point->planeTime);
		dynamic_cast<QComboBox*>(ui->mTableWidgetMarkAttribute->cellWidget(0, 2))->setCurrentText(point->weaponname);
		dynamic_cast<QComboBox*>(ui->mTableWidgetMarkAttribute->cellWidget(0, 4))->setCurrentText(point->taskname);
		dynamic_cast<QComboBox*>(ui->mTableWidgetMarkAttribute->cellWidget(0, 5))->setCurrentText(point->ishanded);
		QString xPos = QString::number(point->x, 'f', 5);
		QString yPos = QString::number(point->y, 'f', 5);
		ui->mTableWidgetMarkAttribute->item(0, 3)->setText(QString("%1,%2").arg(xPos).arg(yPos));
	}
}*/

void MainWindow::showJpgAnnotaionWindow(QPoint &pos, bool show, Mappoint *point)
{
	int w = m_pJpgAnnoWidget->geometry().width();
	int h = m_pJpgAnnoWidget->geometry().height();

	m_pJpgAnnoWidget->setGeometry(pos.x(),pos.y(), w, h);
	m_pJpgAnnoWidget->setVisible(show);

    if (point ){
        point->ishanded = point->weaponname;
        m_mapPt = point;
    }
}

void MainWindow::JiqunTuili() //集群推理
{
	if (m_pDlgTuili == nullptr)
		m_pDlgTuili = new DlgTuili(this);

	m_pDlgTuili->show();
	//m_pDlgTuili->raise();
	m_pDlgTuili->activateWindow();

}
//定位接收到的目标图片
void MainWindow::locatePicture(QTableWidgetItem *item)
{
    (void)item;
}
//详情窗口
void MainWindow::showDetailForm()
{
    InformationDetailDialog *pDlg = new InformationDetailDialog(this);
	pDlg->setWindowTitle(QString::fromLocal8Bit("详情"));
	pDlg->mappoint = m_mapPt;
	pDlg->initControl();
	pDlg->setPicture();
	pDlg->show();
	pDlg->raise();
	pDlg->activateWindow();
}
//隐藏显示图片栈,右侧列表框
void MainWindow::hidetable()
{
}
//此函数目前没有用上,目前只是能添加一个文本标注图元
void MainWindow::addSvgAnnotation(QgsPointXY a, QgsPointXY b)
{
	QgsSvgAnnotation *annotation = new QgsSvgAnnotation();
    if (annotation)
    {
      //QgsPointXY mapPos((a.x()+b.x())/2.0, (a.y()+b.y())/2.0 );
	  QgsPointXY mapPos(a.x(), a.y());
      annotation->setMapPosition(mapPos);
      annotation->setMapPositionCrs(m_mapCanvas->mapSettings().destinationCrs());
      annotation->setRelativePosition(QPointF(0.5f, 0.5f));
      annotation->setFrameSizeMm(QSizeF(30, 25));
	  //annotation->setFilePath("./images/svg/transport/transport_airport.svg");
      QgsProject::instance()->annotationManager()->addAnnotation(annotation);

      // select newly added item
      //const auto constItems = m_mapCanvas.items();
      /*for (QGraphicsItem *item : constItems)
      {
        if ( QgsMapCanvasAnnotationItem *annotationItem = dynamic_cast< QgsMapCanvasAnnotationItem * >( item ) )
        {
          if ( annotationItem->annotation() == annotation )
          {
            annotationItem->setSelected( true );
            break;
          }
        }
      }*/
    }
}

//创建一个jpg图片大图图层
QgsAnnotationLayer *MainWindow::addJpgAnnotationToLayer(int id)
{
	QgsCoordinateTransformContext	Context;
	QString s;
	//for (int i = 0; i < 30; i++)
	s = "annotationLayer" + QString("%1").arg(id);
	int iCount = g_PlaneList.count();
	QgsAnnotationLayer *pLayer= new QgsAnnotationLayer(s, QgsAnnotationLayer::LayerOptions(Context));

	m_layers.insert(0, pLayer);
	m_mapCanvas->setLayers(m_layers);
	QgsProject::instance()->addMapLayer(pLayer);
	pLayer->setAutoRefreshEnabled(true);	
	s = "";
	return pLayer;
}
//窗口大小缩放事件
void MainWindow::resizeEvent(QResizeEvent *event)
{
    int w = this->width();
    int listwidth = 200;
    int listheight =790;

	
	//动态设置窗口最大化
	QRect rcMain = this->rect();
	ui->centralwidget->move(0, 0);  //移动frameMap位置
	//ui->centralwidget->setFixedSize(rcMain.width(), rcMain.height());
	//m_mapCanvas->setFixedSize(rcMain.width(), rcMain.height());//移动m_pMapCanvas位置

	//ui->centralwidget->move(0, 0);  //移动frameMap位置
	//ui->centralwidget->setFixedSize(rcMain.width(), rcMain.height());
	//m_mapCanvas->setFixedSize(rcMain.width(), rcMain.height());//移动m_pMapCanvas位置
}

//根据四个经纬度信息，jpg图片文件，在地图上添加一个图元
void MainWindow::addJpgAnnotaion(double leftupY, double leftupX,     double rightupY, double rightupX,
                                 double leftdownY, double leftdownX, double rightdownY, double rightdownX, QString filename)
{
	static int iZIndex = 0;
	QgsPoint p((leftupX + rightdownX) / 2.0, (leftupY + rightdownY) / 2.0); //获取图片的中心点经纬度坐标
	//if (m_zoomLevel >= 18)

	QgsAnnotationMarkerItem2 *pItem = new QgsAnnotationMarkerItem2(p);
	pItem->mLeftup.setX(leftupX);
	pItem->mLeftup.setY(leftupY);
	pItem->mRightUp.setX(rightupX);
	pItem->mRightUp.setY(rightupY);
	pItem->mLeftdown.setX(leftdownX);
	pItem->mLeftdown.setY(leftdownY);
	pItem->mRightdown.setX(rightdownX);
	pItem->mRightdown.setY(rightdownY);

	pItem->mMapCanvas = (QgsMapCanvas*)m_mapCanvas;
	pItem->setZIndex(iZIndex++);
	pItem->setFilePath(filename);
	//g_pAnnotationLayer->addItem(pItem);
	
	//加载jpg等图片
	/*QString mapdir = QCoreApplication::applicationDirPath();
	QString file = mapdir + "/recjpg11/image2189.jpg";
	file = "./plane.png";
	QgsRasterMarkerSymbolLayer		*rasterMarker = new QgsRasterMarkerSymbolLayer(file);
	rasterMarker->setColor(QColor(Qt::red));
	rasterMarker->setOpacity(200);//设置透明度
	rasterMarker->setSize(10);	//设置图片大小

	QgsSymbolLayerList symList;
	symList.append(rasterMarker->clone());//最好用这个clone要不然删除会有内存泄漏

	QgsMarkerSymbol* markSym = new QgsMarkerSymbol(symList);
	markSym->setAngle(90);// plane.Yaw.toInt());	//设置图片的角度

	QgsAnnotationMarkerItem *pMarkerItem = new QgsAnnotationMarkerItem(p); //QgsPoint(leftupX, leftupY)
	pMarkerItem->setSymbol(markSym->clone());//给Item加上Svg图像
	g_pAnnotationLayer->addItem(pMarkerItem);
	m_mapCanvas.refresh();*/
}

//接收情报丰度小图片,回调函数
void MainWindow::addJpgAnnotaionCenter1(Mappoint *mpt, double width, double height, int a)
{
    JpgAnnotation *pa= new JpgAnnotation();
    if (pa)
    {
		pa->setMapPosition(QgsPointXY(mpt->x, mpt->y));
		pa->setMapPositionCrs(m_mapCanvas->mapSettings().destinationCrs());
		pa->setFrameSizeMm(QSizeF(50, 25));
		pa->width	= width;
		pa->height	= height;
		pa->setFilePath(mpt->messageId);				//设置图片路径
		pa->setMapLayer(g_pSmallJpgLayer);				//设置小图片图层
		//pa->mMapCanvas = &m_mapCanvas;
		pa->setFrameOffsetFromReferencePointMm(QPoint(0, 0));
		pa->mppoint = mpt;

		QgsStringMap props;
		props.insert(QStringLiteral("color"), QStringLiteral("255,255,255,0"));
		props.insert(QStringLiteral("style"), QStringLiteral("solid"));
		props.insert(QStringLiteral("style_border"), QStringLiteral("solid"));
		props.insert(QStringLiteral("color_border"), QStringLiteral("255,255,255,0"));
		props.insert(QStringLiteral("width_border"), QStringLiteral("0.3"));
		props.insert(QStringLiteral("joinstyle"), QStringLiteral("miter"));
		pa->setFillSymbol(QgsFillSymbol::createSimple(props));

		QgsProject::instance()->annotationManager()->addAnnotation(pa);
    }
}
//隐藏单个小图片目标
void MainWindow::hideLittleSelectedPicture()
{
    QList<QGraphicsItem *> gItemList = m_mapCanvas->scene()->selectedItems();
    QList<QGraphicsItem *>::iterator it = gItemList.begin();
    for ( ; it != gItemList.end(); ++it)
    {
      QgsMapCanvasAnnotationItem *aItem = dynamic_cast<QgsMapCanvasAnnotationItem *>( *it );
      if ( aItem )
      {
         JpgAnnotation* anotation = reinterpret_cast<JpgAnnotation*>(aItem->annotation());
         aItem->setSelected(false);
         anotation->isrender = false;
       }
    }
}

//隐藏大图片
void MainWindow::hideBigPicture()
{
    QgsRectangle qgsRect =  m_mapCanvas->extent();
    new QgsLayerTreeMapCanvasBridge(QgsProject::instance()->layerTreeRoot(), (QgsMapCanvas*)m_mapCanvas, this);
    QgsLayerTree *qgsroot = QgsProject::instance()->layerTreeRoot();
    QList<QgsLayerTreeNode *> nodelist = qgsroot->children();

    foreach (QgsLayerTreeNode * node, nodelist) {
        if (node->name() == QString("annotationLayer"))
        {
            if (node->itemVisibilityChecked ()){
              node->setItemVisibilityChecked(false);
            }else{
               node->setItemVisibilityChecked(true);
            }
            break;
        }
    }
    m_mapCanvas->setExtent(qgsRect);
}

//隐藏/显示全部小图片,针对整个图层
void MainWindow::hideSmallPicture()      
{
	//bool b = g_pSmallJpgLayer->hasScaleBasedVisibility();
	//g_pSmallJpgLayer->setScaleBasedVisibility(!b);

	QgsRectangle qgsRect = m_mapCanvas->extent();
	new QgsLayerTreeMapCanvasBridge(QgsProject::instance()->layerTreeRoot(), (QgsMapCanvas*)m_mapCanvas, this);
	QgsLayerTree *qgsroot = QgsProject::instance()->layerTreeRoot();
	QList<QgsLayerTreeNode *> nodelist = qgsroot->children();

	foreach(QgsLayerTreeNode * node, nodelist) {
		QString sName = node->name();
		if (node->name() == QString("annotationLayer"))
		{
			
		}
		if (node->name() == QString("eletroPoint.shp"))
		{
			if (node->itemVisibilityChecked()) {
				node->setItemVisibilityChecked(false);
			}
			else {
				node->setItemVisibilityChecked(true);
			}
			break;
		}
	}
	m_mapCanvas->setExtent(qgsRect);
}
//设置菜单和工具栏，命令按钮回调函数
void MainWindow::setEvents()
{
    connect((QgsMapCanvas*)m_mapCanvas,&QgsMapCanvas::xyCoordinates,this,&MainWindow::showMousePoint); //鼠标回调函数
	connect((QgsMapCanvas*)m_mapCanvas,&QgsMapCanvas::scaleChanged, this,&MainWindow::scaleChanged); //地图缩放回调函数

	//工具栏-雷达相关
	connect(ui->actionRadarGo, &QAction::triggered, this, &MainWindow::RadarTouying);					//显示雷达投影
	connect(ui->mActionSelectRadarLayer, &QAction::triggered, this, &MainWindow::selectRadarLayer);		//选中雷达图层
//	connect(ui->mActionSelectRadarDev, &QAction::triggered, this, &MainWindow::selectRadarDevice);		//选中雷达设备
	connect(ui->mActionMoveRadarDev, &QAction::triggered, this, &MainWindow::moveRadarDevice);			//移动雷达设备
	connect(ui->mActionDelRadarDev, &QAction::triggered, this, &MainWindow::deleteRadarDevice);			//删除雷达设备
	connect(ui->mActionFixRadar, &QAction::triggered, this, &MainWindow::fixRadarDevice);				//修改雷达设备
	connect(ui->mActionOutfitMove, &QAction::triggered, this, &MainWindow::OutfitMove);					//装备移动
	connect(ui->mActionRadarTestAirList, &QAction::triggered, this, &MainWindow::RadarTestAirList);		//雷达探测无人机列表

	//工具栏-无人机任务区域相关
	connect(ui->mActionAddTaskRect, &QAction::triggered, this, &MainWindow::AddAirTaskArea);			//添加无人机任务区域
	connect(ui->mActionSelTaskLayer, &QAction::triggered, this, &MainWindow::SelectTaskLayer);			//选择无人机任务区域图层
	connect(ui->mActionSelTaskArea, &QAction::triggered, this, &MainWindow::SelectTaskArea);			//选择无人机任务区域
	connect(ui->mActionFixTaskArea, &QAction::triggered, this, &MainWindow::FixTaskArea);				//修改无人机任务区域
	connect(ui->mActionMoveTaskArea, &QAction::triggered, this, &MainWindow::MoveTaskArea);				//移动无人机任务区域
	connect(ui->mActionDelTaskArea, &QAction::triggered, this, &MainWindow::DeleteTaskArea);			//删除无人机任务区域
	connect(ui->mActionRadarNo, &QAction::triggered, this, &MainWindow::ShowRadarNoArea);				//显示雷达已覆盖任务区域
	connect(ui->mActionRadarOverList, &QAction::triggered, this, &MainWindow::AreaRadarList);			//显示任务区域所覆盖的雷达列表
	connect(ui->mActionTaskGuihua, &QAction::triggered, this, &MainWindow::TaskGuihua);					//任务规划

    //菜单-设置
    connect(ui->mActionLoadShp,SIGNAL(triggered()),this,SLOT(addVectorlayers()));				//打开sharp文件图层
    //connect( ui->mActionLoadRaster, SIGNAL( triggered() ), this, SLOT( addRasterlayers()));	//打开加载光栅tif文件图层
    connect(ui->mActionEditStyle, SIGNAL( triggered() ), this, SLOT( editVectorSymbol() ));		//编辑风格
    connect(ui->mActionSetLayers, SIGNAL( triggered() ), this, SLOT(settingLayer()));			//设置图层
	connect(ui->mActionHidePictureTable, &QAction::triggered, this, &MainWindow::hidetable);	//隐藏显示图片栈

    connect(ui->mActionEquipment,&QAction::triggered, this, &MainWindow::equipmentManager);		//装备库管理
    connect(ui->mActionHideBigPicture,&QAction::triggered, this,&MainWindow::hideBigPicture);	//隐藏大图片
	connect(ui->mActionHideSmallJpg,&QAction::triggered, this, &MainWindow::hideSmallPicture);	//隐藏显示小图片
	connect(ui->mActionHideTable,	&QAction::triggered, this, &MainWindow::hideTable);			//隐藏显示表格
	connect(ui->mActionConnect,   &QAction::triggered, this, &MainWindow::connectUDP);			//配置UDP端口并连接
	connect(ui->mActionStartStop, &QAction::triggered, this, &MainWindow::startStopReceive);	//启动/停止接收
	connect(ui->mActionRefreshGJ, &QAction::triggered, this, &MainWindow::RefreshGj);			//刷新轨迹线

    //菜单-绘制，
    connect(ui->mActionMoveView,SIGNAL(triggered()),this,SLOT(moveView()));					//平移视点
    connect(ui->mActionDrawPoint,SIGNAL(triggered()),this,SLOT(addDrawPoint()));			//添加修改目标点
    connect(ui->mActionDrawLine,SIGNAL(triggered()),this,SLOT(addDrawLine()));				//添加线目标
    connect(ui->mActionDrawPolygon,SIGNAL(triggered()),this,SLOT(addDrawPolygon()));		//添加面目标
    connect(ui->mActionCircle,SIGNAL(triggered()),this,SLOT(drawCircle2point()));			//添加圆标注，菜单中的按钮
	connect(ui->mActionCircle2point, SIGNAL(triggered()), this, SLOT(drawCircle2point()));  //添加圆标注,工具栏中的按钮
    connect(ui->mActionSaveLayer,SIGNAL(triggered()),this,SLOT(saveToLayer()));				//保存

    connect(ui->mActionEditingLine,SIGNAL(triggered()),this,SLOT(editLine()));           //编辑线图层
    connect(ui->mActionVertext,SIGNAL(triggered()),this,SLOT(editPolygon()));            //编辑面图层
    //connect(ui->mActionSelectElectricMangative,SIGNAL( triggered() ),this,SLOT( selectElecticMangnetism()));
    //connect(ui->mActionToggleEditing,SIGNAL( triggered() ),this,SLOT(toggleEditing()));		//

    connect(ui->mActionDeleteSelected, &QAction::triggered, this, [=] {                   //删除标注
        deleteSelected( nullptr, nullptr, true );
        m_lblSelectTargetCount->setText("");});

    connect(ui->mActionMoveFeature, &QAction::triggered, this, [ = ] { m_mapCanvas->setMapTool(mMoveFeature );} );//移动标注
    connect(ui->mActionSetFeature, SIGNAL( triggered() ), this, SLOT( modifyFeatures()));  //修改标注

    connect(ui->mActionSelectPoint,SIGNAL( triggered() ),this,SLOT(selectPoint()));			//工具栏-选择目标点图层
    connect(ui->mActionSelectLine,SIGNAL( triggered() ),this,SLOT(selectLine()));			//选择线图层
    connect(ui->mActionSelectPolygon,SIGNAL( triggered() ),this,SLOT(selectPolygon()) );	//选择面图层
    connect(ui->mActionSelectPicture,&QAction::triggered,this,&MainWindow::selectPicture);	//选择图片
    connect(ui->mActionMeaureLength1, SIGNAL( triggered() ),this,SLOT(measureLength()));	//测量长度
    connect(ui->mActionMeasureArea1, SIGNAL( triggered() ), this,SLOT(measureArea()));		//测量面积,measureArea
    connect(ui->mActiontesttest, SIGNAL(triggered()), this, SLOT(testMenu()));				//测试菜单回调函数
	connect(ui->mActionDTback, SIGNAL(triggered()), this, SLOT(dataBack()));				//数据回放
	connect(ui->mAction3D, SIGNAL(triggered()), this, SLOT(show2Dor3Dwindow()));			//显示2dor3d窗口

    /*connect(ui->mActiontesttest,&QAction::triggered, this,[ = ] {                           //测试菜单回调函数
       addJpgAnnotaion(38.1371879578,116.2976303101,38.1353721619,116.2973480225,
                       38.1372795105,116.2958755493,38.1355133057,116.2957534790,"./aweb.jpg");
    });*/

    //菜单-规划
    connect(ui->mActionAirCount, SIGNAL(triggered()), this, SLOT(getPlaneCount()));       //获取飞机信息
    connect(ui->mActionInitalAbundance, SIGNAL( triggered()), this, SLOT(createGrid()));   //战场范围设置
    connect(ui->mActionAbundance, SIGNAL(triggered()), this, SLOT(sendBundiance()));      //发送情报丰度
    connect(ui->mActionGlobalPlan, SIGNAL(triggered()), this, SLOT(ScanPathPlan()));      //全局规划
    connect(ui->mActionLook, SIGNAL(triggered()), this, SLOT(lookDialogShow()));          //情报导出查询

    //重新计数
    connect(ui->mActionTaskSelect,&QAction::triggered,this,[=]{
		g_pTargetLayer->startEditing();
		g_pTargetLayer->selectAll();
		g_pTargetLayer->deleteSelectedFeatures();
		g_pTargetLayer->commitChanges();
		g_pTargetLayer->triggerRepaint();
    });

	connect(ui->mActionMiddle,SIGNAL(triggered()), this, SLOT(mapMiddle()));//地图居中,center
	connect(ui->mActionTuili, SIGNAL(triggered()), this, SLOT(JiqunTuili()));//集群推理

    //以下为主窗口回调函数
    connect(this, &MainWindow::recevieImage,		this,&MainWindow::RecevieLook);				//接收无人机图片
	connect(this, &MainWindow::AddJpgAnnotaionMsg,	this,&MainWindow::addJpgAnnotaionCenter1);	//接收情报丰度小图片,回调函数

    connect(QgsProject::instance()->annotationManager(), &QgsAnnotationManager::annotationAdded, this, &MainWindow::annotationCreated);
    connect(ui->mActionHideTarge,&QAction::triggered,this,&MainWindow::hideLittleSelectedPicture);//隐藏小图片目标
}

//测试菜单
float g_x= 121.931905;
void MainWindow::testMenu()
{
	QString mPath = QCoreApplication::applicationDirPath();
	QString file = mPath + "/recjpg11/image2189.jpg";
	addJpgAnnotaion(25.0036, 121.9158, 25.0036, 121.9168,
					25.0026, 121.9158, 25.0026, 121.9168, file);
	return;
	file = mPath + "/recjpg/air1-0.jpg";
	//addJpgAnnotaion(38.1340406, 116.2969577, 38.134396, 116.2927126,
		//38.1364061, 116.297246, 38.136793102, 116.2931152, file);
	
	
	
	//yaw=0,[air.pitch=90, [air.roll=0
	//addJpgAnnotaion(38.1382004, 116.2889075, 38.1381237, 116.303599355,
		//38.129884, 116.2888992, 38.129992, 116.303534, "./a222.jpg");

	file = mPath + "/recjpg11/image2189.jpg";
	addJpgAnnotaion(38.1352448759, 116.29561764, 38.1352795, 116.2949483,
		38.13562162, 116.2956429, 38.13565655, 116.29498784, file);
	//addJpgAnnotaion(38.1371879578, 116.2976303101, 38.1353721619, 116.2973480225,
		//38.1372795105, 116.2958755493, 38.1355133057, 116.2957534790, "./aweb.jpg");
	//g_pAnnotationLayer->rendererChanged();
	//QgsFeatureRenderer * layerRenderer = g_pSmallJpgLayer->renderer();
	//m_mapCanvas.freeze(false);
	//m_mapCanvas.refresh();
	WId wid=m_mapCanvas->winId();// .parentWidget()->window()->winId();

	//::SendMessage((HWND)wid, BM_CLICK,0,0);
	return;
	
	// 模拟鼠标左键按下
	// 创建鼠标点击事件（左键按下和释放）
	INPUT input[2] = {};

	input[0].type = INPUT_MOUSE;
	input[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;  // 左键按下
	//input[0].mi.dx = 1500;	input[0].mi.dy = 100;
	input[1].type = INPUT_MOUSE;
	input[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;    // 左键释放
	//input[1].mi.dx = 1500;	input[1].mi.dy = 100;
	// 发送按下和释放的事件
	SendInput(2, input, sizeof(INPUT));
	return;
	
	bool b = m_mapCanvas->mapSettings().destinationCrs().isValid();
    //QMessageBox::information(this,"SUCCESS", QString::fromLocal8Bit("情报丰度读取成功!"));

    tag_PlaneMessage message;
    //message.planeId		= QString( "%1" ).arg( int(1) );
    //message.planetype	= QString( "%1" ).arg( int(1) );
    //message.planetime	= QString( "%1" ).arg(1);
    //message.taskname	= QString( "%1" ).arg( int(1));

    double planePositionX=116.242807;
    double planePositionY=38.154527;
    double planePositionZ=11;
    float planeYaw=0,planeRoll=1,planePitch=1;

    message.planeX	= QString("%1").arg(planePositionX,0,'g',14 );
    message.planeY	= QString("%1").arg(planePositionY,0,'g',14 );
    message.xZ		= QString("%1").arg(planePositionZ,0,'g',14 );
    message.Yaw		= QString("%1").arg(planeYaw,0,'g',14 );
    message.Roll	= QString("%1").arg(planeRoll,0,'g',14 );
    message.Pitch	= QString("%1").arg(planePitch,0,'g',14);
    m_planeVector.append(message);
    addPlaneLayer(message);          //根据收到的飞机消息，在图层m_PlaneLayer中添加一个飞机

	QgsPointXY c(planePositionX, planePositionY);
	QgsPointXY d(planePositionX, planePositionY+0.01);

	addSvgAnnotation(c,d);  //在地图上添加一个标注
}

//启动停止接收 mavlink 无人机数据
void MainWindow::startStopReceive()
{
	if (m_pRecThread==nullptr)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("软件还没有接收过无人机 mavlink数据 !"));
		return;
	}

	QString mPath = QCoreApplication::applicationDirPath();

	if (m_pRecThread->m_paused)
	{
		m_pRecThread->m_paused = false;
		ZeroMemory(&m_ShExecInfo, sizeof(SHELLEXECUTEINFO));
		m_ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		m_ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		m_ShExecInfo.lpVerb = ("open");

		//输入要调用的exe文件路径  
		m_ShExecInfo.lpFile = "QtGui.exe";// "ffplay.exe";  // python3
		char buf[128] = {0};
		strcpy(buf, mPath.toStdString().c_str());
		m_ShExecInfo.lpDirectory = buf;
		m_ShExecInfo.lpParameters = "1"; //若没有命令行参数，可为NULL 
		m_ShExecInfo.nShow = SW_HIDE;// SW_SHOW;
		//ShellExecuteEx(&m_ShExecInfo);//::ShellExecute(m_hWnd, "open", "python3.exe rtsp3.py", NULL, NULL, SW_SHOW);
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("启动接收 mavlink 数据!"));
	}
	else
	{
		m_pRecThread->m_paused = true;
		if (m_ShExecInfo.hProcess != NULL)
		{
			TerminateProcess(m_ShExecInfo.hProcess, 0);
			//等待程序运行完毕  
			WaitForSingleObject(m_ShExecInfo.hProcess, INFINITE);
		}
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("停止接收 mavlink 数据!"));
	}
}

// 配置UDP接收端口并连接 / 断开切换
void MainWindow::connectUDP()
{
	// ── 已连接：点击"断开"→ 停止数据接入 ──────────────────────────────
	if (m_pRecThread != nullptr)
	{
		if (m_pAirLayerTimer) m_pAirLayerTimer->stop();
		if (m_pInterpTimer)   m_pInterpTimer->stop();

		// 删除所有标牌，从画布场景同步移除
		for (biaopai *pPai : m_planeIDvec)
			delete pPai;
		m_planeIDvec.clear();
		m_planeVector.clear();
		m_latestPlaneData.clear();
		m_pendingNewPlanes.clear();	// 丢弃断开前尚未提交的新飞机队列

		// 断开并销毁所有 FixPlaneThread，防止 stale m_cachedFid 在重连后污染新 feature
		for (FixPlaneThread *pThread : m_planeThreadVec)
		{
			QObject::disconnect(this, &MainWindow::FixPlaneMsg, pThread, &FixPlaneThread::FixPlane);
			pThread->terminate();
			pThread->wait(500);
			delete pThread;
		}
		m_planeThreadVec.clear();
		ui->mPlaneWidget->setRowCount(0);	// 清空数据表格，防止重连后重复显示
		m_PlaneNumEditer->setText(QString::fromLocal8Bit("收到飞机数量: 0"));

		// 清理无人机 QGIS 图层内所有图元
		if (g_pAirLayer)
		{
			if (!g_pAirLayer->isEditable()) g_pAirLayer->startEditing();
			QgsFeature _feat;
			QgsFeatureIterator _fit = g_pAirLayer->getFeatures();
			while (_fit.nextFeature(_feat))
				g_pAirLayer->deleteFeature(_feat.id());
			g_pAirLayer->commitChanges();
			g_pAirLayer->triggerRepaint();
		}

		// 停止接收线程
		m_pRecThread->m_stopped = true;
		if (!m_pRecThread->wait(2000))
		{
			m_pRecThread->terminate();
			m_pRecThread->wait();	// terminate() 是异步的，必须等待线程真正结束再 delete
		}
		delete m_pRecThread;
		m_pRecThread = nullptr;

		// 排空中转队列，防止 TransitThread 将断开前的残留消息发给 registerPlane
		g_transitMutex.lock();
		g_transitList.clear();
		g_transitMutex.unlock();

		// 恢复按钮为"连接"状态
		ui->mActionConnect->setText(QString::fromLocal8Bit("连接"));
		ui->mActionConnect->setToolTip(QString::fromLocal8Bit("配置UDP接收端口并连接"));
		ui->mActionConnect->setIcon(QIcon(":/images/themes/default/mActionWms.svg"));

		if (m_pAirLayerTimer) m_pAirLayerTimer->start();
		if (m_pInterpTimer)   m_pInterpTimer->start();
		return;
	}

	// ── 未连接：点击"连接"→ 配置端口并接入数据 ──────────────────────────
	QString mPath = QCoreApplication::applicationDirPath();
	QSettings settings(mPath + "/config.ini", QSettings::IniFormat);
	int currentPort = settings.value("UDPPort/planePort", 12300).toInt();

	bool ok;
	int port = QInputDialog::getInt(this,
		QString::fromLocal8Bit("配置UDP接收端口"),
		QString::fromLocal8Bit("请输入无人机数据接收端口："),
		currentPort, 1024, 65535, 1, &ok);
	if (!ok)
		return;

	// 保存新端口到配置文件
	settings.setValue("UDPPort/planePort", port);
	settings.sync();

	// 重置注册节流计时器，确保重连后立即注册所有飞机（连接时重置，不在断开时重置）
	{ extern DWORD iLastTick; iLastTick = 0; }

	// 启动接收线程
	m_pRecThread = new RecMavlinkThread();
	m_pRecThread->m_pDlg = this;
	m_pRecThread->start();

	// 若中转线程尚未创建（首次连接），则创建之
	if (m_pTransitThread == nullptr)
	{
		m_pTransitThread = new TransitThread();
		connect(m_pTransitThread, &TransitThread::receviePlaneMsg, this, &MainWindow::registerPlane);
		m_pTransitThread->start();
	}

	// 切换按钮为"断开"状态
	ui->mActionConnect->setText(QString::fromLocal8Bit("断开"));
	ui->mActionConnect->setToolTip(QString::fromLocal8Bit("断开无人机数据接入"));
	ui->mActionConnect->setIcon(QIcon(":/images/themes/default/mActionStop.svg"));
	// 状态栏简短提示，不弹 QMessageBox（QMessageBox 的本地事件循环会在飞机注册前处理信号，干扰首帧渲染）
	statusBar()->showMessage(QString::fromLocal8Bit("已连接端口 %1，等待无人机数据…").arg(port), 5000);
}

// void MainWindow::mapMiddle()//地图居中,center
// {
// 	QgsPointXY center(116.28965, 38.15358); //泊头
// 	//QgsPointXY center(108.7959, 37.9229);	//靖边
// 	//QgsPointXY center(121.94059, 25.01320);//台湾
// 	m_mapCanvas->setCenter(center);  //设置地图居中
// 	m_mapCanvas->zoomScale(10000);
// 	m_mapCanvas->refresh();
// }
void MainWindow::mapMiddle()//地图居中
{
	QgsPointXY center;
#ifdef TAIWAN
	center.set(121.94059, 25.01320);//台湾 
#endif

#ifdef BOTOU
	center.set(116.28965, 38.15358); //泊头
#endif

#ifdef HUAYIN
	center.set(109.9872, 34.6056); //华阴县 
#endif	

#ifdef JINGBIAN
	center.set(108.7959, 37.9229);	//靖边
#endif	
	
	m_mapCanvas->zoomScale(10000);
	m_mapCanvas->setCenter(center);  //设置地图居中
}
//战场范围设置
void MainWindow::createGrid()
{
#ifdef HUAYIN
	return;
#endif

    //如果之前创建过，就删除它
    if (m_gridDialog)
        delete m_gridDialog;

	m_mapCanvas->setCurrentLayer(g_pPolygonLayer);
//	g_pPlaneLayer->startEditing();

    m_gridDialog = new GridDialog(this);
    m_gridDialog->mCanvas = (QgsMapCanvas*)m_mapCanvas;
    m_gridDialog->setLayersOptions(); //   m_gridDialog->setWindowTitle(QString::fromLocal8Bit("战场初始设置"));
    m_gridDialog->show();
    //m_gridDialog->raise();//增加，提高(数量、水平等)；提升；引起；抚养；举起；筹款；饲养,
    m_gridDialog->activateWindow();
}
//设置图层，回调函数
void MainWindow::settingLayer()
{
    QSetLayerDialog *setLayerDialog = new QSetLayerDialog(this, (QgsMapCanvas*)m_mapCanvas);
    setLayerDialog->setWindowTitle(QString::fromLocal8Bit("图层设置" ));
    setLayerDialog->show();
    setLayerDialog->raise();
    setLayerDialog->activateWindow();
}

void MainWindow::equipmentManager()	//装备库管理
{
	DlgEquipment dlg;
	dlg.exec();
}

//电子磁性图层，回调函数
void MainWindow::selectElecticMangnetism()
{
    m_mapCanvas->setCurrentLayer(m_electroMagnetism);
}

//渲染目标点图层风格
void MainWindow::targetLayerMarker()
{
	QString mDir = QCoreApplication::applicationDirPath();
	QgsSvgMarkerSymbolLayer *svgMarker = new QgsSvgMarkerSymbolLayer(mDir + "/images/svg/transport/point.svg");
	svgMarker->setColor(QColor(Qt::red));
	svgMarker->setSize(20);

	QgsSymbolLayerList symlist;
	symlist.append(svgMarker);	//
	QgsSingleSymbolRenderer *symRendere = new QgsSingleSymbolRenderer(new QgsMarkerSymbol(symlist));
	g_pTargetLayer->setRenderer(symRendere);
}

//设置网格图层风格
void MainWindow::gridLayerMarker()
{
	QString mDir = QCoreApplication::applicationDirPath();
    QgsExpression expressioncolor(" color_hsla( 10, \"dundance1\"  * 100,\"dundance1\"  * 100,120) " );
    QgsSvgMarkerSymbolLayer *svgMarker = new QgsSvgMarkerSymbolLayer(mDir +"/images/svg/religion/place_of_worship_unknown3.svg");
    svgMarker->setColor( QColor(0,0,0,200));
    svgMarker->setSize( 20);
    svgMarker->setDataDefinedProperty(QgsSymbolLayer::PropertyFillColor, QgsProperty::fromExpression(expressioncolor));

    QgsSymbolLayerList symlist;
    symlist.append(svgMarker);
    
    QgsSingleSymbolRenderer *symRendere = new QgsSingleSymbolRenderer(new QgsMarkerSymbol(symlist));
	g_pGirdLayer->setRenderer(symRendere);
}
//设置线图层风格
void MainWindow::lineLayerMarker()
{
    QgsExpression expressioncolor("color_hsla(  \"id\"  % 1000 * 1.2 * 300.0 / count(\"id\")  ,100,50,125)");
    
	QgsSimpleLineSymbolLayer* symbolayer =  new QgsSimpleLineSymbolLayer();
    symbolayer->setDataDefinedProperty( QgsSymbolLayer::PropertyStrokeColor, QgsProperty::fromExpression(expressioncolor) );
    symbolayer->setWidth(1);	//QColor redColor = Qt::red;
	symbolayer->setColor(QColor(Qt::red));

	//设置无人机轨迹线风格
	QgsSimpleLineSymbolLayer* sylayer = new QgsSimpleLineSymbolLayer();
	sylayer->setDataDefinedProperty(QgsSymbolLayer::PropertyStrokeColor, QgsProperty::fromExpression(expressioncolor));
	sylayer->setWidth(0.1);	//
	sylayer->setColor(QColor(Qt::blue));

    QgsSymbolLayerList symlist, slist;
    symlist.append(symbolayer);
	slist.append(sylayer);
    
	g_pLineLayer->setRenderer(new QgsSingleSymbolRenderer(new QgsLineSymbol(symlist)));
	//m_pPlaneGjLayer->setRenderer(new QgsSingleSymbolRenderer(new QgsLineSymbol(slist)));
}

//设置电子磁性图层风格
void MainWindow::electicMagnetismMarker()
{
    QgsSimpleLineSymbolLayer* symbolayer=new QgsSimpleLineSymbolLayer();
    symbolayer->setDataDefinedProperty(QgsSymbolLayer::PropertyStrokeColor, QgsProperty::fromValue(QColor(0,255,0)));
    symbolayer->setWidth(1.0);
    QgsSymbolLayerList symlist;
    symlist.append(symbolayer);

    QgsLineSymbol *markSym = new QgsLineSymbol(symlist);
    QgsSingleSymbolRenderer * symRendere = new QgsSingleSymbolRenderer( markSym );
	m_electroMagnetism->setRenderer(symRendere);
}

//设置无人机图层风格
void MainWindow::planeLayerMarker(QgsVectorLayer *pLayer)
{
    QgsExpression expressionAngle("\"yaw\"");
	QString mDir = QCoreApplication::applicationDirPath();
    QgsSvgMarkerSymbolLayer *svgMarker = new QgsSvgMarkerSymbolLayer(mDir +"/images/air.svg");
    svgMarker->setColor(QColor(Qt::red));
    svgMarker->setSize(10);
    svgMarker->setDataDefinedProperty(QgsSymbolLayer::PropertyAngle, QgsProperty::fromExpression(expressionAngle));

    QgsSymbolLayerList symlist;
    symlist.append(svgMarker);

    QgsMarkerSymbol *pMark= new QgsMarkerSymbol(symlist);    
	pLayer->setRenderer(new QgsSingleSymbolRenderer(pMark));
}

//渲染轨迹线点图层
void MainWindow::guijiLinePointMarker()			
{
	QString mDir = QCoreApplication::applicationDirPath();
	QgsSvgMarkerSymbolLayer* svgMarker = new QgsSvgMarkerSymbolLayer(mDir + "/images/point.svg");//黑色轨迹
	svgMarker->setSize(2);
	//svgMarker->setColor(QColor(Qt::blue));//这个语句不好用

	QgsSymbolLayerList symList;
	symList.append(svgMarker);

	QgsMarkerSymbol* markSym = new QgsMarkerSymbol(symList);	
	g_pPointLayer->setRenderer(new QgsSingleSymbolRenderer(markSym));
}

void MainWindow::radarDeviceMarker()			//渲染雷达设备图层
{
	QString mDir = QCoreApplication::applicationDirPath();	
	QSettings settings(mDir + "/config.ini", QSettings::IniFormat);
	QString slist = settings.value("DeviceType/list").toString();
	QStringList list = slist.split("|");

	for (int i = 0; i < list.size(); i++)
	{
		QgsSvgMarkerSymbolLayer* svgMarker = nullptr;
		if (i ==0) svgMarker = new QgsSvgMarkerSymbolLayer(mDir + "/images/qgis1.svg");
		if (i ==1) svgMarker = new QgsSvgMarkerSymbolLayer(mDir + "/images/qgis2.svg");
		if (i ==2) svgMarker = new QgsSvgMarkerSymbolLayer(mDir + "/images/qgis3.svg");
		if (i ==3) svgMarker = new QgsSvgMarkerSymbolLayer(mDir + "/images/qgis4.svg");
		if (i ==4) svgMarker = new QgsSvgMarkerSymbolLayer(mDir + "/images/qgis5.svg");

		svgMarker->setSize(10); //svgMarker->setColor(QColor(Qt::blue));//这个语句不好用

		QgsSymbolLayerList symList;
		symList.append(svgMarker);

		QgsMarkerSymbol* markSym = new QgsMarkerSymbol(symList);
		gRadarLayerList[i]->setRenderer(new QgsSingleSymbolRenderer(markSym));
	}
	
}
//渲染雷达轨迹图层
void MainWindow::radarDevTrackMarker()		
{
	QString mDir = QCoreApplication::applicationDirPath();
	QgsSvgMarkerSymbolLayer* svgMarker = new QgsSvgMarkerSymbolLayer(mDir + "/images/red.svg");//红色轨迹
	svgMarker->setSize(2);
	//svgMarker->setColor(QColor(Qt::red));//这个语句不好用

	QgsSymbolLayerList symList;
	symList.append(svgMarker);

	QgsMarkerSymbol* markSym = new QgsMarkerSymbol(symList);
	g_pRadarPtLayer->setRenderer(new QgsSingleSymbolRenderer(markSym));
}

//渲染无人机任务区域图层
void MainWindow::airTaskAreaMarker()	
{
	//QgsExpression expressionAngle("\"yaw\"");
	//渲染图层
	QgsSymbol	*symbol = QgsSymbol::defaultSymbol(g_pAirTaskPolyLayer->geometryType());	
	qreal		opacity = 0.3;		//透明度

	symbol->setColor(QColor(255, 0, 250));//红色
	symbol->setOpacity(opacity);

	g_pAirTaskPolyLayer->setRenderer(new QgsSingleSymbolRenderer(symbol));
}
//打开sharp 文件图层
void MainWindow::addVectorlayers()
{
      QgsVectorLayer* vecLayer=nullptr;

      QString fileName = QFileDialog::getOpenFileName(this, QString::fromLocal8Bit("打开矢量数据"), "", "*.shp");
      QStringList temp = fileName.split('/');
      QString basename = temp.at(temp.size() - 1);
      qDebug()<<basename;
      vecLayer = new QgsVectorLayer(fileName, basename, "ogr");
      vecLayer->setLabelsEnabled(true);
      if(fileName.isNull())
          return;

      if (!vecLayer->isValid())
      {
          QMessageBox::critical(this, "error", QString::fromLocal8Bit("图层无效！"));
          return;
      }
      int currentFieldIndexOf = vecLayer->fields().indexOf( "class" );
      QSet<QVariant> unique = vecLayer->uniqueValues( currentFieldIndexOf );

      QVariantList uniqueValues = unique.toList();

      QgsCategoryList cats = QgsCategorizedSymbolRenderer::createCategories( uniqueValues, QgsSymbol::defaultSymbol( vecLayer->geometryType() ), vecLayer, "class" );
      QColor startColor;
      int num = 0;
      int num1 = 0;

      for(auto iter = cats.begin();iter != cats.end(); ++iter)
      {
          QColor c(0,5+num,255-num1);
          QRgb mRgb = qRgb(c.red(),c.green(),c.blue());
          startColor.setRgba(mRgb);
          iter->symbol()->setColor(startColor);
          num += 50;
          num1 += 50;
      }
      std::unique_ptr< QgsCategorizedSymbolRenderer > r = qgis::make_unique< QgsCategorizedSymbolRenderer >( "class", cats );

      vecLayer->setRenderer(r->clone());
      m_layers.insert(0,vecLayer);
      m_mapCanvas->setLayers(m_layers);

      QgsProject::instance()->addMapLayer(vecLayer);
      m_mapCanvas->zoomToFullExtent();
      m_mapCanvas->refresh();
}
//平移地图工具
void MainWindow::moveView()
{
    m_mapCanvas->setMapTool(mToolPan);
    mToolPan->activate();
}
//添加修改目标点工具
void MainWindow::addDrawPoint()
{
    m_mapCanvas->setMapTool(mMapToolAddTarget);
    m_mapCanvas->setCurrentLayer(g_pTargetLayer);
}
//添加线目标工具
void MainWindow::addDrawLine()
{
	m_mapCanvas->setCurrentLayer(g_pLineLayer);
	g_pLineLayer->startEditing();
	m_mapCanvas->setMapTool(mMapToolAddline);
}
//添加面目标工具
void MainWindow::addDrawPolygon()
{
   m_mapCanvas->setCurrentLayer(g_pPolygonLayer);
   g_pPolygonLayer->startEditing();
   m_mapCanvas->setMapTool(mMapToolAddPolygon);
}
//添加圆形图层标注工具
void MainWindow::drawCircle2point()
{
    m_mapCanvas->setCurrentLayer(g_pPolygonLayer);
	g_pPolygonLayer->startEditing();
    m_mapCanvas->setMapTool(mMapToolCircle2d);
}

//保存图层工具
void MainWindow::saveToLayer()
{    
    m_mapCanvas->setCurrentLayer(g_pTargetLayer);
	g_pTargetLayer->startEditing();
    saveEdits(g_pTargetLayer,true, true);

	m_mapCanvas->setCurrentLayer(g_pLineLayer);
	g_pLineLayer->startEditing();
	saveEdits(g_pLineLayer, true, true);

	m_mapCanvas->setCurrentLayer(g_pPolygonLayer);
	g_pPolygonLayer->startEditing();
	saveEdits(g_pPolygonLayer, true, true);

	m_mapCanvas->setCurrentLayer(g_pPointLayer);
	g_pPointLayer->startEditing();
	saveEdits(g_pPointLayer, true, true);

	QString mPath = QCoreApplication::applicationDirPath();
	QSettings settings(mPath + "/config.ini", QSettings::IniFormat);
	QString slist = settings.value("DeviceType/list").toString();
	QStringList list = slist.split("|");

	for (int i = 0; i < list.size(); i++)
	{
		m_mapCanvas->setCurrentLayer(gRadarLayerList[i]);
		gRadarLayerList[i]->startEditing();
		saveEdits(gRadarLayerList[i], true, true);
	}

	m_mapCanvas->setCurrentLayer(g_pAirTaskPolyLayer);
	g_pAirTaskPolyLayer->startEditing();
	saveEdits(g_pAirTaskPolyLayer, true, true);
}
//菜单-工具栏-选中目标点图层工具
void MainWindow::selectPoint()
{
    m_mapCanvas->setMapTool(mToolSelect);
    m_mapCanvas->setCurrentLayer(g_pTargetLayer);
	g_pTargetLayer->startEditing();
}
//选中线图层工具
void MainWindow::selectLine()
{
    mToolSelect->setAction( ui->mActionSelectLine);
    m_mapCanvas->setMapTool(mToolSelect);
    m_mapCanvas->setCurrentLayer(g_pLineLayer);
	g_pLineLayer->startEditing();
}
//选中面图层
void MainWindow::selectPolygon()
{
    m_mapCanvas->setMapTool( mToolSelect);
    m_mapCanvas->setCurrentLayer(g_pPolygonLayer);
	g_pPolygonLayer->startEditing();
}
//选中图片图层
void MainWindow::selectPicture()
{
   m_mapCanvas->setMapTool( mMaptoolJpgAnnotation);
}
void MainWindow::SingleColor(QgsVectorLayer* vlayer)
{
    //渲染图层
    QgsSymbol *symbol = QgsSymbol::defaultSymbol(vlayer->geometryType());
    QSettings settings("HelloPlane", "suzs");

    QColor color  = settings.value(vlayer->name() + "/color",QColor(100,100,100)).value<QColor>();
    qreal opacity = settings.value(vlayer->name() + "/opacity",0.5).value<qreal>();
    symbol->setColor(color);
    symbol->setOpacity(opacity);

    QgsSingleSymbolRenderer *mypRenderer = new QgsSingleSymbolRenderer(symbol);
    vlayer->setRenderer(mypRenderer);
}

//根据fileName文件名称，加载sharp图层文件
void MainWindow::LoadVectorlayers(QString fileName,QgsVectorLayer** gloablayer)
{
    QFile file(fileName);
    if(file.exists()==false)
    {
        QMessageBox::information(this,"SUCCESS",QString::fromLocal8Bit("LoadVectorlayers函数，文件没有发现"));
        QMessageBox::information(this,"SUCCESS",fileName);
        return;
    }

    QgsVectorLayer* vecLayer=nullptr;
    QStringList temp = fileName.split('/');
    QString basename = temp.at(temp.size() - 1);

	//QgsVectorLayer *pMemoryLayer = new QgsVectorLayer("Polygon", "MemoryTestMap", "memory");
	vecLayer = new QgsVectorLayer(fileName, basename, "ogr");
    vecLayer->setLabelsEnabled(true);
    *gloablayer =  vecLayer;

	m_layers.append(vecLayer);

    /*if (m_layers.count() > 0) {
    m_layers.insert(0,vecLayer);
    }else{
    m_layers.append(vecLayer);
    }*/

    m_mapCanvas->setLayers(m_layers);
    QgsProject::instance()->addMapLayer(vecLayer);
    
    //m_mapCanvas->refresh();
}
//编辑线图层
void MainWindow::editLine()
{
     m_mapCanvas->setCurrentLayer(g_pLineLayer);
	 g_pLineLayer->startEditing();
     m_mapCanvas->setMapTool(mMapToolVertext) ;
}
//编辑面图层
void MainWindow::editPolygon()
{
    m_mapCanvas->setCurrentLayer(g_pPolygonLayer);
	g_pPolygonLayer->startEditing();
    m_mapCanvas->setMapTool(mMapToolVertext);
}

void MainWindow::commitError(QgsVectorLayer *vlayer)
{
	const QStringList commitErrors = vlayer->commitErrors();
	if (!vlayer->allowCommit() && commitErrors.empty())
		return;

	QgsMessageViewer *mv = new QgsMessageViewer();
	mv->setWindowTitle(tr("Commit Errors"));
	mv->setMessageAsPlainText(tr("Could not commit changes to layer %1").arg(vlayer->name())
		+ "\n\n"
		+ tr("Errors: %1\n").arg(commitErrors.join(QLatin1String("\n  ")))
	);

	QToolButton *showMore = new QToolButton();
	// store pointer to vlayer in data of QAction
	QAction *act = new QAction(showMore);
	act->setData(QVariant(QMetaType::QObjectStar, &vlayer));
	act->setText(tr("Show more"));
	showMore->setStyleSheet(QStringLiteral("background-color: rgba(255, 255, 255, 0); color: black; text-decoration: underline;"));
	showMore->setCursor(Qt::PointingHandCursor);
	showMore->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
	showMore->addAction(act);
	showMore->setDefaultAction(act);
	connect(showMore, &QToolButton::triggered, mv, &QDialog::exec);
	connect(showMore, &QToolButton::triggered, showMore, &QObject::deleteLater);

	// no timeout set, since notice needs attention and is only shown first time layer is labeled
	QgsMessageBarItem *errorMsg = new QgsMessageBarItem(tr("Commit errors"), tr("Could not commit changes to layer %1").arg(vlayer->name()),
		showMore, Qgis::Warning, 0, m_pInfoBar);
	m_pInfoBar->pushItem(errorMsg);
}

//初始化右侧表格窗口
void MainWindow::initRightList()
{
}

//初始化打击侦察详情窗口
void MainWindow::initJpgAnnoWidget()
{
    ui->mTableWidgetMarkAttribute->setColumnCount( 5);
    int i = 0;
    //ui->mTableWidgetMarkAttribute->setHorizontalHeaderItem( i++, new  QTableWidgetItem(QString::fromLocal8Bit("打击")));
    //ui->mTableWidgetMarkAttribute->setHorizontalHeaderItem( i++, new  QTableWidgetItem(QString::fromLocal8Bit("侦察")) );
    //ui->mTableWidgetMarkAttribute->setHorizontalHeaderItem( i++, new  QTableWidgetItem(QString::fromLocal8Bit("祥情")) );
    //ui->mTableWidgetMarkAttribute->setHorizontalHeaderItem( i++, new  QTableWidgetItem(QString::fromLocal8Bit("放弃")));
    //ui->mTableWidgetMarkAttribute->setHorizontalHeaderItem( i++, new  QTableWidgetItem(QString::fromLocal8Bit("确认")));
    ui->mTableWidgetMarkAttribute->verticalHeader()->setVisible(false);
    ui->mTableWidgetMarkAttribute->horizontalHeader()->setVisible(false);

    int collomwidth = 80;
    ui->mTableWidgetMarkAttribute->setColumnWidth( 0,collomwidth );
    ui->mTableWidgetMarkAttribute->setColumnWidth( 1,collomwidth );
    ui->mTableWidgetMarkAttribute->setColumnWidth( 2,collomwidth );
    ui->mTableWidgetMarkAttribute->setColumnWidth( 3,collomwidth );
    ui->mTableWidgetMarkAttribute->setColumnWidth( 4,collomwidth );

    QSize buttonsize(70,50);
    ui->mTableWidgetMarkAttribute->insertRow(0);
    ui->mTableWidgetMarkAttribute->setRowHeight(0,50);

    QFont font = ui->mTableWidgetMarkAttribute->font();
    font.setPixelSize(30);  

    QPushButton *attackButton = new QPushButton;
    attackButton->setText(QString::fromLocal8Bit("打击"));
    attackButton->setFixedSize(buttonsize );

    QPushButton *detectButton = new QPushButton;
    detectButton->setText(QString::fromLocal8Bit("侦察"));
    detectButton->setFixedSize(buttonsize );

    QPushButton *detailButton =new QPushButton;
    detailButton->setText(QString::fromLocal8Bit("详情"));
    detailButton->setFixedSize( buttonsize );

    QPushButton *abundonButton = new QPushButton;
    abundonButton->setText(QString::fromLocal8Bit("放弃"));
    abundonButton->setFixedSize( buttonsize );

    QPushButton *okButton = new QPushButton;
    okButton->setText(QString::fromLocal8Bit("确认"));
    okButton->setFixedSize( buttonsize );

    connect(attackButton,  &QAbstractButton::clicked, this,&MainWindow::updateSqlite );//打击
    connect(detectButton,  &QAbstractButton::clicked, this,&MainWindow::updateSqlite );//侦察
	connect(detailButton, &QAbstractButton::clicked, this, &MainWindow::showDetailForm);//详情
	connect(abundonButton, &QAbstractButton::clicked, this,&MainWindow::updateSqliteBoundance);//放弃
    connect(okButton,      &QAbstractButton::clicked, this,&MainWindow::updateSqlite );//确认    

    ui->mTableWidgetMarkAttribute->setCellWidget(0, 0, attackButton);
    ui->mTableWidgetMarkAttribute->setCellWidget(0, 1, detectButton);
    ui->mTableWidgetMarkAttribute->setCellWidget(0, 2, detailButton);
    ui->mTableWidgetMarkAttribute->setCellWidget(0, 3, abundonButton);
    ui->mTableWidgetMarkAttribute->setCellWidget(0, 4, okButton );

    int listwidth =440;	//打击侦察详情窗口,宽度
    int listheight=110; //打击侦察详情窗口，高度

	//创建打击侦察详情窗口,指针对象
	m_pJpgAnnoWidget = new QWidget(this);
	m_pJpgAnnoWidget->setGeometry(0,140,listwidth,listheight);

    QGridLayout *pLayout = new QGridLayout(m_pJpgAnnoWidget);
	pLayout->addWidget(ui->mTableWidgetMarkAttribute,0,0);
	pLayout->setSpacing(0);

	m_pJpgAnnoWidget->setVisible(false); //设置打击侦察详情窗口为隐藏
}
//初始化编号，时间类型，位置对应任务，窗口
void MainWindow::initWidgetAttribute()
{
    ui->mTableWidgetMarkAttribute->setColumnCount(7);
    int i = 0;
    ui->mTableWidgetMarkAttribute->setHorizontalHeaderItem( i++, new  QTableWidgetItem(QString::fromLocal8Bit("编号") ) );
    ui->mTableWidgetMarkAttribute->setHorizontalHeaderItem( i++, new  QTableWidgetItem(QString::fromLocal8Bit("时间")) );
    ui->mTableWidgetMarkAttribute->setHorizontalHeaderItem( i++, new  QTableWidgetItem(QString::fromLocal8Bit("类型")));
    ui->mTableWidgetMarkAttribute->setHorizontalHeaderItem( i++, new  QTableWidgetItem(QString::fromLocal8Bit("位置")) );
    ui->mTableWidgetMarkAttribute->setHorizontalHeaderItem( i++, new  QTableWidgetItem(QString::fromLocal8Bit("对应任务")));
    ui->mTableWidgetMarkAttribute->setHorizontalHeaderItem( i++, new  QTableWidgetItem(QString::fromLocal8Bit("是否")));
    ui->mTableWidgetMarkAttribute->setHorizontalHeaderItem( i++, new  QTableWidgetItem( "" ) );
    ui->mTableWidgetMarkAttribute->verticalHeader()->setVisible(false);
    int unitcolumwidth = 60;
    ui->mTableWidgetMarkAttribute->setColumnWidth(0,unitcolumwidth);
    ui->mTableWidgetMarkAttribute->setColumnWidth(1,unitcolumwidth*2);
    ui->mTableWidgetMarkAttribute->setColumnWidth(2,unitcolumwidth*2);
    ui->mTableWidgetMarkAttribute->setColumnWidth(3,unitcolumwidth*2);
    ui->mTableWidgetMarkAttribute->setColumnWidth(4,unitcolumwidth*2);
    ui->mTableWidgetMarkAttribute->setColumnWidth(5,unitcolumwidth*1.2);
    ui->mTableWidgetMarkAttribute->setColumnWidth(6,unitcolumwidth);

    ui->mTableWidgetMarkAttribute->insertRow( 0 );
    ui->mTableWidgetMarkAttribute->setItem( 0,0,  new QTableWidgetItem ("") );
    ui->mTableWidgetMarkAttribute->setItem( 0,1,  new QTableWidgetItem ("") );
    ui->mTableWidgetMarkAttribute->setItem( 0,3,  new QTableWidgetItem ("") );

    int listwidth = 730;
    int listheight= 80;

	m_pJpgAnnoWidget = new QWidget(this);
	m_pJpgAnnoWidget->setGeometry(0,140,listwidth,listheight);

    QGridLayout* pLayout = new QGridLayout(m_pJpgAnnoWidget);
	pLayout->addWidget(ui->mTableWidgetMarkAttribute,0,0);
	pLayout->setSpacing( 0 );

	m_pJpgAnnoWidget->setVisible(false);

    QStringList typelist = WeaponTypeList.split("|");
    QStringList tasklist = g_taskList.split("|");
    QComboBox *typebox = new QComboBox;
    typebox->setEditable(true);
    typebox->addItems( typelist );
    QComboBox *taskbox = new QComboBox;
    taskbox->setEditable(true);
    taskbox->addItems( tasklist );

    QComboBox *okbox = new QComboBox;
    okbox->setEditable(true);
    okbox->addItem(QString::fromLocal8Bit("未定"));
    okbox->addItem(QString::fromLocal8Bit("是"));
    okbox->addItem(QString::fromLocal8Bit("否"));

    QPushButton *okbutton = new QPushButton;
    okbutton->setText(QString::fromLocal8Bit("确认"));
    connect(okbutton,&QAbstractButton::clicked, this,&MainWindow::updateSqlite);

    ui->mTableWidgetMarkAttribute->setCellWidget( 0,2,typebox );
    ui->mTableWidgetMarkAttribute->setCellWidget( 0,4,taskbox );
    ui->mTableWidgetMarkAttribute->setCellWidget( 0,5,okbox );
    ui->mTableWidgetMarkAttribute->setCellWidget( 0,6,okbutton );
}
//保存目标点和多边形，图层
void MainWindow::saveEdits( QgsMapLayer *layer, bool leaveEditable, bool triggerRepaint )
{
    QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>( layer );
    if ( !vlayer || !vlayer->isEditable() || !vlayer->isModified() )
		return;

    if (!vlayer->commitChanges(!leaveEditable))
    {
		//mSaveRollbackInProgress = false;
		commitError( vlayer );
    }
    vlayer->triggerRepaint();
}

QgsAttributeEditorContext MainWindow::createAttributeEditorContext()
{
	QgsAttributeEditorContext context;
	context.setVectorLayerTools(mVectorLayerTools); //设置面图层工具
	context.setMapCanvas((QgsMapCanvas*)m_mapCanvas);
	context.setCadDockWidget(mDock);
	context.setMainMessageBar(m_pInfoBar);
	return context;
}

//删除目标点，图元
void MainWindow::deleteSelected(QgsMapLayer *layer, QWidget *parent, bool checkFeaturesVisible )
{
	g_pTargetLayer->startEditing();
	g_pPolygonLayer->startEditing();

	if (!layer)
		layer = m_mapCanvas->currentLayer();

	if (!parent)
		parent = this;

	if (!layer)
	{
		m_pInfoBar->pushMessage(QString::fromLocal8Bit("没有选择层"), QString::fromLocal8Bit("要删除图元, 你必须先选择一个层"), Qgis::Info, 6);
		return;
	}

	QgsVectorLayer *vlayer = qobject_cast<QgsVectorLayer *>(layer);
	if (!vlayer)
	{
		m_pInfoBar->pushMessage(QString::fromLocal8Bit("没有选择层"), QString::fromLocal8Bit("只能删除矢量层的图元"), Qgis::Info, 6);
		return;
	}

	if (!(vlayer->dataProvider()->capabilities() & QgsVectorDataProvider::DeleteFeatures))
	{
		m_pInfoBar->pushMessage(tr("不能删除"), QString::fromLocal8Bit("不支持删除操作"), Qgis::Info,6);
		return;
	}

	if (!vlayer->isEditable())
	{
		m_pInfoBar->pushMessage(tr("层不能编辑"), QString::fromLocal8Bit("当前层不能编辑. 请选择开始编辑."), Qgis::Info, 6);
		return;
	}

	//validate selection
	const int numberOfSelectedFeatures = vlayer->selectedFeatureCount();
	if (numberOfSelectedFeatures == 0)
	{
		m_pInfoBar->pushMessage(tr("没有图元"), QString::fromLocal8Bit("当前层没有图元"), Qgis::Info, 6);
		return;
	}
	//display a warning
	if (checkFeaturesVisible)
	{
		QgsFeature feat;
		QgsFeatureIterator it = vlayer->getSelectedFeatures(QgsFeatureRequest().setNoAttributes());
		bool allFeaturesInView = true;
		QgsRectangle viewRect =m_mapCanvas->mapSettings().mapToLayerCoordinates(vlayer, m_mapCanvas->extent());

		while (it.nextFeature(feat))
		{
			if (allFeaturesInView && !viewRect.intersects(feat.geometry().boundingBox()))
			{
				allFeaturesInView = false;
				break;
			}
		}

		if (!allFeaturesInView)
		{
			// for extra safety to make sure we are not removing geometries by accident
			int res = QMessageBox::warning((QWidget*)m_mapCanvas, tr("Delete %n feature(s) from layer \"%1\"", nullptr, numberOfSelectedFeatures).arg(vlayer->name()),
				tr("Some of the selected features are outside of the current map view. Would you still like to continue?"),
				QMessageBox::Yes | QMessageBox::No);

			if (res != QMessageBox::Yes)
				return;
		}
	}

	QgsVectorLayerUtils::QgsDuplicateFeatureContext infoContext;
	if (QgsVectorLayerUtils::impactsCascadeFeatures(vlayer, vlayer->selectedFeatureIds(), QgsProject::instance(), infoContext, QgsVectorLayerUtils::IgnoreAuxiliaryLayers))
	{
		QString childrenInfo;
		int childrenCount = 0;
		const auto infoContextLayers = infoContext.layers();
		for (QgsVectorLayer *chl : infoContextLayers)
		{
			childrenCount += infoContext.duplicatedFeatures(chl).size();
			childrenInfo += (tr("%1 feature(s) on layer \"%2\", ").arg(infoContext.duplicatedFeatures(chl).size()).arg(chl->name()));
		}

		// for extra safety to make sure we know that the delete can have impact on children and joins
		int res = QMessageBox::question((QWidget*)m_mapCanvas, tr("Delete at least %1 feature(s) on other layer(s)").arg(childrenCount),
			tr("Delete %1 feature(s) on layer \"%2\", %3 as well\nand all of its other descendants.\nDelete these features?").arg(numberOfSelectedFeatures).arg(vlayer->name()).arg(childrenInfo),
			QMessageBox::Yes | QMessageBox::No);

		if (res != QMessageBox::Yes)
			return;
	}

	vlayer->beginEditCommand(tr("Features deleted"));
	int deletedCount = 0;

	QgsVectorLayer::DeleteContext context(true, QgsProject::instance());
	if (!vlayer->deleteSelectedFeatures(&deletedCount, &context))
	{
		m_pInfoBar->pushMessage(tr("Problem deleting features"),
			tr("A problem occurred during deletion from layer \"%1\". %n feature(s) not deleted.", nullptr, numberOfSelectedFeatures - deletedCount).arg(vlayer->name()),
			Qgis::Warning);
	}
	else
	{
		const QList<QgsVectorLayer *> contextLayers = context.handledLayers(false);
		// if it affects more than one non-auxiliary layer, print feedback for all descendants
		if (contextLayers.size() > 1)
		{
			deletedCount = 0;
			QString feedbackMessage;
			for (QgsVectorLayer *contextLayer : contextLayers)
			{
				feedbackMessage += tr("%1 on layer %2. ").arg(context.handledFeatures(contextLayer).size()).arg(contextLayer->name());
				deletedCount += context.handledFeatures(contextLayer).size();
			}
			m_pInfoBar->pushMessage(tr("%1 features deleted: %2").arg(deletedCount).arg(feedbackMessage), Qgis::Success);
		}
		m_pStatusBar->showMessage(tr("%n feature(s) deleted.", "number of features deleted", deletedCount));
	}
	vlayer->endEditCommand();
}

//添加主界面tif格式地图文件
void MainWindow::addRasterlayers()
{
	QString mPath = QCoreApplication::applicationDirPath();
#ifdef BOTOU
	QString fx(mPath + "/botou-map/15x15km/mytif17.tif");
	QString name = QString::fromLocal8Bit("泊头");
#endif

#ifdef TAIWAN
	QString fx(mPath + "/TW-map/tw16L.tif");	 //TW-test1
	//QString fx(mPath + "/TW-map/twsiwei.tif");
	QString name = QString::fromLocal8Bit("台东");
#endif
    
#ifdef HUAYIN
	QString fx(mPath + "/huayin/huayin15L.tif");
	QString name = QString::fromLocal8Bit("华阴县");
#endif

	g_pRasterLayer =new QgsRasterLayer(fx,name,"gdal");

    Q_CHECK_PTR(g_pRasterLayer);
    if (!g_pRasterLayer->isValid())
    {
		QMessageBox::critical(this, "error", QString::fromLocal8Bit("layer valid ....图层无效！"));
		return;
    }
	
    m_layers.append(g_pRasterLayer);
    m_basemapPath = fx;   // 记录当前底图路径，供地图管理对话框标记高亮
    m_tifExtent   = g_pRasterLayer->extent();  // 持久保存TIF范围
    m_mapCanvas->setLayers(m_layers);
    QgsProject::instance()->addMapLayer(g_pRasterLayer);
}

//根据图层所在文件路径，添加一个图层
void MainWindow::addRasterlayers(QString pathname,QString layername)
{
    QString qgisPath = QCoreApplication::applicationDirPath();

    pathname=qgisPath+pathname;

    QMessageBox::critical(this, "error", pathname);
    QgsRasterLayer* rasterLayer=new QgsRasterLayer(pathname,layername,"gdal");

    Q_CHECK_PTR( rasterLayer );
    if (!rasterLayer->isValid())
    {
        QMessageBox::critical(this, "error", QString::fromLocal8Bit("layer valid aaaa无效 ！"));
        return;
    }
    m_mapCanvas->setExtent(rasterLayer->extent());
    m_layers.insert( 0,rasterLayer );

    m_mapCanvas->setLayers(m_layers);
    QgsProject::instance()->addMapLayer(rasterLayer);
    m_mapCanvas->zoomToFullExtent();
    m_mapCanvas->refresh();
}

// 切换TIF底图（由地图管理对话框触发）
void MainWindow::switchBaseMap(const QString& filePath, const QString& layerName)
{
    // 1. 记录旧底图在 m_layers 中的位置，然后移除
    int idx = g_pRasterLayer ? m_layers.indexOf(g_pRasterLayer) : -1;
    if (idx >= 0)
        m_layers.removeAt(idx);

    // 2. 从项目注销（QgsProject 拥有所有权，内部会 deleteLater）
    if (g_pRasterLayer) {
        QgsProject::instance()->removeMapLayer(g_pRasterLayer->id());
        g_pRasterLayer = nullptr;
    }

    // 3. 加载新 TIF
    QgsRasterLayer* newLayer = new QgsRasterLayer(filePath, layerName, "gdal");
    if (!newLayer->isValid()) {
        QMessageBox::warning(this,
            QString::fromLocal8Bit("错误"),
            QString::fromLocal8Bit("地图文件无效:\n") + filePath);
        delete newLayer;
        return;
    }
    g_pRasterLayer = newLayer;
    m_basemapPath  = filePath;
    m_tifExtent    = g_pRasterLayer->extent();   // 持久保存TIF范围，供切换在线地图时恢复

    // 4. 将 TIF 底图追加到末尾（最后 = 渲染最底层/背景），矢量图层保持在前面（前景）。
    // 关键修复：addToLegend=false —— 不将 TIF 加入图层树。
    //   • bridge 只监听图层树变化。若 TIF 不在树中，bridge 就不会因 addMapLayer 触发
    //     deferredSetCanvasLayers，也就不会把 TIF 插入图层树顶部（前景），遮挡装备图标。
    //   • removeMapLayer 仅对"曾在树中"的图层触发 bridge 回调（第一次切换时初始 TIF
    //     是通过 addToLegend=true 加入的，所以会触发一次 bridge 回调；后续切换不再触发）。
    //   • 我们的 singleShot(0) 排在 bridge 的 singleShot(0) 之后执行（FIFO），
    //     始终最后重置图层顺序、CRS、范围和装备标签，确保装备图标可见。
    m_layers.append(g_pRasterLayer);
    QgsProject::instance()->addMapLayer(g_pRasterLayer, false);  // 不加入图层树！

    QgsCoordinateReferenceSystem tifCrs    = g_pRasterLayer->crs();
    QgsRectangle                 tifExtent = g_pRasterLayer->extent();
    QTimer::singleShot(0, this, [this, tifExtent]() {
        // 恢复正确图层顺序（TIF 在末尾 = 背景）
        m_mapCanvas->setLayers(m_layers);
        // 注意：不调用 setDestinationCrs()，镜像启动时的行为。
        // 启动时 canvas CRS 始终为无效（invalid），setDestinationCrs 从未被调用，
        // annotation 的 mapPositionCrs 也是无效 CRS（未显式设置）。
        // 当两者都是 invalid 时，QGIS updatePosition() 跳过坐标变换，直接以
        // WGS84 度数定位，恰好与 canvas extent（也是WGS84度数）对应，标签正确显示。
        // 若调用 setDestinationCrs(validCRS)，canvas CRS 变为有效而 annotation CRS
        // 仍是 invalid，两者不等 → QGIS 尝试构造 QgsCoordinateTransform(invalid, valid)
        // → 变换失败或产生错误坐标 → 标签偏移到屏幕外不可见。
        ShowRadarTip();
        ShowTaskAreaTip();
        m_mapCanvas->setExtent(tifExtent);
        m_mapCanvas->clearCache();
        m_mapCanvas->refresh();
    });
}

// 加载在线瓦片地图为底图（由地图管理对话框触发）
void MainWindow::loadOnlineTileMap(const QString& url, const QString& layerName)
{
    // ── Step 1: 确定目标范围（优先使用持久保存的 TIF 范围） ─────────────────────
    // m_tifExtent 在 switchBaseMap() 加载 TIF 时保存，不随在线地图切换而清空。
    // 若未加载过 TIF，则回退到当前 canvas 范围。
    QgsRectangle targetExtent;
    if (!m_tifExtent.isEmpty()) {
        targetExtent = m_tifExtent;
    } else if (g_pRasterLayer && !m_basemapPath.isEmpty()) {
        targetExtent = g_pRasterLayer->extent();
        m_tifExtent  = targetExtent;
    } else {
        targetExtent = m_mapCanvas->extent();
    }

    // ── Step 2: 加载 WMS/XYZ 图层 ────────────────────────────────────────────────
    // 注意：不在此处调用 setDestinationCrs()！
    // setDestinationCrs() 会把当前范围从"无效CRS"重投影到新CRS；由于无效CRS无法解析，
    // QGIS fallback 为整个投影的全球范围，导致范围扩展为全球。
    // CRS 设置推迟到 Step 5（timer）中，在 setLayers()+setExtent() 之后调用，确保不干扰范围。
    QgsRasterLayer* pLayer = new QgsRasterLayer(url, layerName, "wms");
    if (!pLayer->isValid()) {
        QMessageBox::warning(this,
            QString::fromLocal8Bit("错误"),
            QString::fromLocal8Bit("在线地图加载失败:\n") + url);
        delete pLayer;
        return;
    }

    // ── Step 3: 替换底图图层 ──────────────────────────────────────────────────────
    int idx = g_pRasterLayer ? m_layers.indexOf(g_pRasterLayer) : -1;
    if (idx >= 0)
        m_layers.removeAt(idx);
    if (g_pRasterLayer) {
        QgsProject::instance()->removeMapLayer(g_pRasterLayer->id());
        g_pRasterLayer = nullptr;
    }
    QgsProject::instance()->addMapLayer(pLayer);
    g_pRasterLayer = pLayer;
    m_basemapPath  = QString();
    m_layers.insert(qMax(idx, 0), g_pRasterLayer);
    m_mapCanvas->setLayers(m_layers);

    // ── Step 4: bridge singleShot(0) 回调完成后统一恢复 CRS、图层顺序、范围 ────
    // • bridge 的 setCanvasLayers() 会按图层树顺序调用 setLayers()：
    //   新加入的 WMS 图层位于树顶部（z序最高），覆盖装备/飞机矢量图层 → 图标不可见。
    //   因此 timer 中必须再次调用 setLayers(m_layers) 恢复正确顺序（WMS 在底部）。
    // • setDestinationCrs() 放在 setExtent() 之前调用：
    //   内部会将当前范围从旧CRS重投影到新CRS（可能扩展为全球），
    //   随后 setExtent(China) 立即覆盖，最终范围正确。
    QTimer::singleShot(50, this, [this, targetExtent]() {
        // 1. 设置 canvas CRS 为 lon-first WGS84，使 WMS provider 正确把 canvas 范围转换到 EPSG:3857。
        //    问题：setDestinationCrs() 内部用 authid 做相等判断。
        //    无效 TIF CRS（authid=""）和 createFromProj 得到的 CRS（无 authid=""）比较 ""=="" → no-op，CRS 从未改变。
        //    解法：绕过 setDestinationCrs()，直接操作底层 QgsMapSettings，强制写入新 CRS。
        QgsCoordinateReferenceSystem crsWgs84;
        if (crsWgs84.createFromProj("+proj=longlat +datum=WGS84 +no_defs") && crsWgs84.isValid()) {
            QgsMapSettings &ms = const_cast<QgsMapSettings &>(m_mapCanvas->mapSettings());
            ms.setDestinationCrs(crsWgs84);
            qDebug() << "[OnlineMap] CRS set via mapSettings, authid=" << crsWgs84.authid()
                     << "valid=" << crsWgs84.isValid();
        } else {
            qDebug() << "[OnlineMap] WARNING: crsWgs84 invalid, CRS not updated";
        }
        // 2. 恢复图层顺序：WMS 在 m_layers 末尾（最后 = 渲染底层/背景），矢量装备图层在前（前景可见）
        m_mapCanvas->setLayers(m_layers);
        qDebug() << "[OnlineMap] setLayers count=" << m_layers.count()
                 << "extentBefore=" << m_mapCanvas->extent().toString();
        // 3. 恢复中国范围（targetExtent 来自 TIF，单位为度）
        if (!targetExtent.isEmpty()) {
            m_mapCanvas->setExtent(targetExtent);
            qDebug() << "[OnlineMap] setExtent to" << targetExtent.toString()
                     << "extentAfter=" << m_mapCanvas->extent().toString();
        }
        // 4. 重建装备标签（mapPosition 在新CRS下重新锚定）
        ShowRadarTip();
        m_mapCanvas->clearCache();
        m_mapCanvas->refresh();
    });
}

// 打开地图管理对话框
void MainWindow::showMapManager()
{
    if (!m_pDlgMapManager) {
        m_pDlgMapManager = new DlgMapManager(
            (QgsMapCanvas*)m_mapCanvas, m_basemapPath,
            m_p2DMapView, m_pWebEngineView, this);
        connect(m_pDlgMapManager, &DlgMapManager::switchBaseMapRequested,
                this, &MainWindow::switchBaseMap);
        connect(m_pDlgMapManager, &DlgMapManager::loadOnlineMapRequested,
                this, &MainWindow::loadOnlineTileMap);
        connect(m_pDlgMapManager, &DlgMapManager::loadShpRequested,
                this, &MainWindow::addVectorlayers);
        connect(m_pDlgMapManager, &DlgMapManager::switchMapModeRequested,
                this, &MainWindow::switchMapViewMode);
    }
    m_pDlgMapManager->setCurrentBasemap(m_basemapPath);
    m_pDlgMapManager->show();
    m_pDlgMapManager->raise();
    m_pDlgMapManager->activateWindow();
}


//编辑风格
void MainWindow::editVectorSymbol()
{
    QgsVectorLayer *layer =dynamic_cast< QgsVectorLayer *>(m_mapCanvas->currentLayer());
    if ( !layer )
      return;

    QgsSingleSymbolRenderer *singleRenderer = dynamic_cast< QgsSingleSymbolRenderer * >( layer->renderer() );
    if ( !singleRenderer )
      return;

    std::unique_ptr< QgsSymbol > symbol( singleRenderer->symbol() ? singleRenderer->symbol()->clone() : nullptr );
    QgsSymbolSelectorDialog dlg( symbol.get(), QgsStyle::defaultStyle(), layer, nullptr );
    dlg.setWindowTitle(QString::fromLocal8Bit("图元风格" ) );

    QgsSymbolWidgetContext context;
    context.setMapCanvas((QgsMapCanvas*)m_mapCanvas);
    context.setMessageBar(m_pInfoBar);
    dlg.setContext(context);

    if (dlg.exec())
    {
      singleRenderer->setSymbol(symbol.release());
      layer->triggerRepaint();
      layer->emitStyleChanged();
      QgsProject::instance()->setDirty( true );
    }

    QSettings settings("HelloPlane", "suzs");
    QColor color = singleRenderer->symbol()->color();
    settings.setValue(layer->name() + "/color", color);
    settings.setValue(layer->name() + "/opacity", singleRenderer->symbol()->opacity());
    settings.sync();
}

void MainWindow::chanegeDialogLabel( QgsAttributeDialog* dialog, QString ltstring )
{
    dialog->setWindowTitle(QString::fromLocal8Bit("设置图元属性"));
    QList<QLabel *> widgets = dialog->findChildren<QLabel *>();
   for (int i=0; i< widgets.size(); i++)
   {     
       QString value =  widgets.at(i)->text();
       if ( value == "id")  widgets.at(i)->setText(QString::fromLocal8Bit("编号"));
       
       if ( value == "name") widgets.at(i)->setText(QString::fromLocal8Bit("名称"));
       
       if ( value == "class" ) widgets.at(i)->setText(QString::fromLocal8Bit("类别"));
       
       if ( value == "position")widgets.at(i)->setText(QString::fromLocal8Bit("位置"));
       
       if ( value == "abundance" )
       {
            widgets.at(i)->setText(QString::fromLocal8Bit("情报丰度"));
       }
       if ( value == "task") widgets.at(i)->setText(QString::fromLocal8Bit("对应任务"));
       
       if (value == "height")
       {
            widgets.at(i)->setText(QString::fromLocal8Bit("高度"));
            widgets.at(i)->setVisible(false);
       }
       if (value == "radius")
       {
            widgets.at(i)->setText(QString::fromLocal8Bit("半径"));
            widgets.at(i)->setVisible(false);
       }
       if (value == "begintime")
       {
           widgets.at(i)->setText(QString::fromLocal8Bit("开始"));
           widgets.at(i)->setVisible(false);
       }
       if (value == "endtime")
       {
           widgets.at(i)->setText(QString::fromLocal8Bit("结束"));
           widgets.at(i)->setVisible(false);
       }
   }

   QList<QgsFilterLineEdit *> widgets1 = dialog->findChildren<QgsFilterLineEdit *>();
   for ( int i=0; i< widgets1.size(); i++  )
   {
       if (widgets1[i]->objectName()== "position" )
       {
            if ( ltstring != "" )
            {
                widgets1[i]->setText( ltstring );
            }
       }

        if ( widgets1[i]->objectName() == "radius" || widgets1[i]->objectName() == "height" ||
             widgets1[i]->objectName() == "begintime" || widgets1[i]->objectName() == "endtime")
        {
             widgets1.at(i)->setVisible(false);
        }
   }
}

//设置标注图元,只包括面，圆，线
void MainWindow::modifyFeatures()
{
	QMessageBox msgBox; 	
	msgBox.setText("设置标注图元 > 49.");
	//msgBox.exec();

    QgsMapLayer *activeMapLayer = m_mapCanvas->currentLayer();
    if ( !activeMapLayer)
    {
		m_pInfoBar->pushMessage(tr( "No active layer" ), tr( "Please select a layer in the layer list" ),Qgis::Warning );
        return;
    }

    QgsVectorLayer *vl = qobject_cast<QgsVectorLayer *>( activeMapLayer );
    if ( !vl )
    {
		m_pInfoBar->pushMessage(tr( "Invalid layer" ),tr( "The merge features tool only works on vector layers." ),Qgis::Warning );
		return;
    }
    
	if ( vl == g_pTargetLayer)//如果是目标点图层，就直接返回
      return;
    
    if ( !vl->isEditable())
    {
		m_pInfoBar->pushMessage(tr( "Layer not editable" ),tr( "Modifying features can only be done for layers in editing mode." ),Qgis::Warning );
		return;
    }

    //dummy feature
    QgsFeature f;
    QgsAttributeEditorContext context(createAttributeEditorContext());
    context.setAllowCustomUi( false);
    context.setVectorLayerTools(mVectorLayerTools ); //面图层工具
    context.setCadDockWidget(mDock);
    context.setMapCanvas((QgsMapCanvas*)m_mapCanvas);

    QgsAttributeDialog *pDlg = new QgsAttributeDialog(vl, &f, false, this, true, context);
	pDlg->userid = "selectpoint";
	pDlg->setMode(QgsAttributeEditorContext::MultiEditMode );
	pDlg->setAttribute( Qt::WA_DeleteOnClose);
	pDlg->show();
    chanegeDialogLabel(pDlg); //修改对话框内控件标题,
}

//获取无人机个数
void MainWindow::getPlaneCount()
{	
	/*std::string url = "file:///D:/SRTM/huayin15/index.html";

	// 使用ShellExecute打开网页
	HINSTANCE result = ShellExecute(NULL, "open", "file:///D:/SRTM/huayin15/index.html", NULL, NULL, SW_SHOWNORMAL);

	Sleep(100);

	HWND m_wnd = 0;
	m_wnd=::FindWindow(NULL, "华阴");

	LONG style = ::GetWindowLong(m_wnd, GWL_STYLE);
	style &= ~WS_POPUP;
	style |= WS_CHILD;
	SetWindowLongW(m_wnd, GWL_STYLE, style);

	::SetWindowLong(m_wnd, GWL_STYLE, style & ~(WS_CAPTION));// | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	//::SetWindowPos(m_wnd, NULL, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE | SWP_FRAMECHANGED);

	::SetParent(m_wnd, (HWND)m_pWebEngineView->winId());// 设置新的父窗口句柄	

	m_mapCanvas->hide();
	m_pWebEngineView->show();		//m_pWebEngineView->winId()
	QRect rc = m_mapCanvas->rect();
	m_pWebEngineView->setGeometry(0, 0, rc.width(), rc.height());*/

	//Dlg3Dtest *pDlg = new Dlg3Dtest(this);
	//m_pDBBackDlg->setWindowTitle(QString::fromLocal8Bit("无人机数据回放"));
	//pDlg->show();
	//return;
	QThread *current=QThread::currentThread();
	current->setPriority(QThread::HighPriority);
    // 20.438164086,-34.051642567 20.43817381,-34.05161061 20.438173445,-34.051609030
    //m_planeVector.clear();
    //m_planeVector.clear();//清理飞机图层内的飞机
//	g_pPlaneLayer->removeItem("1");
	//g_pPlaneLayer->deleteFeatures(g_pPlaneLayer->allFeatureIds());
	if (m_pDlgAddTaskArea == nullptr)
	{
		m_pDlgAddTaskArea = new DlgAddTaskArea(this);
	}

	if (m_pDlgAirList == nullptr)
	{
		m_pDlgAirList = new Dialog_AirList(this);
	}
	else
	{
		while (m_pDlgAirList->ui.mAirTable->rowCount() > 0)
		{
			m_pDlgAirList->ui.mAirTable->removeRow(0);
		}		
	}
    //QMessageBox::information(this,tr("成功"),tr("获取无人机个数"));
    char buf[128];
    sprintf(buf,"收到飞机数量: %d", m_planeVector.count());
    //QString str=QString("收到飞机数量: %1").arg( planeVector.count()) ;
    m_PlaneNumEditer->setText(QString::fromLocal8Bit(buf));	
	QString mPath = QCoreApplication::applicationDirPath();
	//创建一个接收mavlink协议，无人机数据线程
	if (m_pRecThread == nullptr)
	{

		g_pAirLayer->startEditing();
		//删除无人机图层内所有无人机图元
		QgsFeature feat;
		QgsFeatureIterator fit = g_pAirLayer->getFeatures();
		while (fit.nextFeature(feat))
			g_pAirLayer->deleteFeature(feat.id());

		//删除无人机的轨迹线图层内所有点图元
		g_pPointLayer->startEditing();		
		fit = g_pPointLayer->getFeatures();
		while (fit.nextFeature(feat))
			g_pPointLayer->deleteFeature(feat.id());

		g_pRadarPtLayer->startEditing();
		fit = g_pRadarPtLayer->getFeatures();
		while (fit.nextFeature(feat))
			g_pRadarPtLayer->deleteFeature(feat.id());

		m_pRecThread = new RecMavlinkThread();
		m_pRecThread->m_pDlg = this;
		m_pRecThread->start();

		//创建一个mavlink协议数据转发线程
		TransitThread	*pTransitThread = new TransitThread();
		connect(pTransitThread, &TransitThread::receviePlaneMsg, this, &MainWindow::registerPlane);		
		pTransitThread->start();
		
		ZeroMemory(&m_ShExecInfo, sizeof(SHELLEXECUTEINFO));
		m_ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
		m_ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
		m_ShExecInfo.lpVerb =("open");

		//输入要调用的exe文件路径  
		m_ShExecInfo.lpFile = "QtGui.exe";// , s.toStdString().c_str());
		strcpy(buf, mPath.toStdString().c_str());
		m_ShExecInfo.lpDirectory = buf;
		m_ShExecInfo.lpParameters	="1";	//若没有命令行参数，可为NULL 
		m_ShExecInfo.nShow = SW_HIDE;		// SW_SHOW;
		//ShellExecuteEx(&m_ShExecInfo);	//::ShellExecute(m_hWnd, "open", "python3.exe rtsp3.py", NULL, NULL, SW_SHOW);
	}
	else
		QMessageBox::information(this, "SUCCESS", QString::fromLocal8Bit("已经启动接收数据了............."));
		//setWindowTitle(QString::fromLocal8Bit("已经启动接收数据了............."));
}

//注册无人机
// 节流时间戳（供 processAllPlaneUpdates 使用，静态变量跨调用持久）
static DWORD s_lastModifyTableTick = 0;  // ModifyTable 节流（200ms）
static DWORD s_lastIsRadarPolyTick = 0;  // isRadarInPoly 节流（500ms）

void MainWindow::registerPlane(tag_PlaneMessage *p)
{
	// 已知飞机：O(1) 写入缓冲，立即返回，不做任何耗时操作。
	// 所有 QGIS 图层写入、UI 更新均移至 processAllPlaneUpdates()（100ms 定时器调用），
	// 主线程事件队列得以快速清空，鼠标/缩放等 UI 事件不再被高频消息阻塞。
	if (m_latestPlaneData.contains(p->ID))
	{
		if (p->planeX.isEmpty())
		{
			// msg199：仅含群控模式/同步时间字段，不含坐标 — 只更新模式字段，保留位置数据
			auto &existing = m_latestPlaneData[p->ID];
			existing.qkCmdMode  = p->qkCmdMode;
			existing.qkRunMode  = p->qkRunMode;
			existing.fkRunMode  = p->fkRunMode;
			existing.fkSysStatus= p->fkSysStatus;
			existing.jqtbTime   = p->jqtbTime;
		}
		else
		{
			// msg195：含完整位置/传感器数据，全量更新；保留 msg199 写入的字段，避免覆盖
			auto &ex = m_latestPlaneData[p->ID];
			p->qkCmdMode   = ex.qkCmdMode;
			p->qkRunMode   = ex.qkRunMode;
			p->fkRunMode   = ex.fkRunMode;
			p->fkSysStatus = ex.fkSysStatus;
			p->jqtbTime    = ex.jqtbTime;
			ex = *p;
		}
		delete p;
		return;
	}

	// 首次收到此飞机消息：msg199 无位置数据，等待 msg195 再注册
	if (p->planeX.isEmpty())
	{
		delete p;
		return;
	}

	// 首次收到此飞机消息：写入缓冲并加入待注册队列，
	// 实际 QGIS 写入由 processAllPlaneUpdates() 批量完成（一次 commitChanges）
	m_latestPlaneData[p->ID] = *p;
	m_pendingNewPlanes.append(*p);
	delete p;
}

// 批量处理缓冲的飞机位置数据，由 onAirLayerRefreshTimer() 每100ms调用一次。
// canvas 空闲时才执行，彻底避免渲染期间写锁竞争。
void MainWindow::processAllPlaneUpdates()
{
	if (m_latestPlaneData.isEmpty())
		return;
	bool isDrawingNow = m_mapCanvas && m_mapCanvas->isDrawing();

	DWORD now = GetTickCount();
	bool doTable = !isDrawingNow && (now - s_lastModifyTableTick >= 200);
	bool doPoly  = !isDrawingNow && (now - s_lastIsRadarPolyTick  >= 500);
	if (doTable) s_lastModifyTableTick = now;
	if (doPoly)  s_lastIsRadarPolyTick  = now;

	// 批量注册新飞机：一次性写入 QGIS，替代每架飞机独立 commitChanges（80架从40s降到<1s）
	if (!isDrawingNow && !m_pendingNewPlanes.isEmpty() && g_pAirLayer)
	{
		g_pAirLayer->startEditing();
		for (const auto &plane : m_pendingNewPlanes)
		{
			QgsGeometry geometry = QgsGeometry::fromPointXY(
				QgsPointXY(plane.planeX.toDouble(), plane.planeY.toDouble()));
			if (!geometry.isGeosValid())
				continue;
			QgsFeature ftpt;
			ftpt.setGeometry(geometry);
			ftpt.setAttributes(QgsAttributes()
				<< plane.ID.toInt() << plane.planeY << plane.planeX
				<< QString::number(0) << plane.Yaw);
			g_pAirLayer->addFeature(ftpt);

			// 为每架新飞机创建位置更新线程和标牌
			FixPlaneThread *pPlaneThread = new FixPlaneThread();
			connect(this, &MainWindow::FixPlaneMsg, pPlaneThread, &FixPlaneThread::FixPlane);
			pPlaneThread->m_id = plane.ID.toInt();
			pPlaneThread->start();
			m_planeThreadVec.append(pPlaneThread);

			m_planeVector.append(plane);

			biaopai *pPai = new biaopai((QgsMapCanvas*)m_mapCanvas);
			m_planeIDvec.append(pPai);
			pPai->setString(plane.ID);
			pPai->setPos(QgsPointXY(plane.planeX.toDouble(), plane.planeY.toDouble()));
			// Leaflet 激活时隐藏，避免与 Leaflet marker 双重显示并产生渲染速度差异
			if (m_leafletReady) pPai->setVisible(false);

			tag_PlaneMessage *pp = const_cast<tag_PlaneMessage*>(&plane);
			insertToTable(pp);
			if (m_pDlgTuili)
			{
				m_pDlgTuili->insertToTable(pp);
				if (m_pDlgTuili->m_pTuiliThread)
				{
					m_pDlgTuili->m_pTuiliThread->m_planeVec.append(plane);
					m_pDlgTuili->m_pTuiliThread->m_iPlaneCount = m_planeVector.count();
				}
			}
		}
		g_pAirLayer->commitChanges();	// 一次提交，无论多少架飞机
		g_pAirLayer->triggerRepaint();
		m_PlaneNumEditer->setText(
			QString(QString::fromLocal8Bit("收到飞机数量: %1")).arg(m_planeVector.count()));
		m_pendingNewPlanes.clear();
	}

	// 批量 JS 字符串：整个循环只产生 1 次 IPC 调用（替代每架飞机独立调用）
	bool has2D = m_leafletReady && m_p2DMapView;
	bool has3D = m_3dReady && m_pWebEngineView;
	QString batch2D, batch3D;

	if (doTable)
		ui->mPlaneWidget->setUpdatesEnabled(false);

	for (auto it = m_latestPlaneData.begin(); it != m_latestPlaneData.end(); ++it)
	{
		tag_PlaneMessage *p = &it.value();

		// 修改无人机图层位置（QGIS 写入，canvas 绘制时跳过）
		if (!isDrawingNow) emit FixPlaneMsg(p);

		// 同步飞机位置到 Leaflet 2D 地图（累积到批量字符串）
		if (has2D)
		{
			double pAlt = p->hZ.toDouble();
			if (pAlt <= 0) pAlt = p->xZ.toDouble();
			batch2D += QString("updatePlane('%1',%2,%3,'\\u65e0\\u4eba\\u673a#%1',%4,%5);")
				.arg(p->ID)
				.arg(p->planeY.toDouble(), 0, 'f', 7)
				.arg(p->planeX.toDouble(), 0, 'f', 7)
				.arg(p->Yaw.toDouble(),    0, 'f', 1)
				.arg(pAlt, 0, 'f', 0);
		}

		// 同步飞机位置到 3D 视图（累积到批量字符串）
		if (has3D)
		{
			double alt = p->hZ.toDouble();
			if (alt <= 0) alt = p->xZ.toDouble();
			batch3D += QString("if(typeof updatePlane3D==='function')updatePlane3D('%1',%2,%3,%4,%5);")
				.arg(p->ID)
				.arg(p->planeX.toDouble(), 0, 'f', 7)
				.arg(p->planeY.toDouble(), 0, 'f', 7)
				.arg(alt, 0, 'f', 1)
				.arg(p->Yaw.toDouble(), 0, 'f', 1);
		}

		// 始终更新 QGIS canvas 标牌位置（biaopai 在 Leaflet 激活时已 setVisible(false)，
		// 更新内部坐标不产生视觉冲突，且保证切回 QGIS 2D 视图时立即显示正确位置）
		{
			int pid = p->ID.toInt();
			for (int i = 0; i < m_planeIDvec.count(); i++)
			{
				if (m_planeIDvec[i]->m_id == pid)
				{
					m_planeIDvec[i]->setPos(QgsPointXY(p->planeX.toDouble(), p->planeY.toDouble()));
					m_planeIDvec[i]->setYaw(p->Yaw.toDouble());
					break;
				}
			}
		}

		// 同步装载于该无人机的探测设备位置
		if (!m_radarUavMount.isEmpty())
		{
			QgsPointXY uavPt(p->planeX.toDouble(), p->planeY.toDouble());
			double uavAlt = p->hZ.toDouble();
			if (uavAlt <= 0) uavAlt = p->xZ.toDouble();

			for (auto mit = m_radarUavMount.begin(); mit != m_radarUavMount.end(); ++mit)
			{
				if (mit.value() != p->ID)
					continue;

				int radarId = mit.key();

				// RadarTip 标签：QgsMapCanvasAnnotationItem 路径，无需 canvas 空闲
				for (RadarTip *r : m_radarTipList)
				{
					if (r->m_id == radarId) { r->setPos(uavPt); break; }
				}

				// QGIS 图层几何更新（canvas 绘制时跳过，避免写锁竞争）
				if (!isDrawingNow)
				{
					for (int i = 0; i < gRadarLayerList.size(); i++)
					{
						QgsFeatureIterator fit = gRadarLayerList[i]->getFeatures(
							QString("\"electircid\"=%1").arg(radarId));
						QgsFeature f;
						if (!fit.nextFeature(f))
							continue;

						if (!gRadarLayerList[i]->isEditable())
							gRadarLayerList[i]->startEditing();
						QgsGeometry newGeom = QgsGeometry::fromPointXY(uavPt);
						gRadarLayerList[i]->changeGeometry(f.id(), newGeom);
						if (uavAlt > 0)
							gRadarLayerList[i]->changeAttributeValue(f.id(), 4, (int)uavAlt);
						if (doPoly)
							updateMountedRadarProjection(radarId, f, uavPt);
						gRadarLayerList[i]->triggerRepaint();  // 刷新 SVG 图标位置
						break;  // 同一设备只在一个图层中存在
					}
				}

				// Leaflet + 3D（无论 canvas 是否在绘制，始终推送）
				if (has2D)
					batch2D += QString("if(typeof moveRadar==='function')moveRadar(%1,%2,%3);")
						.arg(radarId).arg(uavPt.y(), 0, 'f', 7).arg(uavPt.x(), 0, 'f', 7);
				if (has3D)
					batch3D += QString("if(typeof moveRadar3D==='function')moveRadar3D(%1,%2,%3,%4);")
						.arg(radarId).arg(uavPt.x(), 0, 'f', 7).arg(uavPt.y(), 0, 'f', 7).arg(uavAlt, 0, 'f', 1);
			}
		}

		// 表格更新（200ms 节流）
		if (doTable)
		{
			if (m_pDlgTuili) m_pDlgTuili->ModifyTable(p);
			if (!ui->mPlaneWidget->isHidden())
				ModifyTable(p);
		}

		// 雷达多边形包含检查（500ms 节流）
		if (doPoly)
		{
			g_planeMutex.lock();
			QList<int> list = isRadarInPoly(*p);
			if (list.size() > 0)
			{
				foreach(int r, list) {
					g_RadarTrackList.append(QgsPointXY(p->planeX.toDouble(), p->planeY.toDouble()));
					if (m_pDlgAirList)
						m_pDlgAirList->insert(p->ID, QString("%1").arg(r));
				}
				// 被探测到：Leaflet 航线变红
				if (has2D)
					batch2D += QString("if(typeof setPlaneDetected==='function')setPlaneDetected('%1');").arg(p->ID);
			}
			else
			{
				g_PlaneList.append(QgsPointXY(p->planeX.toDouble(), p->planeY.toDouble()));
				// 未被探测：恢复橙色航线
				if (has2D)
					batch2D += QString("if(typeof setPlaneNormal==='function')setPlaneNormal('%1');").arg(p->ID);
			}
			g_planeMutex.unlock();
		}
	}

	if (doTable)
		ui->mPlaneWidget->setUpdatesEnabled(true);

	// 批量执行：整批飞机只产生 1 次 IPC 调用（从 80 次降为 1 次）
	if (has2D && !batch2D.isEmpty())
		m_p2DMapView->page()->runJavaScript(batch2D);
	if (has3D && !batch3D.isEmpty())
		m_pWebEngineView->page()->runJavaScript(batch3D);
}

//初始化接收电磁信息socket
void MainWindow::udpRecieveEleticMagnetismInit()
{    
    if (m_pUdpElectric== nullptr)
    {
		m_pUdpElectric = new QUdpSocket(this);
		m_pUdpElectric->bind(QHostAddress(localIP), localEletricPort.toInt());
        connect(m_pUdpElectric,SIGNAL(readyRead()),this,SLOT(processEleticMagnetismDatagram()));
    }
}

void MainWindow::processEleticMagnetismDatagram()
{
    QString		rarray;
    QByteArray	array;

    while(m_pUdpElectric->hasPendingDatagrams() )
    {
        QByteArray array;
        array.resize(m_pUdpElectric->pendingDatagramSize() );
		m_pUdpElectric->readDatagram(array.data(), array.size());

        char* ch = array.data();
        if ( *ch == char(47) )
        {
            // "messageid"  "deviceid"  "y"  "x"  "z"
            //"angleX"  "angleY"  "objectid"  "objvaild"  "obj_x"  "obj_y"  "obj_z"
            uint32_t timeid = *( (uint32_t*)(ch+1) );
            char deviceid = *( (char*)(ch+5) );
            int32_t y = *( (int32_t*)(ch+6) );
            int32_t x = *( (int32_t*)(ch+10) );
            int32_t z = *( (int32_t*)(ch+14) );
            float angleX =  *( (float*)(ch+18) );
            float angleY =  *( (float*)(ch+22) );
            uint16_t objectid = *( (uint16_t*)(ch+26) );
            uint8_t objvaild = *( (uint8_t*)(ch+28) );
            int32_t obj_y = *( (int32_t*)(ch+29) );
            int32_t obj_x = *( (int32_t*)(ch+33) );
            int32_t obj_z = *( (int32_t*)(ch+37) );

            if ( objvaild != uint8_t(3) || (deviceid != char(0)) )
            {
                return;
            }

			g_pTargetLayer->startEditing();
            QgsFeature featuretarget =  QgsFeature();

            double pos_x =  static_cast<int>(x);      pos_x =0.0000001*pos_x;
            double pos_y = static_cast<int>(y);       pos_y =0.0000001*pos_y;

            QgsGeometry geometry = QgsGeometry::fromPointXY( QgsPointXY(pos_x,pos_y) );
         //   qDebug()<<pointTarget.asPoint().x()<<","<<pointTarget.asPoint().y()<<"/n";

            featuretarget.setGeometry(geometry);
            if ( !geometry.isGeosValid()) {
                QMessageBox::information(this,"提醒","无效坐标修改！");
            }

            featuretarget.setAttributes( QgsAttributes()<< (int)g_pTargetLayer->featureCount()+1 <<"" <<"" << "" <<""<<""
                                         << QString::number(timeid)
                                                      << QString(deviceid)
                                                      << QString::number(y)
                                                      << QString::number(x)
                                                      << QString::number(z)
                                                      << QString::number(angleX,'f',6)
                                                      << QString::number(angleY,'f',6)
                                                      << QString(objectid)
                                                      << QString(objvaild)
                                                      << QString::number(obj_y)
                                                      <<""
                                                      << QString::number(obj_z)
                                                      << QString::number(obj_x)
                                                      <<"" );
			g_pTargetLayer->addFeature( featuretarget);
			g_pTargetLayer->triggerRepaint();
            m_mapCanvas->setCenter(QgsPointXY(pos_x,pos_y));
      }
    }
}
//全局规划,时候，构造数据包
void MainWindow::pythonInterface(QByteArray& willSendData)
{
    uint16_t counter =  stlTargetPoint.size() ;
    willSendData.append(char(1));//pack head
    willSendData.append(char(0));//plane
    uint16_t size = m_planeVector.size();
    willSendData.append((char*)(&size),sizeof(uint16_t));//talal plane count
    uint16_t index = 0;

    for ( int i = 0; i <m_planeVector.size(); i++)
    {
          willSendData.append( (char*)(&index),sizeof(uint16_t));
          uint16_t id = m_planeVector[i].ID.toUShort();
          willSendData.append( (char*)(&id),sizeof(uint16_t));
          double x = m_planeVector[i].planeX.toDouble();
          willSendData.append( (char*)(&x),sizeof(double) );
          double y = m_planeVector[i].planeY.toDouble();
          willSendData.append( (char*)(&y),sizeof(double));

          double z = double(0);
          willSendData.append( (char*)(&z),sizeof(double)  );
          index ++ ;
    }

    willSendData.append(char(1));//object
    willSendData.append((char*)(&counter),sizeof(uint16_t) );//talal object count
    index = 0;

    for(auto iter = stlTargetPoint.begin();iter != stlTargetPoint.end();++iter)
    {
       // iter->plan_x = m_mapCanvas.getCoordinateTransform()->transform(iter->track_point).x();
        //iter->plan_y = m_mapCanvas.getCoordinateTransform()->transform(iter->track_point).y();
        iter->targetNum = index;
        willSendData.append( (char*)(&index),sizeof(uint16_t) );
        double x = double(iter->plan_x);
        willSendData.append( (char*)(&x),sizeof(double) );
        double y = double(iter->plan_y);
        willSendData.append( (char*)(&y),sizeof(double) );
        double z = double(0);
        willSendData.append( (char*)(&z),sizeof(double)  );
        index++;
    }
}

int MainWindow::getContainedGridPoint(QgsFeature feature)
{
	QgsMapLayer* layer = m_mapCanvas->currentLayer();
	m_mapCanvas->setCurrentLayer(g_pGirdLayer);
	QgsFeatureIds featureIds = QgsMapToolSelectUtils::getMatchingFeatures((QgsMapCanvas*)m_mapCanvas, feature.geometry(), true, false);
	QgsFeatureIterator  i = g_pGirdLayer->getFeatures(featureIds);
	QgsFeature featuregrid;
	while (i.nextFeature(featuregrid))
	{
		QgsGeometry geometry = featuregrid.geometry();
		TargetPoint targetpoint;
		targetpoint.plan_x = geometry.asPoint().x();
		targetpoint.plan_y = geometry.asPoint().y();
		targetpoint.type = QGS_POLY;
		stlTargetPoint.push_back(targetpoint);
		//break;
	}
	m_mapCanvas->setCurrentLayer(layer);
	return 0;
}
//全局规划
void MainWindow::ScanPathPlan()
{
    stlTargetPoint.clear();

    selectPointToTargetVector(g_pTargetLayer,(int)QGS_POINT);
    selectPointToTargetVector(g_pPolygonLayer,(int)QGS_POLY);

    QByteArray msg;
    pythonInterface( msg);//全局规划,时候，构造数据包

	//如果没有创建过此socket就创建它
    if (m_pUdpAlogrithm==  nullptr)
    {
		m_pUdpAlogrithm = new QUdpSocket( this );
        connect(m_pUdpAlogrithm,SIGNAL(readyRead()),this,SLOT(read_data()));
		m_pUdpAlogrithm->bind( QHostAddress(localIP), localAlorithmPort.toInt());
    }
	m_pUdpAlogrithm->writeDatagram(msg, QHostAddress(algorithmsIp), algorithmsPort.toInt());
    //QString str = QString("发送目标数量: %1").arg(stlTargetPoint.size());
    char buf[128];
    sprintf(buf,"发送目标数量: %d", int(stlTargetPoint.size()));

    QMessageBox::information(this, QString::fromLocal8Bit("发送成功"), QString::fromLocal8Bit(buf));
}

void MainWindow::resolveVectorLayerDependencies(QgsVectorLayer *vl, QgsMapLayer::StyleCategories categories)
{
  if (vl && vl->isValid())
  {
    const auto constDependencies { findBrokenLayerDependencies( vl, categories ) };
    for ( const QgsVectorLayerRef &dependency : constDependencies )
    {
      // Check for projects without layer dependencies (see 7e8c7b3d0e094737336ff4834ea2af625d2921bf)
      if ( QgsProject::instance()->mapLayer( dependency.layerId ) || ( dependency.name.isEmpty() && dependency.source.isEmpty() ) )
      {
        continue;
      }
      // try to aggressively resolve the broken dependencies
      bool loaded = false;
      const QString providerName { vl->dataProvider()->name() };
      QgsProviderMetadata *providerMetadata { QgsProviderRegistry::instance()->providerMetadata( providerName ) };
      if ( providerMetadata )
      {
        // Retrieve the DB connection (if any)
        std::unique_ptr< QgsAbstractDatabaseProviderConnection > conn { static_cast<QgsAbstractDatabaseProviderConnection *>( providerMetadata->createConnection( vl->dataProvider()->uri().uri(), {} ) ) };
        if ( conn )
        {
          QString tableSchema;
          QString tableName;
          const QVariantMap sourceParts = providerMetadata->decodeUri( dependency.source );

          // This part should really be abstracted out to the connection classes or to the providers directly.
          // Different providers decode the uri differently, for example we don't get the table name out of OGR
          // but the layerName/layerId instead, so let's try different approaches

          // This works for GPKG
          tableName = sourceParts.value( QStringLiteral( "layerName" ) ).toString();

          // This works for PG and spatialite
          if ( tableName.isEmpty() )
          {
            tableName = sourceParts.value( QStringLiteral( "table" ) ).toString();
            tableSchema = sourceParts.value( QStringLiteral( "schema" ) ).toString();
          }

          // Helper to find layers in connections
          auto layerFinder = [ &conn, &dependency, &providerName ]( const QString & tableSchema, const QString & tableName ) -> bool
          {
            // First try the current schema (or no schema if it's not supported from the provider)
            try
            {
              const QString layerUri { conn->tableUri( tableSchema, tableName )};
              // Load it!
              std::unique_ptr< QgsVectorLayer > newVl = qgis::make_unique< QgsVectorLayer >( layerUri, dependency.name, providerName );
              if ( newVl->isValid() )
              {
                QgsProject::instance()->addMapLayer( newVl.release() );
                return true;
              }
            }
            catch ( QgsProviderConnectionException & )
            {
              // Do nothing!
            }
            return false;
          };

          loaded = layerFinder( tableSchema, tableName );

          // Try different schemas
          if ( ! loaded && conn->capabilities().testFlag( QgsAbstractDatabaseProviderConnection::Capability::Schemas ) && ! tableSchema.isEmpty() )
          {
            const QStringList schemas { conn->schemas() };
            for ( const QString &schemaName : schemas )
            {
              if ( schemaName != tableSchema )
              {
                loaded = layerFinder( schemaName, tableName );
              }
              if ( loaded )
              {
                break;
              }
            }
          }
        }
      }
      if ( ! loaded )
      {
        const QString msg { tr( "layer '%1' requires layer '%2' to be loaded but '%2' could not be found, please load it manually if possible." ).arg( vl->name(), dependency.name ) };
		m_pInfoBar->pushWarning( tr( "Missing layer form dependency" ), msg );
      }
      else
      {
		  m_pInfoBar->pushSuccess( tr( "Missing layer form dependency" ), tr( "Layer dependency '%2' required by '%1' was automatically loaded." )
                                   .arg( vl->name() )
                                   .arg( dependency.name ) );
      }
    }
  }
}

const QList<QgsVectorLayerRef> MainWindow::findBrokenLayerDependencies(QgsVectorLayer *vl, QgsMapLayer::StyleCategories categories ) const
{
  QList<QgsVectorLayerRef> brokenDependencies;

  /*if ( categories.testFlag( QgsMapLayer::StyleCategory::Forms ) )
  {
    for ( int i = 0; i < vl->fields().count(); i++ )
    {
      const QgsEditorWidgetSetup setup = QgsGui::editorWidgetRegistry()->findBest( vl, vl->fields().field( i ).name() );
      QgsFieldFormatter *fieldFormatter = QgsApplication::fieldFormatterRegistry()->fieldFormatter( setup.type() );
      if ( fieldFormatter )
      {
        const QList<QgsVectorLayerRef> constDependencies { fieldFormatter->layerDependencies( setup.config() ) };
        for ( const QgsVectorLayerRef &dependency : constDependencies )
        {
          // I guess we need and isNull()/isValid() method for the ref
          if ( dependency.layer ||
               ! dependency.name.isEmpty() ||
               ! dependency.source.isEmpty() ||
               ! dependency.layerId.isEmpty() )
          {
            const QgsVectorLayer *depVl { QgsVectorLayerRef( dependency ).resolveWeakly( QgsProject::instance(), QgsVectorLayerRef::MatchType::Name ) };
            if ( ! depVl || ! depVl->isValid() )
            {
              brokenDependencies.append( dependency );
            }
          }
        }
      }
    }
  }

  if ( categories.testFlag( QgsMapLayer::StyleCategory::Relations ) )
  {
    // Check for layer weak relations
    const QList<QgsWeakRelation> constWeakRelations { vl->weakRelations() };
    for ( const QgsWeakRelation &rel : constWeakRelations )
    {
      QgsRelation relation
      {
          rel.resolvedRelation( QgsProject::instance(), QgsVectorLayerRef::MatchType::Name )
      };

      QgsVectorLayerRef dependency;
      bool found = false;
      if ( ! relation.isValid() )
      {
        // This is the big question: do we really
        // want to automatically load the referencing layer(s) too?
        // This could potentially lead to a cascaded load of a
        // long list of layers.
        // The code is in place but let's leave it disabled for now.
        if ( relation.referencedLayer() == vl )
        {
          // Do nothing because vl is the referenced layer
#if 0
          dependency = rel.referencingLayer();
          found = true;
#endif
        }
        else if ( relation.referencingLayer() == vl )
        {
          dependency = rel.referencedLayer();
          found = true;
        }

        if ( found )
        {
          // Make sure we don't add it twice if it was already added by the form widgets check
          bool refFound = false;
          for ( const QgsVectorLayerRef &otherRef : qgis::as_const( brokenDependencies ) )
          {
            if ( dependency.layerId == otherRef.layerId || ( dependency.source == otherRef.source && dependency.provider == otherRef.provider ) )
            {
              refFound = true;
              break;
            }
          }
          if ( ! refFound )
          {
            brokenDependencies.append( dependency );
          }
        }
      }
    }
  }*/
  return brokenDependencies;
}

//设置工具栏图标
void MainWindow::setActionSvg()
{
	QString mPath = QCoreApplication::applicationDirPath();
    ui->mActionMoveView->setIcon(QIcon( ":/images/themes/default/mActionPan.svg" ) );
    ui->mActionDrawPoint->setIcon(QIcon( ":/images/themes/default/mIconPointLayer.svg" ));
    ui->mActionDrawLine->setIcon(QIcon(":/images/themes/default/mIconLineLayer.svg" ));
    ui->mActionDrawPolygon->setIcon(QIcon(":/images/themes/default/mIconPolygonLayer.svg" ));
    ui->mActionCircle2point->setIcon(QIcon(":/images/themes/default/mActionCircle2Points.svg" ));
    ui->mActionSaveLayer->setIcon(  QIcon(":/images/themes/default/mActionFileSave.svg"));
	ui->mActionMeaureLength1->setIcon(QIcon(":/images/themes/default/mActionMeasure.svg"));
	ui->mActionMeasureArea1->setIcon(QIcon(":/images/themes/default/mActionMeasureArea.svg"));
	ui->mActionDTback->setIcon(QIcon(":/images/themes/default/mActionPlay.svg"));
	ui->mAction3D->setIcon(QIcon(":/images/themes/default/3d.svg"));
	ui->actionRadarGo->setIcon(QIcon(mPath +"/images/qgis.svg"));

	// 设置菜单
	ui->mActionLoadShp->setIcon(QIcon(":/images/themes/default/mActionAddOgrLayer.svg"));
	ui->mActionSetLayers->setIcon(QIcon(":/images/themes/default/mActionMapSettings.svg"));
	ui->mActionStartStop->setIcon(QIcon(":/images/themes/default/mActionStart.svg"));
	ui->mActionRefreshGJ->setIcon(QIcon(":/images/themes/default/mActionRefresh.svg"));

	// 装备库工具栏
	ui->mActionEquipment->setIcon(QIcon(":/images/themes/default/dbmanager.svg"));

	// 其他菜单 / 调度工具栏
	ui->mActionAirCount->setIcon(QIcon(":/images/themes/default/mActionIdentify.svg"));
	ui->mActionTaskSelect->setIcon(QIcon(":/images/themes/default/mActionSum.svg"));
	ui->mActionTuili->setIcon(QIcon(":/images/themes/default/mActionProcessSelected.svg"));
	ui->mActionLook->setIcon(QIcon(":/images/themes/default/mActionExport.svg"));
	ui->mActionMiddle->setIcon(QIcon(":/images/themes/default/mActionPanTo.svg"));

	// 装备工具栏 (toolBar_3)
	ui->mActionSelectRadarLayer->setIcon(QIcon(":/images/themes/default/mIconVector.svg"));
	ui->mActionMoveRadarDev->setIcon(QIcon(":/images/themes/default/mActionMoveFeature.svg"));
	ui->mActionDelRadarDev->setIcon(QIcon(":/images/themes/default/mActionDeleteSelected.svg"));
	ui->mActionFixRadar->setIcon(QIcon(":/images/themes/default/mActionOptions.svg"));
	ui->mActionOutfitMove->setIcon(QIcon(":/images/themes/default/mActionMoveFeatureCopy.svg"));
	ui->mActionRadarTestAirList->setIcon(QIcon(":/images/themes/default/mActionOpenTable.svg"));

	// 任务工具栏 (toolBar_4)
	ui->mActionSelTaskLayer->setIcon(QIcon(":/images/themes/default/mIconPolygonLayer.svg"));
	ui->mActionSelTaskArea->setIcon(QIcon(":/images/themes/default/mActionSelectRectangle.svg"));
	ui->mActionFixTaskArea->setIcon(QIcon(":/images/themes/default/mActionToggleEditing.svg"));
	ui->mActionMoveTaskArea->setIcon(QIcon(":/images/themes/default/mActionMoveFeaturePoint.svg"));
	ui->mActionDelTaskArea->setIcon(QIcon(":/images/themes/default/mActionDeleteSelectedFeatures.svg"));
	ui->mActionRadarNo->setIcon(QIcon(":/images/themes/default/mTaskComplete.svg"));
	ui->mActionRadarOverList->setIcon(QIcon(":/images/themes/default/legend.svg"));
	ui->mActionTaskGuihua->setIcon(QIcon(":/images/themes/default/mActionNewMap.svg"));

    // toolBar_2 动态按钮（轨迹 + 地图管理）
    if (!m_pActTrack) {
        m_pActTrack = new QAction(
            QIcon(":/images/themes/default/mActionToggleEditing.svg"),
            QString::fromLocal8Bit("轨迹记录"), this);
        m_pActTrack->setCheckable(true);
        connect(m_pActTrack, &QAction::triggered, this, &MainWindow::toggleTrack);
        ui->toolBar_2->addAction(m_pActTrack);
    }
    if (!m_pActClearTrack) {
        m_pActClearTrack = new QAction(
            QIcon(":/images/themes/default/mActionDeleteSelected.svg"),
            QString::fromLocal8Bit("清除轨迹"), this);
        connect(m_pActClearTrack, &QAction::triggered, this, &MainWindow::clearTrajectory);
        ui->toolBar_2->addAction(m_pActClearTrack);
    }
    if (!m_pActMapManager) {
        m_pActMapManager = new QAction(
            QIcon(":/images/themes/default/mActionMapSettings.svg"),
            QString::fromLocal8Bit("地图管理"), this);
        connect(m_pActMapManager, &QAction::triggered, this, &MainWindow::showMapManager);
        ui->toolBar_2->addAction(m_pActMapManager);
    }

    // 恢复工具栏文字：图标+文字显示
    ui->toolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->toolBar_2->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->toolBar_3->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->toolBar_4->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->mToolBarSchedule->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

    qDebug()<<"setActionSvg()  ----------------------";
}

//发送情报丰度
QString MainWindow::sendBundiance()
{
    int option = 0;
    DialogSendGrid dlg(&option);
    if (dlg.exec() == QDialog::Rejected ){
        return "";
    }

    QString s("");
    QgsFeatureIterator featureIt = g_pGirdLayer->getFeatures();
    QgsFeature feature;
    QByteArray msg;
    QByteArray msgtotal;

    int     featureCounter = g_pGirdLayer->featureCount() - 1;
    int     counter = 0;
    double  proportion  = 111319.5;//米和纬度转换
    double  left,right,top,bottom = 0.0;
    char    totalrow = 0;
    char    totalcol = 0;
    double  width  = 0;
    double  initvalue = 0;

    QString str = QString("number=%1").arg(featureCounter);
    //QMessageBox::information(this, QString::fromLocal8Bit("提示"), str);

    while (featureIt.nextFeature(feature))
    {
        char row =  (char)feature.attribute("row").toUInt();
        char col =  (char)feature.attribute("col").toUInt();
        msg.append( row );
        msg.append( col );

        char* pchar = nullptr;
        double centery = feature.attribute("centery").toDouble();
        pchar = ( char* )&centery;
        msg.append( pchar, sizeof (double));

        if ( ( 0 == row ) && (0==col))
        {
           initvalue = centery;
        }

        double centerx = feature.attribute("centerx").toDouble();
        pchar = ( char* )&centerx;
        msg.append( pchar, sizeof (double));

        width           = feature.attribute("width").toDouble();
        double height   = feature.attribute("height").toDouble();
        double boundanc0= feature.attribute("bundance0").toDouble();
        pchar = (char*)&boundanc0;
        msg.append(pchar, sizeof (double));

        double increase = feature.attribute("increase").toDouble();
        pchar = ( char* )&increase;
        msg.append( pchar, sizeof (double));

        double capacity = feature.attribute("capacity").toDouble();
        pchar = ( char* )&capacity;
        msg.append( pchar, sizeof (double));

        if (counter==0)
        {
              left = centerx - 0.5 * width / proportion / cos( initvalue *3.14/180.0 );
              bottom = centery - 0.5 * height / proportion;
        }

        if (featureCounter==  counter )
        {
            right = centerx + 0.5 * width / proportion / cos( initvalue *3.14/180.0 );
            top = centery + 0.5 * width / proportion  ;
            totalrow = row + 1;
            totalcol = col + 1;
        }
        counter ++ ;
        //qDebug() << id << "," << centerx << ","<<centery<< ","<<width<< ","<<height<< ","<<class1<< ","<<boundanc0<< ",";
    }

    msgtotal.append((char* )&bottom, sizeof (double) );
    msgtotal.append((char* )&left, sizeof (double) );
    msgtotal.append((char* )&top, sizeof (double) );
    msgtotal.append((char* )&right, sizeof (double) );
    msgtotal.append((char* )&width, sizeof( double ) );
    msgtotal.append(totalrow);
    msgtotal.append(totalcol);

    QByteArray allarray;
    allarray.append(char(4));
    allarray.append(char(option+1));//发送类型，1给飞机重置，2仅发送给地面站
    allarray.append(msgtotal);
    allarray.append(msg);

    //qDebug() << allarray.size() <<"\n";

	//如果没有创建这个socket就创建它
    if (m_pUdpBundance == nullptr)
    {  
		m_pUdpBundance = new QUdpSocket(this);
		m_pUdpBundance->bind(QHostAddress(localIP), localBundancePort.toInt());
		connect(m_pUdpBundance,SIGNAL(readyRead()),this,SLOT(read_bundanceReturn()));		
    }

	m_pUdpBundance->writeDatagram(allarray, QHostAddress(bundanceIP), bundancePort.toInt());
    QMessageBox::information(this, QString::fromLocal8Bit("提示"),QString::fromLocal8Bit("情报丰度已发送!"));
    return s;
}

void MainWindow::selectPointToTargetVector(QgsVectorLayer* layer,int type)
{
   if (type == (int)QGS_POINT)//如果是目标点图层
   {
    QgsFeatureList selectedFeatureList =  layer->selectedFeatures();
    for ( int i=0;i<selectedFeatureList.size();i++ )
    {
        TargetPoint targetpoint;
        QgsFeature feature = selectedFeatureList.at(i);
        QString name =  feature.attribute("name").value<QString>();
        QString classes =  feature.attribute("class").value<QString>();
        QString abundance = feature.attribute("abundance").value<QString>();
        QString task = feature.attribute("task").value<QString>();

        targetpoint.plan_x = double(feature.attribute("x").value<QString>().toInt())/10000000.0;
        targetpoint.plan_y = double(feature.attribute("y").value<QString>().toInt())/10000000.0;
		targetpoint.type = (targetType)type;

        stlTargetPoint.push_back(targetpoint);
    }
   }
   else
	if (type ==(int)QGS_POLY)//如果是面图层
    {
        QgsFeatureList selectedFeatureList =  layer->selectedFeatures();
        for ( int i=0;i<selectedFeatureList.size();i++)
        {
            QgsFeature feature = selectedFeatureList.at(i);            
            QString name =  feature.attribute("name").value<QString>();
            QString classes =  feature.attribute("class").value<QString>();
            QString position = feature.attribute("position").value<QString>();
            QString abundance = feature.attribute("abundance").value<QString>();
            QString task = feature.attribute("task").value<QString>();
            QgsVertexIterator it = feature.geometry().vertices();
            while (it.hasNext())
            {
                TargetPoint targetpoint;
                QgsPoint point = it.next();
                targetpoint.plan_x = point.x();
                targetpoint.plan_y = point.y();
                targetpoint.type   = (targetType)type;
                stlTargetPoint.push_back(targetpoint);
            }
            getContainedGridPoint( feature );
        }
   }
}

//接收全局规划网络udpLog数据,回调函数
void MainWindow::read_data()
{
    //resive_information.clear();   qDebug()<<"read";
    QByteArray array;

    while(m_pUdpAlogrithm->hasPendingDatagrams())
    {
        array.resize(m_pUdpAlogrithm->pendingDatagramSize());
		m_pUdpAlogrithm->readDatagram(array.data(), array.size());//      qDebug()<<array.size();

        if (m_pUdpSendTask == nullptr)
        {
			m_pUdpSendTask = new QUdpSocket(this);
			m_pUdpSendTask->bind(QHostAddress(localIP), localTaskPort.toInt());
            connect(m_pUdpSendTask,SIGNAL(readyRead()),this,SLOT(processTaskResult()));
        }

		//把接收到的全局规划数据报再发送出去
		m_pUdpSendTask->writeDatagram(array, QHostAddress(sendTaskIP), sendTaskPort.toInt());
    }
    QMessageBox::information(this,QString::fromLocal8Bit("成功"),QString::fromLocal8Bit("规划完成"));
}

//接收无人机图片,并添加到情报查询导出窗口的列表框中
void MainWindow::RecevieLook(const QByteArray& array)
{
	//如果还没有创建情报查询导出对话框，就创建此对话框
    /*if (!m_pLookDlg)
    {
		m_pLookDlg = new LookDialog();
		m_pLookDlg->setMapCavas(&m_mapCanvas);
		m_pLookDlg->setPlanes(m_planeVector);
        connect(m_pLookDlg, &LookDialog::addJpgAnnotaionCenter1Message, this,&MainWindow::addJpgAnnotaionCenter1);
    }*/

    if (!array.isNull())
    {
		QString currentID=addToRightTable(array);	//把接收到的无人机信息，添加到右侧表格中		
		appendImage(currentID, array);				//把接收到的无人机信息，添加到情报查询导出窗口的表格中
    }
}

//把接收到的无人机图片信息，添加到右侧表格中，每执行一次就添加一个
QString MainWindow::addToRightTable(const QByteArray &array)
{
    const char* ch = array.data();
    char  objectId   = *(ch+1);			//武器类型id    
	//int planePositionY = *((int*)(ch + 6));//纬度	//int planePositionX = *((int*)(ch + 10));//经度;
	//double pX = planePositionY / 10000000.0;
	double pX = *((double*)(ch + 2));	//经度;
	double pY = *((double*)(ch + 10));	//纬度
    
    QStringList typelist = WeaponTypeList.split("|");

    int row = ui->mTableWidget->rowCount();
    ui->mTableWidget->insertRow(0);

    QDateTime dt = QDateTime::currentDateTime();
    QTableWidgetItem* item = new QTableWidgetItem (QString("%1").arg(row+1));
    item->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter );

	//创建一个临时表格内每条记录无人机指针对象
    Mappoint *pt = new Mappoint();
	pt->x = pX;
	pt->y = pY;

	QString currentID	= dt.toString("yyyyMMddhhmmss") + QString::number(row+1);
	pt->messageId		= currentID;
	pt->rowid			= QString::number(row+1);

    if (( objectId<typelist.count() ) && (objectId>=0)){
		pt->weaponname = typelist.at(objectId);
    }

	pt->planeTime = dt.toString("hh:mm:ss yyyy-MM-dd");
    qlonglong vaiid = reinterpret_cast<qlonglong>(pt);
    item->setData(Qt::UserRole,QVariant(vaiid));	//设置每条记录Mappoint指针对象地址

    ui->mTableWidget->setItem(0, 0, item);
    ui->mTableWidget->setItem(0, 1, new QTableWidgetItem(pt->weaponname));

	return currentID;
}

void MainWindow::updateSqlite()
{
    if (m_mapPt)  
	{
        QObject *pObject = this->sender();
        QPushButton *pButton = qobject_cast<QPushButton *>(pObject);

        m_mapPt->taskname = pButton->text();

        QDialogTarget diaglog(this);
        diaglog.mCanvas = (QgsMapCanvas*)m_mapCanvas;
        diaglog.userinvoke = "image";
        diaglog.setWindowTitle(QString::fromLocal8Bit("发送任务"));
        diaglog.setImageTarget(m_mapPt);

        if ( diaglog.exec() == QDialog::Accepted)
        {
			createSqliteDatabase();
            QString update_sql = "update objectimage set  taskname=:taskname, typename=:typename,ishanded=:ishanded "
                                 " where messageId=:messageId";

            QSqlQuery sql_query(g_sqliteDbase);
            sql_query.prepare(update_sql);
            sql_query.bindValue(":messageId",m_mapPt->messageId);
            sql_query.bindValue(":taskname", m_mapPt->taskname);
            sql_query.bindValue(":typename", m_mapPt->weaponname);
            sql_query.bindValue(":ishanded", m_mapPt->ishanded );

            if( !sql_query.exec() )
            {
                qDebug() << sql_query.lastError();
            }
			m_pJpgAnnoWidget->setVisible(false);

            int count = ui->mTableWidget->rowCount();
            for( int i=0; i<count; i++ ){
                Mappoint *p = (Mappoint *)(qlonglong)ui->mTableWidget->item(i,0)->data(Qt::UserRole).value<qlonglong>();
                if ( p->messageId ==m_mapPt->messageId ){
                    ui->mTableWidget->item(i,1)->setText(m_mapPt->ishanded);
                    ui->mTableWidget->item(i,0)->setForeground(Qt::red);
                    ui->mTableWidget->item(i,1)->setForeground(Qt::red);
                    ui->mTableWidget->item(i,1)->setSelected(false);
                    ui->mTableWidget->item(i,0)->setSelected(false);
                    break;
                }
            }
        }

        if (m_mapPt->taskname == "放弃" ){
            QList<QGraphicsItem *> gItemList = m_mapCanvas->scene()->selectedItems();
            QList<QGraphicsItem *>::iterator it = gItemList.begin();
            for ( ; it != gItemList.end(); ++it )
            {
              QgsMapCanvasAnnotationItem *aItem = dynamic_cast<QgsMapCanvasAnnotationItem *>( *it );
              if ( aItem )
              {
                 JpgAnnotation* anotation = reinterpret_cast<JpgAnnotation*>(aItem->annotation());
                 if ( anotation->mppoint->messageId == m_mapPt->messageId )
                 {
                     aItem->setSelected(false);
                     anotation->isrender = false;
                     break;
                 }
              }
            }
        }
    }
}
//放弃,操作
void MainWindow::updateSqliteBoundance()
{
    if (m_mapPt)  
	{
        QObject *pObject = this->sender();
        QPushButton *pButton = qobject_cast<QPushButton *>(pObject);

        m_mapPt->taskname = pButton->text();
		createSqliteDatabase();
        QString update_sql = "update objectimage "
                             " set  taskname = :taskname, typename = :typename, ishanded = :ishanded "
                             " where messageId = :messageId" ;

        QSqlQuery sql_query(g_sqliteDbase);
        sql_query.prepare(update_sql);
        sql_query.bindValue(":messageId",m_mapPt->messageId);
        sql_query.bindValue(":taskname", m_mapPt->taskname );
        sql_query.bindValue(":typename", m_mapPt->weaponname );
        sql_query.bindValue(":ishanded", m_mapPt->ishanded );

        if( !sql_query.exec())
        {
            qDebug() << sql_query.lastError();
        }
		m_pJpgAnnoWidget->setVisible(false);

        int count = ui->mTableWidget->rowCount();
        for( int i=0; i<count; i++ ){
            Mappoint *p = (Mappoint *)(qlonglong)ui->mTableWidget->item(i,0)->data(Qt::UserRole).value<qlonglong>();
            if ( p->messageId == m_mapPt->messageId ){
                ui->mTableWidget->item(i,1)->setText(m_mapPt->ishanded);
                ui->mTableWidget->item(i,0)->setForeground(Qt::red);
                ui->mTableWidget->item(i,1)->setForeground(Qt::red);
                ui->mTableWidget->item(i,1)->setSelected(false);
                ui->mTableWidget->item(i,0)->setSelected(false);

                break;
            }
        }
     }

    if (m_mapPt->taskname=="放弃")
	{
        QList<QGraphicsItem *> gItemList = m_mapCanvas->scene()->selectedItems();
        QList<QGraphicsItem *>::iterator it = gItemList.begin();
        for ( ; it != gItemList.end(); ++it)
        {
          QgsMapCanvasAnnotationItem *aItem=dynamic_cast<QgsMapCanvasAnnotationItem *>(*it);
          if (aItem)
          {
             JpgAnnotation* anotation = reinterpret_cast<JpgAnnotation*>(aItem->annotation());
             if ( anotation->mppoint->messageId == m_mapPt->messageId)
             {
                 aItem->setSelected(false);
                 anotation->isrender = false;
                 break;
             }
          }
        }
    }
}
//情报导出查询
void MainWindow::lookDialogShow()
{
    if (!m_pLookDlg)
    {
		m_pLookDlg = new LookDialog(this);
		m_pLookDlg->setMapCavas((QgsMapCanvas*)m_mapCanvas);
		m_pLookDlg->setPlanes(m_planeVector);
    }
	m_pLookDlg->show();
	m_pLookDlg->raise();
	m_pLookDlg->activateWindow();
}

