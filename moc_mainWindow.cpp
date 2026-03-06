/****************************************************************************
** Meta object code from reading C++ file 'mainWindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "./mainWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[134];
    char stringdata0[1733];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 11), // "FixPlaneMsg"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 17), // "tag_PlaneMessage*"
QT_MOC_LITERAL(4, 42, 19), // "receviePlaneMessage"
QT_MOC_LITERAL(5, 62, 10), // "QgsPointXY"
QT_MOC_LITERAL(6, 73, 8), // "qgsPoint"
QT_MOC_LITERAL(7, 82, 12), // "recevieImage"
QT_MOC_LITERAL(8, 95, 5), // "array"
QT_MOC_LITERAL(9, 101, 18), // "AddJpgAnnotaionMsg"
QT_MOC_LITERAL(10, 120, 9), // "Mappoint*"
QT_MOC_LITERAL(11, 130, 5), // "width"
QT_MOC_LITERAL(12, 136, 6), // "height"
QT_MOC_LITERAL(13, 143, 1), // "a"
QT_MOC_LITERAL(14, 145, 14), // "showMousePoint"
QT_MOC_LITERAL(15, 160, 1), // "p"
QT_MOC_LITERAL(16, 162, 12), // "scaleChanged"
QT_MOC_LITERAL(17, 175, 17), // "UdpSockerRecvData"
QT_MOC_LITERAL(18, 193, 8), // "AddMenu1"
QT_MOC_LITERAL(19, 202, 8), // "AddMenu2"
QT_MOC_LITERAL(20, 211, 8), // "AddMenu3"
QT_MOC_LITERAL(21, 220, 8), // "AddMenu4"
QT_MOC_LITERAL(22, 229, 8), // "AddMenu5"
QT_MOC_LITERAL(23, 238, 12), // "RadarTouying"
QT_MOC_LITERAL(24, 251, 16), // "selectRadarLayer"
QT_MOC_LITERAL(25, 268, 17), // "selectRadarDevice"
QT_MOC_LITERAL(26, 286, 1), // "s"
QT_MOC_LITERAL(27, 288, 15), // "moveRadarDevice"
QT_MOC_LITERAL(28, 304, 17), // "deleteRadarDevice"
QT_MOC_LITERAL(29, 322, 14), // "fixRadarDevice"
QT_MOC_LITERAL(30, 337, 10), // "OutfitMove"
QT_MOC_LITERAL(31, 348, 14), // "timer1_timeout"
QT_MOC_LITERAL(32, 363, 22), // "onAirLayerRefreshTimer"
QT_MOC_LITERAL(33, 386, 22), // "processAllPlaneUpdates"
QT_MOC_LITERAL(34, 409, 16), // "RadarTestAirList"
QT_MOC_LITERAL(35, 426, 15), // "SelectTaskLayer"
QT_MOC_LITERAL(36, 442, 14), // "AddAirTaskArea"
QT_MOC_LITERAL(37, 457, 14), // "SelectTaskArea"
QT_MOC_LITERAL(38, 472, 11), // "FixTaskArea"
QT_MOC_LITERAL(39, 484, 12), // "MoveTaskArea"
QT_MOC_LITERAL(40, 497, 14), // "DeleteTaskArea"
QT_MOC_LITERAL(41, 512, 15), // "ShowRadarNoArea"
QT_MOC_LITERAL(42, 528, 13), // "AreaRadarList"
QT_MOC_LITERAL(43, 542, 10), // "TaskGuihua"
QT_MOC_LITERAL(44, 553, 15), // "addVectorlayers"
QT_MOC_LITERAL(45, 569, 15), // "addRasterlayers"
QT_MOC_LITERAL(46, 585, 8), // "pathname"
QT_MOC_LITERAL(47, 594, 9), // "layername"
QT_MOC_LITERAL(48, 604, 16), // "editVectorSymbol"
QT_MOC_LITERAL(49, 621, 12), // "settingLayer"
QT_MOC_LITERAL(50, 634, 16), // "equipmentManager"
QT_MOC_LITERAL(51, 651, 25), // "hideLittleSelectedPicture"
QT_MOC_LITERAL(52, 677, 14), // "hideBigPicture"
QT_MOC_LITERAL(53, 692, 16), // "hideSmallPicture"
QT_MOC_LITERAL(54, 709, 9), // "hideTable"
QT_MOC_LITERAL(55, 719, 16), // "startStopReceive"
QT_MOC_LITERAL(56, 736, 10), // "connectUDP"
QT_MOC_LITERAL(57, 747, 9), // "RefreshGj"
QT_MOC_LITERAL(58, 757, 11), // "toggleTrack"
QT_MOC_LITERAL(59, 769, 15), // "clearTrajectory"
QT_MOC_LITERAL(60, 785, 14), // "showMapManager"
QT_MOC_LITERAL(61, 800, 13), // "switchBaseMap"
QT_MOC_LITERAL(62, 814, 8), // "filePath"
QT_MOC_LITERAL(63, 823, 9), // "layerName"
QT_MOC_LITERAL(64, 833, 17), // "loadOnlineTileMap"
QT_MOC_LITERAL(65, 851, 3), // "url"
QT_MOC_LITERAL(66, 855, 8), // "moveView"
QT_MOC_LITERAL(67, 864, 12), // "addDrawPoint"
QT_MOC_LITERAL(68, 877, 11), // "addDrawLine"
QT_MOC_LITERAL(69, 889, 14), // "addDrawPolygon"
QT_MOC_LITERAL(70, 904, 16), // "drawCircle2point"
QT_MOC_LITERAL(71, 921, 11), // "saveToLayer"
QT_MOC_LITERAL(72, 933, 8), // "editLine"
QT_MOC_LITERAL(73, 942, 11), // "editPolygon"
QT_MOC_LITERAL(74, 954, 14), // "deleteSelected"
QT_MOC_LITERAL(75, 969, 12), // "QgsMapLayer*"
QT_MOC_LITERAL(76, 982, 5), // "layer"
QT_MOC_LITERAL(77, 988, 8), // "QWidget*"
QT_MOC_LITERAL(78, 997, 6), // "parent"
QT_MOC_LITERAL(79, 1004, 20), // "checkFeaturesVisible"
QT_MOC_LITERAL(80, 1025, 14), // "modifyFeatures"
QT_MOC_LITERAL(81, 1040, 11), // "selectPoint"
QT_MOC_LITERAL(82, 1052, 10), // "selectLine"
QT_MOC_LITERAL(83, 1063, 13), // "selectPolygon"
QT_MOC_LITERAL(84, 1077, 13), // "selectPicture"
QT_MOC_LITERAL(85, 1091, 13), // "measureLength"
QT_MOC_LITERAL(86, 1105, 11), // "measureArea"
QT_MOC_LITERAL(87, 1117, 8), // "testMenu"
QT_MOC_LITERAL(88, 1126, 8), // "dataBack"
QT_MOC_LITERAL(89, 1135, 10), // "JiqunTuili"
QT_MOC_LITERAL(90, 1146, 16), // "show2Dor3Dwindow"
QT_MOC_LITERAL(91, 1163, 17), // "switchMapViewMode"
QT_MOC_LITERAL(92, 1181, 4), // "mode"
QT_MOC_LITERAL(93, 1186, 13), // "getPlaneCount"
QT_MOC_LITERAL(94, 1200, 10), // "createGrid"
QT_MOC_LITERAL(95, 1211, 13), // "sendBundiance"
QT_MOC_LITERAL(96, 1225, 12), // "ScanPathPlan"
QT_MOC_LITERAL(97, 1238, 14), // "lookDialogShow"
QT_MOC_LITERAL(98, 1253, 9), // "mapMiddle"
QT_MOC_LITERAL(99, 1263, 15), // "addJpgAnnotaion"
QT_MOC_LITERAL(100, 1279, 7), // "leftupY"
QT_MOC_LITERAL(101, 1287, 7), // "leftupX"
QT_MOC_LITERAL(102, 1295, 8), // "rightupY"
QT_MOC_LITERAL(103, 1304, 8), // "rightupX"
QT_MOC_LITERAL(104, 1313, 9), // "leftdownY"
QT_MOC_LITERAL(105, 1323, 9), // "leftdownX"
QT_MOC_LITERAL(106, 1333, 10), // "rightdownY"
QT_MOC_LITERAL(107, 1344, 10), // "rightdownX"
QT_MOC_LITERAL(108, 1355, 8), // "filename"
QT_MOC_LITERAL(109, 1364, 22), // "addJpgAnnotaionCenter1"
QT_MOC_LITERAL(110, 1387, 8), // "mappoint"
QT_MOC_LITERAL(111, 1396, 9), // "hidetable"
QT_MOC_LITERAL(112, 1406, 29), // "udpRecieveEleticMagnetismInit"
QT_MOC_LITERAL(113, 1436, 30), // "processEleticMagnetismDatagram"
QT_MOC_LITERAL(114, 1467, 23), // "selectElecticMangnetism"
QT_MOC_LITERAL(115, 1491, 9), // "read_data"
QT_MOC_LITERAL(116, 1501, 11), // "RecevieLook"
QT_MOC_LITERAL(117, 1513, 15), // "addToRightTable"
QT_MOC_LITERAL(118, 1529, 12), // "updateSqlite"
QT_MOC_LITERAL(119, 1542, 21), // "updateSqliteBoundance"
QT_MOC_LITERAL(120, 1564, 17), // "annotationCreated"
QT_MOC_LITERAL(121, 1582, 14), // "QgsAnnotation*"
QT_MOC_LITERAL(122, 1597, 10), // "annotation"
QT_MOC_LITERAL(123, 1608, 22), // "showJpgAnnotaionWindow"
QT_MOC_LITERAL(124, 1631, 7), // "QPoint&"
QT_MOC_LITERAL(125, 1639, 3), // "pos"
QT_MOC_LITERAL(126, 1643, 4), // "show"
QT_MOC_LITERAL(127, 1648, 5), // "point"
QT_MOC_LITERAL(128, 1654, 13), // "locatePicture"
QT_MOC_LITERAL(129, 1668, 17), // "QTableWidgetItem*"
QT_MOC_LITERAL(130, 1686, 4), // "item"
QT_MOC_LITERAL(131, 1691, 14), // "showDetailForm"
QT_MOC_LITERAL(132, 1706, 13), // "registerPlane"
QT_MOC_LITERAL(133, 1720, 12) // "planeMessage"

    },
    "MainWindow\0FixPlaneMsg\0\0tag_PlaneMessage*\0"
    "receviePlaneMessage\0QgsPointXY\0qgsPoint\0"
    "recevieImage\0array\0AddJpgAnnotaionMsg\0"
    "Mappoint*\0width\0height\0a\0showMousePoint\0"
    "p\0scaleChanged\0UdpSockerRecvData\0"
    "AddMenu1\0AddMenu2\0AddMenu3\0AddMenu4\0"
    "AddMenu5\0RadarTouying\0selectRadarLayer\0"
    "selectRadarDevice\0s\0moveRadarDevice\0"
    "deleteRadarDevice\0fixRadarDevice\0"
    "OutfitMove\0timer1_timeout\0"
    "onAirLayerRefreshTimer\0processAllPlaneUpdates\0"
    "RadarTestAirList\0SelectTaskLayer\0"
    "AddAirTaskArea\0SelectTaskArea\0FixTaskArea\0"
    "MoveTaskArea\0DeleteTaskArea\0ShowRadarNoArea\0"
    "AreaRadarList\0TaskGuihua\0addVectorlayers\0"
    "addRasterlayers\0pathname\0layername\0"
    "editVectorSymbol\0settingLayer\0"
    "equipmentManager\0hideLittleSelectedPicture\0"
    "hideBigPicture\0hideSmallPicture\0"
    "hideTable\0startStopReceive\0connectUDP\0"
    "RefreshGj\0toggleTrack\0clearTrajectory\0"
    "showMapManager\0switchBaseMap\0filePath\0"
    "layerName\0loadOnlineTileMap\0url\0"
    "moveView\0addDrawPoint\0addDrawLine\0"
    "addDrawPolygon\0drawCircle2point\0"
    "saveToLayer\0editLine\0editPolygon\0"
    "deleteSelected\0QgsMapLayer*\0layer\0"
    "QWidget*\0parent\0checkFeaturesVisible\0"
    "modifyFeatures\0selectPoint\0selectLine\0"
    "selectPolygon\0selectPicture\0measureLength\0"
    "measureArea\0testMenu\0dataBack\0JiqunTuili\0"
    "show2Dor3Dwindow\0switchMapViewMode\0"
    "mode\0getPlaneCount\0createGrid\0"
    "sendBundiance\0ScanPathPlan\0lookDialogShow\0"
    "mapMiddle\0addJpgAnnotaion\0leftupY\0"
    "leftupX\0rightupY\0rightupX\0leftdownY\0"
    "leftdownX\0rightdownY\0rightdownX\0"
    "filename\0addJpgAnnotaionCenter1\0"
    "mappoint\0hidetable\0udpRecieveEleticMagnetismInit\0"
    "processEleticMagnetismDatagram\0"
    "selectElecticMangnetism\0read_data\0"
    "RecevieLook\0addToRightTable\0updateSqlite\0"
    "updateSqliteBoundance\0annotationCreated\0"
    "QgsAnnotation*\0annotation\0"
    "showJpgAnnotaionWindow\0QPoint&\0pos\0"
    "show\0point\0locatePicture\0QTableWidgetItem*\0"
    "item\0showDetailForm\0registerPlane\0"
    "planeMessage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      96,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  494,    2, 0x06 /* Public */,
       4,    1,  497,    2, 0x06 /* Public */,
       7,    1,  500,    2, 0x06 /* Public */,
       9,    4,  503,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    1,  512,    2, 0x0a /* Public */,
      16,    1,  515,    2, 0x0a /* Public */,
      17,    0,  518,    2, 0x0a /* Public */,
      18,    0,  519,    2, 0x0a /* Public */,
      19,    0,  520,    2, 0x0a /* Public */,
      20,    0,  521,    2, 0x0a /* Public */,
      21,    0,  522,    2, 0x0a /* Public */,
      22,    0,  523,    2, 0x0a /* Public */,
      23,    0,  524,    2, 0x0a /* Public */,
      24,    0,  525,    2, 0x0a /* Public */,
      25,    1,  526,    2, 0x0a /* Public */,
      27,    0,  529,    2, 0x0a /* Public */,
      28,    0,  530,    2, 0x0a /* Public */,
      29,    0,  531,    2, 0x0a /* Public */,
      30,    0,  532,    2, 0x0a /* Public */,
      31,    0,  533,    2, 0x0a /* Public */,
      32,    0,  534,    2, 0x0a /* Public */,
      33,    0,  535,    2, 0x0a /* Public */,
      34,    0,  536,    2, 0x0a /* Public */,
      35,    0,  537,    2, 0x0a /* Public */,
      36,    0,  538,    2, 0x0a /* Public */,
      37,    0,  539,    2, 0x0a /* Public */,
      38,    0,  540,    2, 0x0a /* Public */,
      39,    0,  541,    2, 0x0a /* Public */,
      40,    0,  542,    2, 0x0a /* Public */,
      41,    0,  543,    2, 0x0a /* Public */,
      42,    0,  544,    2, 0x0a /* Public */,
      43,    0,  545,    2, 0x0a /* Public */,
      44,    0,  546,    2, 0x0a /* Public */,
      45,    0,  547,    2, 0x0a /* Public */,
      45,    2,  548,    2, 0x0a /* Public */,
      48,    0,  553,    2, 0x0a /* Public */,
      49,    0,  554,    2, 0x0a /* Public */,
      50,    0,  555,    2, 0x0a /* Public */,
      51,    0,  556,    2, 0x0a /* Public */,
      52,    0,  557,    2, 0x0a /* Public */,
      53,    0,  558,    2, 0x0a /* Public */,
      54,    0,  559,    2, 0x0a /* Public */,
      55,    0,  560,    2, 0x0a /* Public */,
      56,    0,  561,    2, 0x0a /* Public */,
      57,    0,  562,    2, 0x0a /* Public */,
      58,    0,  563,    2, 0x0a /* Public */,
      59,    0,  564,    2, 0x0a /* Public */,
      60,    0,  565,    2, 0x0a /* Public */,
      61,    2,  566,    2, 0x0a /* Public */,
      64,    2,  571,    2, 0x0a /* Public */,
      66,    0,  576,    2, 0x0a /* Public */,
      67,    0,  577,    2, 0x0a /* Public */,
      68,    0,  578,    2, 0x0a /* Public */,
      69,    0,  579,    2, 0x0a /* Public */,
      70,    0,  580,    2, 0x0a /* Public */,
      71,    0,  581,    2, 0x0a /* Public */,
      72,    0,  582,    2, 0x0a /* Public */,
      73,    0,  583,    2, 0x0a /* Public */,
      74,    3,  584,    2, 0x0a /* Public */,
      74,    2,  591,    2, 0x2a /* Public | MethodCloned */,
      74,    1,  596,    2, 0x2a /* Public | MethodCloned */,
      74,    0,  599,    2, 0x2a /* Public | MethodCloned */,
      80,    0,  600,    2, 0x0a /* Public */,
      81,    0,  601,    2, 0x0a /* Public */,
      82,    0,  602,    2, 0x0a /* Public */,
      83,    0,  603,    2, 0x0a /* Public */,
      84,    0,  604,    2, 0x0a /* Public */,
      85,    0,  605,    2, 0x0a /* Public */,
      86,    0,  606,    2, 0x0a /* Public */,
      87,    0,  607,    2, 0x0a /* Public */,
      88,    0,  608,    2, 0x0a /* Public */,
      89,    0,  609,    2, 0x0a /* Public */,
      90,    0,  610,    2, 0x0a /* Public */,
      91,    1,  611,    2, 0x0a /* Public */,
      93,    0,  614,    2, 0x0a /* Public */,
      94,    0,  615,    2, 0x0a /* Public */,
      95,    0,  616,    2, 0x0a /* Public */,
      96,    0,  617,    2, 0x0a /* Public */,
      97,    0,  618,    2, 0x0a /* Public */,
      98,    0,  619,    2, 0x0a /* Public */,
      99,    9,  620,    2, 0x0a /* Public */,
     109,    4,  639,    2, 0x0a /* Public */,
     111,    0,  648,    2, 0x0a /* Public */,
     112,    0,  649,    2, 0x0a /* Public */,
     113,    0,  650,    2, 0x0a /* Public */,
     114,    0,  651,    2, 0x0a /* Public */,
     115,    0,  652,    2, 0x0a /* Public */,
     116,    1,  653,    2, 0x0a /* Public */,
     117,    1,  656,    2, 0x0a /* Public */,
     118,    0,  659,    2, 0x0a /* Public */,
     119,    0,  660,    2, 0x0a /* Public */,
     120,    1,  661,    2, 0x0a /* Public */,
     123,    3,  664,    2, 0x0a /* Public */,
     128,    1,  671,    2, 0x0a /* Public */,
     131,    0,  674,    2, 0x0a /* Public */,
     132,    1,  675,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void, QMetaType::QByteArray,    8,
    QMetaType::Void, 0x80000000 | 10, QMetaType::Double, QMetaType::Double, QMetaType::Int,    2,   11,   12,   13,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,   15,
    QMetaType::Void, QMetaType::Double,   15,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   26,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   46,   47,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   62,   63,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   65,   63,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 75, 0x80000000 | 77, QMetaType::Bool,   76,   78,   79,
    QMetaType::Void, 0x80000000 | 75, 0x80000000 | 77,   76,   78,
    QMetaType::Void, 0x80000000 | 75,   76,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,   92,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::QString,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::QString,  100,  101,  102,  103,  104,  105,  106,  107,  108,
    QMetaType::Void, 0x80000000 | 10, QMetaType::Double, QMetaType::Double, QMetaType::Int,  110,   11,   12,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    8,
    QMetaType::QString, QMetaType::QByteArray,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 121,  122,
    QMetaType::Void, 0x80000000 | 124, QMetaType::Bool, 0x80000000 | 10,  125,  126,  127,
    QMetaType::Void, 0x80000000 | 129,  130,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,  133,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->FixPlaneMsg((*reinterpret_cast< tag_PlaneMessage*(*)>(_a[1]))); break;
        case 1: _t->receviePlaneMessage((*reinterpret_cast< const QgsPointXY(*)>(_a[1]))); break;
        case 2: _t->recevieImage((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 3: _t->AddJpgAnnotaionMsg((*reinterpret_cast< Mappoint*(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< const int(*)>(_a[4]))); break;
        case 4: _t->showMousePoint((*reinterpret_cast< const QgsPointXY(*)>(_a[1]))); break;
        case 5: _t->scaleChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->UdpSockerRecvData(); break;
        case 7: _t->AddMenu1(); break;
        case 8: _t->AddMenu2(); break;
        case 9: _t->AddMenu3(); break;
        case 10: _t->AddMenu4(); break;
        case 11: _t->AddMenu5(); break;
        case 12: _t->RadarTouying(); break;
        case 13: _t->selectRadarLayer(); break;
        case 14: _t->selectRadarDevice((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 15: _t->moveRadarDevice(); break;
        case 16: _t->deleteRadarDevice(); break;
        case 17: _t->fixRadarDevice(); break;
        case 18: _t->OutfitMove(); break;
        case 19: _t->timer1_timeout(); break;
        case 20: _t->onAirLayerRefreshTimer(); break;
        case 21: _t->processAllPlaneUpdates(); break;
        case 22: _t->RadarTestAirList(); break;
        case 23: _t->SelectTaskLayer(); break;
        case 24: _t->AddAirTaskArea(); break;
        case 25: _t->SelectTaskArea(); break;
        case 26: _t->FixTaskArea(); break;
        case 27: _t->MoveTaskArea(); break;
        case 28: _t->DeleteTaskArea(); break;
        case 29: _t->ShowRadarNoArea(); break;
        case 30: _t->AreaRadarList(); break;
        case 31: _t->TaskGuihua(); break;
        case 32: _t->addVectorlayers(); break;
        case 33: _t->addRasterlayers(); break;
        case 34: _t->addRasterlayers((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 35: _t->editVectorSymbol(); break;
        case 36: _t->settingLayer(); break;
        case 37: _t->equipmentManager(); break;
        case 38: _t->hideLittleSelectedPicture(); break;
        case 39: _t->hideBigPicture(); break;
        case 40: _t->hideSmallPicture(); break;
        case 41: _t->hideTable(); break;
        case 42: _t->startStopReceive(); break;
        case 43: _t->connectUDP(); break;
        case 44: _t->RefreshGj(); break;
        case 45: _t->toggleTrack(); break;
        case 46: _t->clearTrajectory(); break;
        case 47: _t->showMapManager(); break;
        case 48: _t->switchBaseMap((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 49: _t->loadOnlineTileMap((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 50: _t->moveView(); break;
        case 51: _t->addDrawPoint(); break;
        case 52: _t->addDrawLine(); break;
        case 53: _t->addDrawPolygon(); break;
        case 54: _t->drawCircle2point(); break;
        case 55: _t->saveToLayer(); break;
        case 56: _t->editLine(); break;
        case 57: _t->editPolygon(); break;
        case 58: _t->deleteSelected((*reinterpret_cast< QgsMapLayer*(*)>(_a[1])),(*reinterpret_cast< QWidget*(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 59: _t->deleteSelected((*reinterpret_cast< QgsMapLayer*(*)>(_a[1])),(*reinterpret_cast< QWidget*(*)>(_a[2]))); break;
        case 60: _t->deleteSelected((*reinterpret_cast< QgsMapLayer*(*)>(_a[1]))); break;
        case 61: _t->deleteSelected(); break;
        case 62: _t->modifyFeatures(); break;
        case 63: _t->selectPoint(); break;
        case 64: _t->selectLine(); break;
        case 65: _t->selectPolygon(); break;
        case 66: _t->selectPicture(); break;
        case 67: _t->measureLength(); break;
        case 68: _t->measureArea(); break;
        case 69: _t->testMenu(); break;
        case 70: _t->dataBack(); break;
        case 71: _t->JiqunTuili(); break;
        case 72: _t->show2Dor3Dwindow(); break;
        case 73: _t->switchMapViewMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 74: _t->getPlaneCount(); break;
        case 75: _t->createGrid(); break;
        case 76: { QString _r = _t->sendBundiance();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 77: _t->ScanPathPlan(); break;
        case 78: _t->lookDialogShow(); break;
        case 79: _t->mapMiddle(); break;
        case 80: _t->addJpgAnnotaion((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5])),(*reinterpret_cast< double(*)>(_a[6])),(*reinterpret_cast< double(*)>(_a[7])),(*reinterpret_cast< double(*)>(_a[8])),(*reinterpret_cast< QString(*)>(_a[9]))); break;
        case 81: _t->addJpgAnnotaionCenter1((*reinterpret_cast< Mappoint*(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 82: _t->hidetable(); break;
        case 83: _t->udpRecieveEleticMagnetismInit(); break;
        case 84: _t->processEleticMagnetismDatagram(); break;
        case 85: _t->selectElecticMangnetism(); break;
        case 86: _t->read_data(); break;
        case 87: _t->RecevieLook((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 88: { QString _r = _t->addToRightTable((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 89: _t->updateSqlite(); break;
        case 90: _t->updateSqliteBoundance(); break;
        case 91: _t->annotationCreated((*reinterpret_cast< QgsAnnotation*(*)>(_a[1]))); break;
        case 92: _t->showJpgAnnotaionWindow((*reinterpret_cast< QPoint(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< Mappoint*(*)>(_a[3]))); break;
        case 93: _t->locatePicture((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 94: _t->showDetailForm(); break;
        case 95: _t->registerPlane((*reinterpret_cast< tag_PlaneMessage*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 58:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QWidget* >(); break;
            }
            break;
        case 59:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QWidget* >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MainWindow::*)(tag_PlaneMessage * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::FixPlaneMsg)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(const QgsPointXY & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::receviePlaneMessage)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(const QByteArray & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::recevieImage)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MainWindow::*)(Mappoint * , double , double , const int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MainWindow::AddJpgAnnotaionMsg)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject MainWindow::staticMetaObject = { {
    &QMainWindow::staticMetaObject,
    qt_meta_stringdata_MainWindow.data,
    qt_meta_data_MainWindow,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 96)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 96;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 96)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 96;
    }
    return _id;
}

// SIGNAL 0
void MainWindow::FixPlaneMsg(tag_PlaneMessage * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MainWindow::receviePlaneMessage(const QgsPointXY & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MainWindow::recevieImage(const QByteArray & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MainWindow::AddJpgAnnotaionMsg(Mappoint * _t1, double _t2, double _t3, const int _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)), const_cast<void*>(reinterpret_cast<const void*>(&_t4)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
