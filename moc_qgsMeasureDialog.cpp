/****************************************************************************
** Meta object code from reading C++ file 'qgsMeasureDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "./qgsMeasureDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'qgsMeasureDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_QgsMeasureDialog_t {
    QByteArrayData data[12];
    char stringdata0[116];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_QgsMeasureDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_QgsMeasureDialog_t qt_meta_stringdata_QgsMeasureDialog = {
    {
QT_MOC_LITERAL(0, 0, 16), // "QgsMeasureDialog"
QT_MOC_LITERAL(1, 17, 6), // "reject"
QT_MOC_LITERAL(2, 24, 0), // ""
QT_MOC_LITERAL(3, 25, 7), // "restart"
QT_MOC_LITERAL(4, 33, 10), // "closeEvent"
QT_MOC_LITERAL(5, 44, 12), // "QCloseEvent*"
QT_MOC_LITERAL(6, 57, 1), // "e"
QT_MOC_LITERAL(7, 59, 14), // "updateSettings"
QT_MOC_LITERAL(8, 74, 12), // "unitsChanged"
QT_MOC_LITERAL(9, 87, 5), // "index"
QT_MOC_LITERAL(10, 93, 10), // "crsChanged"
QT_MOC_LITERAL(11, 104, 11) // "projChanged"

    },
    "QgsMeasureDialog\0reject\0\0restart\0"
    "closeEvent\0QCloseEvent*\0e\0updateSettings\0"
    "unitsChanged\0index\0crsChanged\0projChanged"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_QgsMeasureDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x0a /* Public */,
       3,    0,   50,    2, 0x0a /* Public */,
       4,    1,   51,    2, 0x0a /* Public */,
       7,    0,   54,    2, 0x0a /* Public */,
       8,    1,   55,    2, 0x08 /* Private */,
      10,    0,   58,    2, 0x08 /* Private */,
      11,    0,   59,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    9,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void QgsMeasureDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<QgsMeasureDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->reject(); break;
        case 1: _t->restart(); break;
        case 2: _t->closeEvent((*reinterpret_cast< QCloseEvent*(*)>(_a[1]))); break;
        case 3: _t->updateSettings(); break;
        case 4: _t->unitsChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->crsChanged(); break;
        case 6: _t->projChanged(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject QgsMeasureDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_QgsMeasureDialog.data,
    qt_meta_data_QgsMeasureDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *QgsMeasureDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *QgsMeasureDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_QgsMeasureDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int QgsMeasureDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE
