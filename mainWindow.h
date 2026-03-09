#ifndef MAINWINDOW_H
#define MAINWINDOW_H

class DlgMapManager;   // forward declaration — avoids circular include

#include "comm.h"
#include <QMainWindow>
#include <QHash>
#include <QUdpSocket>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QTableWidgetItem>
#include <QImage>
#include <QtWebEngineWidgets/QWebEngineView>

#include <qgsRasterlayer.h>
#include <qgsApplication.h>
#include <qgsSettings.h>
#include <qgsGui.h>
#include <qgsCoordinateUtils.h>
#include <qgsMaplayer.h>
#include <qgsMapCanvas.h>
#include <qgsVectorLayer.h>
#include <qgsCategorizedSymbolRenderer.h>
#include <qgsMaptoolPan.h>
#include <qgsAdvancedDigitizingDockWidget.h>
#include <qgsStatusBar.h>
#include <qgsMessageBar.h>
#include <qgsLayerTreeview.h>
#include <qgsLayerTreeMapCanvasBridge.h>
#include <qgsAttributeEditorContext.h>
#include <qgsVectorLayerref.h>
#include <qgsMaptoolExtent.h>
#include <qgsRulebasedRenderer.h>
#include <qgsAnnotationMarkerItem.h>
#include <qgsMarkerSymbolLayer.h>

#include "dataConversion.h"
#include "addPolygonTool.h"
#include "vertextool/qgsVertextool.h"

#include "qgsMaptoolSelect.h"
#include "qgsMapToolSelectUtils.h"
#include "qgsMaptoolAddFeature.h"
#include "qgsGuiVectorLayerTools.h"
#include "qgsMaptoolMoveFeature.h"
#include "qgsAttributeDialog.h"
#include "qgsMaptoolCircle2points.h"
#include "qgsMeasureTool.h"
#include "qgsSvgAnnotation.h"
#include "qgsMaptoolSvgAnnotation.h"
#include "qgsAnnotationLayer.h"
#include "qgsAnnotationMarkerItem2.h"
#include "qgsMaptoolJpgAnnotation.h"
#include "qgsMaptoolAddTarget.h"
#include "qgsMapToolRotateFeature.h"

#include "modifyBoundance.h"
#include "lookDialog.h"
#include "gridDialog.h"
#include "informationDetailDialog.h"
#include "qDialogTarget.h"
#include "qgsmapCanvasZjr.h"
#include "RadarTip.h"
#include "TaskAreaTip.h"
#include "Dialog_AirList.h"
#include "dlgAddTaskArea.h"
#include "DlgModifyTaskArea.h"

class QgsMapCanvasZjr;
class biaopai;
class dataBackDlg;
class mavlink5Thread;
class RecMavlinkThread;
class FixPlaneThread;
class TransitThread;
class QsetRadarDlg;
class DlgTuili;

QT_BEGIN_NAMESPACE
namespace Ui {
	class QgsMapCanvasZjr;
	class MainWindow; 
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    static MainWindow               *sInstance;				//实例指针
	QgsMapCanvasZjr                 *m_mapCanvas=nullptr;	//地图画布对象
	QUdpSocket						*m_pUdpsocket = nullptr;//接收dds程序发来的图片消息套接字
	dataBackDlg						*m_pDBBackDlg = nullptr;//mavlink协议无人机数据回放查询
	bool							m_bBacking	=false;		//无人机数据是否正在回放，初始值false表示没有回放
	QsetRadarDlg					*m_pSetRadarDlg=nullptr;//设置雷达对话框
	SHELLEXECUTEINFO				m_ShExecInfo;			//控制外部接收mavlink数据程序结构

    SHELLEXECUTEINFO				m_ShExecTransit;		//外部转发 mavlink数据程序结构
	QSqlQuery						*m_pSqlQuery = nullptr;
	QVector	<RadarTip*>				m_radarTipList;			//全部无人机风格符号列表
	QVector	<TaskAreaTip*>			m_TAreaTipList;			//全部无人机风格符号列表
	QStringList						m_DevList;				//雷达装备设备id列表，对应根目录下的radar1.txt
	QList	<RadarInfo>				m_RadarTrack;			//雷达轨迹点列表
	int								m_iRaderTckID=0;		//雷达轨迹点执行id
	Dialog_AirList					*m_pDlgAirList = nullptr;//雷达探测到的无人机列表窗口
	DlgAddTaskArea					*m_pDlgAddTaskArea = nullptr;//添加无人机任务区域对话框
	DlgModifyTaskArea				*m_pDlgModifyArea = nullptr;//修改任务区域
    DlgTuili                        *m_pDlgTuili = nullptr; //推理窗口

