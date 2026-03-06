#pragma once
#define MAVLINK_MSG_ID_planeInfo	4
//#define MAVLINK_NEED_BYTE_SWAP		11
#pragma pack(1)
typedef struct __mavlink_planeinfo_t {
 int32_t Lat;		/*<  Lat*/
 int32_t Lng;		/*<  Lng*/
 int16_t Alt;		/*<  Alt-id*/
 int16_t Altasl;	/*<  Altasl-id*/
 int8_t Picth;		/*<  Picth*/
 int8_t Roll;		/*<  Roll*/
 int8_t Yaw;			/*<Yaw*/
 uint8_t id;			/*<当前航点id*/
 uint8_t status;		/*<status 复合状态*/
 uint8_t AirSpeed;		/*<AirSpeed*/
 uint8_t GroundSpeed;	/*<GroundSpeed*/
 uint8_t GpsStatus;		/*<GpsStatus*/
 uint16_t Voltage;		/*<Voltage*/
 uint8_t gpsNum;		/*<gpsNumgps星数*/
 uint8_t taskType;		/*<taskType*/
 uint8_t gjMode;		/*<gjMode*/
 uint8_t qkRunMode;		/*qkRunMode*/
 uint8_t qkCmdMode;		/*qkCmdMode*/
 uint8_t fkRunMode;		/*fkRunMode*/
 uint8_t fkSysStatus;	/*fkSysStatus*/
 uint32_t jqtbTime;		/*集群时间jqtbTime*/
} mavlink_planeinfo_t;

#pragma pack()
#define MAVLINK_MSG_ID_planeInfo_LEN		33
#define MAVLINK_MSG_ID_planeInfo_MIN_LEN	33
#define MAVLINK_MSG_ID_4_LEN		33
#define MAVLINK_MSG_ID_4_MIN_LEN	33

#define MAVLINK_MSG_ID_planeInfo_CRC	114
#define MAVLINK_MSG_ID_4_CRC			114

