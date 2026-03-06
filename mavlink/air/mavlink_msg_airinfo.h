#pragma once
// MESSAGE airInfo PACKING

#define MAVLINK_MSG_ID_airInfo 62


typedef struct __mavlink_airinfo_t {
 uint32_t y; /*<  y*/
 uint32_t x; /*<  x*/
 uint16_t objectID; /*<  id*/
 uint16_t unitID; /*<  unit-id*/
 uint16_t planeID; /*<  plane-id*/
 uint16_t groupID; /*<  group-id*/
 uint16_t z; /*<  height*/
 uint16_t count; /*<  count*/
 uint8_t type; /*<  dzType*/
 uint8_t status; /*<  status*/
} mavlink_airinfo_t;

#define MAVLINK_MSG_ID_airInfo_LEN 22
#define MAVLINK_MSG_ID_airInfo_MIN_LEN 22
#define MAVLINK_MSG_ID_62_LEN 22
#define MAVLINK_MSG_ID_62_MIN_LEN 22

#define MAVLINK_MSG_ID_airInfo_CRC 236
#define MAVLINK_MSG_ID_62_CRC 236



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_airInfo { \
    62, \
    "airInfo", \
    10, \
    {  { "objectID", NULL, MAVLINK_TYPE_UINT16_T, 0, 8, offsetof(mavlink_airinfo_t, objectID) }, \
         { "type", NULL, MAVLINK_TYPE_UINT8_T, 0, 20, offsetof(mavlink_airinfo_t, type) }, \
         { "unitID", NULL, MAVLINK_TYPE_UINT16_T, 0, 10, offsetof(mavlink_airinfo_t, unitID) }, \
         { "planeID", NULL, MAVLINK_TYPE_UINT16_T, 0, 12, offsetof(mavlink_airinfo_t, planeID) }, \
         { "groupID", NULL, MAVLINK_TYPE_UINT16_T, 0, 14, offsetof(mavlink_airinfo_t, groupID) }, \
         { "y", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_airinfo_t, y) }, \
         { "x", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_airinfo_t, x) }, \
         { "z", NULL, MAVLINK_TYPE_UINT16_T, 0, 16, offsetof(mavlink_airinfo_t, z) }, \
         { "count", NULL, MAVLINK_TYPE_UINT16_T, 0, 18, offsetof(mavlink_airinfo_t, count) }, \
         { "status", NULL, MAVLINK_TYPE_UINT8_T, 0, 21, offsetof(mavlink_airinfo_t, status) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_airInfo { \
    "airInfo", \
    10, \
    {  { "objectID", NULL, MAVLINK_TYPE_UINT16_T, 0, 8, offsetof(mavlink_airinfo_t, objectID) }, \
         { "type", NULL, MAVLINK_TYPE_UINT8_T, 0, 20, offsetof(mavlink_airinfo_t, type) }, \
         { "unitID", NULL, MAVLINK_TYPE_UINT16_T, 0, 10, offsetof(mavlink_airinfo_t, unitID) }, \
         { "planeID", NULL, MAVLINK_TYPE_UINT16_T, 0, 12, offsetof(mavlink_airinfo_t, planeID) }, \
         { "groupID", NULL, MAVLINK_TYPE_UINT16_T, 0, 14, offsetof(mavlink_airinfo_t, groupID) }, \
         { "y", NULL, MAVLINK_TYPE_UINT32_T, 0, 0, offsetof(mavlink_airinfo_t, y) }, \
         { "x", NULL, MAVLINK_TYPE_UINT32_T, 0, 4, offsetof(mavlink_airinfo_t, x) }, \
         { "z", NULL, MAVLINK_TYPE_UINT16_T, 0, 16, offsetof(mavlink_airinfo_t, z) }, \
         { "count", NULL, MAVLINK_TYPE_UINT16_T, 0, 18, offsetof(mavlink_airinfo_t, count) }, \
         { "status", NULL, MAVLINK_TYPE_UINT8_T, 0, 21, offsetof(mavlink_airinfo_t, status) }, \
         } \
}
#endif

/**
 * @brief Pack a airinfo message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param objectID  id
 * @param type  dzType
 * @param unitID  unit-id
 * @param planeID  plane-id
 * @param groupID  group-id
 * @param y  y
 * @param x  x
 * @param z  height
 * @param count  count
 * @param status  status
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_airinfo_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               uint16_t objectID, uint8_t type, uint16_t unitID, uint16_t planeID, uint16_t groupID, uint32_t y, uint32_t x, uint16_t z, uint16_t count, uint8_t status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_airInfo_LEN];
    _mav_put_uint32_t(buf, 0, y);
    _mav_put_uint32_t(buf, 4, x);
    _mav_put_uint16_t(buf, 8, objectID);
    _mav_put_uint16_t(buf, 10, unitID);
    _mav_put_uint16_t(buf, 12, planeID);
    _mav_put_uint16_t(buf, 14, groupID);
    _mav_put_uint16_t(buf, 16, z);
    _mav_put_uint16_t(buf, 18, count);
    _mav_put_uint8_t(buf, 20, type);
    _mav_put_uint8_t(buf, 21, status);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_airInfo_LEN);
#else
    mavlink_airinfo_t packet;
    packet.y = y;
    packet.x = x;
    packet.objectID = objectID;
    packet.unitID = unitID;
    packet.planeID = planeID;
    packet.groupID = groupID;
    packet.z = z;
    packet.count = count;
    packet.type = type;
    packet.status = status;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_airInfo_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_airInfo;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_airInfo_MIN_LEN, MAVLINK_MSG_ID_airInfo_LEN, MAVLINK_MSG_ID_airInfo_CRC);
}

/**
 * @brief Pack a airinfo message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 *
 * @param objectID  id
 * @param type  dzType
 * @param unitID  unit-id
 * @param planeID  plane-id
 * @param groupID  group-id
 * @param y  y
 * @param x  x
 * @param z  height
 * @param count  count
 * @param status  status
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_airinfo_pack_status(uint8_t system_id, uint8_t component_id, mavlink_status_t *_status, mavlink_message_t* msg,
                               uint16_t objectID, uint8_t type, uint16_t unitID, uint16_t planeID, uint16_t groupID, uint32_t y, uint32_t x, uint16_t z, uint16_t count, uint8_t status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_airInfo_LEN];
    _mav_put_uint32_t(buf, 0, y);
    _mav_put_uint32_t(buf, 4, x);
    _mav_put_uint16_t(buf, 8, objectID);
    _mav_put_uint16_t(buf, 10, unitID);
    _mav_put_uint16_t(buf, 12, planeID);
    _mav_put_uint16_t(buf, 14, groupID);
    _mav_put_uint16_t(buf, 16, z);
    _mav_put_uint16_t(buf, 18, count);
    _mav_put_uint8_t(buf, 20, type);
    _mav_put_uint8_t(buf, 21, status);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_airInfo_LEN);
#else
    mavlink_airinfo_t packet;
    packet.y = y;
    packet.x = x;
    packet.objectID = objectID;
    packet.unitID = unitID;
    packet.planeID = planeID;
    packet.groupID = groupID;
    packet.z = z;
    packet.count = count;
    packet.type = type;
    packet.status = status;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_airInfo_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_airInfo;
#if MAVLINK_CRC_EXTRA
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_airInfo_MIN_LEN, MAVLINK_MSG_ID_airInfo_LEN, MAVLINK_MSG_ID_airInfo_CRC);
#else
    return mavlink_finalize_message_buffer(msg, system_id, component_id, _status, MAVLINK_MSG_ID_airInfo_MIN_LEN, MAVLINK_MSG_ID_airInfo_LEN);
#endif
}

/**
 * @brief Pack a airinfo message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param objectID  id
 * @param type  dzType
 * @param unitID  unit-id
 * @param planeID  plane-id
 * @param groupID  group-id
 * @param y  y
 * @param x  x
 * @param z  height
 * @param count  count
 * @param status  status
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_airinfo_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   uint16_t objectID,uint8_t type,uint16_t unitID,uint16_t planeID,uint16_t groupID,uint32_t y,uint32_t x,uint16_t z,uint16_t count,uint8_t status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_airInfo_LEN];
    _mav_put_uint32_t(buf, 0, y);
    _mav_put_uint32_t(buf, 4, x);
    _mav_put_uint16_t(buf, 8, objectID);
    _mav_put_uint16_t(buf, 10, unitID);
    _mav_put_uint16_t(buf, 12, planeID);
    _mav_put_uint16_t(buf, 14, groupID);
    _mav_put_uint16_t(buf, 16, z);
    _mav_put_uint16_t(buf, 18, count);
    _mav_put_uint8_t(buf, 20, type);
    _mav_put_uint8_t(buf, 21, status);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_airInfo_LEN);
#else
    mavlink_airinfo_t packet;
    packet.y = y;
    packet.x = x;
    packet.objectID = objectID;
    packet.unitID = unitID;
    packet.planeID = planeID;
    packet.groupID = groupID;
    packet.z = z;
    packet.count = count;
    packet.type = type;
    packet.status = status;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_airInfo_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_airInfo;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_airInfo_MIN_LEN, MAVLINK_MSG_ID_airInfo_LEN, MAVLINK_MSG_ID_airInfo_CRC);
}

/**
 * @brief Encode a airinfo struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param airinfo C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_airinfo_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_airinfo_t* airinfo)
{
    return mavlink_msg_airinfo_pack(system_id, component_id, msg, airinfo->objectID, airinfo->type, airinfo->unitID, airinfo->planeID, airinfo->groupID, airinfo->y, airinfo->x, airinfo->z, airinfo->count, airinfo->status);
}

/**
 * @brief Encode a airinfo struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param airinfo C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_airinfo_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_airinfo_t* airinfo)
{
    return mavlink_msg_airinfo_pack_chan(system_id, component_id, chan, msg, airinfo->objectID, airinfo->type, airinfo->unitID, airinfo->planeID, airinfo->groupID, airinfo->y, airinfo->x, airinfo->z, airinfo->count, airinfo->status);
}

/**
 * @brief Encode a airinfo struct with provided status structure
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param status MAVLink status structure
 * @param msg The MAVLink message to compress the data into
 * @param airinfo C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_airinfo_encode_status(uint8_t system_id, uint8_t component_id, mavlink_status_t* _status, mavlink_message_t* msg, const mavlink_airinfo_t* airinfo)
{
    return mavlink_msg_airinfo_pack_status(system_id, component_id, _status, msg,  airinfo->objectID, airinfo->type, airinfo->unitID, airinfo->planeID, airinfo->groupID, airinfo->y, airinfo->x, airinfo->z, airinfo->count, airinfo->status);
}

/**
 * @brief Send a airinfo message
 * @param chan MAVLink channel to send the message
 *
 * @param objectID  id
 * @param type  dzType
 * @param unitID  unit-id
 * @param planeID  plane-id
 * @param groupID  group-id
 * @param y  y
 * @param x  x
 * @param z  height
 * @param count  count
 * @param status  status
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_airinfo_send(mavlink_channel_t chan, uint16_t objectID, uint8_t type, uint16_t unitID, uint16_t planeID, uint16_t groupID, uint32_t y, uint32_t x, uint16_t z, uint16_t count, uint8_t status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_airInfo_LEN];
    _mav_put_uint32_t(buf, 0, y);
    _mav_put_uint32_t(buf, 4, x);
    _mav_put_uint16_t(buf, 8, objectID);
    _mav_put_uint16_t(buf, 10, unitID);
    _mav_put_uint16_t(buf, 12, planeID);
    _mav_put_uint16_t(buf, 14, groupID);
    _mav_put_uint16_t(buf, 16, z);
    _mav_put_uint16_t(buf, 18, count);
    _mav_put_uint8_t(buf, 20, type);
    _mav_put_uint8_t(buf, 21, status);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_airInfo, buf, MAVLINK_MSG_ID_airInfo_MIN_LEN, MAVLINK_MSG_ID_airInfo_LEN, MAVLINK_MSG_ID_airInfo_CRC);
#else
    mavlink_airinfo_t packet;
    packet.y = y;
    packet.x = x;
    packet.objectID = objectID;
    packet.unitID = unitID;
    packet.planeID = planeID;
    packet.groupID = groupID;
    packet.z = z;
    packet.count = count;
    packet.type = type;
    packet.status = status;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_airInfo, (const char *)&packet, MAVLINK_MSG_ID_airInfo_MIN_LEN, MAVLINK_MSG_ID_airInfo_LEN, MAVLINK_MSG_ID_airInfo_CRC);
#endif
}

/**
 * @brief Send a airinfo message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_airinfo_send_struct(mavlink_channel_t chan, const mavlink_airinfo_t* airinfo)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_airinfo_send(chan, airinfo->objectID, airinfo->type, airinfo->unitID, airinfo->planeID, airinfo->groupID, airinfo->y, airinfo->x, airinfo->z, airinfo->count, airinfo->status);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_airInfo, (const char *)airinfo, MAVLINK_MSG_ID_airInfo_MIN_LEN, MAVLINK_MSG_ID_airInfo_LEN, MAVLINK_MSG_ID_airInfo_CRC);
#endif
}

#if MAVLINK_MSG_ID_airInfo_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_airinfo_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  uint16_t objectID, uint8_t type, uint16_t unitID, uint16_t planeID, uint16_t groupID, uint32_t y, uint32_t x, uint16_t z, uint16_t count, uint8_t status)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_uint32_t(buf, 0, y);
    _mav_put_uint32_t(buf, 4, x);
    _mav_put_uint16_t(buf, 8, objectID);
    _mav_put_uint16_t(buf, 10, unitID);
    _mav_put_uint16_t(buf, 12, planeID);
    _mav_put_uint16_t(buf, 14, groupID);
    _mav_put_uint16_t(buf, 16, z);
    _mav_put_uint16_t(buf, 18, count);
    _mav_put_uint8_t(buf, 20, type);
    _mav_put_uint8_t(buf, 21, status);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_airInfo, buf, MAVLINK_MSG_ID_airInfo_MIN_LEN, MAVLINK_MSG_ID_airInfo_LEN, MAVLINK_MSG_ID_airInfo_CRC);
#else
    mavlink_airinfo_t *packet = (mavlink_airinfo_t *)msgbuf;
    packet->y = y;
    packet->x = x;
    packet->objectID = objectID;
    packet->unitID = unitID;
    packet->planeID = planeID;
    packet->groupID = groupID;
    packet->z = z;
    packet->count = count;
    packet->type = type;
    packet->status = status;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_airInfo, (const char *)packet, MAVLINK_MSG_ID_airInfo_MIN_LEN, MAVLINK_MSG_ID_airInfo_LEN, MAVLINK_MSG_ID_airInfo_CRC);
#endif
}
#endif

#endif

// MESSAGE airInfo UNPACKING


/**
 * @brief Get field objectID from airinfo message
 *
 * @return  id
 */
static inline uint16_t mavlink_msg_airinfo_get_objectID(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  8);
}