    QWebEngineView                  *m_pWebEngineView = nullptr;//3D图像地图窗口（CesiumJS）
    QWebEngineView                  *m_p2DMapView = nullptr;    //Leaflet 2D地图窗口
    int                             m_mapViewMode = 0;          //0=QGIS 1=Leaflet2D 2=3D
    bool                            m_leafletReady = false;     //Leaflet页面是否加载完成

    void    refreshRadarCombox();       //刷新雷达组合框控件
    void    loadRadarUavMount();        //从 radar_mount.ini 加载探测设备与无人机的装载关系
    void    updateMountedRadarProjection(int radarId, const QgsFeature& radarFeat, QgsPointXY newPt); //实时更新装载设备的探测范围投影
	void	ReadIniFile();				//读取ini配置文件	
    void	gridLayerMarker();			//渲染表格图层风格
    void	planeLayerMarker(QgsVectorLayer *);        //渲染飞机图层风格
    void	targetLayerMarker();		//渲染目标图层风格
    void	lineLayerMarker();			//渲染线图层风格
    void	electicMagnetismMarker();	//渲染图层
	void	guijiLinePointMarker();		//渲染轨迹线点图层
	void	radarDeviceMarker();		//渲染雷达设备图层
	void	radarDevTrackMarker();		//渲染雷达轨迹图层
	void	airTaskAreaMarker();		//渲染无人机任务区域图层
	void	insertToTable(tag_PlaneMessage *p);//添加无人机到表格控件中
	void	ModifyTable(tag_PlaneMessage *p);	//实时修改表格无人机数据
    static void chanegeDialogLabel( QgsAttributeDialog *dialog, QString ltstring = "" );
	QList<QgsPointXY>	GetTYPolygon(QgsPointXY devPt,float sAngle, float tAngle,float pitch,int length);
	void	ShowRadarTip();				//显示雷达设备名称
	void	ShowTaskAreaTip();			//显示任务区域名称
	QList	<int>	isRadarPolygon(tag_PlaneMessage p);	//判断当前点是否在雷达扇形区域内，返回值大于0表示在扇形区域内，并返回雷达设备id
	QList	<int>	isRadarInPoly(tag_PlaneMessage p);	//判断当前点是否在雷达扇形区域内，返回值大于0表示在扇形区域内，并返回雷达设备id
	QList	<QgsPointXY>			getTrack(QgsPointXY p1, QgsPointXY p2, int speed);
	void							ShowShortMenu(QMouseEvent * e);

	RecMavlinkThread				*m_pRecThread		= nullptr;//无人机mavlink协议接收线程
	TransitThread					*m_pTransitThread	= nullptr;//数据中转线程
	mavlink5Thread					*m_pMavThread		= nullptr;
	
    QgsAdvancedDigitizingDockWidget *mDock				= nullptr;
    QgsGuiVectorLayerTools          *mVectorLayerTools	= nullptr;	//面图层工具
    QLineEdit                       *m_PlaneNumEditer;  //显示飞机数量文本框
    QComboBox                       *m_pRaderLayerBox=nullptr;//雷达图层组合框
    QgsExpressionContext            mExpressionContext; //异常上下文
    QVector<tag_PlaneMessage>       m_planeVector;      //接收到的实时的mavlink类型无人机消息列表
	QVector<biaopai*>				m_planeIDvec;		//接收到的实时的mavlink类型无人机消息列表
	QVector<FixPlaneThread*>		m_planeThreadVec;	//各飞机对应的位置更新线程（断开时统一清理，防止stale fid污染）
	QHash<QString, tag_PlaneMessage> m_latestPlaneData;	//各飞机最新位置缓冲（由registerPlane写入，由定时器批量处理）
    QList <QgsMapLayer*>            m_layers;			//全局图层列表
	//QgsVectorLayer					*m_pPlaneGjLayer;	//无人机轨迹图层

	QgsVectorLayer *CreateDeviceLayer(QColor color);//创建设备投影内存图层	
	void CreateLineLayer();	//创建临时无人机线轨迹内存图层
	void addFeatureToMap(Mappoint *pUser);
	bool insertPlaneMessage(Mappoint *message, QByteArray& bytearray);
	void appendImage(QString currentID, const QByteArray& array);
    void addOpenSourceRasterLayer(const QString& url, const QString& basename, const QString& providerKey="");