#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_planeInfo { \
    4, \
    "planeInfo", \
    21, \
    {  { "Lat", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_planeinfo_t, Lat) }, \
         { "Lng", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_planeinfo_t, Lng) }, \
         { "Alt", NULL, MAVLINK_TYPE_INT16_T, 0, 12, offsetof(mavlink_planeinfo_t, Alt) }, \
         { "Altasl", NULL, MAVLINK_TYPE_INT16_T, 0, 14, offsetof(mavlink_planeinfo_t, Altasl) }, \
         { "Picth", NULL, MAVLINK_TYPE_INT8_T, 0, 18, offsetof(mavlink_planeinfo_t, Picth) }, \
         { "Roll", NULL, MAVLINK_TYPE_INT8_T, 0, 19, offsetof(mavlink_planeinfo_t, Roll) }, \
         { "Yaw", NULL, MAVLINK_TYPE_INT8_T, 0, 20, offsetof(mavlink_planeinfo_t, Yaw) }, \
         { "id", NULL, MAVLINK_TYPE_UINT8_T, 0, 21, offsetof(mavlink_planeinfo_t, id) }, \
         { "status", NULL, MAVLINK_TYPE_UINT8_T, 0, 22, offsetof(mavlink_planeinfo_t, status) }, \
         { "AirSpeed", NULL, MAVLINK_TYPE_UINT8_T, 0, 23, offsetof(mavlink_planeinfo_t, AirSpeed) }, \
         { "GroundSpeed", NULL, MAVLINK_TYPE_UINT8_T, 0, 24, offsetof(mavlink_planeinfo_t, GroundSpeed) }, \
         { "GpsStatus", NULL, MAVLINK_TYPE_UINT8_T, 0, 25, offsetof(mavlink_planeinfo_t, GpsStatus) }, \
         { "Voltage", NULL, MAVLINK_TYPE_UINT16_T, 0, 16, offsetof(mavlink_planeinfo_t, Voltage) }, \
         { "gpsNum", NULL, MAVLINK_TYPE_UINT8_T, 0, 26, offsetof(mavlink_planeinfo_t, gpsNum) }, \
         { "taskType", NULL, MAVLINK_TYPE_UINT8_T, 0, 27, offsetof(mavlink_planeinfo_t, taskType) }, \
         { "gjMode", NULL, MAVLINK_TYPE_UINT8_T, 0, 28, offsetof(mavlink_planeinfo_t, gjMode) }, \
         { "qkRunMode", NULL, MAVLINK_TYPE_UINT8_T, 0, 29, offsetof(mavlink_planeinfo_t, qkRunMode) }, \
         { "qkCmdMode", NULL, MAVLINK_TYPE_UINT8_T, 0, 30, offsetof(mavlink_planeinfo_t, qkCmdMode) }, \
         { "fkRunMode", NULL, MAVLINK_TYPE_UINT8_T, 0, 31, offsetof(mavlink_planeinfo_t, fkRunMode) }, \
         { "fkSysStatus", NULL, MAVLINK_TYPE_UINT8_T, 0, 32, offsetof(mavlink_planeinfo_t, fkSysStatus) }, \
         { "jqtbTime", NULL, MAVLINK_TYPE_UINT32_T, 0, 8, offsetof(mavlink_planeinfo_t, jqtbTime) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_planeInfo {"planeInfo", 21, \
    {    { "Lat", NULL, MAVLINK_TYPE_INT32_T, 0, 0, offsetof(mavlink_planeinfo_t, Lat) }, \
         { "Lng", NULL, MAVLINK_TYPE_INT32_T, 0, 4, offsetof(mavlink_planeinfo_t, Lng) }, \
         { "Alt", NULL, MAVLINK_TYPE_INT16_T, 0, 8, offsetof(mavlink_planeinfo_t, Alt) }, \
         { "Altasl",NULL,MAVLINK_TYPE_INT16_T, 0,10, offsetof(mavlink_planeinfo_t, Altasl) }, \
         { "Picth", NULL, MAVLINK_TYPE_INT8_T, 0,12, offsetof(mavlink_planeinfo_t, Picth) }, \
         { "Roll", NULL, MAVLINK_TYPE_INT8_T, 0, 13, offsetof(mavlink_planeinfo_t, Roll) }, \
         { "Yaw", NULL, MAVLINK_TYPE_INT8_T, 0,  14, offsetof(mavlink_planeinfo_t, Yaw) }, \
         { "id", NULL, MAVLINK_TYPE_UINT8_T, 0,  15, offsetof(mavlink_planeinfo_t, id) }, \
         { "status", NULL, MAVLINK_TYPE_UINT8_T,0,16, offsetof(mavlink_planeinfo_t, status) }, \
         { "AirSpeed", NULL, MAVLINK_TYPE_UINT8_T,0,17, offsetof(mavlink_planeinfo_t, AirSpeed) }, \
         { "GroundSpeed", NULL, MAVLINK_TYPE_UINT8_T,0,18, offsetof(mavlink_planeinfo_t, GroundSpeed) }, \
         { "GpsStatus", NULL, MAVLINK_TYPE_UINT8_T, 0, 19, offsetof(mavlink_planeinfo_t, GpsStatus) }, \
         { "Voltage", NULL, MAVLINK_TYPE_UINT16_T, 0,  20, offsetof(mavlink_planeinfo_t, Voltage) }, \
         { "gpsNum", NULL, MAVLINK_TYPE_UINT8_T, 0,    22, offsetof(mavlink_planeinfo_t, gpsNum) }, \
         { "taskType", NULL, MAVLINK_TYPE_UINT8_T, 0,  23, offsetof(mavlink_planeinfo_t, taskType) }, \
         { "gjMode", NULL, MAVLINK_TYPE_UINT8_T, 0,    24, offsetof(mavlink_planeinfo_t, gjMode) }, \
         { "qkRunMode", NULL, MAVLINK_TYPE_UINT8_T, 0, 25, offsetof(mavlink_planeinfo_t, qkRunMode) }, \
         { "qkCmdMode", NULL, MAVLINK_TYPE_UINT8_T, 0, 26, offsetof(mavlink_planeinfo_t, qkCmdMode) }, \
         { "fkRunMode", NULL, MAVLINK_TYPE_UINT8_T, 0, 27, offsetof(mavlink_planeinfo_t, fkRunMode) }, \
         { "fkSysStatus", NULL, MAVLINK_TYPE_UINT8_T,0,28, offsetof(mavlink_planeinfo_t, fkSysStatus) }, \
         { "jqtbTime", NULL, MAVLINK_TYPE_UINT32_T, 0, 29, offsetof(mavlink_planeinfo_t, jqtbTime) }, \
         } \
}
#endif

static inline uint16_t mavlink_msg_planeinfo_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               int32_t Lat, int32_t Lng, int16_t Alt, int16_t Altasl, int8_t Picth, int8_t Roll, int8_t Yaw, uint8_t id, uint8_t status, uint8_t AirSpeed, uint8_t GroundSpeed, uint8_t GpsStatus, uint16_t Voltage, uint8_t gpsNum, uint8_t taskType, uint8_t gjMode, uint8_t qkRunMode, uint8_t qkCmdMode, uint8_t fkRunMode, uint8_t fkSysStatus, uint32_t jqtbTime)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_planeInfo_LEN];
    _mav_put_int32_t(buf, 0, Lat);
    _mav_put_int32_t(buf, 4, Lng);    
    _mav_put_int16_t(buf, 12, Alt);
    _mav_put_int16_t(buf, 14, Altasl);
    _mav_put_uint16_t(buf, 16, Voltage);
    _mav_put_int8_t(buf, 18, Picth);
    _mav_put_int8_t(buf, 19, Roll);
    _mav_put_int8_t(buf, 20, Yaw);
    _mav_put_uint8_t(buf, 21, id);
    _mav_put_uint8_t(buf, 22, status);
    _mav_put_uint8_t(buf, 23, AirSpeed);
    _mav_put_uint8_t(buf, 24, GroundSpeed);
    _mav_put_uint8_t(buf, 25, GpsStatus);
    _mav_put_uint8_t(buf, 26, gpsNum);
    _mav_put_uint8_t(buf, 27, taskType);
    _mav_put_uint8_t(buf, 28, gjMode);
    _mav_put_uint8_t(buf, 29, qkRunMode);
    _mav_put_uint8_t(buf, 30, qkCmdMode);
    _mav_put_uint8_t(buf, 31, fkRunMode);
    _mav_put_uint8_t(buf, 32, fkSysStatus);
	_mav_put_uint32_t(buf,33, jqtbTime);
    memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_planeInfo_LEN);