/**
 * @brief Get field type from airinfo message
 *
 * @return  dzType
 */
static inline uint8_t mavlink_msg_airinfo_get_type(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  20);
}

/**
 * @brief Get field unitID from airinfo message
 *
 * @return  unit-id
 */
static inline uint16_t mavlink_msg_airinfo_get_unitID(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  10);
}

/**
 * @brief Get field planeID from airinfo message
 *
 * @return  plane-id
 */
static inline uint16_t mavlink_msg_airinfo_get_planeID(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  12);
}

/**
 * @brief Get field groupID from airinfo message
 *
 * @return  group-id
 */
static inline uint16_t mavlink_msg_airinfo_get_groupID(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  14);
}

/**
 * @brief Get field y from airinfo message
 *
 * @return  y
 */
static inline uint32_t mavlink_msg_airinfo_get_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  0);
}

/**
 * @brief Get field x from airinfo message
 *
 * @return  x
 */
static inline uint32_t mavlink_msg_airinfo_get_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint32_t(msg,  4);
}

/**
 * @brief Get field z from airinfo message
 *
 * @return  height
 */
static inline uint16_t mavlink_msg_airinfo_get_z(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  16);
}

/**
 * @brief Get field count from airinfo message
 *
 * @return  count
 */
static inline uint16_t mavlink_msg_airinfo_get_count(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint16_t(msg,  18);
}

