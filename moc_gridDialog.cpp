/****************************************************************************
** Meta object code from reading C++ file 'gridDialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "stdafx.h"
#include "./gridDialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'gridDialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_GridDialog_t {
    QByteArrayData data[16];
    char stringdata0[200];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GridDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GridDialog_t qt_meta_stringdata_GridDialog = {
    {
QT_MOC_LITERAL(0, 0, 10), // "GridDialog"
QT_MOC_LITERAL(1, 11, 13), // "extentChanged"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 12), // "QgsRectangle"
QT_MOC_LITERAL(4, 39, 1), // "r"
QT_MOC_LITERAL(5, 41, 17), // "validationChanged"
QT_MOC_LITERAL(6, 59, 5), // "valid"
QT_MOC_LITERAL(7, 65, 22), // "toggleDialogVisibility"
QT_MOC_LITERAL(8, 88, 7), // "visible"
QT_MOC_LITERAL(9, 96, 28), // "setOutExtentFromDrawOnCanvas"
QT_MOC_LITERAL(10, 125, 11), // "extentDrawn"
QT_MOC_LITERAL(11, 137, 6), // "extent"
QT_MOC_LITERAL(12, 144, 18), // "setRoadTypeOptions"
QT_MOC_LITERAL(13, 163, 5), // "index"
QT_MOC_LITERAL(14, 169, 19), // "createGridAbundance"
QT_MOC_LITERAL(15, 189, 10) // "cleartGird"

    },
    "GridDialog\0extentChanged\0\0QgsRectangle\0"
    "r\0validationChanged\0valid\0"
    "toggleDialogVisibility\0visible\0"
    "setOutExtentFromDrawOnCanvas\0extentDrawn\0"
    "extent\0setRoadTypeOptions\0index\0"
    "createGridAbundance\0cleartGird"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GridDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       5,    1,   57,    2, 0x06 /* Public */,
       7,    1,   60,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   63,    2, 0x0a /* Public */,
      10,    1,   64,    2, 0x0a /* Public */,
      12,    1,   67,    2, 0x0a /* Public */,
      14,    0,   70,    2, 0x0a /* Public */,
      15,    0,   71,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Bool,    6,
    QMetaType::Void, QMetaType::Bool,    8,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 3,   11,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void GridDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<GridDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->extentChanged((*reinterpret_cast< const QgsRectangle(*)>(_a[1]))); break;
        case 1: _t->validationChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->toggleDialogVisibility((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 3: _t->setOutExtentFromDrawOnCanvas(); break;
        case 4: _t->extentDrawn((*reinterpret_cast< const QgsRectangle(*)>(_a[1]))); break;
        case 5: _t->setRoadTypeOptions((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->createGridAbundance(); break;
        case 7: _t->cleartGird(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (GridDialog::*)(const QgsRectangle & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GridDialog::extentChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (GridDialog::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GridDialog::validationChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (GridDialog::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&GridDialog::toggleDialogVisibility)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject GridDialog::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_GridDialog.data,
    qt_meta_data_GridDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *GridDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GridDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_GridDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int GridDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void GridDialog::extentChanged(const QgsRectangle & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GridDialog::validationChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GridDialog::toggleDialogVisibility(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