#else
    mavlink_planeinfo_t packet;
    packet.Lat = Lat;
    packet.Lng = Lng;
    packet.Alt = Alt;
    packet.Altasl = Altasl;
    packet.Voltage = Voltage;
    packet.Picth = Picth;
    packet.Roll = Roll;
    packet.Yaw = Yaw;
    packet.id = id;
    packet.status = status;
    packet.AirSpeed = AirSpeed;
    packet.GroundSpeed = GroundSpeed;
    packet.GpsStatus = GpsStatus;
    packet.gpsNum = gpsNum;
    packet.taskType = taskType;
    packet.gjMode = gjMode;
    packet.qkRunMode = qkRunMode;
    packet.qkCmdMode = qkCmdMode;
    packet.fkRunMode = fkRunMode;
    packet.fkSysStatus = fkSysStatus;
	packet.jqtbTime = jqtbTime;
    memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_planeInfo_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_planeInfo;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_planeInfo_MIN_LEN, MAVLINK_MSG_ID_planeInfo_LEN, MAVLINK_MSG_ID_planeInfo_CRC);
}

static inline uint16_t mavlink_msg_planeinfo_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               int32_t Lat, int32_t Lng, int16_t Alt, int16_t Altasl, int8_t Picth, int8_t Roll, int8_t Yaw, uint8_t id, uint8_t status, uint8_t AirSpeed, uint8_t GroundSpeed, uint8_t GpsStatus, uint16_t Voltage, uint8_t gpsNum, uint8_t taskType, uint8_t gjMode, uint8_t qkRunMode, uint8_t qkCmdMode, uint8_t fkRunMode, uint8_t fkSysStatus, uint32_t jqtbTime)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_planeInfo_LEN];
    _mav_put_int32_t(buf, 0, Lat);
    _mav_put_int32_t(buf, 4, Lng);
    _mav_put_uint32_t(buf, 8, jqtbTime);
    _mav_put_int16_t(buf, 12, Alt);
    _mav_put_int16_t(buf, 14, Altasl);
    _mav_put_uint16_t(buf, 16, Voltage);
    _mav_put_int8_t(buf, 18, Picth);
    _mav_put_int8_t(buf, 19, Roll);
    _mav_put_int8_t(buf, 20, Yaw);
    _mav_put_uint8_t(buf, 21, id);
    _mav_put_uint8_t(buf, 22, status);
    _mav_put_uint8_t(buf, 23, AirSpeed);
    _mav_put_uint8_t(buf, 24, GroundSpeed);
    _mav_put_uint8_t(buf, 25, GpsStatus);
    _mav_put_uint8_t(buf, 26, gpsNum);
    _mav_put_uint8_t(buf, 27, taskType);
    _mav_put_uint8_t(buf, 28, gjMode);
    _mav_put_uint8_t(buf, 29, qkRunMode);
    _mav_put_uint8_t(buf, 30, qkCmdMode);
    _mav_put_uint8_t(buf, 31, fkRunMode);
    _mav_put_uint8_t(buf, 32, fkSysStatus);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_planeInfo_LEN);
