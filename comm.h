#ifndef COMM_H
#define COMM_H

#include <qgsProject.h>
#include <QVector>
#include <QPoint>
#include <QSqlDatabase>
#include <iostream>
#include <vector>

#include <qgsVectorLayer.h>
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

#include "qgssymbollayerutils.h"
#include "qgssymbol.h"
#include <qgsMarkerSymbolLayer.h>
#include "qgsAnnotationLayer.h"

#include "mavlink/air/mavlink.h"

#define BUFFER_LENGTH   1024 *500
#define HUAYIN			11		//使用的某个地图，TAIWAN，BOTOU,JINGBIAN,HUAYIN

class MainWindow;
class FixPlaneThread;

//雷达设备信息结构
struct RadarInfo
{
	QString				id;		//雷达id
	int					speed;	//雷达运动速度
	QList	<QgsPointXY> plist;	//雷达运动轨迹点列表
	QList	<QgsPointXY> tlist;	//根据雷达运动速度，添加的轨迹点列表，每隔10米就添加一个轨迹点
};

struct SpliceCheck
{
    uint16_t AirID;         //airID
    uint32_t BigImgID;      //bigImgID
    uint16_t FileCount;
    uint16_t receciedCount;
    qint64	waittime = 0;
    uint32_t timethreshold = 5000;//5000毫秒
};

//接收到的无人机信息结构
struct Mappoint : QObjectUserData {
    QString messageId;
    QString rowid;		//行id，序号
    QString planeId;	//无人机id
    QString taskname;	//任务类型
    QString weaponname;	//武器类型
    double x;			//无人机纬度
    double y;			//无人机经度
    double xmin;
    double ymin;
    double xmax;
    double ymax;
    double angleX;
    double angleY;
    QString planeTime;
    QString fileName;	//文件名称
    QString ishanded;
    void*  picture;		//接收到的图片指针
};

// 连接数据库的一些必要信息
struct ConnectionInfo
{
    const char* host;            // 主机地址
    const char* user;            // 用户名
    const char* password;        // 密码
    const char* database;        // 数据库名
    unsigned int port;           // 端口号
    const char*    unix_socket;  // unix连接标识
    unsigned long clientflag;    // 客户端连接标志

    ConnectionInfo() :
        user("root"),
        password("123456"),
        database("test"),
        host("127.0.0.1"),
        port(3306),
        unix_socket(nullptr),
        clientflag(0)
    {
    }
};

//目标图元类型，也是图层类型
enum targetType
{
    NONE = 0,
    QGS_POINT,	//点
    QGS_LINE,	//线
    QGS_POLY	//面
};

enum AbundenceType //富足
{
    HEIGHT_SPEED_ROAD = 0,
    COUNTRY_HIGHWAY,
    NATIONAL_HIGHWAY,
    RURAL_ROAD,
    OTHER
};

#pragma pack(1)

//mavlink协议，目标设置接口定义，无人机控制结构,包大小22个字节,messageid为62
typedef struct PlaneCtr
{	
	uint16_t objectID;	//目标id,对应shp图层数据文件的objectid字段
	uint8_t type;		//目标动作类型，0：普通目标，1：多目标同时攻击，2：目标点侦察，3：摧毁
	int16_t unitID;		//目标所属战术单元id
	int16_t planeID;	//目标所属成员id,	对应mavlink协议的系统ID，sysID
	int16_t groupID;	//目标所属战斗组id,	对应mavlink协议的组件ID,compID
	int32_t y;			//目标纬度
	int32_t x;			//目标经度
	int16_t z;			//目标高度
	int16_t count;		//目标容量
	uint8_t status;		//目标执行状态，0:waiting,1:latched,2:finished,3:occupy
};
#pragma pack()

typedef struct FindLinePoint
{
    double x;
    double y;
};

typedef struct GlobalTask
{
    int airId = 0;
    int pointNum = 0;
    std::vector<int> pathSeq;
};

//网格结构
typedef struct GridInfo
{
    QgsPointXY track_point;
    double plan_x;
    double plan_y;
    int  row;
    int  col;
    double abundanceValue = 0.0;
    double increaseRate = 0.0;
    double abundanceCapacity = 0.0;
    targetType type = NONE;
};

