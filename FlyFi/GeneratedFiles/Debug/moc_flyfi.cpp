/****************************************************************************
** Meta object code from reading C++ file 'flyfi.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.4.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../flyfi.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'flyfi.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.4.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_FlyFi_t {
    QByteArrayData data[9];
    char stringdata[145];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FlyFi_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FlyFi_t qt_meta_stringdata_FlyFi = {
    {
QT_MOC_LITERAL(0, 0, 5), // "FlyFi"
QT_MOC_LITERAL(1, 6, 26), // "on_btnRefreshPorts_clicked"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 18), // "on_btnOpen_clicked"
QT_MOC_LITERAL(4, 53, 23), // "on_sldFreq_valueChanged"
QT_MOC_LITERAL(5, 77, 3), // "val"
QT_MOC_LITERAL(6, 81, 18), // "on_btnPlay_clicked"
QT_MOC_LITERAL(7, 100, 18), // "on_btnStop_clicked"
QT_MOC_LITERAL(8, 119, 25) // "on_action_close_triggered"

    },
    "FlyFi\0on_btnRefreshPorts_clicked\0\0"
    "on_btnOpen_clicked\0on_sldFreq_valueChanged\0"
    "val\0on_btnPlay_clicked\0on_btnStop_clicked\0"
    "on_action_close_triggered"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FlyFi[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   44,    2, 0x0a /* Public */,
       3,    0,   45,    2, 0x0a /* Public */,
       4,    1,   46,    2, 0x0a /* Public */,
       6,    0,   49,    2, 0x0a /* Public */,
       7,    0,   50,    2, 0x0a /* Public */,
       8,    0,   51,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void FlyFi::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        FlyFi *_t = static_cast<FlyFi *>(_o);
        switch (_id) {
        case 0: _t->on_btnRefreshPorts_clicked(); break;
        case 1: _t->on_btnOpen_clicked(); break;
        case 2: _t->on_sldFreq_valueChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 3: _t->on_btnPlay_clicked(); break;
        case 4: _t->on_btnStop_clicked(); break;
        case 5: _t->on_action_close_triggered(); break;
        default: ;
        }
    }
}

const QMetaObject FlyFi::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_FlyFi.data,
      qt_meta_data_FlyFi,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *FlyFi::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FlyFi::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_FlyFi.stringdata))
        return static_cast<void*>(const_cast< FlyFi*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int FlyFi::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
