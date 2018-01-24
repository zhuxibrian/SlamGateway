/****************************************************************************
** Meta object code from reading C++ file 'rwconsumer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../rwconsumer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'rwconsumer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_RWConsumer_t {
    QByteArrayData data[10];
    char stringdata0[100];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RWConsumer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RWConsumer_t qt_meta_stringdata_RWConsumer = {
    {
QT_MOC_LITERAL(0, 0, 10), // "RWConsumer"
QT_MOC_LITERAL(1, 11, 11), // "onConnected"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 14), // "onDisconnected"
QT_MOC_LITERAL(4, 39, 12), // "onSubscribed"
QT_MOC_LITERAL(5, 52, 5), // "topic"
QT_MOC_LITERAL(6, 58, 10), // "onReceived"
QT_MOC_LITERAL(7, 69, 14), // "QMQTT::Message"
QT_MOC_LITERAL(8, 84, 7), // "message"
QT_MOC_LITERAL(9, 92, 7) // "onStart"

    },
    "RWConsumer\0onConnected\0\0onDisconnected\0"
    "onSubscribed\0topic\0onReceived\0"
    "QMQTT::Message\0message\0onStart"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RWConsumer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   39,    2, 0x0a /* Public */,
       3,    0,   40,    2, 0x0a /* Public */,
       4,    1,   41,    2, 0x0a /* Public */,
       6,    1,   44,    2, 0x0a /* Public */,
       9,    0,   47,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void,

       0        // eod
};

void RWConsumer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        RWConsumer *_t = static_cast<RWConsumer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->onConnected(); break;
        case 1: _t->onDisconnected(); break;
        case 2: _t->onSubscribed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->onReceived((*reinterpret_cast< const QMQTT::Message(*)>(_a[1]))); break;
        case 4: _t->onStart(); break;
        default: ;
        }
    }
}

const QMetaObject RWConsumer::staticMetaObject = {
    { &QMQTT::Client::staticMetaObject, qt_meta_stringdata_RWConsumer.data,
      qt_meta_data_RWConsumer,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *RWConsumer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RWConsumer::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_RWConsumer.stringdata0))
        return static_cast<void*>(const_cast< RWConsumer*>(this));
    return QMQTT::Client::qt_metacast(_clname);
}

int RWConsumer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMQTT::Client::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