typedef struct AllGridInfo
{
    QgsPointXY up_left,up_right,down_left,down_right;
    int row;
    int col;
    int scaleValue = 255;
    int Y = 0;
    int K = 0;
    double Q = 0.0;
    GridInfo grid;
    bool operator ==(const targetType intype)
    {
        if(grid.type==intype)
            return true;
        return false;
    }
};

typedef struct TargetPoint
{
    //构造函数
    TargetPoint():track_point(0,0),plan_x(0),plan_y(0),targetNum(0)
    {
    }

    QgsPointXY  track_point;
    double      plan_x;
    double      plan_y;
    int         targetNum;      //目标个数
    targetType  type = NONE;    //目标类型，点，线，面
    bool operator == (const QgsPointXY& point)
    {
        if(this->track_point.x() == point.x() && this->track_point.y() == point.y()) return true;
        return false;
    }
};

//mavlink类型，接收到的无人机消息结构，Message ID :4
typedef struct tag_PlaneMessage
{
	//构造函数
	tag_PlaneMessage():planeY(""),planeX(""),xZ(""),hZ(""), jqtbTime("")
	{
	}
	QString planeY;		//纬度
	QString planeX;		//经度
    QString xZ;			//相对高度
	QString hZ;			//海拔高度
	QString Pitch;		//Pitch
    QString Roll;		//roll
	QString Yaw;		//yaw
	QString ID;			//当前航点序号
	QString status;		//复合状态，Bit:0 安全开关，Bit:1 解锁状态，Bit:2 飞控是否引导模式，Bit:3 飞控原点坐标是否重设
	QString airSpeed;	//空速
	QString groundSpd;	//地速
	QString vN;			//北向速度分量(m/s)
	QString vE;			//东向速度分量(m/s)
	QString vU;			//天向速度分量(m/s)
	QString gpsStatus;	//gps状态
	QString value;		//电压
	QString gpsNum;		//gps星数
	QString taskType;	//飞行任务类型
    QString gjMode;		//轨迹模式
	QString qkRunMode;	//群控运行模式
	QString qkCmdMode;	//群控指令模式
	QString fkRunMode;	//飞控运行模式
	QString fkSysStatus;//飞控系统状态
	QString jqtbTime;	//集群同步时间

	// 自定义赋值操作符
	tag_PlaneMessage& operator=(const tag_PlaneMessage &other) {
		if (this != &other) {
			planeY     = other.planeY;
			planeX     = other.planeX;
			xZ         = other.xZ;
			hZ         = other.hZ;
			Pitch      = other.Pitch;
			Roll       = other.Roll;
			Yaw        = other.Yaw;
			ID         = other.ID;
			status     = other.status;
			airSpeed   = other.airSpeed;
			groundSpd  = other.groundSpd;
			vN         = other.vN;
			vE         = other.vE;
			vU         = other.vU;
			gpsStatus  = other.gpsStatus;
			value      = other.value;
			gpsNum     = other.gpsNum;
			taskType   = other.taskType;
			gjMode     = other.gjMode;
			qkRunMode  = other.qkRunMode;
			qkCmdMode  = other.qkCmdMode;
			fkRunMode  = other.fkRunMode;
			fkSysStatus= other.fkSysStatus;
			jqtbTime   = other.jqtbTime;
		}
		return *this;
	}
};

#pragma pack(1)
//与dds程序通信用结构
struct BigImg
{
	uint8_t		airID;		//无人机id
	uint8_t		type;		//目标类型：0迷彩帐篷、1大白车、2靶标、3装甲指挥车、4频谱管理车、5卫星通信车、6通信指挥车、7炮兵指挥车、8情报指挥车、9坦克
	uint8_t		pack;		//包类型，0为小图，1为大图需要拼接，每接收36次后组成大图，小图分辨率为672*506
	uint32_t	height;		//传输图片高度，3036
	uint32_t	width; 		//传输图片宽度，4032
	uint8_t		ImgNumx;    //切分块数x, 6
	uint8_t		ImgNumy;    //切分块数y, 6
	double      lat;   		//当时无人机的纬度
	double      lng;   		//当时无人机的经度
	double      hgt;   		//当时无人机的高度
	uint8_t     imgx;  		//小图片x轴值，0-5
	uint8_t     imgy;  		//小图片y轴值，0-5
	uint8_t     imgid;  	//小图片id，0-35	
	uint32_t    bigImgID;   //发送端设置的大图片id，同一个大图此值相同，理论上会发36次
	double	    xy[12];		//坐标
};
#pragma pack()

