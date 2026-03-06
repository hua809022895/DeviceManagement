/****************************************************************************
** Meta object code from reading C++ file 'qgsLockedFeature.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "./VertexTool/qgsLockedFeature.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qgsLockedFeature.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QgsLockedFeature_t {
    QByteArrayData data[12];
    char stringdata0[170];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QgsLockedFeature_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QgsLockedFeature_t qt_meta_stringdata_QgsLockedFeature = {
    {
QT_MOC_LITERAL(0, 0, 16), // "QgsLockedFeature"
QT_MOC_LITERAL(1, 17, 16), // "selectionChanged"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 16), // "vertexMapChanged"
QT_MOC_LITERAL(4, 52, 8), // "addError"
QT_MOC_LITERAL(5, 61, 18), // "QgsGeometry::Error"
QT_MOC_LITERAL(6, 80, 18), // "validationFinished"
QT_MOC_LITERAL(7, 99, 14), // "featureDeleted"
QT_MOC_LITERAL(8, 114, 12), // "QgsFeatureId"
QT_MOC_LITERAL(9, 127, 15), // "geometryChanged"
QT_MOC_LITERAL(10, 143, 11), // "QgsGeometry"
QT_MOC_LITERAL(11, 155, 14) // "beforeRollBack"

    },
    "QgsLockedFeature\0selectionChanged\0\0"
    "vertexMapChanged\0addError\0QgsGeometry::Error\0"
    "validationFinished\0featureDeleted\0"
    "QgsFeatureId\0geometryChanged\0QgsGeometry\0"
    "beforeRollBack"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QgsLockedFeature[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    0,   50,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    1,   51,    2, 0x0a /* Public */,
       6,    0,   54,    2, 0x0a /* Public */,
       7,    1,   55,    2, 0x0a /* Public */,
       9,    2,   58,    2, 0x0a /* Public */,
      11,    0,   63,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 5,    2,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    2,
    QMetaType::Void, 0x80000000 | 8, 0x80000000 | 10,    2,    2,
    QMetaType::Void,

       0        // eod
};

void QgsLockedFeature::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QgsLockedFeature *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->selectionChanged(); break;
        case 1: _t->vertexMapChanged(); break;
        case 2: _t->addError((*reinterpret_cast< QgsGeometry::Error(*)>(_a[1]))); break;
        case 3: _t->validationFinished(); break;
        case 4: _t->featureDeleted((*reinterpret_cast< QgsFeatureId(*)>(_a[1]))); break;
        case 5: _t->geometryChanged((*reinterpret_cast< QgsFeatureId(*)>(_a[1])),(*reinterpret_cast< const QgsGeometry(*)>(_a[2]))); break;
        case 6: _t->beforeRollBack(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (QgsLockedFeature::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QgsLockedFeature::selectionChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (QgsLockedFeature::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&QgsLockedFeature::vertexMapChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QgsLockedFeature::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_QgsLockedFeature.data,
    qt_meta_data_QgsLockedFeature,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QgsLockedFeature::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QgsLockedFeature::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QgsLockedFeature.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int QgsLockedFeature::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}

// SIGNAL 0
void QgsLockedFeature::selectionChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void QgsLockedFeature::vertexMapChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