#else
    mavlink_planeinfo_t packet;
    packet.Lat = Lat;
    packet.Lng = Lng;    
    packet.Alt = Alt;
    packet.Altasl = Altasl;
    packet.Voltage = Voltage;
    packet.Picth = Picth;
    packet.Roll = Roll;
    packet.Yaw = Yaw;
    packet.id = id;
    packet.status = status;
    packet.AirSpeed = AirSpeed;
    packet.GroundSpeed = GroundSpeed;
    packet.GpsStatus = GpsStatus;
    packet.gpsNum = gpsNum;
    packet.taskType = taskType;
    packet.gjMode = gjMode;
    packet.qkRunMode = qkRunMode;
    packet.qkCmdMode = qkCmdMode;
    packet.fkRunMode = fkRunMode;
    packet.fkSysStatus = fkSysStatus;
	packet.jqtbTime = jqtbTime;
    memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_planeInfo_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_planeInfo;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_planeInfo_MIN_LEN, MAVLINK_MSG_ID_planeInfo_LEN, MAVLINK_MSG_ID_planeInfo_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_planeInfo_MIN_LEN, MAVLINK_MSG_ID_planeInfo_LEN);
#endif
}

static inline uint16_t mavlink_msg_planeinfo_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                               int32_t Lat,int32_t Lng,int16_t Alt,int16_t Altasl,int8_t Picth,int8_t Roll,int8_t Yaw,uint8_t id,uint8_t status,uint8_t AirSpeed,uint8_t GroundSpeed,uint8_t GpsStatus,uint16_t Voltage,uint8_t gpsNum,uint8_t taskType,uint8_t gjMode,uint8_t qkRunMode,uint8_t qkCmdMode,uint8_t fkRunMode,uint8_t fkSysStatus,uint32_t jqtbTime)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_planeInfo_LEN];
    _mav_put_int32_t(buf, 0, Lat);
    _mav_put_int32_t(buf, 4, Lng);
    _mav_put_uint32_t(buf, 8, jqtbTime);
    _mav_put_int16_t(buf, 12, Alt);
    _mav_put_int16_t(buf, 14, Altasl);
    _mav_put_uint16_t(buf, 16, Voltage);
    _mav_put_int8_t(buf, 18, Picth);
    _mav_put_int8_t(buf, 19, Roll);
    _mav_put_int8_t(buf, 20, Yaw);
    _mav_put_uint8_t(buf, 21, id);
    _mav_put_uint8_t(buf, 22, status);
    _mav_put_uint8_t(buf, 23, AirSpeed);
    _mav_put_uint8_t(buf, 24, GroundSpeed);
    _mav_put_uint8_t(buf, 25, GpsStatus);
    _mav_put_uint8_t(buf, 26, gpsNum);
    _mav_put_uint8_t(buf, 27, taskType);
    _mav_put_uint8_t(buf, 28, gjMode);
    _mav_put_uint8_t(buf, 29, qkRunMode);
    _mav_put_uint8_t(buf, 30, qkCmdMode);
    _mav_put_uint8_t(buf, 31, fkRunMode);
    _mav_put_uint8_t(buf, 32, fkSysStatus);

    memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_planeInfo_LEN);
