/****************************************************************************
** Meta object code from reading C++ file 'qgsMaptoolSelectionHandler.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "./qgsMaptoolSelectionHandler.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qgsMaptoolSelectionHandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QgsDistanceWidget_t {
    QByteArrayData data[8];
    char stringdata0[124];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QgsDistanceWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QgsDistanceWidget_t qt_meta_stringdata_QgsDistanceWidget = {
    {
QT_MOC_LITERAL(0, 0, 17), // "QgsDistanceWidget"
QT_MOC_LITERAL(1, 18, 15), // "distanceChanged"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 8), // "distance"
QT_MOC_LITERAL(4, 44, 23), // "distanceEditingFinished"
QT_MOC_LITERAL(5, 68, 21), // "Qt::KeyboardModifiers"
QT_MOC_LITERAL(6, 90, 9), // "modifiers"
QT_MOC_LITERAL(7, 100, 23) // "distanceEditingCanceled"

    },
    "QgsDistanceWidget\0distanceChanged\0\0"
    "distance\0distanceEditingFinished\0"
    "Qt::KeyboardModifiers\0modifiers\0"
    "distanceEditingCanceled"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QgsDistanceWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   29,    2, 0x06 /* Public */,
       4,    2,   32,    2, 0x06 /* Public */,
       7,    0,   37,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::Double,    3,
    QMetaType::Void, QMetaType::Double, 0x80000000 | 5,    3,    6,
    QMetaType::Void,

       0        // eod
};

void QgsDistanceWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QgsDistanceWidget *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->distanceChanged((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 1: _t->distanceEditingFinished((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< const Qt::KeyboardModifiers(*)>(_a[2]))); break;
        case 2: _t->distanceEditingCanceled(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (QgsDistanceWidget::*)(double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QgsDistanceWidget::distanceChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (QgsDistanceWidget::*)(double , const Qt::KeyboardModifiers & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QgsDistanceWidget::distanceEditingFinished)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (QgsDistanceWidget::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QgsDistanceWidget::distanceEditingCanceled)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QgsDistanceWidget::staticMetaObject = { {
    &QWidget::staticMetaObject,
    qt_meta_stringdata_QgsDistanceWidget.data,
    qt_meta_data_QgsDistanceWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QgsDistanceWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QgsDistanceWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QgsDistanceWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int QgsDistanceWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void QgsDistanceWidget::distanceChanged(double _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void QgsDistanceWidget::distanceEditingFinished(double _t1, const Qt::KeyboardModifiers & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void QgsDistanceWidget::distanceEditingCanceled()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}
struct qt_meta_stringdata_QgsMapToolSelectionHandler_t {
    QByteArrayData data[14];
    char stringdata0[201];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QgsMapToolSelectionHandler_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QgsMapToolSelectionHandler_t qt_meta_stringdata_QgsMapToolSelectionHandler = {
    {
QT_MOC_LITERAL(0, 0, 26), // "QgsMapToolSelectionHandler"
QT_MOC_LITERAL(1, 27, 15), // "geometryChanged"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 21), // "Qt::KeyboardModifiers"
QT_MOC_LITERAL(4, 66, 9), // "modifiers"
QT_MOC_LITERAL(5, 76, 22), // "updateRadiusRubberband"
QT_MOC_LITERAL(6, 99, 6), // "radius"
QT_MOC_LITERAL(7, 106, 18), // "radiusValueEntered"
QT_MOC_LITERAL(8, 125, 6), // "cancel"
QT_MOC_LITERAL(9, 132, 13), // "SelectionMode"
QT_MOC_LITERAL(10, 146, 12), // "SelectSimple"
QT_MOC_LITERAL(11, 159, 13), // "SelectPolygon"
QT_MOC_LITERAL(12, 173, 14), // "SelectFreehand"
QT_MOC_LITERAL(13, 188, 12) // "SelectRadius"

    },
    "QgsMapToolSelectionHandler\0geometryChanged\0"
    "\0Qt::KeyboardModifiers\0modifiers\0"
    "updateRadiusRubberband\0radius\0"
    "radiusValueEntered\0cancel\0SelectionMode\0"
    "SelectSimple\0SelectPolygon\0SelectFreehand\0"
    "SelectRadius"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QgsMapToolSelectionHandler[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       1,   52, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       1,    0,   42,    2, 0x26 /* Public | MethodCloned */,

 // slots: name, argc, parameters, tag, flags
       5,    1,   43,    2, 0x08 /* Private */,
       7,    2,   46,    2, 0x08 /* Private */,
       8,    0,   51,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::Double,    6,
    QMetaType::Void, QMetaType::Double, 0x80000000 | 3,    6,    4,
    QMetaType::Void,

 // enums: name, alias, flags, count, data
       9,    9, 0x0,    4,   57,

 // enum data: key, value
      10, uint(QgsMapToolSelectionHandler::SelectSimple),
      11, uint(QgsMapToolSelectionHandler::SelectPolygon),
      12, uint(QgsMapToolSelectionHandler::SelectFreehand),
      13, uint(QgsMapToolSelectionHandler::SelectRadius),

       0        // eod
};

void QgsMapToolSelectionHandler::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QgsMapToolSelectionHandler *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->geometryChanged((*reinterpret_cast< Qt::KeyboardModifiers(*)>(_a[1]))); break;
        case 1: _t->geometryChanged(); break;
        case 2: _t->updateRadiusRubberband((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->radiusValueEntered((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< Qt::KeyboardModifiers(*)>(_a[2]))); break;
        case 4: _t->cancel(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (QgsMapToolSelectionHandler::*)(Qt::KeyboardModifiers );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QgsMapToolSelectionHandler::geometryChanged)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QgsMapToolSelectionHandler::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_QgsMapToolSelectionHandler.data,
    qt_meta_data_QgsMapToolSelectionHandler,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QgsMapToolSelectionHandler::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QgsMapToolSelectionHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QgsMapToolSelectionHandler.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QgsMapToolSelectionHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void QgsMapToolSelectionHandler::geometryChanged(Qt::KeyboardModifiers _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
