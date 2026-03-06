/****************************************************************************
** Meta object code from reading C++ file 'qgsMaptoolJpgAnnotation.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "./qgsMaptoolJpgAnnotation.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qgsMaptoolJpgAnnotation.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QgsMapToolJpgAnnotation_t {
    QByteArrayData data[8];
    char stringdata0[79];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QgsMapToolJpgAnnotation_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QgsMapToolJpgAnnotation_t qt_meta_stringdata_QgsMapToolJpgAnnotation = {
    {
QT_MOC_LITERAL(0, 0, 23), // "QgsMapToolJpgAnnotation"
QT_MOC_LITERAL(1, 24, 20), // "showLeftTableMessage"
QT_MOC_LITERAL(2, 45, 0), // ""
QT_MOC_LITERAL(3, 46, 7), // "QPoint&"
QT_MOC_LITERAL(4, 54, 3), // "pos"
QT_MOC_LITERAL(5, 58, 4), // "show"
QT_MOC_LITERAL(6, 63, 9), // "Mappoint*"
QT_MOC_LITERAL(7, 73, 5) // "point"

    },
    "QgsMapToolJpgAnnotation\0showLeftTableMessage\0"
    "\0QPoint&\0pos\0show\0Mappoint*\0point"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QgsMapToolJpgAnnotation[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    3,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::Bool, 0x80000000 | 6,    4,    5,    7,

       0        // eod
};

void QgsMapToolJpgAnnotation::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QgsMapToolJpgAnnotation *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->showLeftTableMessage((*reinterpret_cast< QPoint(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2])),(*reinterpret_cast< Mappoint*(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (QgsMapToolJpgAnnotation::*)(QPoint & , bool , Mappoint * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QgsMapToolJpgAnnotation::showLeftTableMessage)) {
                *result = 0;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QgsMapToolJpgAnnotation::staticMetaObject = { {
    &QgsMapTool::staticMetaObject,
    qt_meta_stringdata_QgsMapToolJpgAnnotation.data,
    qt_meta_data_QgsMapToolJpgAnnotation,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QgsMapToolJpgAnnotation::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QgsMapToolJpgAnnotation::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QgsMapToolJpgAnnotation.stringdata0))
        return static_cast<void*>(this);
    return QgsMapTool::qt_metacast(_clname);
}

int QgsMapToolJpgAnnotation::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QgsMapTool::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void QgsMapToolJpgAnnotation::showLeftTableMessage(QPoint & _t1, bool _t2, Mappoint * _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
