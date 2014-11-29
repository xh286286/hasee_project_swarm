/****************************************************************************
** Meta object code from reading C++ file 'danmuwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../winpcap_test/danmuwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'danmuwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_DanmuWindow_t {
    QByteArrayData data[11];
    char stringdata[121];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DanmuWindow_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DanmuWindow_t qt_meta_stringdata_DanmuWindow = {
    {
QT_MOC_LITERAL(0, 0, 11),
QT_MOC_LITERAL(1, 12, 14),
QT_MOC_LITERAL(2, 27, 0),
QT_MOC_LITERAL(3, 28, 1),
QT_MOC_LITERAL(4, 30, 10),
QT_MOC_LITERAL(5, 41, 10),
QT_MOC_LITERAL(6, 52, 11),
QT_MOC_LITERAL(7, 64, 16),
QT_MOC_LITERAL(8, 81, 11),
QT_MOC_LITERAL(9, 93, 11),
QT_MOC_LITERAL(10, 105, 15)
    },
    "DanmuWindow\0broadcastDanmu\0\0s\0informGift\0"
    "getMessage\0getMessage2\0addOneDebugInfor\0"
    "updateLater\0hideandshow\0postDanmuMessge"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DanmuWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       4,    1,   57,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   60,    2, 0x0a /* Public */,
       6,    0,   61,    2, 0x0a /* Public */,
       7,    1,   62,    2, 0x0a /* Public */,
       8,    0,   65,    2, 0x0a /* Public */,
       9,    0,   66,    2, 0x0a /* Public */,
      10,    1,   67,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QJsonObject,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    3,

       0        // eod
};

void DanmuWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        DanmuWindow *_t = static_cast<DanmuWindow *>(_o);
        switch (_id) {
        case 0: _t->broadcastDanmu((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->informGift((*reinterpret_cast< QJsonObject(*)>(_a[1]))); break;
        case 2: _t->getMessage(); break;
        case 3: _t->getMessage2(); break;
        case 4: _t->addOneDebugInfor((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 5: _t->updateLater(); break;
        case 6: _t->hideandshow(); break;
        case 7: _t->postDanmuMessge((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (DanmuWindow::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DanmuWindow::broadcastDanmu)) {
                *result = 0;
            }
        }
        {
            typedef void (DanmuWindow::*_t)(QJsonObject );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&DanmuWindow::informGift)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject DanmuWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_DanmuWindow.data,
      qt_meta_data_DanmuWindow,  qt_static_metacall, 0, 0}
};


const QMetaObject *DanmuWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DanmuWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_DanmuWindow.stringdata))
        return static_cast<void*>(const_cast< DanmuWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int DanmuWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void DanmuWindow::broadcastDanmu(QString _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DanmuWindow::informGift(QJsonObject _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
