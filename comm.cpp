#include "stdafx.h"
#include "comm.h"
#include <QSqlDatabase>
#include "mainWindow.h"
#include "FixPlaneThread.h"

using namespace std;

QMutex					sBatchMutex;
QVector<PlaneCtr*>		g_BatchControls;		//批量发送无人机目标列表
QMutex					gSplicCheckTableMutex;
QVector<SpliceCheck*>	gSplicCheckTable;

QgsRasterLayer			*g_pRasterLayer=nullptr;				//tif类型底图文件图层
QgsVectorLayer			*g_pRadarTaskLayer=nullptr;		//全局-雷达投影图层和任务区域重叠图层
QgsVectorLayer			*g_pRadarTyLayer = nullptr;		//全局-雷达投影图层
//QgsVectorLayer			*g_pRadarLayer=nullptr;			//全局-雷达图层
QVector<QgsVectorLayer*> gRadarLayerList;				//全局-雷达图层列表

QgsVectorLayer			*g_pAirLayer = nullptr;			//全局-无人机图层
QgsVectorLayer			*g_pPointLayer=nullptr;			//全局-点轨迹图层,无人机点数据
QgsVectorLayer			*g_pRadarPtLayer=nullptr;		//全局-点轨迹图层,无人机点数据，雷达设备可覆盖，红色轨迹
QgsVectorLayer			*g_pTargetLayer =nullptr;		//全局-目标点图层
QgsVectorLayer			*g_pLineLayer	=nullptr;		//全局-线图层，战场设置范围
QgsVectorLayer			*g_pPolygonLayer=nullptr;		//全局-多边型面图层
QgsVectorLayer			*g_pAirTaskPolyLayer=nullptr;	//全局-无人机任务区域面图层
QgsVectorLayer			*g_pGirdLayer = nullptr;        //全局-网格，光栅图层
QgsVectorLayer			*m_electroMagnetism = nullptr;
QgsVectorLayer			*g_pSmallJpgLayer= nullptr;		//全局-小图片图层

QMutex					g_planeMutex;					//用于同步g_PlaneList
QVector<QgsPointXY>		g_PlaneList;					//全局无人机轨迹集合
QVector<QgsPointXY>		g_RadarTrackList;				//全局无人机雷达覆盖轨迹集合
QgsVectorLayer			*g_pRoadLayer = nullptr;		//全局-路网图层
QgsVectorLayer			*g_pTRoadLayer = nullptr;		//全局-台湾铁路图层
QgsVectorLayer			*g_pjtzdLayer = nullptr;		//全局-台湾交通站点图层

QVector<AllGridInfo>        stlAllGridInfo;				//全局 -网格信息列表
std::vector<TargetPoint>    stlTargetPoint;				//全局-目标点列表
QMainWindow                 *gMainWindow    = nullptr;
//QSqlDatabase				g_mysqlDbase;				//全局mysql数据库表
QSqlDatabase				g_sqliteDbase;				//全局sqlite3数据库表

//helloPlane.ini文件配置参数
QString              localAlorithmPort;
QString              localBundancePort;
QString              localAttackPort;
QString              localIP;				//用于接收mavlink协议数据的本地ip
QString              localPlanePort;		//接收mavlink协议用端口号
QString              localTaskPort;
QString              localPicture;
QString				localEletricPort;

//Other
QString				algorithmsIp;
QString				algorithmsPort;
QString				bundanceIP;
QString				bundancePort;
QString				getPlaneIP;
QString				getPlanePort;
QString				sendTaskIP;
QString				sendTaskPort;
QString				attackerIP;
QString				attackerPort;

//[WeaponType]
QString				WeaponTypeList;
QString				g_taskList;
QString				g_targetType;
QString				g_targetTask;

QMutex				g_ddsMutex;		//接收dds程序线程，同步用互斥对象
QVector<BigImg>		g_RecievedDDS;	//接收dds程序数据列表
char				g_mainDir[MAX_PATH];//程序主运行目录

QMutex					  g_transitMutex;		//接收recMavlinkThread程序线程，同步用互斥对象
QVector<tag_PlaneMessage> g_transitList;		//接收tag_PlaneMessage数据列表