/**
 * @brief Get field status from airinfo message
 *
 * @return  status
 */
static inline uint8_t mavlink_msg_airinfo_get_status(const mavlink_message_t* msg)
{
    return _MAV_RETURN_uint8_t(msg,  21);
}

/**
 * @brief Decode a airinfo message into a struct
 *
 * @param msg The message to decode
 * @param airinfo C-struct to decode the message contents into
 */
static inline void mavlink_msg_airinfo_decode(const mavlink_message_t* msg, mavlink_airinfo_t* airinfo)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    airinfo->y = mavlink_msg_airinfo_get_y(msg);
    airinfo->x = mavlink_msg_airinfo_get_x(msg);
    airinfo->objectID = mavlink_msg_airinfo_get_objectID(msg);
    airinfo->unitID = mavlink_msg_airinfo_get_unitID(msg);
    airinfo->planeID = mavlink_msg_airinfo_get_planeID(msg);
    airinfo->groupID = mavlink_msg_airinfo_get_groupID(msg);
    airinfo->z = mavlink_msg_airinfo_get_z(msg);
    airinfo->count = mavlink_msg_airinfo_get_count(msg);
    airinfo->type = mavlink_msg_airinfo_get_type(msg);
    airinfo->status = mavlink_msg_airinfo_get_status(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_airInfo_LEN? msg->len : MAVLINK_MSG_ID_airInfo_LEN;
        memset(airinfo, 0, MAVLINK_MSG_ID_airInfo_LEN);
    memcpy(airinfo, _MAV_PAYLOAD(msg), len);
#endif
}