#else
    mavlink_planeinfo_t packet;
    packet.Lat = Lat;
    packet.Lng = Lng;    
    packet.Alt = Alt;
    packet.Altasl = Altasl;
    packet.Voltage = Voltage;
    packet.Picth = Picth;
    packet.Roll = Roll;
    packet.Yaw = Yaw;
    packet.id = id;
    packet.status = status;
    packet.AirSpeed = AirSpeed;
    packet.GroundSpeed = GroundSpeed;
    packet.GpsStatus = GpsStatus;
    packet.gpsNum = gpsNum;
    packet.taskType = taskType;
    packet.gjMode = gjMode;
    packet.qkRunMode = qkRunMode;
    packet.qkCmdMode = qkCmdMode;
    packet.fkRunMode = fkRunMode;
    packet.fkSysStatus = fkSysStatus;
	packet.jqtbTime = jqtbTime;
    memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_planeInfo_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_planeInfo;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_planeInfo_MIN_LEN, MAVLINK_MSG_ID_planeInfo_LEN, MAVLINK_MSG_ID_planeInfo_CRC);
}

//解读,解码包
static inline uint16_t mavlink_msg_planeinfo_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_planeinfo_t* planeinfo)
{
    return mavlink_msg_planeinfo_pack(system_id, component_id, msg, planeinfo->Lat, planeinfo->Lng, planeinfo->Alt, planeinfo->Altasl, planeinfo->Picth, planeinfo->Roll, planeinfo->Yaw, planeinfo->id, planeinfo->status, planeinfo->AirSpeed, planeinfo->GroundSpeed, planeinfo->GpsStatus, planeinfo->Voltage, planeinfo->gpsNum, planeinfo->taskType, planeinfo->gjMode, planeinfo->qkRunMode, planeinfo->qkCmdMode, planeinfo->fkRunMode, planeinfo->fkSysStatus, planeinfo->jqtbTime);
}

static inline uint16_t mavlink_msg_planeinfo_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_planeinfo_t* planeinfo)
{
    return mavlink_msg_planeinfo_pack_chan(system_id, component_id, chan, msg, planeinfo->Lat, planeinfo->Lng, planeinfo->Alt, planeinfo->Altasl, planeinfo->Picth, planeinfo->Roll, planeinfo->Yaw, planeinfo->id, planeinfo->status, planeinfo->AirSpeed, planeinfo->GroundSpeed, planeinfo->GpsStatus, planeinfo->Voltage, planeinfo->gpsNum, planeinfo->taskType, planeinfo->gjMode, planeinfo->qkRunMode, planeinfo->qkCmdMode, planeinfo->fkRunMode, planeinfo->fkSysStatus, planeinfo->jqtbTime);
}

static inline uint16_t mavlink_msg_planeinfo_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_planeinfo_t* planeinfo)
{
    return mavlink_msg_planeinfo_pack_status(system_id, component_id, _status, msg,  planeinfo->Lat, planeinfo->Lng, planeinfo->Alt, planeinfo->Altasl, planeinfo->Picth, planeinfo->Roll, planeinfo->Yaw, planeinfo->id, planeinfo->status, planeinfo->AirSpeed, planeinfo->GroundSpeed, planeinfo->GpsStatus, planeinfo->Voltage, planeinfo->gpsNum, planeinfo->taskType, planeinfo->gjMode, planeinfo->qkRunMode, planeinfo->qkCmdMode, planeinfo->fkRunMode, planeinfo->fkSysStatus, planeinfo->jqtbTime);
}