struct MarkerSymbol
{
	int id;
	QgsMarkerSymbol *pSymb;
};

//extern QMutex                       sMavlinkMutex;
extern QMutex                       sBatchMutex;
extern QMutex                       gSplicCheckTableMutex;
extern QVector<SpliceCheck*>        gSplicCheckTable;
extern QVector<PlaneCtr*>			g_BatchControls;		//批量发送无人机目标列表
extern std::vector<TargetPoint>     stlTargetPoint;	//全局-目标点列表
extern QVector<AllGridInfo>         stlAllGridInfo;	//全局-网格信息列表

extern QgsRasterLayer       *g_pRasterLayer;//tif类型底图文件图层

extern QgsVectorLayer		*g_pRadarTaskLayer;		//全局-雷达投影图层和任务区域重叠图层
extern QgsVectorLayer		*g_pRadarTyLayer;		//全局-雷达投影图层
//extern QgsVectorLayer		*g_pRadarLayer;			//全局-雷达图层
extern QVector<QgsVectorLayer*> gRadarLayerList;	//全局-雷达图层列表
extern QgsVectorLayer		*g_pAirLayer;			//全局-无人机图层
extern QgsVectorLayer       *g_pPointLayer;			//全局-点轨迹图层,无人机点数据，雷达不覆盖,黑色轨迹
extern QgsVectorLayer       *g_pRadarPtLayer;		//全局-点轨迹图层,无人机点数据，雷达设备可覆盖，红色轨迹
extern QgsVectorLayer       *g_pTargetLayer;		//全局-目标点图层,发送的无人机数据
extern QgsVectorLayer       *g_pLineLayer;			//全局-线图层,战场设置范围用
extern QgsVectorLayer       *g_pPolygonLayer;		//全局-面图层
extern QgsVectorLayer       *g_pAirTaskPolyLayer;	//全局-无人机任务区域面图层
extern QgsVectorLayer       *g_pGirdLayer;			//全局-网格，光栅图层
extern QgsVectorLayer       *m_electroMagnetism;	//全局-电子磁性图层
extern QgsVectorLayer       *g_pSmallJpgLayer;		//全局-用于显示接收到的情报丰度小图片
extern QMutex				g_planeMutex;
extern QVector<QgsPointXY>	g_PlaneList;			//全局接收到的航线集合，每次刷新添加此列表里的值，然后清空
extern QVector<QgsPointXY>	g_RadarTrackList;		//全局无人机雷达覆盖轨迹集合，每次刷新添加此列表里的值，然后清空

extern QgsVectorLayer       *g_pRoadLayer;			//全局-道路，路网图层
extern QgsVectorLayer		*g_pTRoadLayer;			//全局-台湾铁路图层
extern QgsVectorLayer		*g_pjtzdLayer;			//全局-台湾交通站点图层

extern QMainWindow          *gMainWindow;			//全局-主窗口指针对象
//extern QSqlDatabase			g_mysqlDbase;			//全局mysql数据库表
extern QSqlDatabase			g_sqliteDbase;			//全局sqlite3数据库表

													
//helloPlane.ini文件配置参数
extern QString              localAlorithmPort;
extern QString              localBundancePort;
extern QString              localAttackPort;
extern QString              localIP;
extern QString              localPlanePort;			//接收mavlink协议用端口号
extern QString              localTaskPort;
extern QString              localPicture;
extern QString				localEletricPort;

//Other
extern QString				algorithmsIp;
extern QString				algorithmsPort;
extern QString				bundanceIP;
extern QString				bundancePort;
extern QString				getPlaneIP;
extern QString				getPlanePort;
extern QString				sendTaskIP;
extern QString				sendTaskPort;
extern QString				attackerIP;
extern QString				attackerPort;

//[WeaponType]
extern QString				WeaponTypeList;
extern QString				g_taskList;
extern QString				g_targetType;
extern QString				g_targetTask;

extern QMutex				g_ddsMutex;			//接收dds程序线程，同步用互斥对象
extern QVector<BigImg>		g_RecievedDDS;		//接收dds程序数据列表
extern char					g_mainDir[MAX_PATH];//程序主运行目录

extern QMutex					 g_transitMutex;		//接收recMavlinkThread程序线程，同步用互斥对象
extern QVector<tag_PlaneMessage> g_transitList;		//接收tag_PlaneMessage数据列表

#endif // COMM_H
