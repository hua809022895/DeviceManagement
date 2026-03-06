/****************************************************************************
** Meta object code from reading C++ file 'mavlink5Thread.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "mavlink5Thread.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mavlink5Thread.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_mavlink5Thread_t {
    QByteArrayData data[6];
    char stringdata0[75];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_mavlink5Thread_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_mavlink5Thread_t qt_meta_stringdata_mavlink5Thread = {
    {
QT_MOC_LITERAL(0, 0, 14), // "mavlink5Thread"
QT_MOC_LITERAL(1, 15, 15), // "signal_setTitle"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 18), // "signal_modifyPlane"
QT_MOC_LITERAL(4, 51, 17), // "tag_PlaneMessage*"
QT_MOC_LITERAL(5, 69, 5) // "plane"

    },
    "mavlink5Thread\0signal_setTitle\0\0"
    "signal_modifyPlane\0tag_PlaneMessage*\0"
    "plane"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_mavlink5Thread[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,
       3,    1,   25,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 4,    5,

       0        // eod
};

void mavlink5Thread::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<mavlink5Thread *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signal_setTitle(); break;
        case 1: _t->signal_modifyPlane((*reinterpret_cast< tag_PlaneMessage*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (mavlink5Thread::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mavlink5Thread::signal_setTitle)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (mavlink5Thread::*)(tag_PlaneMessage * );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&mavlink5Thread::signal_modifyPlane)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject mavlink5Thread::staticMetaObject = { {
    &QThread::staticMetaObject,
    qt_meta_stringdata_mavlink5Thread.data,
    qt_meta_data_mavlink5Thread,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *mavlink5Thread::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *mavlink5Thread::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_mavlink5Thread.stringdata0))
        return static_cast<void*>(this);
    return QThread::qt_metacast(_clname);
}

int mavlink5Thread::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void mavlink5Thread::signal_setTitle()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void mavlink5Thread::signal_modifyPlane(tag_PlaneMessage * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
