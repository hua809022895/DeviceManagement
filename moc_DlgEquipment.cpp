/****************************************************************************
** Meta object code from reading C++ file 'DlgEquipment.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.4)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "DlgEquipment.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'DlgEquipment.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.4. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DlgEquipment_t {
    QByteArrayData data[10];
    char stringdata0[141];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DlgEquipment_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DlgEquipment_t qt_meta_stringdata_DlgEquipment = {
    {
QT_MOC_LITERAL(0, 0, 12), // "DlgEquipment"
QT_MOC_LITERAL(1, 13, 13), // "onItemClicked"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 17), // "QTableWidgetItem*"
QT_MOC_LITERAL(4, 46, 4), // "item"
QT_MOC_LITERAL(5, 51, 15), // "on_addBtn_click"
QT_MOC_LITERAL(6, 67, 15), // "on_Midify_click"
QT_MOC_LITERAL(7, 83, 18), // "on_deleteBtn_click"
QT_MOC_LITERAL(8, 102, 21), // "on_AllDeleteBtn_click"
QT_MOC_LITERAL(9, 124, 16) // "on_exitBtn_click"

    },
    "DlgEquipment\0onItemClicked\0\0"
    "QTableWidgetItem*\0item\0on_addBtn_click\0"
    "on_Midify_click\0on_deleteBtn_click\0"
    "on_AllDeleteBtn_click\0on_exitBtn_click"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DlgEquipment[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x0a /* Public */,
       5,    0,   47,    2, 0x0a /* Public */,
       6,    0,   48,    2, 0x0a /* Public */,
       7,    0,   49,    2, 0x0a /* Public */,
       8,    0,   50,    2, 0x0a /* Public */,
       9,    0,   51,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void DlgEquipment::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DlgEquipment *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onItemClicked((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        case 1: _t->on_addBtn_click(); break;
        case 2: _t->on_Midify_click(); break;
        case 3: _t->on_deleteBtn_click(); break;
        case 4: _t->on_AllDeleteBtn_click(); break;
        case 5: _t->on_exitBtn_click(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DlgEquipment::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_DlgEquipment.data,
    qt_meta_data_DlgEquipment,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DlgEquipment::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DlgEquipment::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DlgEquipment.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int DlgEquipment::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