	//窗口自带事件
	void		mousePressEvent(QMouseEvent *);
	void		mouseReleaseEvent(QMouseEvent *);
	int			m_row;			//累计接收小图片计数
	byte		m_zoomLevel;	//当前缩放级别，1-20
	//uint64_t	m_PtCount = 0;		//累计接收到的点个数
	QTimer		*m_pTimer1=nullptr;		//定时器，刷新雷达设备移动轨迹
	QTimer		*m_pAirLayerTimer=nullptr;	//定时器，每100ms统一刷新一次无人机图层（替代per-message triggerRepaint）
	bool		 m_bTrackEnabled  = false;	//是否开启实时轨迹记录
	bool		 m_bAirLayerDirty = false;	//有新飞机位置数据待渲染，避免无变化时100ms持续触发全图层重绘
	QString		 m_basemapPath;				//当前底图文件绝对路径（供地图管理对话框标记高亮）
	QgsRectangle m_tifExtent;				//TIF底图的地理范围（加载后持久保存，切换在线地图时恢复）
	DlgMapManager *m_pDlgMapManager = nullptr;//地图管理对话框（常驻，非模态）
	QAction		*m_pActMapManager = nullptr;//工具栏"地图管理"按钮
	QAction		*m_pActTrack      = nullptr;//轨迹记录开关按钮（checkable）
	QAction		*m_pActClearTrack = nullptr;//清除轨迹按钮
	QHash<int, QString> m_radarUavMount;	//探测设备装载关系: radarId → uavId（空串=静止）
public slots://定义信号槽,回调函数

    void showMousePoint(const QgsPointXY &p);         //鼠标光标回调函数
	void scaleChanged(double p);	//地图缩放回调函数
	void UdpSockerRecvData();		//udp接收函数
	void AddMenu1();				//添加右键单击菜单
	void AddMenu2();				//添加右键单击菜单
	void AddMenu3();				//添加右键单击菜单
	void AddMenu4();				//添加右键单击菜单
	void AddMenu5();				//添加右键单击菜单

	void RadarTouying();			//是否显示雷达投影，初始不显示
	void selectRadarLayer();		//选择雷达图层
	void selectRadarDevice(QString s);		//选中雷达
	void moveRadarDevice();			//移动雷达
	void deleteRadarDevice();		//删除雷达
	void fixRadarDevice();			//修改雷达参数
	void OutfitMove();				//装备移动
	void timer1_timeout();			//雷达设备移动时，定时器过程函数
	void onAirLayerRefreshTimer();	//无人机图层定时刷新（100ms/次，10fps），替代per-message triggerRepaint
	void processAllPlaneUpdates();	//批量处理 m_latestPlaneData 中缓冲的飞机位置（canvas空闲时调用）
	void RadarTestAirList();		//雷达探测无人机列表

	void SelectTaskLayer();			//选择无人机任务区域图层
	void AddAirTaskArea();			//添加无人机任务区域
	void SelectTaskArea();			//选择无人机任务区域
	void FixTaskArea();				//修改无人机任务区域
	void MoveTaskArea();			//移动无人机任务区域
	void DeleteTaskArea();			//删除无人机任务区域
	void ShowRadarNoArea();			//显示雷达已覆盖任务区域
	void AreaRadarList();			//显示任务区域所覆盖的雷达列表
	void TaskGuihua();				//任务规划

    //菜单-设置
    void addVectorlayers();     //添加图层回调
    void addRasterlayers();
    void addRasterlayers(QString pathname,QString layername);
    void editVectorSymbol();    //设置风格回调
    void settingLayer();        //设置图层回调
	void equipmentManager();	//装备库管理
    void hideLittleSelectedPicture();//隐藏显示图片栈列表框回调
    void hideBigPicture();      //隐藏显示自定义大图回调
	void hideSmallPicture();    //隐藏显示小图片回调
	void hideTable();			//隐藏显示实时表格
	void startStopReceive();	//启动停止接收mavlink无人机数据
	void connectUDP();			//配置UDP端口并（重）连接接收线程
	void RefreshGj();			//刷新轨迹线
	void toggleTrack();			//开关轨迹实时记录
	void clearTrajectory();		//清除地图上的轨迹点
	void showMapManager();		//打开地图管理对话框
	void switchBaseMap(const QString& filePath, const QString& layerName); //切换TIF底图
	void loadOnlineTileMap(const QString& url, const QString& layerName);  //加载在线瓦片为底图