/**
 * @brief Send a planeinfo message
 * @param chan MAVLink channel to send the message
 *
 * @param Lat  Lat
 * @param Lng  Lng
 * @param Alt  Alt-id
 * @param Altasl  Altasl-id
 * @param Picth  Picth
 * @param Roll  Roll
 * @param Yaw  Yaw
 * @param id  id
 * @param status  status
 * @param AirSpeed  AirSpeed
 * @param GroundSpeed  GroundSpeed
 * @param GpsStatus  GpsStatus
 * @param Voltage  Voltage
 * @param gpsNum  gpsNum
 * @param taskType  taskType
 * @param gjMode  gjMode
 * @param qkRunMode  qkRunMode
 * @param qkCmdMode  qkCmdMode
 * @param fkRunMode  fkRunMode
 * @param fkSysStatus  fkSysStatus
 * @param jqtbTime  jqtbTime
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_planeinfo_send(mavlink_channel_t chan, int32_t Lat, int32_t Lng, int16_t Alt, int16_t Altasl, int8_t Picth, int8_t Roll, int8_t Yaw, uint8_t id, uint8_t status, uint8_t AirSpeed, uint8_t GroundSpeed, uint8_t GpsStatus, uint16_t Voltage, uint8_t gpsNum, uint8_t taskType, uint8_t gjMode, uint8_t qkRunMode, uint8_t qkCmdMode, uint8_t fkRunMode, uint8_t fkSysStatus, uint32_t jqtbTime)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_planeInfo_LEN];
    _mav_put_int32_t(buf, 0, Lat);
    _mav_put_int32_t(buf, 4, Lng);
    _mav_put_uint32_t(buf, 8, jqtbTime);
    _mav_put_int16_t(buf, 12, Alt);
    _mav_put_int16_t(buf, 14, Altasl);
    _mav_put_uint16_t(buf, 16, Voltage);
    _mav_put_int8_t(buf, 18, Picth);
    _mav_put_int8_t(buf, 19, Roll);
    _mav_put_int8_t(buf, 20, Yaw);
    _mav_put_uint8_t(buf, 21, id);
    _mav_put_uint8_t(buf, 22, status);
    _mav_put_uint8_t(buf, 23, AirSpeed);
    _mav_put_uint8_t(buf, 24, GroundSpeed);
    _mav_put_uint8_t(buf, 25, GpsStatus);
    _mav_put_uint8_t(buf, 26, gpsNum);
    _mav_put_uint8_t(buf, 27, taskType);
    _mav_put_uint8_t(buf, 28, gjMode);
    _mav_put_uint8_t(buf, 29, qkRunMode);
    _mav_put_uint8_t(buf, 30, qkCmdMode);
    _mav_put_uint8_t(buf, 31, fkRunMode);
    _mav_put_uint8_t(buf, 32, fkSysStatus);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_planeInfo, buf, MAVLINK_MSG_ID_planeInfo_MIN_LEN, MAVLINK_MSG_ID_planeInfo_LEN, MAVLINK_MSG_ID_planeInfo_CRC);
#else
    mavlink_planeinfo_t packet;
    packet.Lat = Lat;
    packet.Lng = Lng;
    packet.jqtbTime = jqtbTime;
    packet.Alt = Alt;
    packet.Altasl = Altasl;
    packet.Voltage = Voltage;
    packet.Picth = Picth;
    packet.Roll = Roll;
    packet.Yaw = Yaw;
    packet.id = id;
    packet.status = status;
    packet.AirSpeed = AirSpeed;
    packet.GroundSpeed = GroundSpeed;
    packet.GpsStatus = GpsStatus;
    packet.gpsNum = gpsNum;
    packet.taskType = taskType;
    packet.gjMode = gjMode;
    packet.qkRunMode = qkRunMode;
    packet.qkCmdMode = qkCmdMode;
    packet.fkRunMode = fkRunMode;
    packet.fkSysStatus = fkSysStatus;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_planeInfo, (const char *)&packet, MAVLINK_MSG_ID_planeInfo_MIN_LEN, MAVLINK_MSG_ID_planeInfo_LEN, MAVLINK_MSG_ID_planeInfo_CRC);
#endif
}

/**
 * @brief Send a planeinfo message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_planeinfo_send_struct(mavlink_channel_t chan, const mavlink_planeinfo_t* planeinfo)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_planeinfo_send(chan, planeinfo->Lat, planeinfo->Lng, planeinfo->Alt, planeinfo->Altasl, planeinfo->Picth, planeinfo->Roll, planeinfo->Yaw, planeinfo->id, planeinfo->status, planeinfo->AirSpeed, planeinfo->GroundSpeed, planeinfo->GpsStatus, planeinfo->Voltage, planeinfo->gpsNum, planeinfo->taskType, planeinfo->gjMode, planeinfo->qkRunMode, planeinfo->qkCmdMode, planeinfo->fkRunMode, planeinfo->fkSysStatus, planeinfo->jqtbTime);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_planeInfo, (const char *)planeinfo, MAVLINK_MSG_ID_planeInfo_MIN_LEN, MAVLINK_MSG_ID_planeInfo_LEN, MAVLINK_MSG_ID_planeInfo_CRC);
#endif
}

#if MAVLINK_MSG_ID_planeInfo_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_planeinfo_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  int32_t Lat, int32_t Lng, int16_t Alt, int16_t Altasl, int8_t Picth, int8_t Roll, int8_t Yaw, uint8_t id, uint8_t status, uint8_t AirSpeed, uint8_t GroundSpeed, uint8_t GpsStatus, uint16_t Voltage, uint8_t gpsNum, uint8_t taskType, uint8_t gjMode, uint8_t qkRunMode, uint8_t qkCmdMode, uint8_t fkRunMode, uint8_t fkSysStatus, uint32_t jqtbTime)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_int32_t(buf, 0, Lat);
    _mav_put_int32_t(buf, 4, Lng);
    _mav_put_uint32_t(buf, 8, jqtbTime);
    _mav_put_int16_t(buf, 12, Alt);
    _mav_put_int16_t(buf, 14, Altasl);
    _mav_put_uint16_t(buf, 16, Voltage);
    _mav_put_int8_t(buf, 18, Picth);
    _mav_put_int8_t(buf, 19, Roll);
    _mav_put_int8_t(buf, 20, Yaw);
    _mav_put_uint8_t(buf, 21, id);
    _mav_put_uint8_t(buf, 22, status);
    _mav_put_uint8_t(buf, 23, AirSpeed);
    _mav_put_uint8_t(buf, 24, GroundSpeed);
    _mav_put_uint8_t(buf, 25, GpsStatus);
    _mav_put_uint8_t(buf, 26, gpsNum);
    _mav_put_uint8_t(buf, 27, taskType);
    _mav_put_uint8_t(buf, 28, gjMode);
    _mav_put_uint8_t(buf, 29, qkRunMode);
    _mav_put_uint8_t(buf, 30, qkCmdMode);
    _mav_put_uint8_t(buf, 31, fkRunMode);
    _mav_put_uint8_t(buf, 32, fkSysStatus);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_planeInfo, buf, MAVLINK_MSG_ID_planeInfo_MIN_LEN, MAVLINK_MSG_ID_planeInfo_LEN, MAVLINK_MSG_ID_planeInfo_CRC);
#else
    mavlink_planeinfo_t *packet = (mavlink_planeinfo_t *)msgbuf;
    packet->Lat = Lat;
    packet->Lng = Lng;
    packet->jqtbTime = jqtbTime;
    packet->Alt = Alt;
    packet->Altasl = Altasl;
    packet->Voltage = Voltage;
    packet->Picth = Picth;
    packet->Roll = Roll;
    packet->Yaw = Yaw;
    packet->id = id;
    packet->status = status;
    packet->AirSpeed = AirSpeed;
    packet->GroundSpeed = GroundSpeed;
    packet->GpsStatus = GpsStatus;
    packet->gpsNum = gpsNum;
    packet->taskType = taskType;
    packet->gjMode = gjMode;
    packet->qkRunMode = qkRunMode;
    packet->qkCmdMode = qkCmdMode;
    packet->fkRunMode = fkRunMode;
    packet->fkSysStatus = fkSysStatus;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_planeInfo, (const char *)packet, MAVLINK_MSG_ID_planeInfo_MIN_LEN, MAVLINK_MSG_ID_planeInfo_LEN, MAVLINK_MSG_ID_planeInfo_CRC);
#endif
}
#endif

#endif

// MESSAGE planeInfo UNPACKING
static inline int32_t mavlink_msg_planeinfo_get_Lat(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  0);
}

static inline int32_t mavlink_msg_planeinfo_get_Lng(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int32_t(msg,  4);
}

static inline int16_t mavlink_msg_planeinfo_get_Alt(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  12);
}

/**
 * @brief Get field Altasl from planeinfo message
 *
 * @return  Altasl-id
 */
