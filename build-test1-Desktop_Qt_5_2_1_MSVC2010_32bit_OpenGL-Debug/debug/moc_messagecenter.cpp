/****************************************************************************
** Meta object code from reading C++ file 'messagecenter.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../test1/messagecenter.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'messagecenter.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_MessageCenter_t {
    QByteArrayData data[11];
    char stringdata[106];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_MessageCenter_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_MessageCenter_t qt_meta_stringdata_MessageCenter = {
    {
QT_MOC_LITERAL(0, 0, 13),
QT_MOC_LITERAL(1, 14, 11),
QT_MOC_LITERAL(2, 26, 0),
QT_MOC_LITERAL(3, 27, 1),
QT_MOC_LITERAL(4, 29, 12),
QT_MOC_LITERAL(5, 42, 4),
QT_MOC_LITERAL(6, 47, 9),
QT_MOC_LITERAL(7, 57, 9),
QT_MOC_LITERAL(8, 67, 5),
QT_MOC_LITERAL(9, 73, 14),
QT_MOC_LITERAL(10, 88, 16)
    },
    "MessageCenter\0sendMessage\0\0s\0globalHotkey\0"
    "UINT\0broadcast\0addOutput\0title\0"
    "registerHotkey\0unregisterHotkey\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MessageCenter[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06,
       4,    2,   47,    2, 0x06,

 // slots: name, argc, parameters, tag, flags
       6,    1,   52,    2, 0x0a,
       7,    1,   55,    2, 0x0a,
       9,    2,   58,    2, 0x0a,
      10,    0,   63,    2, 0x0a,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, 0x80000000 | 5, 0x80000000 | 5,    2,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Bool, QMetaType::QString,    8,
    QMetaType::Bool, 0x80000000 | 5, 0x80000000 | 5,    2,    2,
    QMetaType::Bool,

       0        // eod
};

void MessageCenter::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MessageCenter *_t = static_cast<MessageCenter *>(_o);
        switch (_id) {
        case 0: _t->sendMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->globalHotkey((*reinterpret_cast< UINT(*)>(_a[1])),(*reinterpret_cast< UINT(*)>(_a[2]))); break;
        case 2: _t->broadcast((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 3: { bool _r = _t->addOutput((*reinterpret_cast< QString(*)>(_a[1])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 4: { bool _r = _t->registerHotkey((*reinterpret_cast< UINT(*)>(_a[1])),(*reinterpret_cast< UINT(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        case 5: { bool _r = _t->unregisterHotkey();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = _r; }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MessageCenter::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MessageCenter::sendMessage)) {
                *result = 0;
            }
        }
        {
            typedef void (MessageCenter::*_t)(UINT , UINT );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MessageCenter::globalHotkey)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject MessageCenter::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_MessageCenter.data,
      qt_meta_data_MessageCenter,  qt_static_metacall, 0, 0}
};


const QMetaObject *MessageCenter::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MessageCenter::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_MessageCenter.stringdata))
        return static_cast<void*>(const_cast< MessageCenter*>(this));
    return QWidget::qt_metacast(_clname);
}

int MessageCenter::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void MessageCenter::sendMessage(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MessageCenter::globalHotkey(UINT _t1, UINT _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