    //菜单-绘制
    void moveView();            //平移地图工具回调
    void addDrawPoint();        //添加点目标回调
    void addDrawLine();         //添加线目标回调
    void addDrawPolygon();      //添加面目标回调
    void drawCircle2point();    //添加圆目标回调
    void saveToLayer();         //保存图层信息工具回调
    void editLine();            //编辑路径回调
    void editPolygon();         //编辑多边形回调
    void deleteSelected(QgsMapLayer *layer = nullptr, QWidget *parent = nullptr, bool checkFeaturesVisible = false );//删除标注
    void modifyFeatures();          //设置修改标注，图元属性
    void selectPoint();				//目标点，工具
    void selectLine();              //选择线图层
    void selectPolygon();           //选择面图层工具
    void selectPicture();           //选择图片工具
    void measureLength();           //测量长度工具
    void measureArea();             //测量面积工具
    void testMenu();                //测试菜单
	void dataBack();                //数据回放
    void JiqunTuili();              //集群推理
    void show2Dor3Dwindow();        //切换地图模式（QGIS→Leaflet→3D循环）
    void switchMapViewMode(int mode);//切换到指定地图模式：0=QGIS 1=Leaflet2D 2=3D

    //菜单-规划。。。。
	//void ModifyPlane(tag_PlaneMessage *plane);
	//void ModifyPlane(QString AirID, QString X, QString Y, QString Z, QString yaw, QString roll, QString pitch);
    void getPlaneCount();           //获取飞机信息，个数等等....
    void createGrid();              //战场设置
    QString sendBundiance();        //发送情报丰度
    void ScanPathPlan();            //全局规划
    void lookDialogShow();          //情报导出查询
	void mapMiddle();				//地图居中,center

    //以下为主窗口自己回调函数
    void addJpgAnnotaion(double leftupY,double leftupX,       //添加自定义小图片回调函数
                         double rightupY, double rightupX,
                         double leftdownY,double leftdownX,
                         double rightdownY,double rightdownX,  QString filename);

    void addJpgAnnotaionCenter1(Mappoint *mappoint, double width, double height,int a);

    void hidetable();						//隐藏显示图片栈,右侧列表框    
    void udpRecieveEleticMagnetismInit();
    void processEleticMagnetismDatagram();
    void selectElecticMangnetism();
    void read_data();
    void RecevieLook( const QByteArray& array);			//接收飞机图片信息
    QString addToRightTable(const QByteArray& array);	//添加飞行信息到右边表格中

    void updateSqlite();
    void updateSqliteBoundance();
    void annotationCreated( QgsAnnotation *annotation);
    void showJpgAnnotaionWindow(QPoint& pos, bool show, Mappoint* point); //显示jpgAnnotaion工具操作窗口，有打击，侦察等按钮
    void locatePicture(QTableWidgetItem *item);
    void showDetailForm();
	void registerPlane(tag_PlaneMessage *planeMessage);//向图层中注册飞机

signals:	//定义，信号函数
	void FixPlaneMsg(tag_PlaneMessage *);	//修改无人机位置信号
	
    void receviePlaneMessage(const QgsPointXY& qgsPoint);	//接收飞机消息
    void recevieImage(const QByteArray& array);             //接收图片信息
	void AddJpgAnnotaionMsg(Mappoint *, double width, double height, const int a);

public:
    static MainWindow* instance(){ return sInstance; }

    QgsStatusBar    *m_pStatusBar;    //状态栏对象
    QgsMessageBar   *m_pInfoBar;      //消息栏窗口对象

    QgsAdvancedDigitizingDockWidget *cadDockWidget() { return mDock; }
    //QgsMessageBar* messageBar(){return mInfoBar; }

    void commitError( QgsVectorLayer *vlayer);

    QWidget	*m_pJpgAnnoWidget;		//jpgAnnotaion类型工具临时窗口,有侦察，打击等按钮

    void initRightList();			//初始化右侧表格窗口
    void initJpgAnnoWidget();		//初始化打击侦察详情窗口
    void initWidgetAttribute();		//初始化编号，时间类型，位置对应任务，窗口

    void saveEdits(QgsMapLayer *layer, bool leaveEditable, bool triggerRepaint );
    //bool toggleEditing( QgsMapLayer *layer, bool allowCancel );

    void LoadVectorlayers(QString fileName,QgsVectorLayer** gloablayer);

    QgsAttributeEditorContext createAttributeEditorContext();

