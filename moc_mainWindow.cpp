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
    QByteArrayData data[139];
    char stringdata0[1782];
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
QT_MOC_LITERAL(25, 268, 11), // "onRadarPick"
QT_MOC_LITERAL(26, 280, 2), // "pt"
QT_MOC_LITERAL(27, 283, 15), // "Qt::MouseButton"
QT_MOC_LITERAL(28, 299, 3), // "btn"
QT_MOC_LITERAL(29, 303, 17), // "selectRadarDevice"
QT_MOC_LITERAL(30, 321, 1), // "s"
QT_MOC_LITERAL(31, 323, 15), // "moveRadarDevice"
QT_MOC_LITERAL(32, 339, 17), // "deleteRadarDevice"
QT_MOC_LITERAL(33, 357, 14), // "fixRadarDevice"
QT_MOC_LITERAL(34, 372, 10), // "OutfitMove"
QT_MOC_LITERAL(35, 383, 14), // "timer1_timeout"
QT_MOC_LITERAL(36, 398, 22), // "onAirLayerRefreshTimer"
QT_MOC_LITERAL(37, 421, 13), // "onInterpTimer"
QT_MOC_LITERAL(38, 435, 22), // "processAllPlaneUpdates"
QT_MOC_LITERAL(39, 458, 16), // "RadarTestAirList"
QT_MOC_LITERAL(40, 475, 15), // "SelectTaskLayer"
QT_MOC_LITERAL(41, 491, 14), // "AddAirTaskArea"
QT_MOC_LITERAL(42, 506, 14), // "SelectTaskArea"
QT_MOC_LITERAL(43, 521, 11), // "FixTaskArea"
QT_MOC_LITERAL(44, 533, 12), // "MoveTaskArea"
QT_MOC_LITERAL(45, 546, 14), // "DeleteTaskArea"
QT_MOC_LITERAL(46, 561, 15), // "ShowRadarNoArea"
QT_MOC_LITERAL(47, 577, 13), // "AreaRadarList"
QT_MOC_LITERAL(48, 591, 10), // "TaskGuihua"
QT_MOC_LITERAL(49, 602, 15), // "addVectorlayers"
QT_MOC_LITERAL(50, 618, 15), // "addRasterlayers"
QT_MOC_LITERAL(51, 634, 8), // "pathname"
QT_MOC_LITERAL(52, 643, 9), // "layername"
QT_MOC_LITERAL(53, 653, 16), // "editVectorSymbol"
QT_MOC_LITERAL(54, 670, 12), // "settingLayer"
QT_MOC_LITERAL(55, 683, 16), // "equipmentManager"
QT_MOC_LITERAL(56, 700, 25), // "hideLittleSelectedPicture"
QT_MOC_LITERAL(57, 726, 14), // "hideBigPicture"
QT_MOC_LITERAL(58, 741, 16), // "hideSmallPicture"
QT_MOC_LITERAL(59, 758, 9), // "hideTable"
QT_MOC_LITERAL(60, 768, 16), // "startStopReceive"
QT_MOC_LITERAL(61, 785, 10), // "connectUDP"
QT_MOC_LITERAL(62, 796, 9), // "RefreshGj"
QT_MOC_LITERAL(63, 806, 11), // "toggleTrack"
QT_MOC_LITERAL(64, 818, 15), // "clearTrajectory"
QT_MOC_LITERAL(65, 834, 14), // "showMapManager"
QT_MOC_LITERAL(66, 849, 13), // "switchBaseMap"
QT_MOC_LITERAL(67, 863, 8), // "filePath"
QT_MOC_LITERAL(68, 872, 9), // "layerName"
QT_MOC_LITERAL(69, 882, 17), // "loadOnlineTileMap"
QT_MOC_LITERAL(70, 900, 3), // "url"
QT_MOC_LITERAL(71, 904, 8), // "moveView"
QT_MOC_LITERAL(72, 913, 12), // "addDrawPoint"
QT_MOC_LITERAL(73, 926, 11), // "addDrawLine"
QT_MOC_LITERAL(74, 938, 14), // "addDrawPolygon"
QT_MOC_LITERAL(75, 953, 16), // "drawCircle2point"
QT_MOC_LITERAL(76, 970, 11), // "saveToLayer"
QT_MOC_LITERAL(77, 982, 8), // "editLine"
QT_MOC_LITERAL(78, 991, 11), // "editPolygon"
QT_MOC_LITERAL(79, 1003, 14), // "deleteSelected"
QT_MOC_LITERAL(80, 1018, 12), // "QgsMapLayer*"
QT_MOC_LITERAL(81, 1031, 5), // "layer"
QT_MOC_LITERAL(82, 1037, 8), // "QWidget*"
QT_MOC_LITERAL(83, 1046, 6), // "parent"
QT_MOC_LITERAL(84, 1053, 20), // "checkFeaturesVisible"
QT_MOC_LITERAL(85, 1074, 14), // "modifyFeatures"
QT_MOC_LITERAL(86, 1089, 11), // "selectPoint"
QT_MOC_LITERAL(87, 1101, 10), // "selectLine"
QT_MOC_LITERAL(88, 1112, 13), // "selectPolygon"
QT_MOC_LITERAL(89, 1126, 13), // "selectPicture"
QT_MOC_LITERAL(90, 1140, 13), // "measureLength"
QT_MOC_LITERAL(91, 1154, 11), // "measureArea"
QT_MOC_LITERAL(92, 1166, 8), // "testMenu"
QT_MOC_LITERAL(93, 1175, 8), // "dataBack"
QT_MOC_LITERAL(94, 1184, 10), // "JiqunTuili"
QT_MOC_LITERAL(95, 1195, 16), // "show2Dor3Dwindow"
QT_MOC_LITERAL(96, 1212, 17), // "switchMapViewMode"
QT_MOC_LITERAL(97, 1230, 4), // "mode"
QT_MOC_LITERAL(98, 1235, 13), // "getPlaneCount"
QT_MOC_LITERAL(99, 1249, 10), // "createGrid"
QT_MOC_LITERAL(100, 1260, 13), // "sendBundiance"
QT_MOC_LITERAL(101, 1274, 12), // "ScanPathPlan"
QT_MOC_LITERAL(102, 1287, 14), // "lookDialogShow"
QT_MOC_LITERAL(103, 1302, 9), // "mapMiddle"
QT_MOC_LITERAL(104, 1312, 15), // "addJpgAnnotaion"
QT_MOC_LITERAL(105, 1328, 7), // "leftupY"
QT_MOC_LITERAL(106, 1336, 7), // "leftupX"
QT_MOC_LITERAL(107, 1344, 8), // "rightupY"
QT_MOC_LITERAL(108, 1353, 8), // "rightupX"
QT_MOC_LITERAL(109, 1362, 9), // "leftdownY"
QT_MOC_LITERAL(110, 1372, 9), // "leftdownX"
QT_MOC_LITERAL(111, 1382, 10), // "rightdownY"
QT_MOC_LITERAL(112, 1393, 10), // "rightdownX"
QT_MOC_LITERAL(113, 1404, 8), // "filename"
QT_MOC_LITERAL(114, 1413, 22), // "addJpgAnnotaionCenter1"
QT_MOC_LITERAL(115, 1436, 8), // "mappoint"
QT_MOC_LITERAL(116, 1445, 9), // "hidetable"
QT_MOC_LITERAL(117, 1455, 29), // "udpRecieveEleticMagnetismInit"
QT_MOC_LITERAL(118, 1485, 30), // "processEleticMagnetismDatagram"
QT_MOC_LITERAL(119, 1516, 23), // "selectElecticMangnetism"
QT_MOC_LITERAL(120, 1540, 9), // "read_data"
QT_MOC_LITERAL(121, 1550, 11), // "RecevieLook"
QT_MOC_LITERAL(122, 1562, 15), // "addToRightTable"
QT_MOC_LITERAL(123, 1578, 12), // "updateSqlite"
QT_MOC_LITERAL(124, 1591, 21), // "updateSqliteBoundance"
QT_MOC_LITERAL(125, 1613, 17), // "annotationCreated"
QT_MOC_LITERAL(126, 1631, 14), // "QgsAnnotation*"
QT_MOC_LITERAL(127, 1646, 10), // "annotation"
QT_MOC_LITERAL(128, 1657, 22), // "showJpgAnnotaionWindow"
QT_MOC_LITERAL(129, 1680, 7), // "QPoint&"
QT_MOC_LITERAL(130, 1688, 3), // "pos"
QT_MOC_LITERAL(131, 1692, 4), // "show"
QT_MOC_LITERAL(132, 1697, 5), // "point"
QT_MOC_LITERAL(133, 1703, 13), // "locatePicture"
QT_MOC_LITERAL(134, 1717, 17), // "QTableWidgetItem*"
QT_MOC_LITERAL(135, 1735, 4), // "item"
QT_MOC_LITERAL(136, 1740, 14), // "showDetailForm"
QT_MOC_LITERAL(137, 1755, 13), // "registerPlane"
QT_MOC_LITERAL(138, 1769, 12) // "planeMessage"

    },
    "MainWindow\0FixPlaneMsg\0\0tag_PlaneMessage*\0"
    "receviePlaneMessage\0QgsPointXY\0qgsPoint\0"
    "recevieImage\0array\0AddJpgAnnotaionMsg\0"
    "Mappoint*\0width\0height\0a\0showMousePoint\0"
    "p\0scaleChanged\0UdpSockerRecvData\0"
    "AddMenu1\0AddMenu2\0AddMenu3\0AddMenu4\0"
    "AddMenu5\0RadarTouying\0selectRadarLayer\0"
    "onRadarPick\0pt\0Qt::MouseButton\0btn\0"
    "selectRadarDevice\0s\0moveRadarDevice\0"
    "deleteRadarDevice\0fixRadarDevice\0"
    "OutfitMove\0timer1_timeout\0"
    "onAirLayerRefreshTimer\0onInterpTimer\0"
    "processAllPlaneUpdates\0RadarTestAirList\0"
    "SelectTaskLayer\0AddAirTaskArea\0"
    "SelectTaskArea\0FixTaskArea\0MoveTaskArea\0"
    "DeleteTaskArea\0ShowRadarNoArea\0"
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
      98,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,  504,    2, 0x06 /* Public */,
       4,    1,  507,    2, 0x06 /* Public */,
       7,    1,  510,    2, 0x06 /* Public */,
       9,    4,  513,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    1,  522,    2, 0x0a /* Public */,
      16,    1,  525,    2, 0x0a /* Public */,
      17,    0,  528,    2, 0x0a /* Public */,
      18,    0,  529,    2, 0x0a /* Public */,
      19,    0,  530,    2, 0x0a /* Public */,
      20,    0,  531,    2, 0x0a /* Public */,
      21,    0,  532,    2, 0x0a /* Public */,
      22,    0,  533,    2, 0x0a /* Public */,
      23,    0,  534,    2, 0x0a /* Public */,
      24,    0,  535,    2, 0x0a /* Public */,
      25,    2,  536,    2, 0x0a /* Public */,
      29,    1,  541,    2, 0x0a /* Public */,
      31,    0,  544,    2, 0x0a /* Public */,
      32,    0,  545,    2, 0x0a /* Public */,
      33,    0,  546,    2, 0x0a /* Public */,
      34,    0,  547,    2, 0x0a /* Public */,
      35,    0,  548,    2, 0x0a /* Public */,
      36,    0,  549,    2, 0x0a /* Public */,
      37,    0,  550,    2, 0x0a /* Public */,
      38,    0,  551,    2, 0x0a /* Public */,
      39,    0,  552,    2, 0x0a /* Public */,
      40,    0,  553,    2, 0x0a /* Public */,
      41,    0,  554,    2, 0x0a /* Public */,
      42,    0,  555,    2, 0x0a /* Public */,
      43,    0,  556,    2, 0x0a /* Public */,
      44,    0,  557,    2, 0x0a /* Public */,
      45,    0,  558,    2, 0x0a /* Public */,
      46,    0,  559,    2, 0x0a /* Public */,
      47,    0,  560,    2, 0x0a /* Public */,
      48,    0,  561,    2, 0x0a /* Public */,
      49,    0,  562,    2, 0x0a /* Public */,
      50,    0,  563,    2, 0x0a /* Public */,
      50,    2,  564,    2, 0x0a /* Public */,
      53,    0,  569,    2, 0x0a /* Public */,
      54,    0,  570,    2, 0x0a /* Public */,
      55,    0,  571,    2, 0x0a /* Public */,
      56,    0,  572,    2, 0x0a /* Public */,
      57,    0,  573,    2, 0x0a /* Public */,
      58,    0,  574,    2, 0x0a /* Public */,
      59,    0,  575,    2, 0x0a /* Public */,
      60,    0,  576,    2, 0x0a /* Public */,
      61,    0,  577,    2, 0x0a /* Public */,
      62,    0,  578,    2, 0x0a /* Public */,
      63,    0,  579,    2, 0x0a /* Public */,
      64,    0,  580,    2, 0x0a /* Public */,
      65,    0,  581,    2, 0x0a /* Public */,
      66,    2,  582,    2, 0x0a /* Public */,
      69,    2,  587,    2, 0x0a /* Public */,
      71,    0,  592,    2, 0x0a /* Public */,
      72,    0,  593,    2, 0x0a /* Public */,
      73,    0,  594,    2, 0x0a /* Public */,
      74,    0,  595,    2, 0x0a /* Public */,
      75,    0,  596,    2, 0x0a /* Public */,
      76,    0,  597,    2, 0x0a /* Public */,
      77,    0,  598,    2, 0x0a /* Public */,
      78,    0,  599,    2, 0x0a /* Public */,
      79,    3,  600,    2, 0x0a /* Public */,
      79,    2,  607,    2, 0x2a /* Public | MethodCloned */,
      79,    1,  612,    2, 0x2a /* Public | MethodCloned */,
      79,    0,  615,    2, 0x2a /* Public | MethodCloned */,
      85,    0,  616,    2, 0x0a /* Public */,
      86,    0,  617,    2, 0x0a /* Public */,
      87,    0,  618,    2, 0x0a /* Public */,
      88,    0,  619,    2, 0x0a /* Public */,
      89,    0,  620,    2, 0x0a /* Public */,
      90,    0,  621,    2, 0x0a /* Public */,
      91,    0,  622,    2, 0x0a /* Public */,
      92,    0,  623,    2, 0x0a /* Public */,
      93,    0,  624,    2, 0x0a /* Public */,
      94,    0,  625,    2, 0x0a /* Public */,
      95,    0,  626,    2, 0x0a /* Public */,
      96,    1,  627,    2, 0x0a /* Public */,
      98,    0,  630,    2, 0x0a /* Public */,
      99,    0,  631,    2, 0x0a /* Public */,
     100,    0,  632,    2, 0x0a /* Public */,
     101,    0,  633,    2, 0x0a /* Public */,
     102,    0,  634,    2, 0x0a /* Public */,
     103,    0,  635,    2, 0x0a /* Public */,
     104,    9,  636,    2, 0x0a /* Public */,
     114,    4,  655,    2, 0x0a /* Public */,
     116,    0,  664,    2, 0x0a /* Public */,
     117,    0,  665,    2, 0x0a /* Public */,
     118,    0,  666,    2, 0x0a /* Public */,
     119,    0,  667,    2, 0x0a /* Public */,
     120,    0,  668,    2, 0x0a /* Public */,
     121,    1,  669,    2, 0x0a /* Public */,
     122,    1,  672,    2, 0x0a /* Public */,
     123,    0,  675,    2, 0x0a /* Public */,
     124,    0,  676,    2, 0x0a /* Public */,
     125,    1,  677,    2, 0x0a /* Public */,
     128,    3,  680,    2, 0x0a /* Public */,
     133,    1,  687,    2, 0x0a /* Public */,
     136,    0,  690,    2, 0x0a /* Public */,
     137,    1,  691,    2, 0x0a /* Public */,

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
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 27,   26,   28,
    QMetaType::Void, QMetaType::QString,   30,
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
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   51,   52,
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
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   67,   68,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   70,   68,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 80, 0x80000000 | 82, QMetaType::Bool,   81,   83,   84,
    QMetaType::Void, 0x80000000 | 80, 0x80000000 | 82,   81,   83,
    QMetaType::Void, 0x80000000 | 80,   81,
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
    QMetaType::Void, QMetaType::Int,   97,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::QString,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::Double, QMetaType::QString,  105,  106,  107,  108,  109,  110,  111,  112,  113,
    QMetaType::Void, 0x80000000 | 10, QMetaType::Double, QMetaType::Double, QMetaType::Int,  115,   11,   12,   13,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QByteArray,    8,
    QMetaType::QString, QMetaType::QByteArray,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 126,  127,
    QMetaType::Void, 0x80000000 | 129, QMetaType::Bool, 0x80000000 | 10,  130,  131,  132,
    QMetaType::Void, 0x80000000 | 134,  135,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,  138,

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
        case 14: _t->onRadarPick((*reinterpret_cast< const QgsPointXY(*)>(_a[1])),(*reinterpret_cast< Qt::MouseButton(*)>(_a[2]))); break;
        case 15: _t->selectRadarDevice((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 16: _t->moveRadarDevice(); break;
        case 17: _t->deleteRadarDevice(); break;
        case 18: _t->fixRadarDevice(); break;
        case 19: _t->OutfitMove(); break;
        case 20: _t->timer1_timeout(); break;
        case 21: _t->onAirLayerRefreshTimer(); break;
        case 22: _t->onInterpTimer(); break;
        case 23: _t->processAllPlaneUpdates(); break;
        case 24: _t->RadarTestAirList(); break;
        case 25: _t->SelectTaskLayer(); break;
        case 26: _t->AddAirTaskArea(); break;
        case 27: _t->SelectTaskArea(); break;
        case 28: _t->FixTaskArea(); break;
        case 29: _t->MoveTaskArea(); break;
        case 30: _t->DeleteTaskArea(); break;
        case 31: _t->ShowRadarNoArea(); break;
        case 32: _t->AreaRadarList(); break;
        case 33: _t->TaskGuihua(); break;
        case 34: _t->addVectorlayers(); break;
        case 35: _t->addRasterlayers(); break;
        case 36: _t->addRasterlayers((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 37: _t->editVectorSymbol(); break;
        case 38: _t->settingLayer(); break;
        case 39: _t->equipmentManager(); break;
        case 40: _t->hideLittleSelectedPicture(); break;
        case 41: _t->hideBigPicture(); break;
        case 42: _t->hideSmallPicture(); break;
        case 43: _t->hideTable(); break;
        case 44: _t->startStopReceive(); break;
        case 45: _t->connectUDP(); break;
        case 46: _t->RefreshGj(); break;
        case 47: _t->toggleTrack(); break;
        case 48: _t->clearTrajectory(); break;
        case 49: _t->showMapManager(); break;
        case 50: _t->switchBaseMap((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 51: _t->loadOnlineTileMap((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 52: _t->moveView(); break;
        case 53: _t->addDrawPoint(); break;
        case 54: _t->addDrawLine(); break;
        case 55: _t->addDrawPolygon(); break;
        case 56: _t->drawCircle2point(); break;
        case 57: _t->saveToLayer(); break;
        case 58: _t->editLine(); break;
        case 59: _t->editPolygon(); break;
        case 60: _t->deleteSelected((*reinterpret_cast< QgsMapLayer*(*)>(_a[1])),(*reinterpret_cast< QWidget*(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 61: _t->deleteSelected((*reinterpret_cast< QgsMapLayer*(*)>(_a[1])),(*reinterpret_cast< QWidget*(*)>(_a[2]))); break;
        case 62: _t->deleteSelected((*reinterpret_cast< QgsMapLayer*(*)>(_a[1]))); break;
        case 63: _t->deleteSelected(); break;
        case 64: _t->modifyFeatures(); break;
        case 65: _t->selectPoint(); break;
        case 66: _t->selectLine(); break;
        case 67: _t->selectPolygon(); break;
        case 68: _t->selectPicture(); break;
        case 69: _t->measureLength(); break;
        case 70: _t->measureArea(); break;
        case 71: _t->testMenu(); break;
        case 72: _t->dataBack(); break;
        case 73: _t->JiqunTuili(); break;
        case 74: _t->show2Dor3Dwindow(); break;
        case 75: _t->switchMapViewMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 76: _t->getPlaneCount(); break;
        case 77: _t->createGrid(); break;
        case 78: { QString _r = _t->sendBundiance();
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 79: _t->ScanPathPlan(); break;
        case 80: _t->lookDialogShow(); break;
        case 81: _t->mapMiddle(); break;
        case 82: _t->addJpgAnnotaion((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< double(*)>(_a[4])),(*reinterpret_cast< double(*)>(_a[5])),(*reinterpret_cast< double(*)>(_a[6])),(*reinterpret_cast< double(*)>(_a[7])),(*reinterpret_cast< double(*)>(_a[8])),(*reinterpret_cast< QString(*)>(_a[9]))); break;
        case 83: _t->addJpgAnnotaionCenter1((*reinterpret_cast< Mappoint*(*)>(_a[1])),(*reinterpret_cast< double(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 84: _t->hidetable(); break;
        case 85: _t->udpRecieveEleticMagnetismInit(); break;
        case 86: _t->processEleticMagnetismDatagram(); break;
        case 87: _t->selectElecticMangnetism(); break;
        case 88: _t->read_data(); break;
        case 89: _t->RecevieLook((*reinterpret_cast< const QByteArray(*)>(_a[1]))); break;
        case 90: { QString _r = _t->addToRightTable((*reinterpret_cast< const QByteArray(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< QString*>(_a[0]) = std::move(_r); }  break;
        case 91: _t->updateSqlite(); break;
        case 92: _t->updateSqliteBoundance(); break;
        case 93: _t->annotationCreated((*reinterpret_cast< QgsAnnotation*(*)>(_a[1]))); break;
        case 94: _t->showJpgAnnotaionWindow((*reinterpret_cast< QPoint(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< Mappoint*(*)>(_a[3]))); break;
        case 95: _t->locatePicture((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 96: _t->showDetailForm(); break;
        case 97: _t->registerPlane((*reinterpret_cast< tag_PlaneMessage*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 60:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 1:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QWidget* >(); break;
            }
            break;
        case 61:
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
        if (_id < 98)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 98;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 98)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 98;
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