static inline int16_t mavlink_msg_planeinfo_get_Altasl(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int16_t(msg,  14);
}

static inline int8_t mavlink_msg_planeinfo_get_Picth(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int8_t(msg,  18);
}

static inline int8_t mavlink_msg_planeinfo_get_Roll(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int8_t(msg,  19);
}

static inline int8_t mavlink_msg_planeinfo_get_Yaw(const mavlink_message_t* msg)
{
    return _MAV_RETURN_int8_t(msg,  20);
}

static inline uint8_t mavlink_msg_planeinfo_get_id(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  21);
}

static inline uint8_t mavlink_msg_planeinfo_get_status(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  22);
}

static inline uint8_t mavlink_msg_planeinfo_get_AirSpeed(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  23);
}

static inline uint8_t mavlink_msg_planeinfo_get_GroundSpeed(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  24);
}

static inline uint8_t mavlink_msg_planeinfo_get_GpsStatus(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  25);
}

static inline uint16_t mavlink_msg_planeinfo_get_Voltage(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  16);
}

static inline uint8_t mavlink_msg_planeinfo_get_gpsNum(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  26);
}

/**
 * @brief Get field taskType from planeinfo message
 *
 * @return  taskType
 */
static inline uint8_t mavlink_msg_planeinfo_get_taskType(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  27);
}