    void resolveVectorLayerDependencies( QgsVectorLayer *vectorLayer,
                                         QgsMapLayer::StyleCategories categories = QgsMapLayer::AllStyleCategories );
    const QList< QgsVectorLayerRef > findBrokenLayerDependencies( QgsVectorLayer *vectorLayer,
        QgsMapLayer::StyleCategories categories = QgsMapLayer::StyleCategory::AllStyleCategories ) const;

    void selectPointToTargetVector(QgsVectorLayer *layer, int type);

    /** @brief read_data 调用python算法  */
    void pythonInterface(QByteArray &willSendData);
    int  getContainedGridPoint(QgsFeature feature);

    void closeEvent(QCloseEvent *event);              //用户关闭窗口回调函数
    void SingleColor(QgsVectorLayer *vlayer);         //设置图层风格
   

    //添加svg类型图元标注
    void addSvgAnnotation(QgsPointXY a, QgsPointXY b);
    //添加大图的模拟
	QgsAnnotationLayer * addJpgAnnotationToLayer(int id);	//根据无人机id，添加一个图层
    void resizeEvent(QResizeEvent *event);		//系统自带窗口大小变化事件
protected slots:     
private:
    void setMapCanvas();        //设置地图图层
    void setEvents();           //设置菜单工具栏，主窗口事件回调函数
    bool createSqliteDatabase();//创建sqlite3数据库
	bool createDatabase();      //创建mysql数据库
    //bool insertPlaneMessage(PPlaneMessage message);
    void setActionSvg();
    bool addPlaneLayer(tag_PlaneMessage planeMessage);      //根据参数在飞机图层上添加一个飞机
    void initLeafletMap();                                  //初始化Leaflet 2D地图，写HTML并加载
    

    Ui::MainWindow *ui;

    QgsLayerTreeMapCanvasBridge *m_layerTreeCanvasBridge;
    QDockWidget             *m_layerTreeDock;
    QgsMapToolPan           *mToolPan			= nullptr;      //地图面板平移工具
    QgsMapToolSelect        *mToolSelect		= nullptr;      //选择图元工具

    QgsVertexTool           *mMapToolVertext	= nullptr;      //图元编辑工具
	QgsMapToolAddTarget     *mMapToolAddTarget = nullptr;		//添加目标点工具
	QgsMapToolAddFeature    *mMapToolAddline	= nullptr;		//添加图元工具 线
    QgsMapToolAddFeature    *mMapToolAddPolygon = nullptr;		//添加图元工具 面
	
    QgsMapToolMoveFeature   *mMoveFeature		= nullptr;      //平移图元工具
    QgsMapToolAddFeature    *mMapToolAddCircle	= nullptr;		//添加圆型图元工具
    QgsMapTool              *mMapToolCircle2d	= nullptr;		//添加圆型图元工具
    QgsMeasureTool          *maptoolMeaserLenght= nullptr;		//长度测量工具
    QgsMeasureTool          *maptoolMeaserArea	= nullptr;		//面积测量工具
    QgsMapToolSvgAnnotation *mMaptoolSvgAnnotation = nullptr;	//声明一个svg格式工具
    QgsMapToolJpgAnnotation *mMaptoolJpgAnnotation = nullptr;	//声明一个jpg格式自定义大图，工具

	//QgsMapToolRotateFeature *mpRotateFeatureTool=nullptr;		//任务区域旋转图元工具
    QUdpSocket *m_pUdpAlogrithm = nullptr;		//全局规划用，发送socket
    QUdpSocket *m_pUdpSendTask	= nullptr;		//发送任务socket    
    QUdpSocket *m_pUdpBundance	= nullptr;		//边界socket
    QUdpSocket *m_pUdpElectric	= nullptr;		//电子socket

  
	ModifyBoundance *mMoidyfyBundance	=nullptr;   //边界，消息处理线程
    GridDialog      *m_gridDialog		=nullptr;	//战场范围设置窗口

public:    
    LookDialog      *m_pLookDlg		= nullptr;		//情报查询导出窗口
	Mappoint        *m_mapPt		= nullptr;		//接收到的目标点信息
    BatchSendThread *m_pBatchThread = nullptr;		//战场设置， 批量发送处理线程对象

    QLabel          *m_lblCoordinate        = new QLabel;   //添加一个状态栏小面板
    QLabel          *m_lblSelectTargetCount = new QLabel;   //添加一个状态栏小面板
};
#endif // MAINWINDOW_H
