/****************************************************************************
** Meta object code from reading C++ file 'DlgMapManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "DlgMapManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DlgMapManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DlgMapManager_t {
    QByteArrayData data[14];
    char stringdata0[189];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DlgMapManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DlgMapManager_t qt_meta_stringdata_DlgMapManager = {
    {
QT_MOC_LITERAL(0, 0, 13), // "DlgMapManager"
QT_MOC_LITERAL(1, 14, 22), // "switchBaseMapRequested"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 8), // "filePath"
QT_MOC_LITERAL(4, 47, 9), // "layerName"
QT_MOC_LITERAL(5, 57, 22), // "loadOnlineMapRequested"
QT_MOC_LITERAL(6, 80, 3), // "url"
QT_MOC_LITERAL(7, 84, 16), // "loadShpRequested"
QT_MOC_LITERAL(8, 101, 22), // "switchMapModeRequested"
QT_MOC_LITERAL(9, 124, 4), // "mode"
QT_MOC_LITERAL(10, 129, 11), // "onScanLocal"
QT_MOC_LITERAL(11, 141, 13), // "onSwitchLocal"
QT_MOC_LITERAL(12, 155, 16), // "onSetLeafletTile"
QT_MOC_LITERAL(13, 172, 16) // "onSet3DSatellite"

    },
    "DlgMapManager\0switchBaseMapRequested\0"
    "\0filePath\0layerName\0loadOnlineMapRequested\0"
    "url\0loadShpRequested\0switchMapModeRequested\0"
    "mode\0onScanLocal\0onSwitchLocal\0"
    "onSetLeafletTile\0onSet3DSatellite"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DlgMapManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   54,    2, 0x06 /* Public */,
       5,    2,   59,    2, 0x06 /* Public */,
       7,    0,   64,    2, 0x06 /* Public */,
       8,    1,   65,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   68,    2, 0x08 /* Private */,
      11,    0,   69,    2, 0x08 /* Private */,
      12,    0,   70,    2, 0x08 /* Private */,
      13,    0,   71,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    6,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void DlgMapManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DlgMapManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->switchBaseMapRequested((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->loadOnlineMapRequested((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->loadShpRequested(); break;
        case 3: _t->switchMapModeRequested((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->onScanLocal(); break;
        case 5: _t->onSwitchLocal(); break;
        case 6: _t->onSetLeafletTile(); break;
        case 7: _t->onSet3DSatellite(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DlgMapManager::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DlgMapManager::switchBaseMapRequested)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DlgMapManager::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DlgMapManager::loadOnlineMapRequested)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DlgMapManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DlgMapManager::loadShpRequested)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (DlgMapManager::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DlgMapManager::switchMapModeRequested)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DlgMapManager::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_DlgMapManager.data,
    qt_meta_data_DlgMapManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DlgMapManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DlgMapManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DlgMapManager.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int DlgMapManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void DlgMapManager::switchBaseMapRequested(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DlgMapManager::loadOnlineMapRequested(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DlgMapManager::loadShpRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void DlgMapManager::switchMapModeRequested(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