/** @brief Get field gjMode from planeinfo message
 * @return  gjMode*/
static inline uint8_t mavlink_msg_planeinfo_get_gjMode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  28);
}

/** @brief Get field qkRunMode from planeinfo message
 * @return  qkRunMode*/
static inline uint8_t mavlink_msg_planeinfo_get_qkRunMode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  29);
}

/** @brief Get field qkCmdMode from planeinfo message
 * @return  qkCmdMode */
static inline uint8_t mavlink_msg_planeinfo_get_qkCmdMode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  30);
}

/** @brief Get field fkRunMode from planeinfo message
 * @return  fkRunMode*/
static inline uint8_t mavlink_msg_planeinfo_get_fkRunMode(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  31);
}

/** @brief Get field fkSysStatus from planeinfo message
 * @return  fkSysStatus */
static inline uint8_t mavlink_msg_planeinfo_get_fkSysStatus(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  32);
}

/** * @brief Get field jqtbTime from planeinfo message
 * @return  jqtbTime */
static inline uint32_t mavlink_msg_planeinfo_get_jqtbTime(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  8);
}

/** * @brief Decode a planeinfo message into a struct
 *
 * @param msg The message to decode
 * @param planeinfo C-struct to decode the message contents into */
static inline void mavlink_msg_planeinfo_decode(const mavlink_message_t* msg, mavlink_planeinfo_t* planeinfo){
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    planeinfo->Lat = mavlink_msg_planeinfo_get_Lat(msg);
    planeinfo->Lng = mavlink_msg_planeinfo_get_Lng(msg);
    planeinfo->jqtbTime = mavlink_msg_planeinfo_get_jqtbTime(msg);
    planeinfo->Alt = mavlink_msg_planeinfo_get_Alt(msg);
    planeinfo->Altasl = mavlink_msg_planeinfo_get_Altasl(msg);
    planeinfo->Voltage = mavlink_msg_planeinfo_get_Voltage(msg);
    planeinfo->Picth = mavlink_msg_planeinfo_get_Picth(msg);
    planeinfo->Roll = mavlink_msg_planeinfo_get_Roll(msg);
    planeinfo->Yaw = mavlink_msg_planeinfo_get_Yaw(msg);
    planeinfo->id = mavlink_msg_planeinfo_get_id(msg);
    planeinfo->status = mavlink_msg_planeinfo_get_status(msg);
    planeinfo->AirSpeed = mavlink_msg_planeinfo_get_AirSpeed(msg);
    planeinfo->GroundSpeed = mavlink_msg_planeinfo_get_GroundSpeed(msg);
    planeinfo->GpsStatus = mavlink_msg_planeinfo_get_GpsStatus(msg);
    planeinfo->gpsNum = mavlink_msg_planeinfo_get_gpsNum(msg);
    planeinfo->taskType = mavlink_msg_planeinfo_get_taskType(msg);
    planeinfo->gjMode = mavlink_msg_planeinfo_get_gjMode(msg);
    planeinfo->qkRunMode = mavlink_msg_planeinfo_get_qkRunMode(msg);
    planeinfo->qkCmdMode = mavlink_msg_planeinfo_get_qkCmdMode(msg);
    planeinfo->fkRunMode = mavlink_msg_planeinfo_get_fkRunMode(msg);
    planeinfo->fkSysStatus = mavlink_msg_planeinfo_get_fkSysStatus(msg);
#else
    //uint8_t len = msg->len < MAVLINK_MSG_ID_planeInfo_LEN? msg->len : MAVLINK_MSG_ID_planeInfo_LEN;
    //memset(planeinfo, 0, MAVLINK_MSG_ID_planeInfo_LEN);
    memcpy(planeinfo, _MAV_PAYLOAD(msg), MAVLINK_MSG_ID_planeInfo_LEN);
	//memcpy(planeinfo, _MAV_PAYLOAD(msg), MAVLINK_MSG_ID_planeInfo_LEN+2);
#endif
}
